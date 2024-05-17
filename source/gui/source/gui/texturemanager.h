#pragma once

#include <map>
#include <vector>

#include <core/platform/Window.h>
#include <gml/gml.h>
#include <image/image.h>

namespace gui
{
    class TextureAtlasStore;

    /**
     * Encodes texture coordinates and a texture target for an atlas within an atlas store.
     */
    class TextureAtlasView
    {
    public:
        TextureAtlasView(TextureAtlasStore* store, unsigned int index, const gml::Vec2f& coordinates_start,
                         const gml::Vec2f& coordinates_end);

        /**
         * @return Texture that this view references.
         */
        gl::ITexture2D& get();

        /**
         * @return Texture coordinates of the upper left corner.
         */
        gml::Vec2f coordinates_start();

        /**
         * @return Texture coordinates of the bottom right corner.
         */
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
        /**
         * Encodes the position at which the next image should be inserted.
         */
        gml::Vec2i cursor;
        /**
         * Keeps track of the height of the current line of images in the atlas.
         */
        int y_next_offset = 0;
    };

    /**
     * Stores the textures used by widgets, packed into atlases.
     *
     * Textures are packed into an atlas of fixed size. If an atlas is full, a new one is opened and added to an array
     * of atlases.
     */
    class TextureAtlasStore
    {
    public:
        TextureAtlasStore(const std::shared_ptr<platform::IWindow>& platform,
                          const std::shared_ptr<gl::ITextureCreator>& textureCreator);

        TextureAtlasView add(const img::Image& image);

        TextureAtlasView add(gl::ITexture2D& image);

        /**
         * @return A view into a fully transparent texture to use for non-textured elements.
         */
        TextureAtlasView transparent();

        /**
         * @return The texture atlas at the given index.
         */
        gl::ITexture2D& at(unsigned int i);

    private:
        const int TEXTURE_SIZE = 2048;

        std::shared_ptr<platform::IWindow> platform;
        std::shared_ptr<gl::ITextureCreator> textureCreator;

        std::map<unsigned int, TextureAtlas> textures;
    };
}