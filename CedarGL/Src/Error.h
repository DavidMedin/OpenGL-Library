#pragma once
#include <string>
#include <list>
#include "Export.h"
namespace Cedar {
	extern std::list<std::string*> GL_EXPORT errorList;
	void GL_EXPORT NewError(std::string error);
	std::string GL_EXPORT PollError();
}