#pragma once
#include <stdio.h>
#include <string>
#include "Error.h"
#include "Input.h"
#include "Shader.h"

#include <GLFW/glfw3.h>
 
#include <glm.hpp>
#include <ext.hpp>
#include <GL/glew.h>
#define GLFW_DLL
#include "GlCall.h"
#include "Camera.h"
#include "imgui.h"
#include "examples/imgui_impl_glfw.h"
#include "examples/imgui_impl_opengl3.h"
void WindowSizeCallback(GLFWwindow* window, int width, int height);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
using namespace std;
 

#define GRAPHICS_FLAG_CULL 1

#define Z_TEST 1

void  SetGraphicsFlag(int flag);
bool  GetGraphicsFlag(int flag);
void GraphicsEnable(unsigned int what);
void GraphicsDisable(unsigned int what);

int init(int width, int height, string name);
void PollEvents();
void ClearWindow();
void DrawWindow();
int ShouldCloseWindow();
float GetDeltaTime();

void ImGuiNewFrame();
void ImGuiRender();

//[0] = LineShader
Shader* GetShaders();