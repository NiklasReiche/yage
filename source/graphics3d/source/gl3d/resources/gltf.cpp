#include "gltf.h"
#include <vector>
#include <utils/strings.h>
#include <gml/vector.h>

#include <utils/NotImplementedException.h>

#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include <tiny_gltf.h>

namespace
{
	enum class COMPONENT_TYPE
	{
		BYTE = 5120,
		UNSIGNED_BYTE = 5121,
		SHORT = 5122,
		UNSIGNED_SHORT = 5123,
		UNSIGNED_INT = 5125,
		FLOAT = 5126,
	};
}

namespace gl3d::resources
{
	unsigned int sizeComp(int type)
	{
		switch (type) {
			case (int)COMPONENT_TYPE::BYTE:
			case (int)COMPONENT_TYPE::UNSIGNED_BYTE: return 1;
			case (int)COMPONENT_TYPE::SHORT:
			case (int)COMPONENT_TYPE::UNSIGNED_SHORT: return 2;
			case (int)COMPONENT_TYPE::UNSIGNED_INT:
			case (int)COMPONENT_TYPE::FLOAT: return 4;
			default: return 0;
		}
	}

	unsigned int sizeType(int type)
	{
		switch (type) {
			case TINYGLTF_TYPE_SCALAR: return 1;
			case TINYGLTF_TYPE_VEC2: return 2;
			case TINYGLTF_TYPE_VEC3: return 3;
			case TINYGLTF_TYPE_VEC4: return 4;
			case TINYGLTF_TYPE_MAT2: return 9;
			case TINYGLTF_TYPE_MAT3: return 16;
			default: return 0;
		}
	}

	std::vector<unsigned char> readBuffer(tinygltf::Model model, int i, int offset, int size)
	{
		std::vector<unsigned char> buffer(model.buffers[i].data.begin() + offset,
		                                  model.buffers[i].data.begin() + offset + size);
		return buffer;
	}

	std::vector<unsigned char> readBufferView(tinygltf::Model model, int i, int offset)
	{
		auto& bufferView = model.bufferViews[i];
		return readBuffer(model, bufferView.buffer, bufferView.byteOffset + offset, bufferView.byteLength);
	}

	std::vector<unsigned char> readAccessor(tinygltf::Model model, int i)
	{
		auto& accessor = model.accessors[i];
		return readBufferView(model, accessor.bufferView, accessor.byteOffset);
	}

	std::vector<unsigned char> interleave(std::vector<unsigned char> v1, int s1, std::vector<unsigned char> v2, int s2)
	{
		std::vector<unsigned char> result;
		result.reserve(v1.size() + v2.size());

		for (std::size_t i = 0; i < v1.size() + v2.size(); i += s1 + s2) {
			for (int j = 0; j < s1; ++j) {
				result.push_back(v1[i / (s1 + s2) * s1 + j]);
			}
			for (int j = 0; j < s2; ++j) {
				result.push_back(v2[i / (s1 + s2) * s2 + j]);
			}
		}

		return result;
	}

