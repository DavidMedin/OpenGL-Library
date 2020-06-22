#include <list>
//glm
#include <glm.hpp>
#include <ext.hpp>
#include <gtx/string_cast.hpp>


#include "Export.h"
namespace Cedar {
	class GL_EXPORT Skeleton;
	class GL_EXPORT BoneNode {
	private:
	public:
		//bool kill;

		bool leaf;

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
		void Rotate(glm::quat* quat);
		void Animate(double tick, glm::mat4* parMat);
	};

	class GL_EXPORT Skeleton {
	private:
	public:
		//boneOffsets and boneMatrices are boneCount in size
		glm::mat4* boneOffsets;
		glm::mat4* boneMatrices;

		BoneNode* rootBone;
		unsigned int boneCount;



		Skeleton();

		void Rotate(unsigned int index, glm::quat* quat);

		void Animate(double tick);

		//searches
		BoneNode* IndexSearch(BoneNode* node, int index);
		BoneNode* NameSearch(BoneNode* node, std::string name);
	};
}