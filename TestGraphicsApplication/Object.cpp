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
void Object::Draw(Shader* shad) {
	defaultShader->UniformMatrix("model", modelMatrix);
	for (Mesh* tmpMesh : meshList) {
		tmpMesh->Draw(shad);
	}
}
void Object::Draw() {
	defaultShader->UniformMatrix("model", modelMatrix);
	for (Mesh* tmpMesh : meshList) {
		tmpMesh->Draw();
	}
}