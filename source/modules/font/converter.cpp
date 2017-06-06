#include "converter.h"

namespace font
{
	/*****************************************
	**			FreeType Loader 			**
	*****************************************/

	FT_Loader::~FT_Loader()
	{
		for (unsigned int i = 0; i < faces.size(); ++i)
		{
			FT_Done_Face(faces[i]);
			faces.erase(faces.begin() + i);
		}
		FT_Done_FreeType(ft);
	}

	int FT_Loader::initialize()
	{
		if (FT_Init_FreeType(&ft)) {
			std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
			return 1;
		}
		return 0;
	}

	void FT_Loader::free(FT_Library ft)
	{
		FT_Done_FreeType(ft);
	}
	void FT_Loader::free(FT_Face face)
	{
		FT_Done_Face(face);
	}

	FT_Face FT_Loader::loadFace(std::string filepath)
	{
		FT_Face face;
		FT_Error error = FT_New_Face(ft, filepath.c_str(), 0, &face);
		if (error) {
			std::cout << "ERROR::FREETYPE: Failed to load font \n" << error << std::endl;
		}
		else {
			faces.push_back(face);
		}
		return face;
	}
	FT_GlyphSlot FT_Loader::loadGlyph(FT_Face face, unsigned char c, FT_GLYPH_LOAD_FLAG load_flag)
	{
		FT_Error error = FT_Load_Char(face, c, (FT_Int32)load_flag);
		if (error) {
			std::cout << "ERROR::FREETYTPE: Failed to load Glyph \n" << error << std::endl;
		}
		return face->glyph;
	}


	/*****************************************
	**			SDF Generator	 			**
	*****************************************/
	FontConverter::FontConverter(gl::GraphicsContext* glContext)
		: glContext(glContext) {}


	void FontConverter::generateTextureAtlas(FT_Face face, img::Image & image, int padding)
	{
		gml::Vector2D<int> size = calcTextureSize(face, padding);
		clampTo4(size);

		std::vector<unsigned char> emptyImage(size.x * size.y, 0);
		gl::Texture tempTexture = glContext->create2DTexture(&emptyImage[0], size.x, size.y, gl::ImageFormat::R, 1);

		gml::Vector2D<int> tex_offset(0, 0);
		for (unsigned char c = convertMetrics.c_min; c <= convertMetrics.c_max; ++c)
		{
			// Load character glyph 
			FT_GlyphSlot glyph = ft_loader.loadGlyph(face, c, FT_GLYPH_LOAD_FLAG::MONO);
			FT_Bitmap & bitmap = glyph->bitmap;

			// Extract Bitmap Metrics
			if (bitmap.buffer != nullptr) {
				std::vector<unsigned char> image;
				img::unpackBitmap(bitmap.buffer, image, bitmap.rows, bitmap.width, bitmap.pitch);
				img::flip(image, bitmap.rows, bitmap.width, 1);
				tempTexture.bufferSubData(tex_offset.x + padding, tempTexture.getHeight() - (bitmap.rows + padding), bitmap.width, bitmap.rows, image);
			}
			// Advance texture offset
			tex_offset.x += bitmap.width + (padding * 2);
		}

		tempTexture.getTextureImage(image.data);
		image.channels = 1;
		image.width = size.x;
		image.height = size.y;
	}

	void FontConverter::uploadGlyphBitmaps(FT_Face face, gl::Texture & texture, int padding)
	{
		gml::Vector2D<int> tex_offset(0, 0);
		for (unsigned char c = convertMetrics.c_min; c <= convertMetrics.c_max; ++c)
		{
			// Load character glyph 
			FT_GlyphSlot glyph = ft_loader.loadGlyph(face, c, FT_GLYPH_LOAD_FLAG::MONO);
			FT_Bitmap & bitmap = glyph->bitmap;

			// Extract Bitmap Metrics
			if (bitmap.buffer != nullptr) {
				std::vector<unsigned char> image;
				img::unpackBitmap(bitmap.buffer, image, bitmap.rows, bitmap.width, bitmap.pitch);
				img::flip(image, bitmap.rows, bitmap.width, 1);
				texture.bufferSubData(tex_offset.x + padding, texture.getHeight() - (bitmap.rows + padding), bitmap.width, bitmap.rows, image);
			}
			// Advance texture offset
			tex_offset.x += bitmap.width + (padding * 2);
		}
	}
	void FontConverter::generateSdfTexMetrics(FT_Face face, std::map<unsigned char, TexMetrics> & metrics, int padding)
	{
		gml::Vector2D<int> tex_offset(0, 0);
		for (unsigned char c = convertMetrics.c_min; c <= convertMetrics.c_max; ++c)
		{
			metrics[c] = TexMetrics();
			TexMetrics & coords = metrics[c];

			// load absolute positions
			loadTextureMetrics(face, c, coords, tex_offset, padding);

			// advance texture offset
			tex_offset.x += face->glyph->bitmap.width + (padding * 2);
		}
	}

