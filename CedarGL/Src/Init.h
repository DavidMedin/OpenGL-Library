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
 
#include "Export.h"
#define GRAPHICS_FLAG_CULL 1

#define Z_TEST 1
namespace Cedar {
    //class ImplWindow;
    //void WindowSizeCallback(ImplWindow* window, int width, int height);
    void GL_EXPORT FramebufferSizeCallback(int width, int height);
    //void  SetGraphicsFlag(int flag);
    //bool  GetGraphicsFlag(int flag);
    void GL_EXPORT GraphicsEnable(unsigned int what);
    void GL_EXPORT GraphicsDisable(unsigned int what);

    int GL_EXPORT init(int width, int height, std::string name);
    void GL_EXPORT PollEvents();
    void GL_EXPORT ClearWindow();
    void GL_EXPORT DrawWindow();
    int GL_EXPORT ShouldCloseWindow();
    float GL_EXPORT GetDeltaTime();

    void GL_EXPORT ImGuiNewFrame();
    void GL_EXPORT ImGuiRender();

    //[0] = LineShader
    //[1] = DotShader
    Shader** GL_EXPORT GetShaders();
}