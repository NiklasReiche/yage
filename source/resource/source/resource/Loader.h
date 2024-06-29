#pragma once

#include <string>
#include <map>
#include <span>

namespace yage::res
{
    template<typename ResourceType>
    class Store;

    template<typename ResourceType>
    class Loader
    {
    public:
        virtual ~Loader() = default;

    protected:
        virtual ResourceType load_resource(const std::string& uri) = 0;

        virtual std::vector<ResourceType> load_archive(std::span<std::string> uris)
        {
            std::vector<ResourceType> resources;
            resources.reserve(uris.size());
            for (const std::string& uri : uris) {
                resources.push_back(load_resource(uri));
            }
            return resources;
        }

        // TODO: allow emitting resources directly into the store instead of explicitly giving containers to fill
        virtual void load_archive([[maybe_unused]] const std::string& uri,
            [[maybe_unused]] std::vector<ResourceType>& resources, [[maybe_unused]] std::vector<std::string>& uris)
        {
            throw std::logic_error("This loader does not implement archive loading");
        }

    private:
        friend class Store<ResourceType>;
    };
}
