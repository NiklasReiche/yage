#pragma once

#include <memory>

#include "core/gl/IUniformBlock.h"
#include "math/matrix.h"

using namespace yage;

class ProjectionView
{
public:
	math::Mat4f projection;
	math::Mat4f view;

	ProjectionView() = default;
	explicit ProjectionView(std::unique_ptr<gl::IUniformBlock>& uniformBlock);

	void syncUniforms();
	void syncProjection();
	void syncView();

private:
	std::unique_ptr<gl::IUniformBlock> uniformBlock;
};
