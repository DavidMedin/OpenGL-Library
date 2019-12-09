#pragma once
#include "Error.h"
#ifdef GRAPHICSLIBRARY_EXPORTS
#define GRAPHICSLIBRARY_API __declspec(dllexport)
#include <glew.h>
#include <glfw3.h>
void Key(GLFWwindow* window,int key, int scancode, int action, int mods);
void Mouse(GLFWwindow* window, double xpos, double ypos);
void MouseButton(GLFWwindow* window,int button, int action, int mods);
#else
#define GRAPHICSLIBRARY_API __declspec(dllimport)
#endif

 enum keys {
	A_KEY,
	W_KEY,
	D_KEY,
	S_KEY,
	Q_KEY,
	E_KEY
};
 enum mouseButton {
	 MIDDLE_KEY,
	 LEFT_KEY,
	 RIGHT_KEY
};
GRAPHICSLIBRARY_API bool GetKey(keys key);
GRAPHICSLIBRARY_API bool GetMouseButton(mouseButton button);
GRAPHICSLIBRARY_API void GetMousePos(float* xpos, float* ypos);