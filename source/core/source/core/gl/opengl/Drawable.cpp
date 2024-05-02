#include "Drawable.h"
#include "Context.h"

namespace opengl
{
	Drawable::Drawable(Drawable&& other) noexcept
	{
		*this = std::move(other);
	}

	Drawable& Drawable::operator=(Drawable&& other) noexcept
	{
		if (this != &other)
		{
			vertexArray = std::move(other.vertexArray);
			nVertices = other.nVertices;

			other.nVertices = 0;
		}
		return *this;
	}

    void Drawable::setSubData(unsigned int offset, const std::vector<float>& vertices)
    {
        vertexArray->setSubData(offset, vertices);
    }
}
