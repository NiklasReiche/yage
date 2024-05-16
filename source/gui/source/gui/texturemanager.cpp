#include "texturemanager.h"

namespace gui
{
    TextureAtlasStore::TextureAtlasStore(const std::shared_ptr<platform::IWindow>& platform,
                                         const std::shared_ptr<gl::ITextureCreator>& textureCreator)
            : platform(platform), textureCreator(textureCreator)
    {
        // create
        textures[0].texture = textureCreator->createTexture2D(TEXTURE_SIZE, TEXTURE_SIZE, gl::ImageFormat::RGBA, {});
        auto& textureAtlas = textures[0];

        // Reserved space for transparent texture
        textureAtlas.texture->setSubImage({0, 0, 1, 1}, std::vector<unsigned char>(4, 0));
        textureAtlas.cursor.x() = 1;
        textureAtlas.cursor.y() = 0;
        textureAtlas.y_next_offset = 1;
    }

    TextureAtlasView TextureAtlasStore::add(const img::Image& image)
    {
        unsigned int atlas_index = textures.size() - 1;
        auto* atlas = &textures.at(atlas_index);

        if (atlas->cursor.x() + image.getWidth() > atlas->texture->getWidth()) {
            atlas->cursor.x() = 0;
            atlas->cursor.y() += atlas->y_next_offset;
            atlas->y_next_offset = 0;
        }

        if (atlas->cursor.y() + image.getHeight() > atlas->texture->getHeight()) {
            atlas_index++;
            textures[atlas_index].texture = textureCreator
                    ->createTexture2D(TEXTURE_SIZE, TEXTURE_SIZE, gl::ImageFormat::RGBA, {});
            atlas = &textures.at(atlas_index);
        }

        gml::Vec2f tex_coords_start(
                (0.5f + static_cast<float>(atlas->cursor.x())) / static_cast<float>(atlas->texture->getWidth()),
                (0.5f + static_cast<float>(atlas->cursor.y())) / static_cast<float>(atlas->texture->getHeight()));

        atlas->texture
             ->setSubImage({atlas->cursor.x(), atlas->cursor.y(), image.getWidth(), image.getHeight()},
                           {image.data(), image.getSize()});
        atlas->cursor.x() += image.getWidth();
        atlas->y_next_offset = std::max(atlas->y_next_offset, image.getHeight());

        gml::Vec2f tex_coords_end(
                (0.5f + static_cast<float>(atlas->cursor.x())) / static_cast<float>(atlas->texture->getWidth()),
                (0.5f + static_cast<float>(atlas->cursor.y() + image.getHeight())) /
                static_cast<float>(atlas->texture->getHeight()));

        return {this, 0, tex_coords_start, tex_coords_end};
    }

    TextureAtlasView TextureAtlasStore::add(gl::ITexture2D& image)
    {
        auto data = image.getImage();
        return add(img::Image(image.getWidth(), image.getHeight(), image.getChannels(), data));
    }

    TextureAtlasView TextureAtlasStore::transparent()
    {
        // the first pixel of the first atlas is reserved for transparency
        auto coordinates = gml::Vec2f(0.5f / static_cast<float>(TEXTURE_SIZE), 0.5f / static_cast<float>(TEXTURE_SIZE));
        return {this, 0, coordinates, coordinates};
    }

    gl::ITexture2D& TextureAtlasStore::at(unsigned int i)
    {
        return *textures.at(i).texture;
    }

    TextureAtlasView::TextureAtlasView(TextureAtlasStore* store, unsigned int index,
                                       const gml::Vec2f& coordinates_start, const gml::Vec2f& coordinates_end)
            : m_store(store), m_index(index), m_coordinates_start(coordinates_start),
              m_coordinates_end(coordinates_end)
    {
    }

    gl::ITexture2D& TextureAtlasView::get()
    {
        return m_store->at(m_index);
    }

    gml::Vec2f TextureAtlasView::coordinates_start()
    {
        return m_coordinates_start;
    }

    gml::Vec2f TextureAtlasView::coordinates_end()
    {
        return m_coordinates_end;
    }
}