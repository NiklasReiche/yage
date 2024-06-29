#include "SceneLoader.h"
#include <resource/Store.h>
#include "resources/gltf.h"

namespace yage::gl3d {
    SceneLoader::SceneLoader(const std::shared_ptr<platform::IFileReader>& file_reader, res::Store<Mesh>* mesh_store)
        : m_file_reader(file_reader), m_mesh_store(mesh_store)
    {
    }

    SceneGroup SceneLoader::load_resource(const std::string& uri)
    {
        if (uri.empty()) { // TODO: separate this into custom loader or something
            return SceneGroup("root");
        }

        // TODO: somehow cache the gltf file so we don't read it twice
        std::vector<res::Resource<Mesh>> meshes = m_mesh_store->load_archive(uri);

        std::unordered_map<int, std::reference_wrapper<SceneObject>> mesh_nodes;
        SceneGroup root = std::move(resources::gltf_read_scene(*m_file_reader, uri, mesh_nodes)[0]); // TODO: scene selection

        for (auto& [mesh_index, scene_object] : mesh_nodes) {
            scene_object.get().mesh = meshes[mesh_index];
        }

        return root;
    }
}
