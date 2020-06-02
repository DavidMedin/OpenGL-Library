#include "Object.h"

float imGuiDragSpeed = 0.0025f;

std::list<Node*> nodeList;
std::list<Node*> inFrontList;

Shader* defaultShad;
Camera* defaultCam;

void AddNodeToList(Node* node) {
	nodeList.push_back(node);
}

void UpdateNodes() {
	for (Node* i : nodeList) {
		i->Update();
	}
	glDisable(GL_DEPTH_TEST);
	for (Node* i : inFrontList) {
		i->Draw();
	}
	glEnable(GL_DEPTH_TEST);
}
void ImGuiUpdateNodes() {
	for (Node* i : nodeList) {
		i->ImGuiUpdate();
	}
}
void DrawNodes() {
	for (Node* i : nodeList) {
		i->Draw();
	}
}

void SetInFront(Node* node) {
	inFrontList.push_back(node);
	node->inFront = true;
}
void SetNotInFront(Node* node) {
	inFrontList.remove(node);
	node->inFront = false;
}


void SetDefaultShader(Shader* shad)
{
	defaultShad = shad;
}

Shader* GetDefaultShader()
{
	return defaultShad;
}

void SetDefalutCamera(Camera* cam)
{
	defaultCam = cam;
}

Camera* GetDefaultCamera()
{
	return defaultCam;
}


Object::Object() {
	//model = new glm::mat4(glm::identity<glm::mat4>());
	orien = nullptr;
	orien = new glm::quat(glm::identity<glm::quat>());
	translate = new glm::vec3(0);
	scale = new glm::vec3(1);
	mesh = nullptr;
}
Object::Object(std::string path) {
	mesh = new Mesh(path);
	//model = new glm::mat4(glm::identity<glm::mat4>());
	translate = new glm::vec3(0);
	scale = new glm::vec3(1);
	//orien = nullptr;
	orien = new glm::quat(glm::identity<glm::quat>());
	name = path;
}
Object::Object(std::string path, Node* parent)
{
	mesh = new Mesh(path);
	//model = new glm::mat4(glm::identity<glm::mat4>());
	translate = new glm::vec3(0);
	scale = new glm::vec3(1);
	//orien = nullptr;
	orien = new glm::quat(glm::identity<glm::quat>());

	parent->AddChild(this);
	name = path;
}
void Object::UpdateModelMatrix() {
	glm::mat4* tmp = model;
	glm::mat4 transMat4 = glm::translate(glm::identity<glm::mat4>(), *translate);
	transMat4 = glm::scale(transMat4, *scale);
	model = new glm::mat4(transMat4* mat4_cast(*orien) * (parent != nullptr && parent->model != nullptr ? *parent->model : glm::identity<glm::mat4>()));
	delete tmp;
	//for (Node* obj : children) {
	//	if (obj->type == OBJECT_TYPE) {
	//		((Object*)obj)->UpdateModelMatrix();
	//	}
	//}
	

}
void Object::Draw() {
	Shader* tmpShad = shad == nullptr ? GetDefaultShader() : shad;
	Camera* tmpCam = cam == nullptr ? GetDefaultCamera() : cam;
	tmpShad->UseShader();
	tmpShad->UniformEquals("model",GL_FLOAT_MAT4, model,1);
	
	
	if (GetDrawFlags(DRAWFLAG_TRIANGLE)) {
		for (int i = 0; i < 32; i++) {
			if (mesh->texList[i] != nullptr) {
				mesh->texList[i]->Bind();
			}
		}
	}
	mesh->Draw(tmpShad, tmpCam);
	
}
void Object::Draw(Shader* shad, Camera* cam)
{
	shad->UseShader();
	shad->UniformEquals("model", GL_FLOAT_MAT4, model, 1);


	if (GetDrawFlags(DRAWFLAG_TRIANGLE)) {
		for (int i = 0; i < 32; i++) {
			if (mesh->texList[i] != nullptr) {
				mesh->texList[i]->Bind();
			}
		}
	}
	mesh->Draw(shad, cam);

}
void Object::Translate(glm::vec3 vector) {
	//mat4* tmpMat = this->translate;
	//translate = new mat4(glm::translate(*translate, vector));
	glm::vec3* tmpVec = this->translate;
	translate = new glm::vec3(*translate + vector);
	delete tmpVec;
	UpdateModelMatrix();
}

