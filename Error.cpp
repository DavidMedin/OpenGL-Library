#include "pch.h"
#include "Error.h"
list<string*> errorList;
string PollError() {
	string error;
	if (!errorList.empty()) {
		error = *errorList.front();
		errorList.pop_front();
	}
	else error = "Empty";
	return error;
}
void NewError(string error) {
	string* errorpnt = new string(error);
	errorList.push_back(errorpnt);
}
