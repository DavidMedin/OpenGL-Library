#include "pch.h"
#include "Shader.h"
#define GRAPHICSLIBRARY_EXPORTS 1
using namespace std;
Shader* defaultShader = nullptr;


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
	return tmp;
}
unsigned int Shader::CompileShader(unsigned int type, const string& source) {
	GLuint id = glCreateShader(type);
	const char* src = source.c_str();
	GLCall(glShaderSource(id, 1, &src, nullptr));
	GLCall(glCompileShader(id));

	int result;
	GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
	if (!result) {
		int length;
		GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
		char* error = (char*)malloc(sizeof(char) * length + 1);
		if (error != nullptr) {
			/*for (int i = 0; i < length + 1; i++) {
				error[i] = '\0';
			}*/
			error[length + 1] = '\0';
		}
		GLCall(glGetShaderInfoLog(id, length, &length, error));
		printf("failed to compile %s shader: ", type == GL_VERTEX_SHADER ? "vertex" : "fragment");
		char* nError = nullptr;
		sprintf_s(nError,30, "failed to compile %s shader: ", type == GL_VERTEX_SHADER ? "vertex" : "fragment");
		NewError(nError);
		free(nError);
		printf("%s\n", error);
		free(error);
		GLCall(glDeleteShader(id));
	}

	return id;
}

unsigned int Shader::CreateShaderProgram(const string& vertexShader, const string& fragmentShader) {
	GLuint shader_program = glCreateProgram();
	GLuint vertex_shader = CompileShader(GL_VERTEX_SHADER, vertexShader);
	GLuint fragment_shader = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);
	GLCall(glAttachShader(shader_program, vertex_shader));
	GLCall(glAttachShader(shader_program, fragment_shader));
	GLCall(glLinkProgram(shader_program));
	GLCall(glValidateProgram(shader_program));
	GLCall(glDeleteShader(vertex_shader));
	GLCall(glDeleteShader(fragment_shader));
	return shader_program;
}


Shader::Shader(const char* vertexPath,const char* fragmentPath, bool makeDefault) {
	shader_Program = CreateShaderProgram(ReadShader(vertexPath), ReadShader(fragmentPath));
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

//void Shader::UniformEquals(const char* uniform_Name,unsigned int type,float* value)
//{
//	//Types are:
//		//vertex points,uv data: Library Lvl Access
//		//projection,transform,view: Client Access, data is Library Access
//		//custom data: client Access (arrays)
//		//custom float: Client Access (1 float)
		//Client acessable vertex data???

//	int uni_Pos = glGetUniformLocation(shader_Program, uniform_Name);
//	UseShader();
//	switch (type) {
//	case 1: 
//		GLCall(glUniformMatrix4fv(uni_Pos, 1, GL_FALSE, value));
//		break;
//	case 2:
//		GLCall(glUniform3f(uni_Pos, value[0], value[1], value[2]));
//		break;
//	default:
//		printf("not a valid type\n");
//		break;
//	}
//}

void Shader::UniformMatrix(string uniform_Name, mat4* matrix,unsigned int type) {
	int uni_Pos = glGetUniformLocation(shader_Program, uniform_Name.c_str());
	UseShader();
	GLCall(glUniformMatrix4fv(uni_Pos, 1, GL_FALSE, glm::value_ptr(*matrix)));
}
void Shader::UniformVector(string uniform_Name, vec3* vec) {
	int uni_Pos = glGetUniformLocation(shader_Program, uniform_Name.c_str());
	UseShader();
	GLCall(glUniform3f(uni_Pos, (*vec)[0], (*vec)[1], (*vec)[2]));
}