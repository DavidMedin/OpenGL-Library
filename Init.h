#pragma once
#include <stdio.h>
#include <string>
#include "Error.h"
#include "Input.h"
#include <glfw3.h>
 
#include <glm.hpp>
#include <ext.hpp>
#include <glew.h>
#define GLFW_DLL
#include "GlCall.h"
#include "Camera.h"
#include "Imgui/imgui.h"
#include "examples/imgui_impl_glfw.h"
#include "examples/imgui_impl_opengl3.h"
void WindowSizeCallback(GLFWwindow* window, int width, int height);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
using namespace std;
 

#define GRAPHICS_FLAG_CULL 1
void  SetGraphicsFlag(int flag);
bool  GetGraphicsFlag(int flag);

int init(int width, int height, string name);
void PollEvents();
void ClearWindow();
void DrawWindow();
int ShouldCloseWindow();
float GetDeltaTime();

void ImGuiNewFrame();
void ImGuiRender();
