#include "Object.h"

Object::Object() {
	modelMatrix = new mat4(identity<mat4>());
	orien = nullptr;
	orien = new quat(identity<quat>());
}
Object::Object(string path) {
	meshList.push_back(new Mesh(path));
	modelMatrix = new mat4(identity<mat4>());
	translate = new mat4(identity<mat4>());
	orien = nullptr;
	orien = new quat(identity<quat>());
}
void Object::UpdateModelMatrix() {
	glm::mat4* tmp = modelMatrix;
	modelMatrix = new mat4(*translate* mat4_cast(*orien));
	delete tmp;
}
void Object::Draw(Shader* shad,Camera* cam) {
	shad->UseShader();
	shad->UniformMatrix("model", modelMatrix,TYPE_MODEL);
	
	for (Mesh* tmpMesh : meshList) {
		if (GetDrawFlags(DRAWFLAG_TRIANGLE)) {
			for (int i = 0; i < 32; i++) {
				if (tmpMesh->texList[i] != nullptr) {
					tmpMesh->texList[i]->Bind();
				}
			}
		}
		tmpMesh->Draw(shad,cam);
	}
}
void Object::Draw(Camera* cam) {
	//defaultShader->UniformMatrix("model", modelMatrix);
	for (Mesh* tmpMesh : meshList) {
		tmpMesh->Draw(cam);
	}
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
