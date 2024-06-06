#pragma once

#include <memory>

#include "core/gl/IUniformBlock.h"
#include "gml/matrix.h"

class ProjectionView
{
public:
	gml::Mat4f projection;
	gml::Mat4f view;

	ProjectionView() = default;
	explicit ProjectionView(std::unique_ptr<gl::IUniformBlock>& uniformBlock);

	void syncUniforms();
	void syncProjection();
	void syncView();

private:
	std::unique_ptr<gl::IUniformBlock> uniformBlock;
};