void Object::Rotate(glm::vec3 axis, float angle)
{
	glm::quat newRot = glm::angleAxis(glm::radians(angle), axis);
	glm::quat* tmpQuat = orien;
	orien = new glm::quat(newRot * (*orien));
	delete tmpQuat;
	UpdateModelMatrix();
}

void Object::Update()
{
	//ImGuiUpdate();
	UpdateModelMatrix();
	if(!hide && !inFront) Draw();
	UpdateChildren();
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
		char buffer[16];
		sprintf_s(buffer, "Toggle hide : %d", (int)hide);
		if(ImGui::Button(buffer)) {
			hide = !hide;
		}
		UpdateModelMatrix();
		ImGuiUpdateChildren();
		ImGui::TreePop();
	}

}

int nodeCount = 0;

Node::Node() {
	hide = false;
	shad = nullptr;
	cam = nullptr;
	name = "Node_" + std::to_string(nodeCount);
	nodeCount++;
	model = new glm::mat4(glm::identity<glm::mat4>());
}
void Node::AddChild(Node* child) {
	children.push_back(child);
	child->parent = this;
}

void Node::UpdateChildren()
{
	for (Node* i : children) {
		i->Update();
	}
}

void Node::ImGuiUpdateChildren()
{
	for (Node* i : children) {
		i->ImGuiUpdate();
	}
}



int lightCount = 0;
Light::Light(glm::vec3 color, float intensity) {

	this->color = color;
	this->intensity = intensity;
	translate = glm::vec3();
	name = "Light_" + std::to_string(lightCount);
	lightCount++;
	
}
void Light::Update() {
	UpdateModelMatrix();
	if(!hide && !inFront) Draw();
	UpdateChildren();
}
void Light::UpdateModelMatrix()
{
	glm::mat4* tmp = model;
	model = new glm::mat4(glm::translate(glm::identity<glm::mat4>(),translate) * (parent != nullptr && parent->model != nullptr ? *parent->model : glm::identity<glm::mat4>()));
	delete tmp;
}
void Light::ImGuiUpdate() {

	if (ImGui::TreeNode(name.c_str())) {
		ImGui::DragFloat("x", &translate.x, imGuiDragSpeed);
		ImGui::DragFloat("y", &translate.y, imGuiDragSpeed);
		ImGui::DragFloat("z", &translate.z, imGuiDragSpeed);
		ImGui::ColorEdit3("Color", &color[0]);

		char buffer[16];
		sprintf_s(buffer, "Toggle hide : %d", (int)hide);
		if (ImGui::Button(buffer)) {
			hide = !hide;
		}


		ImGuiUpdateChildren();
		ImGui::TreePop();
	}
}

unsigned int metaLineCount = 0;
MetaLine::MetaLine()
{
	translate = glm::vec3(0);
	scale = glm::vec3(1);
	//nodeList.push_back(this);
	name = "MetaLine_" + std::to_string(metaLineCount);
	metaLineCount++;
	UpdateModelMatrix();

	//private members
	float* mappedData = OpenWriting();
	lastEdit = (float*)malloc(sizeof(float) * 6);
	memcpy(lastEdit, mappedData, sizeof(float) * 6);
	GLCall(CloseWriting());
}

MetaLine::MetaLine(glm::vec3 point1, glm::vec3 point2)
{
	translate = glm::vec3(0);
	scale = glm::vec3(1);
	SetPoint1(point1);
	SetPoint2(point2);
	name = "MetaLine_" + std::to_string(metaLineCount);
	metaLineCount++;
	UpdateModelMatrix();

	//private members
	float* mappedData = OpenWriting();
	lastEdit = (float*)malloc(sizeof(float) * 6);
	memcpy(lastEdit, mappedData, sizeof(float) * 6);
	GLCall(CloseWriting());
}



//void MetaLine::Draw(Shader* shad, Camera* cam)
//{
//	shad->UseShader();
//	shad->UniformEquals("model", GL_FLOAT_MAT4, &modelMatrix,1);
//	GraphicsDisable(Z_TEST);
//	Line::Draw(shad,cam);
//	GraphicsEnable(Z_TEST);
//}

