#include "Object.h"

Object::Object() {
	modelMatrix = new mat4(identity<mat4>());
	orien = nullptr;
	orien = new quat(identity<quat>());
}
Object::Object(string path) {
	meshList.push_back(new Mesh(path));
	modelMatrix = new mat4(identity<mat4>());
	orien = nullptr;
	orien = new quat(identity<quat>());
}
void Object::Draw(Shader* shad,Camera* cam) {
	shad->UseShader();
	shad->UniformMatrix("model", modelMatrix,TYPE_MODEL);
	for (Mesh* tmpMesh : meshList) {
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
	mat4* tmpMat = modelMatrix;
	modelMatrix = new mat4(translate(*modelMatrix, vector));
	delete tmpMat;
}