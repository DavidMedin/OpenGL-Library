#pragma once
#include <glm.hpp>
#include <stdlib.h>
#include "../../../Src/GraphicsLibrary.h"
#define OBJECT_TYPE 1
#define NULL_TYPE 0

class Node {
private:

public:
	Node();
	Node(unsigned int type);
	void AddChild(Node* child);
	unsigned int type;
	std::list<Node*> children;
	Node* parent;
	string name;
};



class Object : Node {
private:

public:
	glm::mat4* translate;
	glm::mat4* modelMatrix;
	glm::quat* orien;
	Mesh* mesh;
	Object();
	Object(string path);
	Object(string path, Node* parent);
	//will want to add a variadic function to take in as many shaders as mesh objects
	void Draw(Shader* shad,Camera* cam);
	void Draw(Camera* cam);
	void Translate(vec3 vector);
	void Rotate(vec3 axis, float angle);
	void UpdateModelMatrix();
};