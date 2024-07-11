#pragma once

#include <cassert>
#include <cstddef>
#include <memory>
#include <optional>
#include <queue>
#include <vector>

namespace yage::gl
{
    template<typename Base>
    class StoreBase;

    template<typename Base, typename Derived>
        requires std::derived_from<Derived, Base>
    class Store;

    using StoreId = std::size_t;

    template<typename T>
    class Handle final
    {
    public:
        /**
         * Constructs an empty (i.e. invalid) handle.
         */
        Handle();

        ~Handle();

        Handle(const Handle& other) = delete;

        Handle(Handle&& other) noexcept;

        Handle& operator=(const Handle& other) = delete;

        Handle& operator=(Handle&& other) noexcept;

        /**
         * Returns a base-type pointer to the associated resource.
         */
        T* operator->() const; // TODO: const-correctness?

        /**
         * Returns a base-type reference to the associated resource.
         */
        T& operator*() const; // TODO: const-correctness?

        /**
         * Returns a derived-type reference to the associated resource.
         */
        template<typename T_>
            requires std::derived_from<T_, T>
        T_& get() const;

        /**
         * Explicitly destroys the associated resource and invalidates this handle.
         */
        void reset();

        /**
         * @return Whether this Handle is empty (i.e. invalid).
         */
        [[nodiscard]] bool empty() const;

        /**
         * Moves this rvalue Handle to a shared Handle.
         */
        std::shared_ptr<Handle> as_shared() &&;

    private:
        /**
         * Reference to the store of this handle.
         * Since a handle might outlive its store, we need to reason about dangling pointers here.
         */
        /* TODO there might be ways to achieve dangling pointer safety without forcing stores to be shared_pt:
         * - Let the store hold its generated handles to invalidate them upon store destruction.
         *   This would necessitate either storing handles as shared_ptr (bad) or using custom reference counting in
         *   handles.
         * - Introduce a connector component that is referenced by a store and its handles. The store invalidates
         *   the connector upon destruction. The connector would probably still need to be implemented with
         *   shared_ptr/weak_ptr sematics.
         */
        std::weak_ptr<StoreBase<T>> m_store;
        /**
         * Unique identifier within the store for checking validity.
         */
        StoreId m_id;
        /**
         * Array index where this resource was created.
         */
        std::size_t m_index;

        Handle(std::weak_ptr<StoreBase<T>> store, std::size_t id, std::size_t index);

        template<typename Base, typename Derived>
            requires std::derived_from<Derived, Base>
        friend class Store;
    };

    template<typename Base>
    class StoreBase
    {
    public:
        virtual ~StoreBase() = default;

        virtual Base& lookup(const Handle<Base>& handle) = 0;

        [[nodiscard]] virtual bool contains(const Handle<Base>& handle) const = 0;

    protected:
        virtual void destroy(const Handle<Base>& handle) = 0;

        friend class Handle<Base>;
    };

    template<typename Base, typename Derived>
        requires std::derived_from<Derived, Base>
    class Store final : public StoreBase<Base>, public std::enable_shared_from_this<Store<Base, Derived>>
    {
    public:
        Base& lookup(const Handle<Base>& handle) override;

        [[nodiscard]] bool contains(const Handle<Base>& handle) const override;

        template<typename... Args>
        Handle<Base> create(Args&&... args);

        void clear() noexcept;

    private:
        /**
         * Dynamic array of resources with holes for destroyed resources (modeled with std::optional to get well-defined
         * destruction).
         */
        std::vector<std::optional<Derived>> m_resources;
        /**
         * Unique identifiers for stored resources to check validity of handles. Non-overwritten, destroyed resources
         * have id '0'.
         */
        std::vector<StoreId> m_ids;
        /**
         * Keeps track of holes, so that resource vector only grows when necessary.
         */
        std::queue<std::size_t> m_free_queue;
        /**
         * Id '0' is reserved for destroyed entries.
         */
        StoreId m_next_id = 0;

        void destroy(const Handle<Base>& handle) override;

        friend class Handle<Base>;
    };

    template<typename T>
    Handle<T>::Handle()
        : m_store(),
          m_id(0),
          m_index(0)
    {
    }