void MetaLine::Update()
{
	//ImGuiUpdate();
	if(!hide && !inFront) Draw();
	UpdateChildren();
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
				GLCall(CloseWriting());
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

		char buffer[16];
		sprintf_s(buffer, "Toggle hide : %d", (int)hide);
		if (ImGui::Button(buffer)) {
			hide = !hide;
		}

		UpdateModelMatrix();
		ImGuiUpdateChildren();
		ImGui::TreePop();
	}
}

void MetaLine::UpdateModelMatrix()
{
	glm::mat4* sacrificeMat = model;
	glm::mat4* tmpMat = new glm::mat4(glm::scale(glm::identity<glm::mat4>(),scale));
	model = new glm::mat4(glm::translate(*tmpMat, translate) * (parent!= nullptr&&parent->model != nullptr ? *parent->model : glm::identity<glm::mat4>()));
	delete tmpMat;
	delete sacrificeMat;
}

void MetaLine::Draw()
{
	Shader* tmpShad = shad == nullptr ? GetDefaultShader() : shad;
	Camera* tmpCam = cam == nullptr ? GetDefaultCamera() : cam;
	tmpShad->UseShader();
	tmpShad->UniformEquals("model", GL_FLOAT_MAT4, model,1);
	GraphicsDisable(Z_TEST);
	Line::Draw(tmpShad,tmpCam);
	GraphicsEnable(Z_TEST);
}

unsigned int dotCount = 0;
Dot::Dot()
{
	SetInFront(this);

	depthTest = false;
	translate = glm::vec3(0);
	color = glm::vec3(1);
	UpdateModelMatrix();
	dotCount++;
	float* point = (float*)calloc(sizeof(float) * 3, 1);
	unsigned int* index = (unsigned int*)calloc(sizeof(unsigned int),1);
	mesh = new Mesh(point, sizeof(float) * 3, index, 1);
	mesh->drawMode = GL_POINTS;

	shad = GetShaders()[1];
}

Dot::Dot(glm::vec3 pos)
{
	SetInFront(this);

	depthTest = false;
	translate = pos;
	color = glm::vec3(1);
	UpdateModelMatrix();
	name = "Dot_" + std::to_string(dotCount);
	dotCount++;

	float* point = (float*)calloc(sizeof(float) * 3,1);
	unsigned int* index = (unsigned int*)calloc(sizeof(unsigned int), 1);
	mesh = new Mesh(point, sizeof(float) * 3, index, 1);
	mesh->drawMode = GL_POINTS;

	shad = GetShaders()[1];


}

void Dot::Update()
{
	UpdateModelMatrix();
	if(!hide && !inFront) Draw();
	UpdateChildren();
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

		char buffer[16];
		sprintf_s(buffer, "Toggle hide : %d", (int)hide);
		if (ImGui::Button(buffer)) {
			hide = !hide;
		}


		ImGuiUpdateChildren();
		ImGui::TreePop();
	}
}

void Dot::UpdateModelMatrix()
{
	glm::mat4* tmp = model;
	model = new glm::mat4(glm::translate(glm::identity<glm::mat4>(), translate) * (parent != nullptr && parent->model != nullptr ? *parent->model : glm::identity<glm::mat4>()));
	delete tmp;
}

void Dot::Draw()
{
	Shader* tmpShad = shad == nullptr ? GetDefaultShader() : shad;
	Camera* tmpCam = cam == nullptr ? GetDefaultCamera() : cam;
	tmpShad->UseShader();
		bool is = glIsEnabled(GL_DEPTH_TEST);
	depthTest ? glEnable(GL_DEPTH_TEST) : glDisable(GL_DEPTH_TEST);
	float beforeSize;
	glGetFloatv(GL_POINT_SIZE, &beforeSize);
	glPointSize(pointSize);
	tmpShad->UniformEquals("color", GL_FLOAT_VEC3, &color,1);
	tmpShad->UniformEquals("model", GL_FLOAT_MAT4, model,1);
	mesh->Draw(tmpShad, tmpCam);
	if (is != depthTest) depthTest ? glDisable(GL_DEPTH_TEST) : glEnable(GL_DEPTH_TEST);
	glPointSize(beforeSize);
}


