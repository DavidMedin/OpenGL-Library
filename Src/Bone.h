//assimp
#include <assImp/Importer.hpp>
#include <assImp/scene.h>
#include <assimp/postprocess.h>


//glm
#include <glm.hpp>
#include <ext.hpp>
#include <gtx/string_cast.hpp>

//std
#include <stdio.h>
#include <string>
#include <list>
#include <iostream>
class BoneNode {
private:
public:
	bool kill;

	std::string name;
	unsigned int index;
	std::list<BoneNode*> children;
	BoneNode* parent;

	BoneNode();
	//recursivly goes through scene node
	BoneNode(aiNode* node, aiMesh* mesh);
};

class Skeleton {
private:
public:
	//boneOffsets and boneMatrices are boneCount in size
	glm::vec3* boneOffsets; //this might be a mat4
	glm::mat4* boneMatrices;
	BoneNode* rootBone;
	unsigned int boneCount;

	Skeleton();
	Skeleton(aiNode* node, aiMesh* mesh);
};