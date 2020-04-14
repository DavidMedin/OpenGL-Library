#pragma once
#include <glm.hpp>
#include <stdlib.h>
#include "Maths.h"
#include "../../../Src/GraphicsLibrary.h"

class Node {
private:

public:
	Node();
	void AddChild(Node* child);
	std::list<Node*> children;
	Node* parent;
	string name;
	virtual void Update() = 0;
};



class Object : public Node {
private:

public:
	glm::vec3* translate;
	glm::vec3* scale;
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
	void Update();
	void ImGuiUpdate();
	void UpdateModelMatrix();
};

class Light : Node {
private:

public:
	vec3 translate;
	vec3 color;
	float intensity;

	Light(vec3 color, float intensity);
	void Update();
	void ImGuiUpdate();
};

class MetaLine :public Line,public Node {
private:
	float* lastEdit;
public:
	vec3 translate;
	vec3 scale;
	mat4 modelMatrix;
	MetaLine();
	MetaLine(vec3 point1, vec3 point2);
	void Draw(Camera* cam);
	void Draw(Shader* shad, Camera* cam);
	void Update();
	void ImGuiUpdate();
	void UpdateModelMatrix();
};

class Dot :public Node {
private:
	float pointSize = 10;
public:
	bool depthTest;

	vec3 translate;
	vec3 color;
	mat4 modelMatrix;
	string name;
	Mesh* mesh;
	Dot();
	Dot(vec3 pos);
	void Update();
	void ImGuiUpdate();
	void UpdateModelMatrix();
	void Draw(Camera* cam);
	void Draw(Shader* shad, Camera* cam);
};

void UpdateNodes();

