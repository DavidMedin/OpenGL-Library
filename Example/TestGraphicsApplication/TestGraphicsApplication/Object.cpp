#include "Object.h"

float imGuiDragSpeed = 0.0025f;

list<Node*> nodeList;

void UpdateNodes() {
	for (Node* i : nodeList) {
		i->Update();
	}
}

Object::Object() {
	modelMatrix = new mat4(identity<mat4>());
	orien = nullptr;
	orien = new quat(identity<quat>());
	translate = new vec3(0);
	scale = new vec3(1);
	mesh = nullptr;
}
Object::Object(string path) {
	mesh = new Mesh(path);
	modelMatrix = new mat4(identity<mat4>());
	translate = new vec3(0);
	scale = new vec3(1);
	orien = nullptr;
	orien = new quat(identity<quat>());
	name = path;
	nodeList.push_back(this);
}
Object::Object(string path, Node* parent)
{
	mesh = new Mesh(path);
	modelMatrix = new mat4(identity<mat4>());
	translate = new vec3(0);
	scale = new vec3(1);
	orien = nullptr;
	orien = new quat(identity<quat>());

	parent->children.push_back(this);
	this->parent = parent;
	name = path;
}
void Object::UpdateModelMatrix() {
	//glm::mat4* parentMatrix = parent != nullptr && parent->type == OBJECT_TYPE ? ((Object*)parent)->modelMatrix : new mat4(glm::identity<mat4>());
	glm::mat4* tmp = modelMatrix;
	glm::mat4 transMat4 = glm::translate(identity<mat4>(), *translate);
	transMat4 = glm::scale(transMat4, *scale);
	modelMatrix = new mat4(transMat4* mat4_cast(*orien) /** *parentMatrix*/);
	delete tmp;
	//for (Node* obj : children) {
	//	if (obj->type == OBJECT_TYPE) {
	//		((Object*)obj)->UpdateModelMatrix();
	//	}
	//}

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
	//mat4* tmpMat = this->translate;
	//translate = new mat4(glm::translate(*translate, vector));
	vec3* tmpVec = this->translate;
	translate = new vec3(*translate + vector);
	delete tmpVec;
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

void Object::Update()
{
	ImGuiUpdate();
}

void Object::ImGuiUpdate() {

	if (ImGui::TreeNode(name.c_str())) {
		ImGui::DragFloat("x", &translate->x, imGuiDragSpeed);
		ImGui::DragFloat("y", &translate->y, imGuiDragSpeed);
		ImGui::DragFloat("z", &translate->z, imGuiDragSpeed);
		ImGui::Text("Scale");
		ImGui::DragFloat("Scale.x", &scale->x, imGuiDragSpeed);
		ImGui::DragFloat("Scale.y", &scale->y, imGuiDragSpeed);
		ImGui::DragFloat("Scale.z", &scale->z, imGuiDragSpeed);
		UpdateModelMatrix();
		for (Node* i : children) {
			i->Update();
		}
		ImGui::TreePop();
	}

}

int nodeCount = 0;

Node::Node() {
	//type = NULL_TYPE;
	name = "Node_" + to_string(nodeCount);
	nodeCount++;
}
void Node::AddChild(Node* child) {
	children.push_back(child);
	child->parent = this;
}

int lightCount = 0;
Light::Light(vec3 color, float intensity) {
	this->color = color;
	this->intensity = intensity;
	translate = vec3();
	nodeList.push_back(this);
	name = "Light_" + to_string(lightCount);
	lightCount++;
}
void Light::Update() {
	ImGuiUpdate();
}
void Light::ImGuiUpdate() {

	if (ImGui::TreeNode(name.c_str())) {
		ImGui::DragFloat("x", &translate.x, imGuiDragSpeed);
		ImGui::DragFloat("y", &translate.y, imGuiDragSpeed);
		ImGui::DragFloat("z", &translate.z, imGuiDragSpeed);
		ImGui::ColorEdit3("Color", &color[0]);
		for (Node* i : children) {
			i->Update();
		}
		ImGui::TreePop();
	}
}
int particleCount = 0;
Particle::Particle() {
	mesh = new Mesh("../Models/Particle_Base/Particle.dae");
	nodeList.push_back(this);
	name = "Particle_" + to_string(particleCount);
}
Particle::Particle(string path) {
	mesh = new Mesh("../Models/Particle_Base/Particle.dae");
	nodeList.push_back(this);
	name = path;
	delete mesh->texList[0];
	mesh->texList[0] = new Texture(DIFFUSE_SLOT,path);
}