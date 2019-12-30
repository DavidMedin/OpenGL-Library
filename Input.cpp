#include "pch.h"
#include "Input.h"

namespace DataKeys {
	unsigned int A_KEY=0;
	unsigned int W_KEY=0;
	unsigned int D_KEY=0;
	unsigned int S_KEY=0;
	unsigned int Q_KEY=0;
	unsigned int E_KEY=0;
	unsigned int ESC_KEY = 0;
	unsigned int CTRL_KEY = 0;
	unsigned int SPACE_KEY = 0;
};
namespace DataMouseButton {
	unsigned int MIDDLE_KEY = 0;
	unsigned int LEFT_KEY = 0;
	unsigned int RIGHT_KEY = 0;
};



void Key(GLFWwindow* window, int key, int scancode, int action, int mods) {
	bool active = 0;
	if (action == GLFW_RELEASE) active = 0;
	else if (action == GLFW_PRESS || action == GLFW_REPEAT) active = 1; else active = 0;
	switch (key) {
		case GLFW_KEY_A: {
			DataKeys::A_KEY = active;
			break;
		}
		case GLFW_KEY_W: {
			DataKeys::W_KEY = active;
			break;
		}
		case GLFW_KEY_D: {
			DataKeys::D_KEY = active;
			break;
		}
		case GLFW_KEY_S: {
			DataKeys::S_KEY = active;
			break;
		}
		case GLFW_KEY_Q: {
			DataKeys::Q_KEY = active;
			break;
		}
		case GLFW_KEY_E: {
			DataKeys::E_KEY = active;
			break;
		}
		case GLFW_KEY_ESCAPE: {
			DataKeys::ESC_KEY = active;
			break;
		}
		case GLFW_KEY_LEFT_CONTROL: {
			DataKeys::CTRL_KEY = active;
			break;
		}
		case GLFW_KEY_SPACE: {
			DataKeys::SPACE_KEY = active;
			break;
		}
	}
}

float mouseX = 0;
float mouseY = 0;

void Mouse(GLFWwindow* window, double xpos, double ypos) {
	mouseX = (float)xpos;
	mouseY = (float)ypos;
}

void MouseButton(GLFWwindow* window, int button, int action, int mods) {
	bool active = 0;
	if (action == GLFW_RELEASE) active = 0;
	else if (action == GLFW_PRESS || action == GLFW_REPEAT) active = 1; else active = 0;
	switch (button) {
		case GLFW_MOUSE_BUTTON_MIDDLE: {
			DataMouseButton::MIDDLE_KEY = active;
			break;
		}
		case GLFW_MOUSE_BUTTON_LEFT: {
			DataMouseButton::LEFT_KEY = active;
		}
		case GLFW_MOUSE_BUTTON_RIGHT: {
			DataMouseButton::RIGHT_KEY = active;
		}
	}
}

GRAPHICSLIBRARY_API bool GetKey(keys key) {
	switch (key) {
		case keys::A_KEY: {
			return DataKeys::A_KEY;
		}
		case keys::W_KEY: {
			return DataKeys::W_KEY;
		}
		case keys::D_KEY: {
			return DataKeys::D_KEY;
		}
		case keys::S_KEY: {
			return DataKeys::S_KEY;
		}
		case keys::Q_KEY: {
			return DataKeys::Q_KEY;
		}case keys::E_KEY: {
			return DataKeys::E_KEY;
		}
		case keys::ESC_KEY: {
			return DataKeys::ESC_KEY;
		}
		case keys::CTRL_KEY: {
			return DataKeys::CTRL_KEY;
		}
		case keys::SPACE_KEY: {
			return DataKeys::SPACE_KEY;
		}
		default: {
			NewError("That key has not been implemented yet!\n");
			return 0;
		}
	}
}
GRAPHICSLIBRARY_API bool GetMouseButton(mouseButton button) {
	switch (button) {
		case mouseButton::MIDDLE_KEY: {
			return DataMouseButton::MIDDLE_KEY;
		}
		default: {
			NewError("That is not a Button");
			return NULL;
		}
	}
}

GRAPHICSLIBRARY_API void GetMousePos(float* xpos, float* ypos) {
	static float prev_x = mouseX;
	static float prev_y = mouseY;
	//printf("%f\n", prev_x);
	(*xpos) = mouseX - prev_x;
	(*ypos) = mouseY - prev_y;
	prev_x = mouseX;
	prev_y = mouseY;
}

GRAPHICSLIBRARY_API void SetDisabledMouse(bool disabled)
{
	if (disabled){
	glfwSetInputMode(glfwGetCurrentContext(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}
	else {
		glfwSetInputMode(glfwGetCurrentContext(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}
}