	void FontConverter::generateSDF(FT_Face face, img::Image & input, img::Image & output, int loadSize, int loadPadding, int resizeFactor)
	{
		// generate glyph texture metrics
		std::map<unsigned char, TexMetrics> metrics;
		generateSdfTexMetrics(face, metrics, loadPadding);

		// Generate SDF Image
		genDistanceFieldPerGlyph(input, metrics, output);

		// Convert SDF-Image to texture and downscale through mipmaps
		gl::Texture sdf_texture = glContext->create2DTexture(&output.data[0], input.width, input.height, gl::ImageFormat::R, 1);
		sdf_texture.configTextureWrapper(gl::TextureWrapper::CLAMP_TO_BORDER, gl::TextureWrapper::CLAMP_TO_BORDER);
		sdf_texture.configTextureFilter(gl::MipmapOption::LINEAR_LINEAR, gl::TextureFilter::NEAREST);

		// resize
		output.width = input.width / resizeFactor;
		output.height = input.height / resizeFactor;
		output.channels = 1;

		sdf_texture.getTextureImage(output.data, (int)std::log2(resizeFactor));
	}

	/*****************************************
	**			Font  Converter 			**
	*****************************************/

	void FontConverter::loadGlyphMetrics(FT_Face face, unsigned char c, GlyphMetrics & metrics)
	{
		FT_GlyphSlot glyph = ft_loader.loadGlyph(face, c, FT_GLYPH_LOAD_FLAG::UNSCALED);
		metrics.size.x = (float)glyph->metrics.width;
		metrics.size.y = (float)glyph->metrics.height;
		metrics.bearing.x = (float)glyph->metrics.horiBearingX;
		metrics.bearing.y = (float)glyph->metrics.horiBearingY;
		metrics.advance = (float)glyph->metrics.horiAdvance;
	}
	void FontConverter::loadTextureMetrics(FT_Face face, unsigned char c, TexMetrics & metrics, gml::Vector2D<int> offset, int padding)
	{
		FT_GlyphSlot glyph = ft_loader.loadGlyph(face, c, FT_GLYPH_LOAD_FLAG::MONO);
		metrics.left = (float)offset.x;
		metrics.right = (float)(offset.x + (int)glyph->bitmap.width + (padding * 2));
		metrics.top = (float)offset.y;
		metrics.bottom = (float)(offset.y + (int)glyph->bitmap.rows + (padding * 2));
	}

	gml::Vector2D<int> FontConverter::calcTextureSize(FT_Face face, int padding)
	{
		gml::Vector2D<int> size;
		for (unsigned char c = convertMetrics.c_min; c <= convertMetrics.c_max; ++c)
		{
			FT_GlyphSlot glyph = ft_loader.loadGlyph(face, c, FT_GLYPH_LOAD_FLAG::MONO);
			int width = (int)glyph->bitmap.width + padding * 2;
			int height = (int)glyph->bitmap.rows + padding * 2;
			size.x += width;
			size.y = std::max(size.y, height);
		}
		return size;
	}
	void FontConverter::clampTo4(gml::Vector2D<int> & size)
	{
		while (size.x % 4 != 0)
		{
			++size.x;
		}
		while (size.y % 4 != 0)
		{
			++size.y;
		}
	}
	
	void FontConverter::generateCharacters(charactermap & characters)
	{
		for (unsigned char c = convertMetrics.c_min; c <= convertMetrics.c_max; ++c)
		{
			characters[c] = Character();
		}
	}

	void FontConverter::generateGlyphMetrics(FT_Face face, charactermap & characters)
	{
		for (unsigned char c = convertMetrics.c_min; c <= convertMetrics.c_max; ++c)
		{
			Character & character = characters[c];

			// Load Metrics into Character
			loadGlyphMetrics(face, c, character.glyph);
		}
	}
	void FontConverter::generateTexMetrics(FT_Face face, charactermap & characters, int padding)
	{
		gml::Vector2D<int> total_size = calcTextureSize(face, padding);
		clampTo4(total_size);

		TexMetrics tempTexMetrics;
		gml::Vector2D<int> tex_offset(0, 0);
		for (unsigned char c = convertMetrics.c_min; c <= convertMetrics.c_max; ++c)
		{
			Character & character = characters[c];

			// load absolute positions
			loadTextureMetrics(face, c, tempTexMetrics, tex_offset, padding);

			// convert to relative positions
			character.texCoords.left = tempTexMetrics.left / (float)total_size.x;
			character.texCoords.right = tempTexMetrics.right / (float)total_size.x;
			character.texCoords.top	= 1 - tempTexMetrics.top / (float)total_size.y;
			character.texCoords.bottom = 1 - tempTexMetrics.bottom / (float)total_size.y;

			// advance texture offset
			tex_offset.x += face->glyph->bitmap.width + (padding * 2);
		}
	}
	void FontConverter::generateMaxGlyph(const charactermap & characters, GlyphMetrics & maxGlyph)
	{
		for (unsigned char c = convertMetrics.c_min; c <= convertMetrics.c_max; ++c)
		{
			const Character & character = characters.at(c);

			// Update MaxGlyphMetrics
			maxGlyph.size.x = std::max(maxGlyph.size.x, character.glyph.size.x);
			maxGlyph.size.y = std::max(maxGlyph.size.y, character.glyph.size.y);
			maxGlyph.bearing.x = std::max(maxGlyph.bearing.x, character.glyph.bearing.x);
			maxGlyph.bearing.y = std::max(maxGlyph.bearing.y, character.glyph.bearing.y);
			maxGlyph.advance = std::max(maxGlyph.advance, character.glyph.advance);
		}

	}

