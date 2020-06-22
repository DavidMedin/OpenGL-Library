#include "Source.h"
#define WIDTH 1500
#define HEIGHT 1100
#include <Windows.h>
using namespace Cedar;
int main(int argv, char* argc[]) {
	init(WIDTH, HEIGHT, "Default");
	ImGui::SetCurrentContext((ImGuiContext*)ImGuiInit());
	//load shaders
	int renderSwitch = 0;
	Shader* meshShad = new Shader("../Shaders/MeshVs.glsl", "../Shaders/MeshFs.glsl", NULL);
	Shader* lineShad = new Shader("../../../DefaultShaders/LineVs.glsl", "../../../DefaultShaders/LineFs.glsl", NULL);
	Shader* dotShad = new Shader("../../../DefaultShaders/DotVs.glsl", "../../../DefaultShaders/DotFs.glsl", NULL);
	//meshShad->InitializeSPIRVVMDebug();

	//girl model
	Mesh* girl_model = new Mesh("../Models/Girl/Girl.fbx");

	//setup camera
	Camera* cam = new Camera(glm::vec3(0.0f, 0.0f, 1.0));
	cam->NewProjection(33, .1f, 100);
	cam->UpdateViewMatrix();
	//glPointSize(10);

	//mainLight
	glm::vec3 color_mainLight = glm::vec3(1,1,1);
	float intensity_mainLight = .5f;
	glm::vec3 pos_mainLight = glm::vec3(0, .5f, .5f);


	glm::vec2* lightRamp1 = new glm::vec2(.33f, 0.f);
	glm::vec2* lightRamp2 = new glm::vec2(0.878f, .586f);

	//example of storagebuffer and uniformbuffer (basically the same)
	//void* uniData[] = { &vector[0],&value,&matrix[0]};
	//unsigned int types[][2] = { {GL_FLOAT_VEC3,1},{GL_FLOAT,1},{GL_FLOAT_MAT4,1},{GL_FLOAT_MAT4,0} };
	//StorageBuffer* uni = new StorageBuffer(types,4, uniData, 0, &meshShad, 1, "Test");

	glm::quat* rotation = new glm::quat(glm::identity<glm::quat>());
	float tick = 0;
	bool animate = true;
	float speed = 1;
	while (!ShouldCloseWindow()) {
		float dt = GetDeltaTime();
		ClearWindow();
		ImGuiNewFrame();

		float imGuiDragSpeed = 0.0025f;

		ImGui::Begin("Variables");
		ImGui::SetNextItemOpen(true, ImGuiCond_Once);
		if (ImGui::TreeNode("shader")) {
			if (ImGui::TreeNode("light")) {
				ImGui::ColorEdit3("light color##light", &color_mainLight[0]);
				ImGui::DragFloat3("light pos##light", &pos_mainLight[0],.02f);
				ImGui::SliderFloat("light intensity##light", &intensity_mainLight,0,1);
				ImGui::TreePop();
			}
			if (ImGui::TreeNode("LightRamp")) {
				ImGui::SliderFloat2("LightRamp1", (float*)lightRamp1,0,1);
				ImGui::SliderFloat2("LightRamp2", (float*)lightRamp2,0,1);
				ImGui::TreePop();
			}
			ImGui::InputInt("RenderSwitch", &renderSwitch);
			if (ImGui::Button("Reload Shader")) {
				meshShad->Reload();
			}
			if (ImGui::Button("Debug Shader")) {
				meshShad->StartSPIRVVMDebug();
			}
			if (ImGui::Button("Toggle Animate")) {
				animate = !animate;
				glm::quat tmpQuat = glm::identity<glm::quat>();
				girl_model->skelly->Rotate(0, &tmpQuat);
			}
			if (ImGui::Button("Rotate Bone")) {
				glm::quat* tmp = rotation;
				rotation = new glm::quat(glm::rotate(*rotation, glm::radians(5.0f), glm::vec3(1, 0, 0)));
				delete tmp;
				girl_model->skelly->NameSearch(girl_model->skelly->rootBone, "Braid2")->Rotate(rotation);
			}
			ImGui::TreePop();
		}
		ImGui::DragFloat("Cam Speed", &speed, .01f);
		ImGui::End();

		glm::vec3 ambientResult = glm::vec3(color_mainLight * intensity_mainLight);
		meshShad->UniformEquals("ambientColor",GL_FLOAT_VEC3, &ambientResult,1);
		meshShad->UniformEquals("lightPos", GL_FLOAT_VEC3, &pos_mainLight, 1);
		meshShad->UniformEquals("lightColor",GL_FLOAT_VEC3, &color_mainLight,1);
		meshShad->UniformEquals("LightRamp1", GL_FLOAT_VEC2, lightRamp1,1);
		meshShad->UniformEquals("LightRamp2", GL_FLOAT_VEC2, lightRamp2,1);
		meshShad->UniformEquals("renderSwitch", GL_INT, &renderSwitch,1);

		//ImGui::ShowDemoWindow();

		if (GetKey(keys::A_KEY)) {
			cam->Translate(glm::vec3(cos(-glm::radians(cam->GetY()) + glm::radians(90.0f)) * dt, 0.0f, -sin(-glm::radians(cam->GetY()) + glm::radians(90.0f)) * dt) * speed);
		}
		if (GetKey(keys::D_KEY)) {
			cam->Translate(-glm::vec3(cos(-glm::radians(cam->GetY()) + glm::radians(90.0f)) * dt, 0.0f, -sin(-glm::radians(cam->GetY()) + glm::radians(90.0f)) * dt) * speed);
		}
		if (GetKey(keys::W_KEY)) {
			cam->Translate(glm::vec3(cos(-glm::radians(cam->GetY())) * dt, 0.0f, -sin(-glm::radians(cam->GetY())) * dt) * speed);
		}
		if (GetKey(keys::S_KEY)) {
			cam->Translate(-glm::vec3(cos(-glm::radians(cam->GetY())) * dt, 0.0f, -sin(-glm::radians(cam->GetY())) * dt) * speed);
		}
		if (GetKey(keys::CTRL_KEY)) {
			cam->Translate(glm::vec3(0, -1, 0)*dt * speed);
		}

		if (GetKey(keys::SPACE_KEY)) {
			cam->Translate(glm::vec3(0, 1, 0) * dt*speed);
		}
		float x, y;
		GetMousePos(&x, &y);
		if (GetKey(keys::Q_KEY)) {
			SetDisabledMouse(true);
			cam->Rotate(10*-dt * x, glm::vec3(0, 1, 0));
			cam->Rotate(10*-dt * y, cam->GetRightAxis());
		}
		else {
			SetDisabledMouse(false);
		}

		if (animate) {
			girl_model->skelly->Animate(double(std::fmod(tick,35.0f)));
			tick += dt*(35.0f/5.0f);
		}
		girl_model->Draw(meshShad, cam);


		cam->UpdateViewMatrix();
		//Error Handleing
		std::string error = PollError();
		while (error != "Empty") {
			std::cout << error << "\n";
			error = PollError();
		}
	


		ImGuiRender();
		DrawWindow();
		PollEvents();
	}
	ImGUiShutdown();
	return 0;
}