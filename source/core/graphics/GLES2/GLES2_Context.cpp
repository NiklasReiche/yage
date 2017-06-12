#include "GLES2_Context.h"

namespace gles2
{
	GLES2_Context::GLES2_Context(platform::PlatformHandle* systemHandle)
		: systemHandle(systemHandle)
	{
		systemHandle->createContext(&width, &height);

		makeCurrent();
	}
	GLES2_Context::~GLES2_Context()
	{

	}

	void GLES2_Context::makeCurrent()
	{
		systemHandle->makeCurrent();
		
	}
	void GLES2_Context::swapBuffers()
	{
		systemHandle->swapBuffers();
	}
}