#pragma once
#define GL_EXPORT
#ifdef COMPILING_GL
#define GL_EXPORT __declspec(dllexport)
#else
#define GL_EXPORT __declspec(dllimport)
#endif