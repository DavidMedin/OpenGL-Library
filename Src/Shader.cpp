 
#include "Shader.h"
#define GRAPHICSLIBRARY_EXPORTS 1
using namespace std;
Shader* defaultShader = nullptr;


Shader::Shader() {
	shader_Program = 0;
}

string Shader::ReadShader(const char* path) {
	string tmp;
	ifstream shaderFile;
	shaderFile.open(path, ios::in);
	string input;
	while (!shaderFile.eof()) {
		getline(shaderFile, input);
		if (input.find("uniform") != string::npos) {

		}
		tmp.append(input);
		input.clear();
		tmp.append("\n");
	}
	//const char* C_String = (const char*)malloc(tmp.length());
	//if (C_String != nullptr) {
	//	memcpy((void*)C_String, (void*)tmp.c_str(), tmp.length());
	//	return C_String;
	//}
	return tmp;
}
unsigned int Shader::CompileShader(unsigned int type, const char* source) {
	GLuint id = glCreateShader(type);
	const char* src = source;
	GLCall(glShaderSource(id, 1, &src, nullptr));
	GLCall(glCompileShader(id));
	int result;
	GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
	if (!result) {
		int length;
		GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
		char* error = (char*)malloc(sizeof(char) * length);
		GLCall(glGetShaderInfoLog(id, length, &length, error));
		const char* message = nullptr;
		switch (type) {
			case GL_VERTEX_SHADER: {
				message = "vertex";
				break;
			}
			case GL_FRAGMENT_SHADER: {
				message = "fragment";
				break;
			}
			case GL_GEOMETRY_SHADER: {
				message = "geometry";
				break;
			}
			default: {
				message = "unknown shader";
			}
		}
		printf("failed to compile %s shader: ", message);
		cout << error << "\n";
		
		free(error);
		GLCall(glDeleteShader(id));
	}

	return id;
}

unsigned int Shader::CreateShaderProgram(const char* vertexShader, const char* fragmentShader,const char* geometryShader) {
	GLuint shader_program = glCreateProgram();
	GLuint vertex_shader = CompileShader(GL_VERTEX_SHADER, vertexShader);
	GLuint fragment_shader = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);
	GLuint geometry_shader;
	if (geometryShader != nullptr) {
		geometry_shader = CompileShader(GL_GEOMETRY_SHADER, geometryShader);
	}
	GLCall(glAttachShader(shader_program, vertex_shader));
	GLCall(glAttachShader(shader_program, fragment_shader));
	if (geometryShader != nullptr) {
		GLCall(glAttachShader(shader_program, geometry_shader));
	}
	GLCall(glLinkProgram(shader_program));
	GLint rez = NULL;
	GLCall(glGetProgramiv(shader_program, GL_LINK_STATUS, &rez));
	if (rez == NULL || rez != GL_TRUE) {
		GLsizei logSize = 255;
		GLchar* log = (GLchar*)malloc(sizeof(GLchar) * logSize);
		GLsizei actualSize = 0;
		GLCall(glGetProgramInfoLog(shader_program, 255, &actualSize, log));
		cout << actualSize << "\n";
		cout << log << "\n";
		delete log;
	}

	GLCall(glValidateProgram(shader_program));
	rez=NULL;
	GLCall(glGetProgramiv(shader_program, GL_VALIDATE_STATUS, &rez));
	if (rez == NULL || rez != GL_TRUE) {
		GLsizei logSize = 255;
		GLchar* log = (GLchar*)malloc(sizeof(GLchar)*logSize);
		GLsizei actualSize=0;
		GLCall(glGetProgramInfoLog(shader_program, 255, &actualSize, log));
		cout << actualSize << "\n";
		cout << log << "\n";
		delete log;
	}
	GLCall(glDeleteShader(vertex_shader));
	GLCall(glDeleteShader(fragment_shader));
	if (geometryShader != nullptr) {
		GLCall(glDeleteShader(geometry_shader));
	}
	return shader_program;
}


Shader::Shader(const char* vertexPath, const char* fragmentPath,const char* geomPath, bool makeDefault) {
	vertexContent = ReadShader(vertexPath);
	fragmentContent = ReadShader(fragmentPath);
	if (geomPath != nullptr) { 
		vertexContent = ReadShader(geomPath);
		shader_Program = CreateShaderProgram(vertexContent.c_str(), fragmentContent.c_str(), geometryContent.c_str()); 
	}
	else
	{
		shader_Program = CreateShaderProgram(vertexContent.c_str(),fragmentContent.c_str(),NULL);
	}
	if (makeDefault) defaultShader = this;
	GLCall(glUseProgram(shader_Program));
	
}

