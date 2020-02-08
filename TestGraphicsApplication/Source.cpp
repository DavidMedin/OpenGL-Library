#include "Source.h"
int main(int argv, char* argc[]) {
	init(1500, 1100, "Default");

	
	//todo
	//1.) force buffer to slot location (will be used for custom data buffers)
	//2.) bind uniforms to appropriate shader (maybe track the MatrixUniform function to go through and bind them)


	Object* skull = new Object("../Models/Skull/skullLow.dae");
	Object* plane = new Object("../Models/Plane/plane.dae");
	plane->Translate(vec3(0, -.1f, 0));
	plane->Rotate(vec3(1, 0, 0), 180);

	Object* isosphere = new Object("../Models/Icosphere.dae");

	Shader* meshShad = new Shader("../Mesh.shader", true);
	Shader* pointShad = new Shader("../Cube.shader", false);

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

		meshShad->UseShader();
		skull->Draw(meshShad, cam);
		plane->Draw(meshShad, cam);

		DrawFlags(DRAWFLAG_TRIANGLE);
		//pointShad->UseShader();
		SetGraphicsFlag(GRAPHICS_FLAG_CULL); //no culling for the cubes
		//isosphere->Draw(pointShad, cam);
		SetGraphicsFlag(GRAPHICS_FLAG_CULL);
		DrawFlags(DRAWFLAG_TRIANGLE);


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