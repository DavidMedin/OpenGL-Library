#include "Source.h"
int main(int argv, char* argc[]) {
	init(1000, 1000, "Default");

	


	//Mesh* mesh = new Mesh("../Models/Skull/skulllow.dae");
	//Mesh* plane = new Mesh("../Models/Plane/Plane.dae");
	Object* skull = new Object("../Models/Skull/skullLow.dae");
	Object* plane = new Object("../Models/Plane/Plane.dae");

	Shader* shad = new Shader("../Default.vert", "../Default.frag", true);
	mat4* projection = NewProjection(60, .1, 100);

	Camera* cam = new Camera({ 0.0f,0.0f,1.0f });
	//cam->Rotate(90,vec3(0, 1, 0));
	cam->UpdateViewMatrix();
	//shad->UniformVector("color", color);
	shad->UniformMatrix("proj", projection);
	shad->UniformMatrix("view", cam->viewMat);
	//delete color;
	while (!ShouldCloseWindow()) {
		float dt = GetDeltaTime();
		ClearWindow();

		//printf("Rot y = : %f\n",cam->GetY());
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
			cam->Rotate(-10 * dt * x, vec3(0, 1, 0));
			cam->Rotate(-10 * dt * y, cam->GetRightAxis());
		}
		else {
			SetDisabledMouse(false);
		}

		

		cam->UpdateViewMatrix();
		shad->UniformMatrix("view", cam->viewMat);
		
		
		//Error Handleing
		string error = PollError();
		while (error != "Empty") {
			cout << error << "\n";
			error = PollError();
		}
		
		skull->Draw();
		plane->Draw();

		PollEvents();
		DrawWindow();
	}

	return 0;
}