#pragma once
#include <stdio.h>
#include <string>

#include "Error.h"
#include "Input.h"
#include "Shader.h"
#include "GlCall.h"
#include "Camera.h"
#include "Export.h"

#include <glm.hpp>
#include <ext.hpp>

#include "imgui.h"
//#include <Windows.h> 
#define GRAPHICS_FLAG_CULL 1

#define Z_TEST 1


namespace Cedar {
    class GL_EXPORT Context;

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
    //returns ptr offset from module start to imgui context
    GL_EXPORT ImGuiContext* ImGuiInit();
    void GL_EXPORT ImGuiNewFrame();
    void GL_EXPORT ImGuiRender();
    void GL_EXPORT ImGUiShutdown();

    void GL_EXPORT CreateContext(int width, int height, std::string name);
    GL_EXPORT Context* GetCurrentContext();
    void GL_EXPORT SetCurrentContext(Context* context);

    //[0] = LineShader
    //[1] = DotShader
    GL_EXPORT Shader** GetShaders();
    //#define IMGUIINIT(...) ImGui::SetCurrentContext(GetModuleHandleA("CedarGL.dll")+ImGuiInit())
}