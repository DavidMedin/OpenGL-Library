#include "pch.h"
#include "SceneTree.h"

Attribute::Attribute(unsigned int type) {
	this->type = type;
}
Attribute::Attribute() {
	type = NULL;
}

//std::list<void*>* SceneNode::GetAttributesOf(unsigned int type) {
//
//}
void SceneNode::AddAttribute(void* data, unsigned int dataType) {
	attributeList.push_back(data);
	attributeTypes.push_back(dataType);
}
void SceneNode::RemoveAttribute(void* data/*some way of identification*/) {
	attributeList.remove(data);
	//how to get the dataType!
}