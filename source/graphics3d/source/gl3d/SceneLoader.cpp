#include "SceneLoader.h"
#include <resource/Store.h>
#include "resources/gltf.h"
#include "utils/strings.h"

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

        const std::vector<std::string>& path_parts = utils::strip(uri, ".");
        if (path_parts.size() < 2) {
            throw std::invalid_argument("Missing file extension in uri '" + uri + "'");
        }
        const std::string extension = path_parts[1];

        if (extension == "obj") {
            SceneGroup root("root");
            int i = 0;
            for (auto& mesh : meshes) {
                auto& node = root.create_object("mesh" + std::to_string(i));
                node.mesh = mesh;
                i++;
            }
            return root;
        }

        if (extension == "glb" || extension == "gltf") {
            std::unordered_map<int, std::reference_wrapper<SceneObject> > mesh_nodes;
            SceneGroup root = std::move(resources::gltf_read_scene(*m_file_reader, uri, mesh_nodes)[0]);
            // TODO: scene selection

            for (auto& [mesh_index, scene_object]: mesh_nodes) {
                scene_object.get().mesh = meshes[mesh_index];
            }

            return root;
        }

        throw std::invalid_argument("Unsupported file type in uri '" + uri + "'");
    }
}
