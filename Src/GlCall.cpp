#pragma once
 
#include "GlCall.h"
bool GLLogCall(const char* function, const char* file, int line) {
	while (GLenum error = glGetError()) {
		NewError("Gl Error: " + std::to_string(error) + function + file + std::to_string(line) + "\n");
		std::cout << "GL Error :" << error << ": " << function << ": " << file << ": " << line << "\n";
		return false;
	}
	return true;
}

void GLClearError() {
	while (GLenum error = glGetError()) {
		std::cout << error << '\n';
	}
}
