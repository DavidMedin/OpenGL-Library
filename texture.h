#ifdef GRAPHICSLIBRARY_EXPORTS
#define GRAPHICSLIBRARY_API __declspec(dllexport)
	#include <assimp/Importer.hpp>
	#include <assimp/scene.h>
	#include <assimp/postprocess.h>
	#include <glew.h>
	#include <glfw3.h>
	//#include <gl/GL.h>
#else
#define GRAPHICSLIBRARY_API __declspec(dllimport)
#endif
#include "GlCall.h"

class GRAPHICSLIBRARY_API Texture {
	private:
		unsigned int openglID;
		unsigned int height, width;
		float* data;
	public:
		Texture(unsigned int slot, float* data, unsigned int w, unsigned int h);
		void Bind(unsigned int slot = 0);
};