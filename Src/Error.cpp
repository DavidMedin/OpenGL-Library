 
#include "Error.h"
std::list<std::string*> errorList;
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
