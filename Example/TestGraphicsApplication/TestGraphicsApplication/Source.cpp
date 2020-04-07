#include "Source.h"
#define WIDTH 1500
#define HEIGHT 1100




int main(int argv, char* argc[]) {
	init(WIDTH, HEIGHT, "Default");

	glPointSize(10);
	Dot* test = new Dot(vec3(1, 0, 1));
	test->color = vec3(1, 1, 0);

	Object* skull = new Object("../Models/Skull/skullLow.dae");
	Object* plane = new Object("../Models/Plane/plane.dae",(Node*)skull);
	plane->Translate(vec3(0, -.1f, 0));

	MetaLine* bone = new MetaLine(vec3((*skull->mesh->boneOffsets[0])[3][0], (*skull->mesh->boneOffsets[0])[3][1], (*skull->mesh->boneOffsets[0])[3][2]), vec3(1,1,1));
	bone->color = vec3(.2f, 1, .7f);
	bone->size = 10.0f;

	Light* mainLight = new Light(vec3(1,1,1),.25);
	mainLight->translate = TranslateVec(&mainLight->translate, vec3(0, 1, .25));
	vec3* ambientResult = new vec3(mainLight->color * mainLight->intensity);
	vec2* lightRamp1 = new vec2(.33f,0.f);
	vec2* lightRamp2 = new vec2(0.878f,.586f);

	int renderSwitch = 0;

	Shader* meshShad = new Shader("../Shaders/MeshVs.glsl","../Shaders/MeshFs.glsl",NULL, true);
	Shader* lineShad = new Shader("../../../DefaultShaders/LineVs.glsl", "../../../DefaultShaders/LineFs.glsl", NULL, false);


	Camera* cam = new Camera(vec3(0.0f,0.0f,1.0));
	cam->NewProjection(32, .1f, 100);
	cam->UpdateViewMatrix();

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
		UpdateNodes();

		ImGui::End();

		meshShad->UniformEquals("ambientColor",GL_FLOAT_VEC3, ambientResult);
		meshShad->UniformEquals("lightPos",GL_FLOAT_VEC3, &mainLight->translate);
		meshShad->UniformEquals("lightColor",GL_FLOAT_VEC3, &mainLight->color);
		meshShad->UniformEquals("LightRamp1", GL_FLOAT_VEC2, lightRamp1);
		meshShad->UniformEquals("LightRamp2", GL_FLOAT_VEC2, lightRamp2);
		meshShad->UniformEquals("renderSwitch", GL_INT, &renderSwitch);
		//ImGui::ShowDemoWindow();

		if (GetKey(keys::A_KEY)) {
			cam->Translate(vec3(cos(-radians(cam->GetY()) + radians(90.0f)) * dt, 0.0f, -sin(-radians(cam->GetY()) + radians(90.0f)) * dt));
		}
		if (GetKey(keys::D_KEY)) {
			cam->Translate(-vec3(cos(-radians(cam->GetY()) + radians(90.0f)) * dt, 0.0f, -sin(-radians(cam->GetY()) + radians(90.0f)) * dt));
		}
		if (GetKey(keys::W_KEY)) {
			cam->Translate(vec3(cos(-radians(cam->GetY())) * dt, 0.0f, -sin(-radians(cam->GetY())) * dt));
		}
		if (GetKey(keys::S_KEY)) {
			cam->Translate(-vec3(cos(-radians(cam->GetY())) * dt, 0.0f, -sin(-radians(cam->GetY())) * dt));
		}
		if (GetKey(keys::CTRL_KEY)) {
			cam->Translate(vec3(0, -1, 0)*dt);
		}

		if (GetKey(keys::SPACE_KEY)) {
			cam->Translate(vec3(0, 1, 0) * dt);
		}
		float x, y;
		GetMousePos(&x, &y);
		if (GetKey(keys::Q_KEY)) {
			SetDisabledMouse(true);
			cam->Rotate(10*-dt * x, vec3(0, 1, 0));
			cam->Rotate(10*-dt * y, cam->GetRightAxis());
		}
		else {
			SetDisabledMouse(false);
		}

		skull->Draw(meshShad, cam);
		plane->Draw(meshShad, cam);
		bone->Draw(lineShad,cam);

		test->Draw(cam);

		cam->UpdateViewMatrix();
		
		
		//Error Handleing
		string error = PollError();
		while (error != "Empty") {
			cout << error << "\n";
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