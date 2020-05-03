#include "Source.h"
#define WIDTH 1500
#define HEIGHT 1100




int main(int argv, char* argc[]) {
	init(WIDTH, HEIGHT, "Default");

	glPointSize(10);

	Object* skull = new Object("../Models/Skull/BigJaw.fbx");
	glm::quat* firstRotate = new glm::quat(glm::angleAxis(0.0f, glm::vec3(1, 0, 0)));

	Object* plane = new Object("../Models/Plane/plane.dae",(Node*)skull);
	plane->Translate(glm::vec3(0, -.1f, 0));

	Dot* zero = new Dot(glm::vec3(0, 0, 0));
	zero->color = glm::vec3(1, 0, 0);

	Light* mainLight = new Light(glm::vec3(1,1,1),.25);
	mainLight->translate = TranslateVec(&mainLight->translate, glm::vec3(0, 1, .25));
	glm::vec3* ambientResult = new glm::vec3(mainLight->color * mainLight->intensity);
	glm::vec2* lightRamp1 = new glm::vec2(.33f,0.f);
	glm::vec2* lightRamp2 = new glm::vec2(0.878f,.586f);

	int renderSwitch = 0;
	Shader* meshShad = new Shader("../Shaders/MeshVs.glsl","../Shaders/MeshFs.glsl",NULL);

	Camera* cam = new Camera(glm::vec3(0.0f,0.0f,1.0));
	cam->NewProjection(32, .1f, 100);
	cam->UpdateViewMatrix();

	glm::vec3 testVec[2] = { glm::vec3(1,1,1),glm::vec3(1,0,1) };

	float numberArray[2] = { -1,1 };
	glm::vec3 notherVec = glm::vec3(1, 0, 0);
	glm::mat4 matrix = glm::identity<glm::mat4>();
	matrix = glm::translate(matrix, glm::vec3(0, 1, 0));
	glm::vec4 vecs[3] = { glm::vec4(1,1,1,1),glm::vec4(1,0,0,1),glm::vec4(1,1,0,1) };

	void* uniData[] = { &notherVec[0],numberArray, &matrix[0],vecs};
	StorageBuffer* uni = new StorageBuffer({ {GL_FLOAT_VEC3,1},{GL_FLOAT,2} ,{GL_FLOAT_MAT4,1},{GL_FLOAT_VEC4,3} }, (void**)uniData, 0, &meshShad, 1, "Test");

	float tick = 0;

	while (!ShouldCloseWindow()) {
		float dt = GetDeltaTime();
		ClearWindow();
		ImGuiNewFrame();

		float imGuiDragSpeed = 0.0025f;

		ImGui::Begin("Variables");
		ImGui::SetNextItemOpen(true, ImGuiCond_Once);
		if (ImGui::TreeNode("shader")) {
			if (ImGui::TreeNode("LightRamp")) {
				ImGui::SliderFloat2("LightRamp1", (float*)lightRamp1,0,1);
				ImGui::SliderFloat2("LightRamp2", (float*)lightRamp2,0,1);
				ImGui::TreePop();
			}
			ImGui::InputInt("RenderSwitch", &renderSwitch);
			if (ImGui::Button("Reload Shader")) {
				meshShad->Reload();
			}
			ImGui::TreePop();
		}
		ImGui::SliderFloat("frame",&tick, 0, 5);
		UpdateNodes();

		ImGui::End();

		meshShad->UniformEquals("ambientColor",GL_FLOAT_VEC3, ambientResult,1);
		meshShad->UniformEquals("lightPos",GL_FLOAT_VEC3, &mainLight->translate,1);
		meshShad->UniformEquals("lightColor",GL_FLOAT_VEC3, &mainLight->color,1);
		meshShad->UniformEquals("LightRamp1", GL_FLOAT_VEC2, lightRamp1,1);
		meshShad->UniformEquals("LightRamp2", GL_FLOAT_VEC2, lightRamp2,1);
		meshShad->UniformEquals("renderSwitch", GL_INT, &renderSwitch,1);
		//ImGui::ShowDemoWindow();

		if (GetKey(keys::A_KEY)) {
			cam->Translate(glm::vec3(cos(-glm::radians(cam->GetY()) + glm::radians(90.0f)) * dt, 0.0f, -sin(-glm::radians(cam->GetY()) + glm::radians(90.0f)) * dt));
		}
		if (GetKey(keys::D_KEY)) {
			cam->Translate(-glm::vec3(cos(-glm::radians(cam->GetY()) + glm::radians(90.0f)) * dt, 0.0f, -sin(-glm::radians(cam->GetY()) + glm::radians(90.0f)) * dt));
		}
		if (GetKey(keys::W_KEY)) {
			cam->Translate(glm::vec3(cos(-glm::radians(cam->GetY())) * dt, 0.0f, -sin(-glm::radians(cam->GetY())) * dt));
		}
		if (GetKey(keys::S_KEY)) {
			cam->Translate(-glm::vec3(cos(-glm::radians(cam->GetY())) * dt, 0.0f, -sin(-glm::radians(cam->GetY())) * dt));
		}
		if (GetKey(keys::CTRL_KEY)) {
			cam->Translate(glm::vec3(0, -1, 0)*dt);
		}

		if (GetKey(keys::SPACE_KEY)) {
			cam->Translate(glm::vec3(0, 1, 0) * dt);
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

		skull->mesh->skelly->Animate(double(tick));

		skull->Draw(meshShad, cam);
		plane->Draw(meshShad, cam);
		zero->Draw(cam);

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
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	return 0;
}