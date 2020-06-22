 
#include "Error.h"
namespace Cedar {
	std::list<std::string*> GL_EXPORT errorList;
	std::string PollError() {
		std::string error;
		if (!errorList.empty()) {
			error = *errorList.front();
			errorList.pop_front();
		}
		else error = "Empty";
		return error;
	}
	void NewError(std::string error) {
		std::string* errorpnt = new std::string(error);
		errorList.push_back(errorpnt);
	}
}