    template<typename T>
    Handle<T>::~Handle()
    {
        reset();
    }

    template<typename T>
    Handle<T>::Handle(Handle&& other) noexcept
    {
        m_store = std::move(other.m_store);
        m_id = other.m_id;
        m_index = other.m_index;

        other.m_id = 0;
    }

    template<typename T>
    Handle<T>& Handle<T>::operator=(Handle&& other) noexcept
    {
        m_store = std::move(other.m_store);
        m_id = other.m_id;
        m_index = other.m_index;

        other.m_id = 0;

        return *this;
    }

    template<typename T>
    T* Handle<T>::operator->() const
    {
        return &(m_store.lock()->lookup(*this));
    }

    template<typename T>
    T& Handle<T>::operator*() const
    {
        return m_store.lock()->lookup(*this);
    }

    template<typename T>
    template<typename T_>
        requires std::derived_from<T_, T>
    T_& Handle<T>::get() const
    {
        return static_cast<T_&>(m_store.lock()->lookup(*this));
    }

    template<typename T>
    void Handle<T>::reset()
    {
        // If the id is zero, this handle was moved from and is no longer valid, hence we must not destroy the
        // associated resource.
        // If the store pointer is expired, the store and all contained resources were already deleted, hence we must
        // not destroy the associated resource.
        if (m_id != 0 && !m_store.expired()) {
            m_store.lock()->destroy(*this);
        }
        m_id = 0;
    }

    template<typename T>
    bool Handle<T>::empty() const
    {
        return !m_store.expired() && m_store.lock()->contains(*this);
    }

    template<typename T>
    std::shared_ptr<Handle<T>> Handle<T>::as_shared() &&
    {
        return std::make_shared<Handle>(std::move(*this));
    }

    template<typename T>
    Handle<T>::Handle(std::weak_ptr<StoreBase<T>> store, const std::size_t id, const std::size_t index)
        : m_store(std::move(store)),
          m_id(id),
          m_index(index)
    {
    }

    template<typename Base, typename Derived>
        requires std::derived_from<Derived, Base>
    Base& Store<Base, Derived>::lookup(const Handle<Base>& handle)
    {
        if (!contains(handle)) {
            throw std::runtime_error("resource handle is not valid");
        }
        return *m_resources[handle.m_index];
    }

    template<typename Base, typename Derived>
        requires std::derived_from<Derived, Base>
    bool Store<Base, Derived>::contains(const Handle<Base>& handle) const
    {
        return m_ids[handle.m_index] == handle.m_id;
    }

    template<typename Base, typename Derived>
        requires std::derived_from<Derived, Base>
    template<typename... Args>
    Handle<Base> Store<Base, Derived>::create(Args&&... args)
    {
        if (m_free_queue.empty()) {
            m_resources.push_back({});
            m_resources.back().emplace(std::forward<Args>(args)...);
            m_next_id++;
            m_ids.push_back(m_next_id);
            return Handle<Base>(std::enable_shared_from_this<Store>::weak_from_this(), m_next_id,
                                m_resources.size() - 1);
        }

        std::size_t free_index = m_free_queue.front();
        m_resources[free_index].emplace(std::forward<Args>(args)...);
        m_free_queue.pop();
        m_next_id++;
        m_ids[free_index] = m_next_id;
        return Handle<Base>(std::enable_shared_from_this<Store>::weak_from_this(), m_next_id, free_index);
    }

    template<typename Base, typename Derived>
        requires std::derived_from<Derived, Base>
    void Store<Base, Derived>::clear() noexcept
    {
        std::vector<std::optional<Derived>> v;
        m_resources.swap(v);

        std::vector<StoreId> i;
        m_ids.swap(i);

        std::queue<std::size_t> q;
        m_free_queue.swap(q);
    }

    template<typename Base, typename Derived>
        requires std::derived_from<Derived, Base>
    void Store<Base, Derived>::destroy(const Handle<Base>& handle)
    {
        assert(contains(handle));
        m_resources[handle.m_index].reset();
        m_ids[handle.m_index] = 0;
        m_free_queue.push(handle.m_index);
    }
}
