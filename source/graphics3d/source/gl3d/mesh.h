#pragma once

#include <memory>

#include <core/gl/Drawable.h>

#include "material.h"

namespace gl3d
{
    class SubMesh
    {
    public:
        SubMesh(std::shared_ptr<gl::IDrawable> m_drawable, const std::shared_ptr<Material>& m_material);

        [[nodiscard]]
        gl::IDrawable& drawable() const;

        [[nodiscard]]
        Material& material() const;

    private:
        // model as shared_ptr since we might want to combine a drawable with different materials
        std::shared_ptr<gl::IDrawable> m_drawable;
        // model as shared_ptr since we might want to combine a material with different drawables
        std::shared_ptr<Material> m_material;
    };

	class Mesh
	{
	public:
        void add_sub_mesh(std::unique_ptr<SubMesh> sub_mesh);

        [[nodiscard]]
        std::vector<std::unique_ptr<SubMesh>>& sub_meshes();

	private:
		std::vector<std::unique_ptr<SubMesh>> m_sub_meshes;
	};
}