#pragma once

#include <string>

namespace yage::res
{
    template<typename ResourceType>
    class Store;

    template<typename ResourceType>
    class Resource final
    {
    public:
        ResourceType& get();

    private:
        std::string m_uri;
        Store<ResourceType>* m_store;

        Resource(std::string uri, Store<ResourceType>* store)
            : m_uri(std::move(uri)), m_store(store)
        {
        }

        friend class Store<ResourceType>;
    };
}

#include "Store.h"

namespace yage::res
{
    template<typename ResourceType>
    ResourceType& Resource<ResourceType>::get()
    {
        return m_store->get(m_uri);
    }
}
