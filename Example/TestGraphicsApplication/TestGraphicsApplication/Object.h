#pragma once
#include <glm.hpp>
#include <stdlib.h>
#include "Maths.h"
#include "../../../Src/GraphicsLibrary.h"



class Node {
public:
	//self
	std::string name;
	bool hide; //don't render
	//rendering
	Shader* shad;
	Camera* cam;

	glm::mat4* model;

	//relationship
	std::list<Node*> children;
	Node* parent=nullptr;
	Node();
	void AddChild(Node* child);
	
	virtual void Update() = 0;
	void UpdateChildren();
	
	virtual void ImGuiUpdate() = 0;
	void ImGuiUpdateChildren();
	
	virtual void Draw()=0;
};
void AddNodeToList(Node* node);


class Object : public Node {
private:

public:
	glm::vec3* translate;
	glm::vec3* scale;
	glm::quat* orien;


	Mesh* mesh;
	Object();
	Object(std::string path);
	Object(std::string path, Node* parent);
	//will want to add a variadic function to take in as many shaders as mesh objects
	void Translate(glm::vec3 vector);
	void Rotate(glm::vec3 axis, float angle);
	void Update();
	void ImGuiUpdate();
	void UpdateModelMatrix();
	void Draw();
	void Draw(Shader* shad,Camera* cam);
};


class MetaLine :public Line,public Node {
private:
	float* lastEdit;
public:
	glm::vec3 translate;
	glm::vec3 scale;

	MetaLine();
	MetaLine(glm::vec3 point1, glm::vec3 point2);
	void Update();
	void ImGuiUpdate();
	void UpdateModelMatrix();
	void Draw();
	//void Draw(Shader* shad, Camera* cam);
};

class Dot : public Node {
private:
	float pointSize = 10;
public:
	bool depthTest;

	glm::vec3 translate;

	glm::vec3 color;
	std::string name;
	Mesh* mesh;
	Dot();
	Dot(glm::vec3 pos);
	void Update();
	void ImGuiUpdate();
	void UpdateModelMatrix();
	void Draw();
	//void Draw(Shader* shad, Camera* cam);
};

class Light : public Dot {
private:
public:
	float intensity;

	Light(glm::vec3 color, float intensity);
	void Update();
	void UpdateModelMatrix();
	void ImGuiUpdate();
};


class MetaBone : public MetaLine {
private:

	Skeleton* skelly;
	BoneNode* boneRef;

	glm::mat4* offset;

	float pos[6];
	MetaBone(Skeleton* skelly, Node* parent, BoneNode* node,Shader* shad);
public:
	bool includeInvOffset;

	MetaBone();
	MetaBone(Skeleton* skelly,Node* parent,Shader* shad);


	void Update();
	void ImGuiUpdate();
};

void UpdateNodes();
void ImGuiUpdateNodes();
//void DrawNodes();

void SetDefaultShader(Shader* shad);
Shader* GetDefaultShader();

void SetDefalutCamera(Camera* cam);
Camera* GetDefaultCamera();