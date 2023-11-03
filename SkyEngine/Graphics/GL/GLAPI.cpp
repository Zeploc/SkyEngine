// Copyright Skyward Studios, Inc. All Rights Reserved.

#include "SEPCH.h"
#include "GLAPI.h"

#include <fstream>
#include <soil/SOIL2.h>

#include "GLFramebuffer.h"
#include "GLInstance.h"
#include "imgui_impl_opengl3.h"
#include "Render/Shaders/ShaderManager.h"
#include "System/LogManager.h"

IGLAPI::IGLAPI()
{	
}

IGLAPI::~IGLAPI()
{	
}

std::string IGLAPI::GetGraphicsDisplayName()
{
	return "OpenGL";
}

unsigned IGLAPI::CreateBuffer(const MeshData& MeshData)
{
	// TODO: Move buffer/stride determination to shader
	
	GLuint vao;
	GLuint vbo;
	GLuint ebo;

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ebo);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	if (MeshData.HasUVData())
	{
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), static_cast<GLvoid*>(0));
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
	}
	else
	{
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), static_cast<GLvoid*>(0));
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
	}
	
	// const GLsizei StrideSize = bHasTexture ? 12 : 10;
	// glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, StrideSize * sizeof(GLfloat), static_cast<GLvoid*>(nullptr));
	// glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, StrideSize * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	// glEnableVertexAttribArray(0);
	// glEnableVertexAttribArray(1);
	//
	// if (bHasTexture)
	// {
	// 	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, StrideSize * sizeof(GLfloat), (GLvoid*)(7 * sizeof(GLfloat)));
	// 	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, StrideSize * sizeof(GLfloat), (GLvoid*)(9 * sizeof(GLfloat)));
	// 	glEnableVertexAttribArray(2);
	// 	glEnableVertexAttribArray(3);
	// }
	// else
	// {
	// 	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, StrideSize * sizeof(GLfloat), (GLvoid*)(7 * sizeof(GLfloat)));
	// 	glEnableVertexAttribArray(3);
	// }

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBindVertexArray(0);

	return vao;
}

TPointer<CTexture> IGLAPI::GetTexture(const std::string& TextureSource, bool bAA)
{
	// TODO: Can have a derived OpenGlTexture for custom aspects of the texture for OpenGL
	TPointer<CTexture> Texture;

	bool bTextureExists = false;
	for (auto& it : ShaderManager::Textures)
	{
		if (it.first == TextureSource)
		{
			Texture = it.second;
			bTextureExists = true;
		}
	}

	if (TextureSource != "")
	{
		if (bTextureExists == false)
		{
			Texture = std::make_shared<CTexture>();
			glGenTextures(1, &Texture->TextureID);
			glBindTexture(GL_TEXTURE_2D, Texture->TextureID);
			Texture->bBound = true;

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			if (bAA)
			{
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			}
			else
			{
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			}

			Texture->Path = TextureSource;
			unsigned char* image = SOIL_load_image(TextureSource.c_str(), &Texture->Width, &Texture->Height, nullptr, SOIL_LOAD_RGBA);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, Texture->Width, Texture->Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);

			glGenerateMipmap(GL_TEXTURE_2D);
			SOIL_free_image_data(image);
			glBindTexture(GL_TEXTURE_2D, 0);

			ShaderManager::Textures.insert(std::pair(TextureSource, Texture));
			CLogManager::GetInstance()->DisplayLogMessage("Adding Texture, \"" + std::string(TextureSource) + "\", Total Texture Count : " + std::to_string(ShaderManager::Textures.size()));
		}
		else
		{
			glBindTexture(GL_TEXTURE_2D, Texture->TextureID);
		}
	}

	return Texture;	
}

void IGLAPI::BindArray(const std::vector<float>& Vertices, const std::vector<uint32_t>& Indices, unsigned& Vao)
{
	glBindVertexArray(Vao);
	glBufferData(GL_ARRAY_BUFFER, Vertices.size() * sizeof(float), Vertices.data(), GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, Indices.size() * sizeof(uint32_t), Indices.data(), GL_STATIC_DRAW);
	glBindVertexArray(0);
}

TPointer<IGraphicsInstance> IGLAPI::CreateNewInstance()
{
	// Create instance for basic setup before window
	return std::make_shared<GLInstance>();
}

std::string IGLAPI::ReadShader(const char* filename)
{
	std::string shaderCode;
	std::ifstream file(filename, std::ios::in);

	ensure (file.good(), (std::string("Can't read file ") + filename).c_str());

	file.seekg(0, std::ios::end);
	shaderCode.resize(file.tellg());
	file.seekg(0, std::ios::beg);
	file.read(&shaderCode[0], shaderCode.size());
	file.close();
	return shaderCode;
}

bool IGLAPI::CreateShader(uint32_t& ShaderID, GLenum shaderType, std::string source, const char* shaderName)
{
	// TODO: Change to bool return
	int compile_result = 0;

	ShaderID = glCreateShader(shaderType);
	const char* shader_code_ptr = source.c_str();
	const int shader_code_size = static_cast<int>(source.size());

	glShaderSource(ShaderID, 1, &shader_code_ptr, &shader_code_size);
	glCompileShader(ShaderID);
	glGetShaderiv(ShaderID, GL_COMPILE_STATUS, &compile_result);

	//check for errors
	if (compile_result == GL_FALSE)
	{
		int info_log_length = 0;
		glGetShaderiv(ShaderID, GL_INFO_LOG_LENGTH, &info_log_length);
		std::vector<char> shader_log(info_log_length);
		glGetShaderInfoLog(ShaderID, info_log_length, nullptr, &shader_log[0]);
		std::cerr << "ERROR compiling shader: " << shaderName << std::endl << &shader_log[0] << std::endl;
		return false;
	}
	return true;
}

