#pragma once


#ifdef GRAPHICSLIBRARY_EXPORTS
#define GRAPHICSLIBRARY_API __declspec(dllexport)
#include <glew.h>
#include <glfw3.h>

#else
#define GRAPHICSLIBRARY_API __declspec(dllimport)
#endif
#include "Mesh.h"

#define ATTRIBUTE_MESH 0
#define ATTRIBUTE_TEXTURE 1
#define ATTRIBUTE_TRANSFORM 2

GRAPHICSLIBRARY_API class SceneNode {
private:
	list<void*> attributeList; // list of attributes; pointers to things like meshes, tranforms, or even textures (think unity)
	list<unsigned int> attributeTypes; //should be in sync with attributeList and its types
public:
	string name;
	SceneNode* parentNode;
	list<SceneNode*> childNodes;
	
	SceneNode(string path);

	//should be an easy way to export data of the scene.
	//	because what if a user wants to use his own scenePath, they'll need to extract the data from this object

	list<void*>* GetAttributesOf(unsigned int type);
	void AddAttribute(void* data,unsigned int dataType);
	void RemoveAttribute(void* data/*some way of identification*/);
};