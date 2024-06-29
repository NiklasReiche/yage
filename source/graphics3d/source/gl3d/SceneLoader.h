#pragma once

#include <memory>

#include <core/platform/IFileReader.h>
#include <resource/Loader.h>

#include "sceneGraph/sceneGroup.h"

namespace yage::gl3d
{
    class SceneLoader final : public res::Loader<SceneGroup>
    {
    public:
        SceneLoader(const std::shared_ptr<platform::IFileReader>& file_reader, res::Store<Mesh>* mesh_store);

    protected:
        SceneGroup load_resource(const std::string& uri) override;

    private:
        std::shared_ptr<platform::IFileReader> m_file_reader;
        res::Store<Mesh>* m_mesh_store;
    };
}