//void Dot::Draw(Shader* shad, Camera* cam)
//{
//	bool is = glIsEnabled(GL_DEPTH_TEST);
//	depthTest ? glEnable(GL_DEPTH_TEST) : glDisable(GL_DEPTH_TEST);
//	glPointSize(pointSize);
//	shad->UniformEquals("color", GL_FLOAT_VEC3, &color,1);
//	shad->UniformEquals("model", GL_FLOAT_MAT4, &modelMatrix,1);
//	mesh->Draw(shad, cam);
//	if (is != depthTest) depthTest ? glDisable(GL_DEPTH_TEST) : glEnable(GL_DEPTH_TEST);
//}

MetaBone::MetaBone(Skeleton* skelly, Node* parent,Shader* shad)
{
	
	//node stuff
	parent->AddChild(this);
	this->shad = shad;
	SetInFront(this);

	//bone stuff
	includeInvOffset = false;
	this->skelly = skelly;
	boneRef = skelly->IndexSearch(skelly->rootBone, 0);
	name = boneRef->name;
	
	//child stuff
	for (auto i : boneRef->children) {
		if (!(*i->children.begin())->leaf) {
			MetaBone* child = new MetaBone(skelly, this, i,shad);
			AddChild(child);
		}
		else if (!i->leaf) {
			AddChild(new Dot(glm::vec3(glm::inverse(skelly->boneOffsets[i->index]) * glm::vec4(0, 0, 0, 1))));
		}
	}

	//metaline stuff
	scale = glm::vec3(1, 1, 1);
	translate = glm::vec3(0, 0, 0);
	UpdateModelMatrix();

	//line stuff
	offset = &skelly->boneOffsets[boneRef->index];
	delete model;
	model = &skelly->boneMatrices[boneRef->index];
}
MetaBone::MetaBone(Skeleton* skelly, Node* parent, BoneNode* node,Shader* shad) {
	//this stuff
	this->shad = shad;
	SetInFront(this);

	//bone stuff
	includeInvOffset = false;
	this->skelly = skelly;
	boneRef = node;
	name = boneRef->name;

	//child stuff
	for (auto i : boneRef->children) {
		if (!(*i->children.begin())->leaf) {
			MetaBone* child = new MetaBone(skelly, this, i,shad);
			AddChild(child);
		}
		else if (!i->leaf) {
			AddChild(new Dot(glm::vec3(glm::inverse(skelly->boneOffsets[i->index]) * glm::vec4(0, 0, 0, 1))));
		}
	}

	//metaline stuff
	scale = glm::vec3(1, 1, 1);
	translate = glm::vec3(0, 0, 0);
	UpdateModelMatrix();

	//line stuff
	offset = &skelly->boneOffsets[boneRef->index];
	delete model;
	model = &skelly->boneMatrices[boneRef->index];

}


void MetaBone::Update() {
	//ImGuiUpdate();
	glm::vec3* points = (glm::vec3*)OpenWriting();
	*points = glm::vec3(glm::inverse(*offset) * glm::vec4(0, 0, 0, 1));
	//get child offset through the first child of our boneRef, taking its index, indexing the skeletons boneoffset with that and inverting it :)
	glm::mat4* childMat;
	childMat = &skelly->boneOffsets[(*boneRef->children.begin())->index];
	glm::mat4 childOffset = glm::inverse(*childMat);
	points[1] = glm::vec3(childOffset * glm::vec4(0, 0, 0, 1));
	*(glm::vec3*)&pos = *points;
	*(glm::vec3*)& pos[3] = points[1];
	GLCall(CloseWriting());
	if(!hide && !inFront) Draw();
	UpdateChildren();

}

void MetaBone::ImGuiUpdate()
{
	if (ImGui::TreeNode(name.c_str())) {
		ImGui::Text("MetaBone");
		ImGui::Text("Pnt1 : %f,%f,%f", pos[0], pos[1], pos[2]);
		ImGui::Text("Pnt2 : %f,%f,%f", pos[3], pos[4], pos[5]);
		ImGui::ColorEdit3("Color", &color[0]);
		if (ImGui::Button("Toggle include inverse offset")) includeInvOffset = !includeInvOffset;

		char buffer[16];
		sprintf_s(buffer, "Toggle hide : %d", (int)hide);
		if (ImGui::Button(buffer)) {
			hide = !hide;
		}


		ImGuiUpdateChildren();
		ImGui::TreePop();
	}
}



MetaBone::MetaBone() {
}
