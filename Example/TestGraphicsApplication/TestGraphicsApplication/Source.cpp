#include "Source.h"
#define WIDTH 1500
#define HEIGHT 1100




int main(int argv, char* argc[]) {
	init(WIDTH, HEIGHT, "Default");

	
	//todo
	//1.) create imgui rendering, while giving control to the application
	//2.) bind uniforms to appropriate shader (maybe track the MatrixUniform function to go through and bind them)




	Object* skull = new Object("../Models/Skull/skullLow.dae");
	Object* plane = new Object("../Models/Plane/plane.dae");
	plane->Translate(vec3(0, -.1f, 0));

	Light* mainLight = new Light(vec3(1,1,1),.25);
	mainLight->translate = TranslateVec(&mainLight->translate, vec3(0, 1, .25));
	vec3* ambientResult = new vec3(mainLight->color * mainLight->intensity);
	vec2* lightRamp1 = new vec2(.5f,0.f);
	vec2* lightRamp2 = new vec2(.7f,1.f);

	int renderSwitch = 0;

	Shader* meshShad = new Shader("../Shaders/MeshVs.glsl","../Shaders/MeshFs.glsl",NULL, true);

	Camera* cam = new Camera(vec3(0.0f,0.0f,1.0));
	cam->NewProjection(32, .1f, 100);
	cam->UpdateViewMatrix();

	while (!ShouldCloseWindow()) {

		float dt = GetDeltaTime();
		ClearWindow();
		ImGuiNewFrame();


		ImGui::Begin("Shader Variables");
		ImGui::SetNextItemOpen(true, ImGuiCond_Once);
		if (ImGui::TreeNode("Light")) {
			if (ImGui::TreeNode("Position (vec3)")) {
				ImGui::SliderFloat("float.x", &mainLight->translate.x, -1.0f, 1.0f);
				ImGui::SliderFloat("float.y", &mainLight->translate.y, -1.0f, 1.0f);
				ImGui::SliderFloat("float.z", &mainLight->translate.z, -1.0f, 1.0f);
			
				ImGui::TreePop();
			}
			float color[3] = {mainLight->color.x,mainLight->color.y,mainLight->color.z};
			if (ImGui::ColorEdit3("Color", color)) {
				mainLight->color.x = color[0];
				mainLight->color.y = color[1];
				mainLight->color.z = color[2];
			}
			if (ImGui::TreeNode("LightRamp")) {
				ImGui::SliderFloat2("LightRamp1", (float*)lightRamp1,0,1);
				ImGui::SliderFloat2("LightRamp2", (float*)lightRamp2,0,1);
				ImGui::TreePop();
			}
			ImGui::InputInt("RenderSwitch", &renderSwitch);
			ImGui::TreePop();
		}
		
		
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

		cam->UpdateViewMatrix();
		
		
		//Error Handleing
		string error = PollError();
		while (error != "Empty") {
			cout << error << "\n";
			error = PollError();
		}
	
		//ImGuiNewFrame();


		ImGuiRender();
		DrawWindow();
		PollEvents();
	}
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	return 0;
}