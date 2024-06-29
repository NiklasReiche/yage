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

        virtual std::tuple<std::vector<ResourceType>, std::vector<std::string>> load_archive([[maybe_unused]] const std::string& uri)
        {
            throw std::logic_error("This loader does not implement archive loading");
        }

    private:
        friend class Store<ResourceType>;
    };
}
