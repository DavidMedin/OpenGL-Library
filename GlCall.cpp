#pragma once
 
#include "GlCall.h"
using namespace std;
bool GLLogCall(const char* function, const char* file, int line) {
	while (GLenum error = glGetError()) {
		NewError("Gl Error: " + to_string(error) + function + file + to_string(line) + "\n");
		cout << "GL Error :" << error << ": " << function << ": " << file << ": " << line << "\n";
		return false;
	}
	return true;
}
void GLClearError() {
	while (GLenum error = glGetError()) {
		cout << error << '\n';
	}
}
