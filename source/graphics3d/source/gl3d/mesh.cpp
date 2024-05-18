#include "mesh.h"

namespace gl3d
{
    SubMesh::SubMesh(std::shared_ptr<gl::IDrawable> m_drawable, const std::shared_ptr<Material>& m_material)
            : m_drawable(std::move(m_drawable)), m_material(m_material)
    {
    }

    gl::IDrawable& SubMesh::drawable() const
    {
        return *m_drawable;
    }

    Material& SubMesh::material() const
    {
        return *m_material;
    }

    void Mesh::add_sub_mesh(std::unique_ptr<SubMesh> sub_mesh)
    {
        m_sub_meshes.push_back(std::move(sub_mesh));
    }

    std::vector<std::unique_ptr<SubMesh>>& Mesh::sub_meshes()
    {
        return m_sub_meshes;
    }
}
