#pragma once
#include <string>
#include <list>
 
using namespace std;
extern list<string*> errorList;
void NewError(string error);
string PollError();