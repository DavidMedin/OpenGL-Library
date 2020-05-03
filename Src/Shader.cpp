 
#include "Shader.h"
#define GRAPHICSLIBRARY_EXPORTS 1


Shader::Shader() {
	shader_Program = 0;
}

std::string Shader::ReadShader(const char* path) {
	std::string tmp;
	std::ifstream shaderFile;
	shaderFile.open(path, std::ios::in);
	std::string input;
	while (!shaderFile.eof()) {
		getline(shaderFile, input);
		if (input.find("uniform") != std::string::npos) {

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
		std::cout << error << "\n";
		
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
		std::cout << actualSize << "\n";
		std::cout << log << "\n";
		free(log);
	}

	GLCall(glValidateProgram(shader_program));
	rez=NULL;
	GLCall(glGetProgramiv(shader_program, GL_VALIDATE_STATUS, &rez));
	if (rez == NULL || rez != GL_TRUE) {
		GLsizei logSize = 255;
		GLchar* log = (GLchar*)malloc(sizeof(GLchar)*logSize);
		GLsizei actualSize=0;
		GLCall(glGetProgramInfoLog(shader_program, 255, &actualSize, log));
		std::cout << actualSize << "\n";
		std::cout << log << "\n";
		free(log);
	}
	GLCall(glDeleteShader(vertex_shader));
	GLCall(glDeleteShader(fragment_shader));
	if (geometryShader != nullptr) {
		GLCall(glDeleteShader(geometry_shader));
	}
	return shader_program;
}

void Shader::_UniformEquals(int location, void* value, unsigned int type,unsigned int count)
{
	UseShader();

	switch (type) {
	case GL_FLOAT_MAT4:
		GLCall(glUniformMatrix4fv(location, count, GL_FALSE, glm::value_ptr(*(glm::mat4*)value)));
		break;
	case GL_FLOAT_VEC3:
		GLCall(glUniform3fv(location,count, &(*(glm::vec3*)value)[0]));
		break;
	case GL_FLOAT_VEC2:
		GLCall(glUniform2fv(location,count, &(*(glm::vec3*)value)[0]));
		break;
	case GL_INT:
		GLCall(glUniform1iv(location,count, ((int*)value)));
		break;
	case GL_FLOAT:
		GLCall(glUniform1fv(location,count, ((float*)value)));
		break;
	default:
		printf("not a valid type\n"); //Type should be GL_FLOAT_MAT4,GL_FLOAT_VEC3 and the like
		break;
	}
}


Shader::Shader(const char* vertexPath, const char* fragmentPath,const char* geomPath) {
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
	GLCall(glUseProgram(shader_Program));
	
}

Shader::Shader(const char* shaderPath) {
	//read and find identifiers
	// -> @vertex,@fragment,@geometry
	//add to flag var
	char flags = 0;

	//parse the file for identifiers
	std::string tmp;
	std::ifstream shaderFile;
	shaderFile.open(shaderPath, std::ios::in);
	std::string input;

	std::string vertexText;
	std::string fragmentText;
	std::string geometryText;

	while (!shaderFile.eof()) {
		getline(shaderFile, input);
		if (input.find("uniform") != std::string::npos) {

		}

		if (input.find("@vertex") != std::string::npos) {//should be after the vertex shader
			vertexText = tmp;
			tmp.clear();
			//might clear VertexText
			continue;
		}
		else if (input.find("@fragment") != std::string::npos) {
			fragmentText = tmp;
			tmp.clear();
			continue;
		}
		else if (input.find("@geometry") != std::string::npos) {
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

	GLCall(glUseProgram(shader_Program));
}

Shader::~Shader() {
	GLCall(glDeleteProgram(shader_Program));
}

void Shader::UseShader()
{
	GLCall(glUseProgram(shader_Program));
}

unsigned int Shader::GetProgram()
{
	return shader_Program;
}

void Shader::UniformEquals(const char* uniform_Name,unsigned int type,void* value,unsigned int count)
//Type should be GL_FLOAT_MAT4,GL_FLOAT_VEC3 and the like
{
	UseShader(); 
	int uni_Pos = glGetUniformLocation(shader_Program, uniform_Name);
	if (uni_Pos == -1) {
		printf("%s is not a variable in your shader!\n", uniform_Name);
		int rez = NULL;
		GLCall(glGetProgramiv(shader_Program, GL_ACTIVE_UNIFORMS, &rez));
		printf("	There are %d active uniforms in shader!\n", rez);
		GLsizei length;// use size of the buffer
		GLint size; //sizeof the variable
		GLenum type;
		GLchar buffer[64]; //= (char*)malloc(sizeof(char)*64)
		for (GLuint i = 0; i < (unsigned int)rez; i++) {
			GLCall(glGetActiveUniform(shader_Program, i, sizeof(buffer), &length, &size, &type, buffer));
			printf("	Uniform: #%d | Type: %u | Name : %s\n", i, type, buffer);
		}
		return;
	}
	//basicly the same function but whatever
	_UniformEquals(uni_Pos, value, type,count);
}
// value must be an array of values, not an array of pointers
void Shader::ArrayUniformEquals(const char* uniformName, unsigned int type, void* value, unsigned int count)
{
	UseShader();
	//int* locs = (int*)malloc(sizeof(int) * count);
	char name[64];
	for (unsigned int i = 0; i < count; i++) {
		sprintf_s(name, "%s[%i]", uniformName, i);
		int loc = glGetUniformLocation(shader_Program, name);
		if (loc == -1) {
			std::cout << name << " is not a variable in your shader!\n";
		}
		else {
			_UniformEquals(loc, value, type,1);
		}
	}
}

// make uniform equals for arrays


void Shader::UniformMatrix(std::string uniform_Name, glm::mat4* matrix, unsigned int type) {
	int uni_Pos = glGetUniformLocation(shader_Program, uniform_Name.c_str());
	UseShader();
	GLCall(glUniformMatrix4fv(uni_Pos, 1, GL_FALSE, glm::value_ptr(*matrix)));
}
void Shader::UniformVector(std::string uniform_Name, glm::vec3* vec) {
	int uni_Pos = glGetUniformLocation(shader_Program, uniform_Name.c_str());
	UseShader();
	GLCall(glUniform3f(uni_Pos, (*vec)[0], (*vec)[1], (*vec)[2]));
}
void Shader::Reload() {
	unsigned int tmpShader = CreateShaderProgram(vertexContent.c_str(), fragmentContent.c_str(), NULL);
	shader_Program = tmpShader;
}