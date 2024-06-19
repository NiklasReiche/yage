#include "ProjectionView.h"

ProjectionView::ProjectionView(std::unique_ptr<gl::IUniformBlock>& uniformBlock)
{
    this->uniformBlock = std::move(uniformBlock);
    this->uniformBlock->setData(nullptr, 2 * 16 * 4);
    projection = math::matrix::Id4f;
    view = math::matrix::Id4f;
}

void ProjectionView::syncUniforms()
{
    syncProjection();
    syncView();
}

void ProjectionView::syncProjection()
{
    // transpose to convert to column-major format
    uniformBlock->setSubData(0, math::transpose(projection).data(), 16 * 4);
}

void ProjectionView::syncView()
{
    // transpose to convert to column-major format
    uniformBlock->setSubData(16 * 4, math::transpose(view).data(), 16 * 4);
}
