#pragma once
#include <stdio.h>
#include <string>
#include "Error.h"
#include "Input.h"
#ifdef GRAPHICSLIBRARY_EXPORTS
#define GRAPHICSLIBRARY_API __declspec(dllexport)
#include <glm.hpp>
#include <ext.hpp>
#include <glew.h>
#define GLFW_DLL
#include <glfw3.h>
#include "GlCall.h"
#include "Camera.h"
void WindowSizeCallback(GLFWwindow* window, int width, int height);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
using namespace std;
#else
#define GRAPHICSLIBRARY_API __declspec(dllimport)
#endif

#define GRAPHICS_FLAG_CULL 1
void GRAPHICSLIBRARY_API SetGraphicsFlag(int flag);
bool GRAPHICSLIBRARY_API GetGraphicsFlag(int flag);

int GRAPHICSLIBRARY_API init(int width, int height, string name);
void GRAPHICSLIBRARY_API PollEvents();
void GRAPHICSLIBRARY_API ClearWindow();
void GRAPHICSLIBRARY_API DrawWindow();
int GRAPHICSLIBRARY_API ShouldCloseWindow();
float GRAPHICSLIBRARY_API GetDeltaTime();
//list<GLFWwindow*>* GetWindowList();