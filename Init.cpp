/*New Things in this project
library functionality, no window, but will be used for debugging
lua/c/c++ controllability
Threads */
#define GRAPHICSLIBRARY_EXPORTS 1
#include "pch.h"
#include "Init.h"
//using namespace std;
std::list<GLFWwindow*>* windowList;

int graphicsFlag = 1;

void WindowSizeCallback(GLFWwindow* window, int width, int height)
{
	glfwSetWindowSize(window, width, height);
	list<Camera*>* camList = GetCameraList();
	/*for (list<Camera*>::iterator i = camList->begin(); i != camList->end(); i++) {
		i._Ptr->_Myval->RecalculateProjection();
	}*/
	for (Camera* i : *camList) {
		i->RecalculateProjection();
	}
}
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}
void GRAPHICSLIBRARY_API SetGraphicsFlag(int flag)
{
	graphicsFlag = graphicsFlag^flag;
}
bool GRAPHICSLIBRARY_API GetGraphicsFlag(int flag)
{
	return graphicsFlag&flag;
}
int init(int width, int height, string name) {
	if (!glfwInit()) {
		NewError("glfwInit didn't start correctly\n");
		return 0;
	}


	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(width, height, name.c_str(), NULL, NULL);
	if (!window) {
		NewError("window didn't open (glfw)\n");
		glfwTerminate();
		return 0;
	}
	glfwMakeContextCurrent(window);
	//glfwSwapInterval(1);
	glewExperimental = GL_TRUE;
	glewInit();
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);


	//-------Input------------//
	
	glfwSetKeyCallback(glfwGetCurrentContext(), Key);
	glfwSetCursorPosCallback(glfwGetCurrentContext(), Mouse);
	glfwSetMouseButtonCallback(glfwGetCurrentContext(), MouseButton);
	glfwSetWindowSizeCallback(glfwGetCurrentContext(), WindowSizeCallback);
	glfwSetFramebufferSizeCallback(glfwGetCurrentContext(), framebuffer_size_callback);
	return 1;
}

void ClearWindow() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void PollEvents() {
	glfwPollEvents();
}
void DrawWindow() {
	glfwSwapBuffers(glfwGetCurrentContext());
}
int ShouldCloseWindow() {
	return glfwWindowShouldClose(glfwGetCurrentContext()); 
}
float GetDeltaTime() {
	static float prev_dt = (float)glfwGetTime();
	float dt = (float)((float)glfwGetTime() - prev_dt);
	prev_dt = (float)glfwGetTime();
	return dt;
}