bool IGLAPI::CreateShaderProgram(uint32_t& ProgramID, const char* vertexShaderFilename, const char* fragmentShaderFilename, const char* geometryShaderFilename)
{
	//read the shader files and save the code
	std::string vertex_shader_code = ReadShader(vertexShaderFilename);
	std::string fragment_shader_code = ReadShader(fragmentShaderFilename);

	GLuint vertex_shader;
	if (!CreateShader(vertex_shader, GL_VERTEX_SHADER, vertex_shader_code,vertexShaderFilename))
	{
		return false;
	}
	GLuint fragment_shader;
	if (!CreateShader(fragment_shader, GL_FRAGMENT_SHADER, fragment_shader_code,fragmentShaderFilename))
	{
		return false;
	}
	GLuint geometry_shader;

	if (std::string(geometryShaderFilename) != "")
	{
		std::string geometry_shader_code = ReadShader(geometryShaderFilename);
		if (!CreateShader(geometry_shader, GL_GEOMETRY_SHADER, geometry_shader_code,geometryShaderFilename))
		{
			return false;
		}
	}

	int link_result = 0;
	//create the program handle, attatch the shaders and link it
	ProgramID = glCreateProgram();
	glAttachShader(ProgramID, vertex_shader);
	glAttachShader(ProgramID, fragment_shader);
	if (std::string(geometryShaderFilename) != "")
	{
		glAttachShader(ProgramID, geometry_shader);
	}

	glLinkProgram(ProgramID);
	glGetProgramiv(ProgramID, GL_LINK_STATUS, &link_result);
	//check for link errors
	if (link_result == GL_FALSE)
	{
		int info_log_length = 0;
		glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &info_log_length);
		std::vector<char> program_log(info_log_length);
		glGetProgramInfoLog(ProgramID, info_log_length, nullptr, &program_log[0]);
		std::cout << "Shader Loader : LINK ERROR" << std::endl << &program_log[0] << std::endl;
		return false;
	}
	return true;
}

bool IGLAPI::CreateTessProgram(uint32_t& ProgramID, const char* vertexShaderFilename, const char* fragmentShaderFilename, const char* TessControlShaderFilename, const char* TessEvalShaderFilename)
{
	//read the shader files and save the code
	std::string vertex_shader_code = ReadShader(vertexShaderFilename);
	std::string fragment_shader_code = ReadShader(fragmentShaderFilename);
	std::string tess_control_shader_code = ReadShader(TessControlShaderFilename);
	std::string tess_eval_shader_code = ReadShader(TessEvalShaderFilename);

	GLuint vertex_shader;
	if (!CreateShader(vertex_shader, GL_VERTEX_SHADER, vertex_shader_code,"vertex shader"))
	{
		return false;
	}
	GLuint fragment_shader;
	if (!CreateShader(fragment_shader, GL_FRAGMENT_SHADER, fragment_shader_code,"fragment shader"))
	{
		return false;
	}
	GLuint tess_control_shader;
	if (!CreateShader(tess_control_shader, GL_TESS_CONTROL_SHADER, tess_control_shader_code,"tess control shader"))
	{
		return false;
	}
	GLuint tess_eval_shader;
	if (!CreateShader(tess_eval_shader, GL_TESS_EVALUATION_SHADER, tess_eval_shader_code,"tess eval shader"))
	{
		return false;
	}

	int link_result = 0;
	//create the program handle, attatch the shaders and link it
	ProgramID = glCreateProgram();
	glAttachShader(ProgramID, vertex_shader);
	glAttachShader(ProgramID, fragment_shader);
	glAttachShader(ProgramID, tess_control_shader);
	glAttachShader(ProgramID, tess_eval_shader);

	glLinkProgram(ProgramID);
	glGetProgramiv(ProgramID, GL_LINK_STATUS, &link_result);
	//check for link errors
	if (link_result == GL_FALSE)
	{
		int info_log_length = 0;
		glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &info_log_length);
		std::vector<char> program_log(info_log_length);
		glGetProgramInfoLog(ProgramID, info_log_length, nullptr, &program_log[0]);
		std::cout << "Shader Loader : LINK ERROR" << std::endl << &program_log[0] << std::endl;
		return false;
	}
	return true;
}

bool IGLAPI::CreateComputeProgram(uint32_t& ProgramID, const char* ComputeShaderFilename)
{
	//read the shader files and save the code
	std::string compute_shader_code = ReadShader(ComputeShaderFilename);

	GLuint compute_shader;
	if (!CreateShader(compute_shader, GL_COMPUTE_SHADER, compute_shader_code,"compute shader"))
	{
		return false;
	}

	int link_result = 0;
	//create the program handle, attatch the shaders and link it
	ProgramID = glCreateProgram();
	glAttachShader(ProgramID, compute_shader);

	glLinkProgram(ProgramID);
	glGetProgramiv(ProgramID, GL_LINK_STATUS, &link_result);
	//check for link errors
	if (link_result == GL_FALSE)
	{
		int info_log_length = 0;
		glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &info_log_length);
		std::vector<char> program_log(info_log_length);
		glGetProgramInfoLog(ProgramID, info_log_length, nullptr, &program_log[0]);
		std::cout << "Shader Loader : LINK ERROR" << std::endl << &program_log[0] << std::endl;
		return false;
	}
	return true;
}

TPointer<IFramebuffer> IGLAPI::CreateFramebuffer(const SFramebufferSpecification& Specification)
{
	return CreatePointer<GLFramebuffer>(Specification);
}

void IGLAPI::ImGuiInit()
{
	ImGui_ImplOpenGL3_Init("#version 410 core");
}
