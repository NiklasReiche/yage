#include "ProjectionView.h"

namespace yage::gl3d
{
	ProjectionView::ProjectionView(std::unique_ptr<gl::IUniformBlock> uniformBlock)
	{
		m_uniform_block = std::move(uniformBlock);
		m_uniform_block->setData(nullptr, 2 * 16 * 4);
		projection = math::matrix::Id4f;
		view = math::matrix::Id4f;
	}

	void ProjectionView::sync()
	{
		sync_projection();
		sync_view();
	}

	void ProjectionView::sync_projection()
	{
		// transpose to convert to column-major format
		m_uniform_block->setSubData(0, transpose(projection).data(), 16 * 4);
	}

	void ProjectionView::sync_view()
	{
		// transpose to convert to column-major format
		m_uniform_block->setSubData(16 * 4, transpose(view).data(), 16 * 4);
	}

	const gl::IUniformBlock& ProjectionView::ubo() const
	{
		return *m_uniform_block;
	}
}
