
 
#include "Init.h"
//using namespace std;
;
int graphicsFlag = 1;




static void glfw_error_callback(int error, const char* description)
{
	fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}



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
void  SetGraphicsFlag(int flag)
{
	graphicsFlag = graphicsFlag^flag;
}
bool  GetGraphicsFlag(int flag)
{
	return graphicsFlag&flag;
}
int init(int width, int height, string name) {
	glfwSetErrorCallback(glfw_error_callback);

	if (!glfwInit()) {
		NewError("glfwInit didn't start correctly\n");
		return 0;
	}


	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(width, height, name.c_str(), NULL, NULL);
	if (!window) {
		NewError("window didn't open (glfw)\n");
		glfwTerminate();
		return 0;
	}
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);


	glewExperimental = GL_TRUE;
	glewInit();
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);


	//Imgui stuff
	int width_w, height_w;
	glfwGetFramebufferSize(window, &width_w, &height_w);
	glViewport(0, 0, width_w, height_w);
	
	ImGui::CreateContext();

	ImGuiIO& io = ImGui::GetIO(); (void)io;

	ImGui::StyleColorsDark();

	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(NULL);



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


void ImGuiNewFrame() {
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}
void ImGuiRender() {
	//ImGui_ImplOpenGL3_NewFrame();
	//ImGui_ImplGlfw_NewFrame();
	//ImGui::NewFrame();
	//ImGuiFuncVar();
	//ImGui::Begin("Hello, World!");
	//ImGui::Text("Text!");
	//ImGui::End();

	ImGui::Render();
	//int display_w, display_h;
	//glfwGetFramebufferSize(glfwGetCurrentContext(), &display_w, &display_h);
	//glViewport(0, 0, display_w, display_h);
	//glClear(GL_COLOR_BUFFER_BIT);
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
