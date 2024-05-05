#pragma once

#include <map>
#include <vector>
#include "core.h"
#include "gml/gml.h"
#include "image/image.h"

namespace gui
{
    class TextureAtlasStore;

    class TextureAtlasView
    {
    public:
        TextureAtlasView(TextureAtlasStore* m_store, unsigned int m_index, const gml::Vec2f& m_coordinates_start,
                         const gml::Vec2f& m_coordinates_end);

        gl::ITexture2D& get();

        gml::Vec2f coordinates_start();

        gml::Vec2f coordinates_end();

    private:
        TextureAtlasStore* m_store;
        unsigned int m_index;
        gml::Vec2f m_coordinates_start;
        gml::Vec2f m_coordinates_end;
    };

    struct TextureAtlas
    {
        std::unique_ptr<gl::ITexture2D> texture;
        gml::Vec2f cursor;
        int y_nextOffset = 0;
    };

    /**
     * Stores the textures used by widgets, packed into atlases.
     *
     * Textures are packed into an atlas of fixed size. If an atlas is full, a new one is opened and added to an array
     * of atlases.
     */
    class TextureAtlasStore
    {
    private:
        std::shared_ptr<platform::IWindow> platform;
        std::shared_ptr<gl::ITextureCreator> textureCreator;

        std::map<unsigned int, TextureAtlas> textures;
        const int TEXTURE_SIZE = 2048;

    public:
        TextureAtlasStore(const std::shared_ptr<platform::IWindow>& platform,
                          const std::shared_ptr<gl::ITextureCreator>& textureCreator);

        TextureAtlasView add(const img::Image& image);

        TextureAtlasView add(gl::ITexture2D& image);

        TextureAtlasView transparent();

        gl::ITexture2D& at(unsigned int i);
    };
}