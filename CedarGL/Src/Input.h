#pragma once
#include "Error.h"
#include "Export.h" 

namespace Cedar {
	//void GL_EXPORT Key(int key, int scancode, int action, int mods);
	//void GL_EXPORT Mouse(double xpos, double ypos);
	//void GL_EXPORT MouseButton(int button, int action, int mods);



	enum GL_EXPORT keys {
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
	enum GL_EXPORT mouseButton {
		MIDDLE_KEY,
		LEFT_KEY,
		RIGHT_KEY
	};
	bool GL_EXPORT GetKey(keys key);
	bool GL_EXPORT GetMouseButton(mouseButton button);
	void GL_EXPORT GetMousePos(float* xpos, float* ypos);
	void GL_EXPORT SetDisabledMouse(bool disabled);
}