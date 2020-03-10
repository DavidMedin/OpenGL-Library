#include "Object.h"

Object::Object() {
	modelMatrix = new mat4(identity<mat4>());
	orien = nullptr;
	orien = new quat(identity<quat>());
}
Object::Object(string path) {
	mesh = new Mesh(path);
	modelMatrix = new mat4(identity<mat4>());
	translate = new mat4(identity<mat4>());
	orien = nullptr;
	orien = new quat(identity<quat>());
	name = path;
}
Object::Object(string path, Node* parent)
{
	mesh = new Mesh(path);
	modelMatrix = new mat4(identity<mat4>());
	translate = new mat4(identity<mat4>());
	orien = nullptr;
	orien = new quat(identity<quat>());

	parent->children.push_back(this);
	this->parent = parent;
	name = path;
}
void Object::UpdateModelMatrix() {
	glm::mat4* parentMatrix = parent != nullptr && parent->type == OBJECT_TYPE ? ((Object*)parent)->modelMatrix : new mat4(glm::identity<mat4>());
	glm::mat4* tmp = modelMatrix;
	modelMatrix = new mat4(*translate* mat4_cast(*orien) * *parentMatrix);
	delete tmp;
	for (Node* obj : children) {
		if (obj->type == OBJECT_TYPE) {
			((Object*)obj)->UpdateModelMatrix();
		}
	}

}
void Object::Draw(Shader* shad,Camera* cam) {
	shad->UseShader();
	shad->UniformEquals("model",GL_FLOAT_MAT4, modelMatrix);
	
	
	if (GetDrawFlags(DRAWFLAG_TRIANGLE)) {
		for (int i = 0; i < 32; i++) {
			if (mesh->texList[i] != nullptr) {
				mesh->texList[i]->Bind();
			}
		}
	}
	mesh->Draw(shad,cam);
	
}
void Object::Draw(Camera* cam) {
	//defaultShader->UniformMatrix("model", modelMatrix);
	mesh->Draw(cam);
}
void Object::Translate(vec3 vector) {
	mat4* tmpMat = this->translate;
	translate = new mat4(glm::translate(*translate, vector));
	delete tmpMat;
	UpdateModelMatrix();
}

void Object::Rotate(vec3 axis, float angle)
{
	quat newRot = angleAxis(radians(angle), axis);
	quat* tmpQuat = orien;
	orien = new quat(newRot * (*orien));
	delete tmpQuat;
	UpdateModelMatrix();
}


int nodeCount = 0;

Node::Node() {
	type = NULL_TYPE;
	name = "Node" + nodeCount;
	nodeCount++;
}
Node::Node(unsigned int type)
{
	this->type = type;
	name = "Node" + nodeCount;
	nodeCount++;
}
void Node::AddChild(Node* child) {
	children.push_back(child);
	child->parent = this;
}