#pragma once
#include "Error.h"
 
#include <GL/glew.h>
#include <GLFW/glfw3.h>
void Key(GLFWwindow* window,int key, int scancode, int action, int mods);
void Mouse(GLFWwindow* window, double xpos, double ypos);
void MouseButton(GLFWwindow* window,int button, int action, int mods);
 

 enum keys {
	A_KEY,
	W_KEY,
	D_KEY,
	S_KEY,
	Q_KEY,
	E_KEY,
	ESC_KEY,
	CTRL_KEY,
	SPACE_KEY
};
 enum mouseButton {
	 MIDDLE_KEY,
	 LEFT_KEY,
	 RIGHT_KEY
};
bool GetKey(keys key);
bool GetMouseButton(mouseButton button);
void GetMousePos(float* xpos, float* ypos);
void SetDisabledMouse(bool disabled);