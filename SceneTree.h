#pragma once


 
#include <glew.h>
#include <glfw3.h>
//assimp importer
#include <assImp/Importer.hpp>
#include <assImp/scene.h>
#include <assimp/postprocess.h>

 
#include <list>
#include <iostream>
#include <string>
#include "Mesh.h"


//class GRAPHICSLIBRARY_API SceneNode {
//private:
//public:
//	std::list<Attribute*> attributeList; // list of attributes; pointers to things like meshes, tranforms, or even textures (think unity)
//	std::string name;
//	std::string path;
//	SceneNode* parentNode;
//	std::list<SceneNode*> childNodes;
//	
//	SceneNode(std::string path);
//	SceneNode();
//	//should be an easy way to export data of the scene.
//	//	because what if a user wants to use his own scenePath, they'll need to extract the data from this object
//
//	std::list<Attribute*>* GetAttributesOf(unsigned int type);
//	void AddAttribute(Attribute* data,unsigned int dataType);
//	void RemoveAttribute(Attribute* data/*some way of identification*/);
//
//	void Draw(Shader* shad,Camera* cam);
//};
//
// 
//
//void Attribute_AddMesh(SceneNode* node, const aiScene* scene, unsigned int meshIndex);
//#endif