#include "gltf.h"
#include <vector>
#include <nlohmann/json.hpp>
#include <utils/strings.h>
#include <gml/vector.h>

using json = nlohmann::json;

namespace
{
	enum COMPONENT_TYPE
	{
		FLOAT = 5126,
		UNSIGNED_SHORT = 5123
	};

	enum TARGET
	{
		ARRAY_BUFFER = 34962,
		ELEMENT_ARRAY_BUFFER = 34963
	};


}

namespace gl3d::resources
{
	std::tuple<std::unique_ptr<gl::IDrawable>, gl3d::Material> readGltf(
		const platform::IFileReader& fileReader, const std::string& filename,
		gl::IDrawableCreator& )
	{
		auto file = fileReader.openTextFile(filename, platform::IFile::AccessMode::READ);
		auto data = json::parse(file->readAll());

		std::vector<std::unique_ptr<platform::IBinaryFile>> buffers;
		for (auto bufferDef : data["buffers"]) {
			if (!utils::startsWith(bufferDef["uri"], "data:application/octet-stream")) {
				auto tmp = utils::strip(filename, "/");
				tmp.pop_back();
				std::string path = utils::join(tmp);
				buffers.push_back(fileReader.openBinaryFile(path + "/" + bufferDef["uri"].get<std::string>(), platform::IFile::AccessMode::READ));
			}
		}

		auto& meshPrimitive = data["meshes"][0]["primitives"][0];
//		int normalsAccessor = meshPrimitive["attributes"]["POSITION"].get<int>();
//		int indicesAccessor = meshPrimitive["indices"].get<int>();

		int positionsAccessor = meshPrimitive["attributes"]["POSITION"].get<int>();
		int positionsView = data["accessors"][positionsAccessor]["bufferView"];
		auto& bufferView = data["bufferViews"][positionsView];
		std::vector<gml::Vec3f> positions(bufferView["byteLength"].get<int>() / 12);
		buffers[bufferView["buffer"].get<int>()]->seek(bufferView["byteOffset"].get<int>(), platform::IFile::SeekOffset::BEG);
		buffers[bufferView["buffer"].get<int>()]->read(&positions[0], bufferView["byteLength"].get<int>());

		return std::tuple<std::unique_ptr<gl::IDrawable>, gl3d::Material>(nullptr, gl3d::Material());
	}
}