	std::tuple<std::unique_ptr<gl::IDrawable>, gl3d::Material> readGltf(
		const platform::IFileReader& fileReader, const std::string& filename,
		gl::IDrawableCreator& drawableCreator)
	{
		auto tmp = utils::strip(filename, "/");
		std::string filetype = utils::strip(tmp.back(), ".").back();
		tmp.pop_back();
		std::string basedir = utils::join(tmp, "/");
		basedir.pop_back();

		std::string data;
		if (filetype == "gltf") {
			data = fileReader.openTextFile(filename, platform::IFile::AccessMode::READ)->readAll();
		} else if (filetype == "glb") {
			throw NotImplementedException();
		} else {
			throw std::invalid_argument("invalid filetype for gltf");
		}

		tinygltf::Model model;
		tinygltf::TinyGLTF loader;
		std::string err;
		std::string warn;
		bool ret = loader.LoadASCIIFromString(&model, &err, &warn, data.data(), data.length(), basedir);

		// TODO
		if (!warn.empty()) {
			printf("Warn: %s\n", warn.c_str());
		}
		if (!err.empty()) {
			printf("Err: %s\n", err.c_str());
		}
		if (!ret) {
			printf("Failed to parse glTF\n");
		}

		auto& mesh = model.meshes[0];

		auto positions = readAccessor(model, mesh.primitives[0].attributes["POSITION"]);
		auto normals = readAccessor(model, mesh.primitives[0].attributes["NORMAL"]);
		auto indices = readAccessor(model, mesh.primitives[0].indices);

		auto pa = model.accessors[mesh.primitives[0].attributes["POSITION"]];
		auto na = model.accessors[mesh.primitives[0].attributes["NORMAL"]];
		auto ia = model.accessors[mesh.primitives[0].indices];

		auto vertices = interleave(positions, sizeComp(pa.componentType) * sizeType(pa.type),
		                           normals, sizeComp(na.componentType) * sizeType(na.type));

		auto drawable = drawableCreator.createDrawable(vertices,
		                                               indices,
		                                               { sizeType(pa.type), sizeType(na.type) },
		                                               indices.size() / sizeComp(ia.componentType),
		                                               gl::VertexFormat::INTERLEAVED);

		auto& material = model.materials[mesh.primitives[0].material].pbrMetallicRoughness;
		auto pbrMaterial = gl3d::Material();
		pbrMaterial.addVec3("albedo", gml::Vec3f(material.baseColorFactor[0],
		                                          material.baseColorFactor[1],
		                                          material.baseColorFactor[2]));
		pbrMaterial.addFloat("metallic", (float)material.metallicFactor);
		pbrMaterial.addFloat("roughness", (float)material.roughnessFactor);
		pbrMaterial.addFloat("ao", 1);

		return std::tuple<std::unique_ptr<gl::IDrawable>, gl3d::Material>(std::move(drawable), pbrMaterial);
	}

#if 0
	std::tuple<std::unique_ptr<gl::IDrawable>, gl3d::Material> readGltf(
		const platform::IFileReader& fileReader, const std::string& filename,
		gl::IDrawableCreator&)
	{
		auto file = fileReader.openTextFile(filename, platform::IFile::AccessMode::READ);
		auto data = json::parse(file->readAll());

		std::vector<std::unique_ptr<platform::IBinaryFile>> buffers;
		for (auto bufferDef : data["buffers"]) {
			if (!utils::startsWith(bufferDef["uri"], "data:application/octet-stream")) {
				auto tmp = utils::strip(filename, "/");
				tmp.pop_back();
				std::string path = utils::join(tmp);
				buffers.push_back(fileReader.openBinaryFile(path + "/" + bufferDef["uri"].get<std::string>(),
															platform::IFile::AccessMode::READ));
			}
		}

		auto& meshPrimitive = data["meshes"][0]["primitives"][0];
//		int normalsAccessor = meshPrimitive["attributes"]["POSITION"].get<int>();
//		int indicesAccessor = meshPrimitive["indices"].get<int>();

		int positionsAccessor = meshPrimitive["attributes"]["POSITION"].get<int>();
		int positionsView = data["accessors"][positionsAccessor]["bufferView"];
		auto& bufferView = data["bufferViews"][positionsView];
		std::vector<gml::Vec3f> positions(bufferView["byteLength"].get<int>() / 12);
		buffers[bufferView["buffer"].get<int>()]->seek(bufferView["byteOffset"].get<int>(),
													   platform::IFile::SeekOffset::BEG);
		buffers[bufferView["buffer"].get<int>()]->read(&positions[0], bufferView["byteLength"].get<int>());

		return std::tuple<std::unique_ptr<gl::IDrawable>, gl3d::Material>(nullptr, gl3d::Material());
	}





	std::vector<float> readBufferViewFloat(
		nlohmann::json data, int i, int offset, int nElements,
		std::vector<std::unique_ptr<platform::IBinaryFile>> buffers)
	{
		std::vector<float> elements(nElements);

		int bufferIndex = data["bufferView"][i]["buffer"].get<int>();
		int byteOffset = offset + data["bufferView"][i]["byteOffset"].get<int>();

		buffers[bufferIndex]->seek(byteOffset, platform::IFile::SeekOffset::BEG);
		buffers[bufferIndex]->read(elements.data(), nElements * size(COMPONENT_TYPE::FLOAT));

		return elements;
	}

	void readMeshes(nlohmann::json data, std::vector<std::unique_ptr<platform::IBinaryFile>> buffers)
	{
		for (auto mesh : data["meshes"]) {
			auto primitive = mesh["primitives"][0];
			int positionsAccessor = primitive["attributes"]["POSITION"].get<int>();
			auto positions = readBufferViewFloat(data,
												 data["accessor"][positionsAccessor]["bufferView"],
												 data["accessor"][positionsAccessor]["byteOffset"],
												 size(data["accessor"][positionsAccessor]["type"].get<std::string>()) * size,
												 buffers);

			int normalsAccessor = primitive["attributes"]["NORMAL"].get<int>();
			auto normals = readBufferViewFloat(data,
											   data["accessor"][normalsAccessor]["bufferView"],
											   data["accessor"][normalsAccessor]["byteOffset"],
											   size(data["accessor"][normalsAccessor]["type"].get<std::string>()) * 4,
											   buffers);

			int indicesAccessor = primitive["indices"].get<int>();
			int componentType = data["accessor"][normalsAccessor]["componentType"].get<int>();

			std::vector<unsigned int> indices;
			switch (componentType)
			{
				case BYTE:
				case UNSIGNED_BYTE:
				case SHORT:
				case UNSIGNED_SHORT:
					auto rawIndices =
						readBufferViewInteger<std::uin>(data,
																  data["accessor"][normalsAccessor]["bufferView"],
																  data["accessor"][normalsAccessor]["byteOffset"],
																  size(
																	  data["accessor"][normalsAccessor]["type"].get<std::string>()));
				case UNSIGNED_INT:
			}




		}
	}

	std::vector<std::unique_ptr<platform::IBinaryFile>>
	readBuffers(nlohmann::json data, const std::string& basePath, const platform::IFileReader& fileReader)
	{
		std::vector<std::unique_ptr<platform::IBinaryFile>> buffers;
		for (auto buffer : data["buffers"]) {
			if (!utils::startsWith(buffer["uri"], "data:application/octet-stream")) {
				buffers.push_back(fileReader.openBinaryFile(basePath + "/" + buffer["uri"].get<std::string>(),
															platform::IFile::AccessMode::READ));
			}
		}
		return buffers;
	}
#endif
}
