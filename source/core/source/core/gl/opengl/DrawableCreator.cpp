#include "DrawableCreator.h"

#include "Context.h"
#include "Drawable.h"
#include "OpenGL.h"
#include <numeric>

namespace opengl
{
	std::unique_ptr<gl::IDrawable> DrawableCreator::createDrawable(
		const std::span<const float>& vertices,
		const std::vector<unsigned int>& vertexLayout,
		gl::VertexFormat format)
	{
		// check vertex layout
		const unsigned int vertexSize = std::accumulate(vertexLayout.begin(), vertexLayout.end(), 0u);
		if (vertexSize == 0)
			throw std::invalid_argument("Invalid vertex layout");
		
		auto context = lockContextPtr();
		auto drawable = std::unique_ptr<Drawable>(new Drawable(context));
		
		// generate gpu objects
		glGenBuffers(1, &drawable->VBO);
		glGenVertexArrays(1, &drawable->VAO);
		
		// bind objects
		context->bindVertexArray(drawable->VAO);
		context->bindBuffer(GL_ARRAY_BUFFER, drawable->VBO);

		// upload data to gpu
		glBufferData(GL_ARRAY_BUFFER,
			vertices.size() * sizeof(GLfloat),
			vertices.empty() ? nullptr : vertices.data(),
			static_cast<GLenum>(drawable->usage));

		// configure vertex attributes
		const unsigned int nVertices = vertices.size() / vertexSize;

		switch(format)
		{
			case gl::VertexFormat::INTERLEAVED:
			{
				unsigned int attributeOffset = 0;
				for (unsigned int i = 0; i < vertexLayout.size(); i++)
				{
					glEnableVertexAttribArray(i);
					glVertexAttribPointer(
						i, vertexLayout[i],
						GL_FLOAT, GL_FALSE,
						vertexSize * sizeof(GLfloat),
						reinterpret_cast<GLvoid*>(attributeOffset * sizeof(GLfloat)));
					attributeOffset += vertexLayout[i];
				}
				break;
			}
			case gl::VertexFormat::BATCHED:
			{
				unsigned int attributeOffset = 0;
				for (unsigned int i = 0; i < vertexLayout.size(); i++)
				{
					glEnableVertexAttribArray(i);
					glVertexAttribPointer(
						i, vertexLayout[i],
						GL_FLOAT, GL_FALSE,
						vertexLayout[i] * sizeof(GLfloat),
						reinterpret_cast<GLvoid*>(attributeOffset * sizeof(GLfloat)));
					attributeOffset += vertexLayout[i] * nVertices;
				}
				break;
			}
			default:
				throw std::invalid_argument("Unsupported vertex format");
		}

		// construct cpu object
		drawable->id = drawable->VAO;
		//drawable->primitive = PrimitiveType::TRIANGLES;
		//drawable->usage = usage;
		drawable->nVertices = nVertices;
		drawable->layout = vertexLayout;
		drawable->vertexSize = vertexSize;
		drawable->format = format;
		drawable->bufferSize = nVertices * vertexSize;
		drawable->empty = vertices.empty();
		return drawable;
	}
}
