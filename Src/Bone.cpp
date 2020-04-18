#include "Bone.h"

void ConvertAssimpMatrix(glm::vec3* dest, aiMatrix4x4* m) {
	*dest = glm::vec3(m->a4, m->b4, m->c4);
	//return new mat4(
	//	1.0f, 0.0f, 0.0f, 0.0f,
	//	0.0f, 1.0f, 0.0f, 0.0f,
	//	0.0f, 0.0f, 1.0f, 0.0f,
	//	m.a4, m.b4, m.c4, m.d4
	//);
}

unsigned int boneInstances = 0;
BoneNode::BoneNode()
{
	boneInstances++;
	name = "BoneNode_" + std::to_string(boneInstances);
	
	matrix = new glm::mat4(glm::identity<glm::mat4>());
	offset = new glm::vec3(glm::vec3(0, 0, 0));
	
	parent = nullptr;
}

BoneNode::BoneNode(aiNode* node,aiMesh* mesh)
{
	//node should be the root scene node
	//looks for a match for any of the given names
	bool hasBone=false;
	unsigned int boneIndex=0;
	for (int i = 0; i < mesh->mNumBones; i++) {
		if (strcmp(mesh->mBones[i]->mName.C_Str(), node->mName.C_Str())) {//this node is one of our bones!
			hasBone = true;
			boneIndex = i;
		}
	}
	bool hasGoodBone = false;
	for (int i = 0; i < node->mNumChildren; i++) {
		BoneNode* child = new BoneNode(node->mChildren[i], mesh);
		if (child != nullptr) {//won't be null if has bones and stuff
			hasGoodBone = true;
			children.push_back(child);
			child->parent = this;
		}
	}
	if (hasBone || hasGoodBone) {// this is a good node, populate it
		ConvertAssimpMatrix(offset,&mesh->mBones[boneIndex]->mOffsetMatrix);
		matrix = new glm::mat4(glm::identity<glm::mat4>());
	}
	delete this;// kill self if fail
}
