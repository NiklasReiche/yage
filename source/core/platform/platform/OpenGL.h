#pragma once

#ifdef _WIN32
#define APIENTRY __stdcall // See GLAD documentation -- prevents windows.h macros
#endif
#include <glad/glad.h>
#include <GLFW/glfw3.h>
