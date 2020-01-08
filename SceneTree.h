#pragma once


#ifdef GRAPHICSLIBRARY_EXPORTS
#define GRAPHICSLIBRARY_API __declspec(dllexport)
#include <glew.h>
#include <glfw3.h>

#else
#define GRAPHICSLIBRARY_API __declspec(dllimport)
#endif
#include <list>
#include <iostream>
#include <string>

#define ATTRIBUTE_MESH 0
#define ATTRIBUTE_TEXTURE 1
#define ATTRIBUTE_TRANSFORM 2

GRAPHICSLIBRARY_API class Attribute {
public:
	unsigned int type;
	Attribute(unsigned int type);
	Attribute();
};


GRAPHICSLIBRARY_API class SceneNode {
private:
	std::list<void*> attributeList; // list of attributes; pointers to things like meshes, tranforms, or even textures (think unity)
	std::list<unsigned int> attributeTypes; //should be in sync with attributeList and its types
public:
	std::string name;
	SceneNode* parentNode;
	std::list<SceneNode*> childNodes;
	
	SceneNode(std::string path);

	//should be an easy way to export data of the scene.
	//	because what if a user wants to use his own scenePath, they'll need to extract the data from this object

	std::list<void*>* GetAttributesOf(unsigned int type);
	void AddAttribute(void* data,unsigned int dataType);
	void RemoveAttribute(void* data/*some way of identification*/);
};