#pragma once
#include <string>
#include <list>
 
extern std::list<std::string*> errorList;
void NewError(std::string error);
std::string PollError();