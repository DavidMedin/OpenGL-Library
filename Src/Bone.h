//assimp
#include <assImp/Importer.hpp>
#include <assImp/scene.h>
#include <assimp/postprocess.h>


//glm
#include <glm.hpp>
#include <ext.hpp>

#include <stdio.h>
#include <string>
#include <list>

class BoneNode {
private:
public:
	std::string name;
	glm::mat4* matrix;
	glm::vec3* offset; //this might be a mat4

	std::list<BoneNode*> children;
	BoneNode* parent;

	BoneNode();
	//recursivly goes through scene node
	BoneNode(aiNode* node, aiMesh* mesh);
};