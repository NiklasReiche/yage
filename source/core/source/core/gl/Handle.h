#pragma once

#include <cassert>
#include <cstddef>
#include <queue>
#include <vector>
#include <optional>
#include <memory>

namespace yage::gl
{
    class Instance;

    template<typename T>
    class Store;

    template<typename T>
    class Handle;

    using StoreId = std::size_t;

    template<typename T>
    class Handle final
    {
    public:
        ~Handle();

        Handle(const Handle& other) = delete;

        Handle(Handle&& other) noexcept
        {
            m_store = std::move(other.m_store);
            m_id = other.m_id;
            m_index = other.m_index;

            other.m_id = 0;
        }

        Handle& operator=(const Handle& other) = delete;

        Handle& operator=(Handle&& other) noexcept
        {
            m_store = std::move(other.m_store);
            m_id = other.m_id;
            m_index = other.m_index;

            other.m_id = 0;

            return *this;
        }

        T& get() const;

        [[nodiscard]] bool empty() const;

    private:
        std::weak_ptr<Store<T>> m_store;
        /**
         * Unique identifier within the store for checking validity.
         */
        StoreId m_id;
        /**
         * Array index where this resource was created.
         */
        std::size_t m_index;

        Handle(std::weak_ptr<Store<T> > store, std::size_t id, std::size_t index);

        friend class Store<T>;
    };

    template<typename T>
    class Store final : public std::enable_shared_from_this<Store<T>>
    {
    public:
        T& lookup(const Handle<T>& handle);

        [[nodiscard]] bool contains(const Handle<T>& handle) const;

        template<typename... Args>
        Handle<T> create(Args&& ...args);

    private:
        /**
         * Dynamic array of resources with holes for destroyed resources (modeled with std::optional to get well-defined destruction).
         */
        std::vector<std::optional<T>> m_resources;
        /**
         * Unique identifiers for stored resources to check validity of handles. Non-overwritten, destroyed resources have id '0'.
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

        void destroy(const Handle<T>& handle);

        friend class Handle<T>;
    };

    template<typename T>
    Handle<T>::~Handle()
    {
        // if the id is zero, this handle was moved from and is no longer valid, hence we must not destroy the associated resource
        if (m_id != 0) {
            m_store.lock()->destroy(*this);
        }
    }

    template<typename T>
    T& Handle<T>::get() const
    {
        return m_store.lock()->lookup(*this);
    }

    template<typename T>
    bool Handle<T>::empty() const
    {
        return !m_store.expired() && m_store.lock()->contains(*this);
    }

    template<typename T>
    Handle<T>::Handle(std::weak_ptr<Store<T>> store, const std::size_t id, const std::size_t index)
        : m_store(std::move(store)), m_id(id), m_index(index)
    {
    }

    template<typename T>
    T& Store<T>::lookup(const Handle<T>& handle)
    {
        if (!contains(handle)) {
            throw std::runtime_error("resource handle is not valid");
        }
        return *m_resources[handle.m_index];
    }

    template<typename T>
    bool Store<T>::contains(const Handle<T>& handle) const
    {
        return m_ids[handle.m_index] == handle.m_id;
    }

    template<typename T>
    template<typename... Args>
    Handle<T> Store<T>::create(Args&& ...args)
    {
        if (m_free_queue.empty()) {
            m_resources.push_back({});
            m_resources.back().emplace(std::forward<Args>(args)...);
            m_next_id++;
            m_ids.push_back(m_next_id);
            return Handle<T>(std::enable_shared_from_this<Store>::weak_from_this(), m_next_id, m_resources.size() - 1);
        }

        std::size_t free_index = m_free_queue.front();
        m_resources[free_index].emplace(std::forward<Args>(args)...);
        m_free_queue.pop();
        m_next_id++;
        m_ids[free_index] = m_next_id;
        return Handle<T>(std::enable_shared_from_this<Store>::weak_from_this(), m_next_id, free_index);
    }

    template<typename T>
    void Store<T>::destroy(const Handle<T>& handle)
    {
        assert(contains(handle));
        m_resources[handle.m_index].reset();
        m_ids[handle.m_index] = 0;
        m_free_queue.push(handle.m_index);
    }
}
