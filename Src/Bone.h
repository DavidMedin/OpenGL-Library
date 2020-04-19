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

class Skeleton;
class BoneNode {
private:
public:
	bool kill;

	std::string name;

	Skeleton* skelly;
	int index;

	std::list<BoneNode*> children;
	BoneNode* parent;

	//will need a new set of these for each animation
	glm::vec3* posKeys;
	double* posKeyTimes;
	unsigned int posKeyNum;

	glm::quat* rotKeys;
	double* rotKeyTimes;
	unsigned int rotKeyNum;

	glm::vec3* scaKeys;
	double* scaKeyTimes;
	unsigned int scaKeyNum;

	BoneNode();
	//recursivly goes through scene node
	BoneNode(Skeleton* skelly, aiNode* node, aiMesh* mesh);
	void Rotate(glm::quat* quat);
	void Animate(double tick,glm::mat4* parMat);
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
	Skeleton(aiNode* node, aiMesh* mesh, aiScene* scene);
	
	void Rotate(unsigned int index, glm::quat* quat);

	void Animate(double tick);

	//searches
	BoneNode* IndexSearch(BoneNode* node, int index);
	BoneNode* NameSearch(BoneNode* node, std::string name);
};