	void FontConverter::writeFontfile(std::string filename)
	{
		Fontfile fontfile;

		fontfile.fontinfo.encoding = 1;
		fontfile.fontinfo.EM_size = convertMetrics.EM_size;
		fontfile.fontinfo.nChars = convertMetrics.c_max - convertMetrics.c_min + 1;

		fontfile.sdfinfo.width = (uint32_t)convertMetrics.sdfImage.width;
		fontfile.sdfinfo.height = (uint32_t)convertMetrics.sdfImage.height;
		fontfile.sdfinfo.channels = (uint32_t)convertMetrics.sdfImage.channels;

		fontfile.maxglyph.width = (uint32_t)convertMetrics.maxGlyph.size.x;
		fontfile.maxglyph.height = (uint32_t)convertMetrics.maxGlyph.size.y;
		fontfile.maxglyph.x_bearing = (int32_t)convertMetrics.maxGlyph.bearing.x;
		fontfile.maxglyph.y_bearing = (int32_t)convertMetrics.maxGlyph.bearing.y;
		fontfile.maxglyph.advance = (uint32_t)convertMetrics.maxGlyph.advance;

		for (int i = convertMetrics.c_min; i <= convertMetrics.c_max; ++i)
		{
			GLYPH glyph;
			glyph.id = i;

			glyph.width = (uint32_t)convertMetrics.characters[i].glyph.size.x;
			glyph.height = (uint32_t)convertMetrics.characters[i].glyph.size.y;
			glyph.x_bearing = (int32_t)convertMetrics.characters[i].glyph.bearing.x;
			glyph.y_bearing = (int32_t)convertMetrics.characters[i].glyph.bearing.y;
			glyph.advance = (uint32_t)convertMetrics.characters[i].glyph.advance;

			glyph.texCoord_left = convertMetrics.characters[i].texCoords.left;
			glyph.texCoord_right = convertMetrics.characters[i].texCoords.right;
			glyph.texCoord_top = convertMetrics.characters[i].texCoords.top;
			glyph.texCoord_bottom = convertMetrics.characters[i].texCoords.bottom;

			fontfile.glyphs.push_back(glyph);
		}

		fontfile.sdf = std::move(convertMetrics.sdfImage.data);

		fontfile.fileheader.identifier = 0x464f4e54;
		fontfile.fileheader.fsize = sizeof(fontfile);
		fontfile.fileheader.glyphOffset = sizeof(FILEHEADER) + sizeof(FONTINFO) + sizeof(SDFINFO) + sizeof(MAXGLYPH);
		fontfile.fileheader.imgOffset = sizeof(FILEHEADER) + sizeof(FONTINFO) + sizeof(SDFINFO) + sizeof(MAXGLYPH) + sizeof(GLYPH) * fontfile.glyphs.size();


		std::ofstream file(filename, std::ios::binary);
		file.seekp(0, std::ios::beg);
		file.write((char*)&fontfile.fileheader, sizeof(fontfile.fileheader));
		file.write((char*)&fontfile.fontinfo, sizeof(fontfile.fontinfo));
		file.write((char*)&fontfile.sdfinfo, sizeof(fontfile.sdfinfo));
		file.write((char*)&fontfile.maxglyph, sizeof(fontfile.maxglyph));
		file.write((char*)&fontfile.glyphs[0], sizeof(GLYPH) * fontfile.glyphs.size());
		file.write((char*)&fontfile.sdf[0], sizeof(uint8_t) * fontfile.sdf.size());
	}

	void FontConverter::convertFont(std::string filename_TTF, std::string filename_FONT, ENCODING encoding)
	{
		if (encoding == ENCODING::ASCII) {
			convertMetrics.c_min = 32;
			convertMetrics.c_max = 126;
		}

		//convertMetrics.c_min = 65;
		//convertMetrics.c_max = 66;
		int padding = 16;
		int loadSize = 256;
		int resizeFactor = 4;

		ft_loader.initialize();
		FT_Face face = ft_loader.loadFace(filename_TTF);

		generateCharacters(convertMetrics.characters);

		convertMetrics.EM_size = face->units_per_EM;
		generateGlyphMetrics(face, convertMetrics.characters);
		generateMaxGlyph(convertMetrics.characters, convertMetrics.maxGlyph);

		FT_Set_Pixel_Sizes(face, 0, loadSize);
		generateTexMetrics(face, convertMetrics.characters, padding);

		generateTextureAtlas(face, convertMetrics.glyphImage, padding);
		generateSDF(face, convertMetrics.glyphImage, convertMetrics.sdfImage, loadSize, padding, resizeFactor);

		writeFontfile(filename_FONT);
	}
}