#include "Source.h"
#define WIDTH 1500
#define HEIGHT 1100




int main(int argv, char* argc[]) {
	init(WIDTH, HEIGHT, "Default");

	glPointSize(10);

	Object* skull = new Object("../Models/Skull/BigJaw.fbx");
	glm::quat* firstRotate = new glm::quat(glm::angleAxis(0.0f, glm::vec3(1, 0, 0)));

	Object* plane = new Object("../Models/Plane/plane.fbx", (Node*)skull);
	plane->Translate(glm::vec3(0, -.1f, 0));

	Dot* zero = new Dot(glm::vec3(0, 0, 0));
	zero->color = glm::vec3(1, 0, 0);

	Light* mainLight = new Light(glm::vec3(1, 1, 1), .25);
	mainLight->translate = TranslateVec(&mainLight->translate, glm::vec3(0, 1, .25));
	glm::vec3* ambientResult = new glm::vec3(mainLight->color * mainLight->intensity);
	glm::vec2* lightRamp1 = new glm::vec2(.33f, 0.f);
	glm::vec2* lightRamp2 = new glm::vec2(0.878f, .586f);

	int renderSwitch = 0;
	Shader* meshShad = new Shader("../Shaders/MeshVs.glsl", "../Shaders/MeshFs.glsl", NULL);
	meshShad->InitializeSPIRVVMDebug();
	glm::mat4 identity = glm::identity<glm::mat4>();
	meshShad->UniformEquals("identity", GL_FLOAT_MAT4, &identity, 1,GL_VERTEX_SHADER);

	Camera* cam = new Camera(glm::vec3(0.0f, 0.0f, 1.0));
	cam->NewProjection(33, .1f, 100);
	cam->UpdateViewMatrix();


	//void* uniData[] = { &vector[0],&value,&matrix[0]};
	//unsigned int types[][2] = { {GL_FLOAT_VEC3,1},{GL_FLOAT,1},{GL_FLOAT_MAT4,1},{GL_FLOAT_MAT4,0} };
	//StorageBuffer* uni = new StorageBuffer(types,4, uniData, 0, &meshShad, 1, "Test");

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
			if (ImGui::Button("Debug Shader")) {
				meshShad->StartSPIRVVMDebug();
			}
			ImGui::TreePop();
		}
		ImGui::SliderFloat("frame",&tick, 0, 5);
		static int tmpInt = 1;
		UpdateNodes();

		ImGui::End();

		meshShad->UniformEquals("ambientColor",GL_FLOAT_VEC3, ambientResult,1,GL_FRAGMENT_SHADER);
		meshShad->UniformEquals("lightPos",GL_FLOAT_VEC3, &mainLight->translate,1, GL_FRAGMENT_SHADER);
		meshShad->UniformEquals("lightColor",GL_FLOAT_VEC3, &mainLight->color,1, GL_FRAGMENT_SHADER);
		meshShad->UniformEquals("LightRamp1", GL_FLOAT_VEC2, lightRamp1,1, GL_FRAGMENT_SHADER);
		meshShad->UniformEquals("LightRamp2", GL_FLOAT_VEC2, lightRamp2,1, GL_FRAGMENT_SHADER);
		meshShad->UniformEquals("renderSwitch", GL_INT, &renderSwitch,1, GL_FRAGMENT_SHADER);

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