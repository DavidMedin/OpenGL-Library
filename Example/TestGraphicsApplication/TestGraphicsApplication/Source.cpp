#include "Source.h"
#define WIDTH 1500
#define HEIGHT 1100




int main(int argv, char* argc[]) {
	init(WIDTH, HEIGHT, "Default");

	
	//todo
	//1. add variable resolution dithering
	//seperate shade color (ambient) from "ambientResult" in source.cpp
	//3. armatures - animation


	Node* rootNode = new Node();
	Object* skull = new Object("../Models/Skull/skullLow.dae",rootNode);
	Object* plane = new Object("../Models/Plane/plane.dae",(Node*)skull);
	plane->Translate(vec3(0, -.1f, 0));

	Light* mainLight = new Light(vec3(1,1,1),.25);
	mainLight->translate = TranslateVec(&mainLight->translate, vec3(0, 1, .25));
	vec3* ambientResult = new vec3(mainLight->color * mainLight->intensity);
	vec2* lightRamp1 = new vec2(.33f,0.f);
	vec2* lightRamp2 = new vec2(0.878f,.586f);

	int renderSwitch = 0;

	Shader* meshShad = new Shader("../Shaders/MeshVs.glsl","../Shaders/MeshFs.glsl",NULL, true);

	Camera* cam = new Camera(vec3(0.0f,0.0f,1.0));
	cam->NewProjection(32, .1f, 100);
	cam->UpdateViewMatrix();

	while (!ShouldCloseWindow()) {

		float dt = GetDeltaTime();
		ClearWindow();
		ImGuiNewFrame();

		float imGuiDragSpeed = 0.25f;

		ImGui::Begin("Variables");
		ImGui::SetNextItemOpen(true, ImGuiCond_Once);
		if (ImGui::TreeNode("Light")) {
			if (ImGui::TreeNode("Position (vec3)")) {
				ImGui::DragFloat("float.x", &mainLight->translate.x, imGuiDragSpeed);
				ImGui::DragFloat("float.y", &mainLight->translate.y, imGuiDragSpeed);
				ImGui::DragFloat("float.z", &mainLight->translate.z, imGuiDragSpeed);
			
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
			if (ImGui::Button("Reload Shader")) {
				meshShad->Reload();
			}
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("Scene")) {
			std::list<Node*>::iterator current = rootNode->children.begin();
			std::list<std::list<Node*>::iterator> stack;
			stack.push_back(current);
			while (1) { //find condition later
				if (!(*stack.back())->children.empty()) {
					current = (*stack.back())->children.begin();
					stack.push_back(current);
					//Imgui code here

				}
				else { //current's children list is empty
					stack.pop_back();
					if ((*stack.back())->children.end() == current) {
						std::list<Node*>::iterator tmp = stack.back();
						stack.pop_back();
						stack.push_back(tmp++);
					}
					else {
						//if iterator isn't last
						//stack.pop_back();
						current++;
						stack.push_back(current);
					}
				}
				
			}
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