#pragma once
#include <string>
#include <list>
#ifdef GRAPHICSLIBRARY_EXPORTS
#define GRAPHICSLIBRARY_API __declspec(dllexport)
#else
#define GRAPHICSLIBRARY_API __declspec(dllimport)
#endif
using namespace std;
extern list<string*> errorList;
void NewError(string error);
string GRAPHICSLIBRARY_API PollError();