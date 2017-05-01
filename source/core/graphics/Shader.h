#pragma once

#include <map>

#include "Platform.h"
#include "GType.h"

namespace gl
{
	struct Shader
	{
		Guint id;
		std::map<std::string, Gint> uniformLocations;
	};
}