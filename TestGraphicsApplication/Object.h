#pragma once
#include <glm.hpp>
#include <stdlib.h>
#include <GraphicsLibrary.h>

class Object {
private:

public:	
	glm::mat4* modelMatrix;
	glm::quat* orien;
	std::list<Mesh*> meshList;
	Object();
	Object(string path);
	//will want to add a variadic function to take in as many shaders as mesh objects
	void Draw(Shader* shad,Camera* cam);
	void Draw(Camera* cam);
	void Translate(vec3 vector);
};