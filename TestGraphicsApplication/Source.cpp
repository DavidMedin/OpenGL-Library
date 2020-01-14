#include "Source.h"
int main(int argv, char* argc[]) {
	init(1900, 1200, "Default");

	



	Object* skull = new Object("../Models/Skull/skullLow.dae");
	Mesh* skullMesh = new Mesh("../Models/Skull/skullLow.dae");
	//SceneNode* scene = new SceneNode("../Models/Scene.dae");
	//mat4* skullTransform = new mat4(identity<mat4>());
	//mat4* skullMeshTransform = new mat4(translate(identity<mat4>(), vec3(0.2f, 0, 0)));

	/*for (SceneNode* i : scene->childNodes) {
		printf("|\n");
		for (Attribute* a : i->attributeList) {
			switch (a->type) {
				case 10: {
					printf("-> Attribute_Mesh\n");
					break;
				}
				case 11: {
					printf("-> Attribute_Texture\n");
					break;
				}
				case 12: {
					printf("-> Attribute_Tranform\n");
					break;
				}
			}
		}
		printf("___\n");
	}*/

	Shader* shad = new Shader("../Default.vert", "../Default.frag", true);

	Camera* cam = new Camera(vec3(0.0f,0.0f,1.0));
	cam->NewProjection(32, .1f, 100);
	cam->UpdateViewMatrix();
	while (!ShouldCloseWindow()) {
		float dt = GetDeltaTime();
		ClearWindow();

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

		//for (SceneNode* node : scene->childNodes) {
		//	node->Draw(shad, cam);
		//}
		skull->Draw(shad, cam);
		

		cam->UpdateViewMatrix();
		
		
		//Error Handleing
		string error = PollError();
		while (error != "Empty") {
			cout << error << "\n";
			error = PollError();
		}

		PollEvents();
		DrawWindow();
	}

	return 0;
}