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
unsigned int metaLineCount = 0;
MetaLine::MetaLine()
{
	translate = vec3(0);
	scale = vec3(1);
	nodeList.push_back(this);
	name = "MetaLine_" + to_string(metaLineCount);
	metaLineCount++;
	UpdateModelMatrix();

	//private members
	float* mappedData = OpenWriting();
	lastEdit = (float*)malloc(sizeof(float) * 6);
	memcpy(lastEdit, mappedData, sizeof(float) * 6);
	GLCall(ClosedWriting());
}

MetaLine::MetaLine(vec3 point1, vec3 point2)
{
	translate = vec3(0);
	scale = vec3(1);
	SetPoint1(point1);
	SetPoint2(point2);
	nodeList.push_back(this);
	name = "MetaLine_" + to_string(metaLineCount);
	metaLineCount++;
	UpdateModelMatrix();

	//private members
	float* mappedData = OpenWriting();
	lastEdit = (float*)malloc(sizeof(float) * 6);
	memcpy(lastEdit, mappedData, sizeof(float) * 6);
	GLCall(ClosedWriting());
}

void MetaLine::Draw(Camera* cam)
{
	Shader** shaders = GetShaders();
	shaders[0]->UniformEquals("model", GL_FLOAT_MAT4, &modelMatrix);
	GraphicsDisable(Z_TEST);
	Line::Draw(cam);
	GraphicsEnable(Z_TEST);
}

void MetaLine::Draw(Shader* shad, Camera* cam)
{
	shad->UseShader();
	shad->UniformEquals("model", GL_FLOAT_MAT4, &modelMatrix);
	GraphicsDisable(Z_TEST);
	Line::Draw(shad,cam);
	GraphicsEnable(Z_TEST);
}

void MetaLine::Update()
{
	ImGuiUpdate();
}

void MetaLine::ImGuiUpdate()
{
	float store = 1;
	if (ImGui::TreeNode(name.c_str())) {
		ImGui::Text("Point Pos'");

		const char* strings[6] = {"x##Point1", "y##Point1","z##Point1","x##Point2","y##Point2","z##Point2"};
		for (int i = 0; i < 6; i++) {
			if (ImGui::DragFloat(strings[i], &lastEdit[i], imGuiDragSpeed)) { //something was edited
				float* mappedData = OpenWriting();
				if (mappedData != nullptr) {
					memcpy(mappedData, lastEdit, sizeof(float) * 6);
				}
				else {
					printf("OpenWriting returned nullptr\n");
					__debugbreak();
				}
				GLCall(ClosedWriting());
			}
		}
		ImGui::Text("Translate");
		ImGui::DragFloat("x##Trans", &translate.x, imGuiDragSpeed);
		ImGui::DragFloat("y##Trans", &translate.y, imGuiDragSpeed);
		ImGui::DragFloat("z##Trans", &translate.z, imGuiDragSpeed);
		ImGui::Text("Scale");
		ImGui::DragFloat("x##Scale", &scale.x, imGuiDragSpeed);
		ImGui::DragFloat("y##Scale", &scale.y, imGuiDragSpeed);
		ImGui::DragFloat("z##Scale", &scale.z, imGuiDragSpeed);

		ImGui::ColorEdit3("Color", &color[0]);
		ImGui::SliderFloat("Line Width", &size,1,10);
		UpdateModelMatrix();
		for (Node* i : children) {
			i->Update();
		}
		ImGui::TreePop();
	}
}

void MetaLine::UpdateModelMatrix()
{
	mat4* tmpMat = new mat4(glm::scale(identity<mat4>(),scale));
	modelMatrix = glm::translate(*tmpMat, translate);
	delete tmpMat;
}

unsigned int dotCount = 0;
Dot::Dot()
{
	depthTest = false;
	translate = vec3(0);
	color = vec3(1);
	UpdateModelMatrix();
	dotCount++;
	float* point = (float*)malloc(sizeof(float) * 3);
	point[0] = 0;
	point[1] = 0;
	point[2] = 0;
	mesh = new Mesh(point, sizeof(float) * 3);
	mesh->drawMode = GL_POINTS;
	nodeList.push_back(this);
}

Dot::Dot(vec3 pos)
{
	depthTest = false;
	translate = pos;
	color = vec3(1);
	UpdateModelMatrix();
	name = "Dot_" + to_string(dotCount);
	dotCount++;
	float* point = (float*)malloc(sizeof(float) * 3);
	point[0] = 0;
	point[1] = 0;
	point[2] = 0;
	mesh = new Mesh(point, sizeof(float) * 3);
	mesh->drawMode = GL_POINTS;
	nodeList.push_back(this);

}

void Dot::Update()
{
	ImGuiUpdate();
}

void Dot::ImGuiUpdate()
{
	if (ImGui::TreeNode(name.c_str())) {
		const char* names[] = { "x##DotPos","y##DotPos","z##DotPos" };
		for (int i = 0; i < 3; i++) {
			if (ImGui::DragFloat(names[i], &translate[i], imGuiDragSpeed)) {
				UpdateModelMatrix();
			}
		}
		ImGui::ColorEdit3("Color##DotColor", &color[0]);
		ImGui::DragFloat("Size##DotSize", &pointSize, 1);
		if (ImGui::Button("Cull##DotCull")) {
			depthTest = !depthTest;
		}
		for (Node* i : children) {
			i->Update();
		}
		ImGui::TreePop();
	}
}

void Dot::UpdateModelMatrix()
{
	modelMatrix = glm::translate(identity<mat4>(), translate);
}

void Dot::Draw(Camera* cam)
{
	glPushAttrib(GL_ENABLE_BIT);
	depthTest ? glEnable(GL_DEPTH_TEST) : glDisable(GL_DEPTH_TEST);
	glPointSize(pointSize);
	Shader** shads = GetShaders();
	shads[1]->UniformEquals("color", GL_FLOAT_VEC3, &color);
	shads[1]->UniformEquals("model", GL_FLOAT_MAT4, &modelMatrix);
	mesh->Draw(shads[1], cam);
	glPopAttrib();
}

void Dot::Draw(Shader* shad, Camera* cam)
{
	glPushAttrib(GL_ENABLE_BIT);
	depthTest ? glEnable(GL_DEPTH_TEST) : glDisable(GL_DEPTH_TEST);
	glPointSize(pointSize);
	shad->UniformEquals("color", GL_FLOAT_VEC3, &color);
	shad->UniformEquals("model", GL_FLOAT_MAT4, &modelMatrix);
	mesh->Draw(shad, cam);
	glPopAttrib();
}
