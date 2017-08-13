#pragma once

#include "skybox.h"
#include "camera.h"

namespace graphics3d
{
	struct Scene
	{
		Skybox skybox;
		Camera camera;
	};
}