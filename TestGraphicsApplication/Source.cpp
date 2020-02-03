#include "Source.h"
int main(int argv, char* argc[]) {
	init(1900, 1200, "Default");

	



	Object* skull = new Object("../Models/Skull/skullLow.dae");
	Object* plane = new Object("../Models/Plane/plane.dae");
	plane->Translate(vec3(0, -.1f, 0));

	//Shader* shad = new Shader("../Default.vert", "../Default.frag", true);
	Shader* shad = new Shader("../Shader.shader", true);

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

		
		skull->Draw(shad, cam);
		plane->Draw(shad, cam);

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