Shader::Shader(const char* shaderPath, bool makeDefault) {
	//read and find identifiers
	// -> @vertex,@fragment,@geometry
	//add to flag var
	char flags = 0;

	//parse the file for identifiers
	string tmp;
	ifstream shaderFile;
	shaderFile.open(shaderPath, ios::in);
	string input;

	string vertexText;
	string fragmentText;
	string geometryText;

	while (!shaderFile.eof()) {
		getline(shaderFile, input);
		if (input.find("uniform") != string::npos) {

		}

		if (input.find("@vertex") != string::npos) {//should be after the vertex shader
			vertexText = tmp;
			tmp.clear();
			//might clear VertexText
			continue;
		}
		else if (input.find("@fragment") != string::npos) {
			fragmentText = tmp;
			tmp.clear();
			continue;
		}
		else if (input.find("@geometry") != string::npos) {
			geometryText = tmp;
			tmp.clear();
			continue;
		}

		tmp.append(input);
		input.clear();
		tmp.append("\n");
	}
	//compiling shaders
	shader_Program = glCreateProgram();
	GLuint vertex_shader = CompileShader(GL_VERTEX_SHADER, vertexText.c_str());
	GLuint fragment_shader = CompileShader(GL_FRAGMENT_SHADER, fragmentText.c_str());
	GLuint geometry_shader = NULL;
	//combine shaders
	GLCall(glAttachShader(shader_Program, vertex_shader));
	GLCall(glAttachShader(shader_Program, fragment_shader));
	if (!geometryText.empty()) {
		geometry_shader = CompileShader(GL_GEOMETRY_SHADER, geometryText.c_str());
		GLCall(glAttachShader(shader_Program, geometry_shader));
	}
	GLCall(glLinkProgram(shader_Program));
	GLCall(glValidateProgram(shader_Program));
	GLCall(glDeleteShader(vertex_shader));
	GLCall(glDeleteShader(fragment_shader));
	if (geometry_shader != NULL) {
		GLCall(glDeleteShader(geometry_shader));
	}
	if (makeDefault) defaultShader = this;

	GLCall(glUseProgram(shader_Program));
}

Shader::~Shader() {
	GLCall(glDeleteProgram(shader_Program));
}

void Shader::UseShader()
{
	GLCall(glUseProgram(shader_Program));
}

void Shader::UniformEquals(const char* uniform_Name,unsigned int type,void* value)
//Type should be GL_FLOAT_MAT4,GL_FLOAT_VEC3 and the like
{
	int uni_Pos = glGetUniformLocation(shader_Program, uniform_Name);
	if (uni_Pos == -1) {
		printf("%s is not a variable in your shader!", uniform_Name);
		return;
	}
	UseShader();
	
	switch (type) {
	case GL_FLOAT_MAT4: 
		GLCall(glUniformMatrix4fv(uni_Pos, 1, GL_FALSE, glm::value_ptr(*(mat4*)value)));
		break;
	case GL_FLOAT_VEC3:
		GLCall(glUniform3f(uni_Pos, (*(vec3*)value)[0], (*(vec3*)value)[1], (*(vec3*)value)[2]));
		break;
	case GL_FLOAT_VEC2:
		GLCall(glUniform2f(uni_Pos, (*(vec2*)value)[0], (*(vec2*)value)[1]));
		break;
	case GL_INT:
		GLCall(glUniform1i(uni_Pos, *((int*)value)));
		break;
	case GL_FLOAT:
		GLCall(glUniform1f(uni_Pos, *((float*)value)));
		break;
	default:
		printf("not a valid type\n"); //Type should be GL_FLOAT_MAT4,GL_FLOAT_VEC3 and the like
		break;
	}
}

void Shader::UniformMatrix(string uniform_Name, mat4* matrix, unsigned int type) {
	int uni_Pos = glGetUniformLocation(shader_Program, uniform_Name.c_str());
	UseShader();
	GLCall(glUniformMatrix4fv(uni_Pos, 1, GL_FALSE, glm::value_ptr(*matrix)));
}
void Shader::UniformVector(string uniform_Name, vec3* vec) {
	int uni_Pos = glGetUniformLocation(shader_Program, uniform_Name.c_str());
	UseShader();
	GLCall(glUniform3f(uni_Pos, (*vec)[0], (*vec)[1], (*vec)[2]));
}
void Shader::Reload() {
	unsigned int tmpShader = CreateShaderProgram(vertexContent.c_str(), fragmentContent.c_str(), NULL);
	shader_Program = tmpShader;
}