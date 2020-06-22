#pragma once
#include <iostream>
#include "Error.h"
#include "Export.h"
#define ASSERT(x) if(!(x)) __debugbreak();
#define GLCall(x) GLClearError();\
	x;\
	ASSERT(GLLogCall(#x, __FILE__, __LINE__))
namespace Cedar {
	void GL_EXPORT GLClearError();
	bool GL_EXPORT GLLogCall(const char* function, const char* file, int line);
}

