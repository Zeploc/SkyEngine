// Copyright Skyward Studios, Inc. All Rights Reserved.

#include "SEPCH.h"
#include "GLAPI.h"

#include <fstream>
#include <glm/gtc/type_ptr.inl>
#include <soil/SOIL2.h>

#include "GLFramebuffer.h"
#include "imgui_impl_opengl3.h"
#include "Math/Matrix.h"
#include "Render/Materials/InternalMaterial.h"
#include "Render/Meshes/Mesh.h"
#include "Render/Shaders/ShaderManager.h"
#include "System/LogManager.h"

void GLAPIENTRY MessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam);

IGLAPI::IGLAPI()
{	
}

IGLAPI::~IGLAPI()
{	
}

void IGLAPI::Init()
{
	GLenum InitResult = glewInit();
	// OpenGL init
	if (InitResult != GLEW_OK)
	{
		CLogManager::GetInstance()->DisplayLogError("Failed to init glew");
		return;
	}

	glEnable(GL_DEBUG_OUTPUT);
	// TODO: Debug messages
	//glDebugMessageCallback(MessageCallback, NULL);
	// glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, NULL, GL_TRUE);
			
	glCullFace(GL_BACK); // Cull the Back faces
	glFrontFace(GL_CW); // Front face is Clockwise order
	glEnable(GL_CULL_FACE); // Turn on the back face culling
		
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

std::string IGLAPI::GetGraphicsDisplayName()
{
	return "OpenGL";
}

unsigned IGLAPI::CreateVertexBuffer(const MeshData& MeshData)
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

void IGLAPI::BindVertexArray(const std::vector<float>& Vertices, const std::vector<uint32_t>& Indices, unsigned& Vao)
{
	glBindVertexArray(Vao);
	glBufferData(GL_ARRAY_BUFFER, Vertices.size() * sizeof(float), Vertices.data(), GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, Indices.size() * sizeof(uint32_t), Indices.data(), GL_STATIC_DRAW);
	glBindVertexArray(0);
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

void IGLAPI::RenderMesh(TPointer<CMeshComponent> Mesh)
{	
	glBindVertexArray(Mesh->vao);
	glDrawElements(GL_TRIANGLES, Mesh->IndicesCount, GL_UNSIGNED_INT, nullptr);
	glBindVertexArray(0);
}

void IGLAPI::CleanupMesh(TPointer<CMeshComponent> Mesh)
{
	if (!Mesh)
	{
		return;
	}
	glDeleteVertexArrays(1, &Mesh->vao);
	// TODO: Look into further cleanup
}

void IGLAPI::ApplyMVP(uint32_t Program, Matrix4 View, Matrix4 Projection, STransform Transform)
{
	glm::mat4 ModelMatrix = Transform.GetModelMatrix();
	glUniformMatrix4fv(glGetUniformLocation(Program, "model"), 1, GL_FALSE, value_ptr(ModelMatrix));
	glUniformMatrix4fv(glGetUniformLocation(Program, "view"), 1, GL_FALSE, glm::value_ptr(View.ToGLM()));//view));//
	glUniformMatrix4fv(glGetUniformLocation(Program, "proj"), 1, GL_FALSE, glm::value_ptr(Projection.ToGLM()));

	glm::mat4 MVP = Projection.ToGLM() * View.ToGLM() * ModelMatrix;
	glUniformMatrix4fv(glGetUniformLocation(Program, "MVP"), 1, GL_FALSE, value_ptr(MVP));
}

void IGLAPI::RenderImGui()
{
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());	
}

void IGLAPI::BindShader(uint32_t ShaderProgramID)
{
	// TODO: Check if overhead and not change if current shader program is active
	glUseProgram(ShaderProgramID);	
	glFrontFace(GL_CW);
	// TODO: properly work out where blending should be changed
	// In case it was enabled
	glDisable(GL_BLEND);
	// Clear previous texture?
	glBindTexture(GL_TEXTURE_2D, 0);
}

int32_t IGLAPI::GetAttributeLocation(const uint32_t ShaderProgram, std::string AttributeName)
{	
	return glGetUniformLocation(ShaderProgram, AttributeName.c_str());
}

void IGLAPI::PassAttributeToShader(int32_t ShaderID, float Attribute)
{
	glUniform1f(ShaderID, Attribute);
}

void IGLAPI::PassAttributeToShader(int32_t ShaderID, int Attribute)
{
	glUniform1i(ShaderID, Attribute);
}

void IGLAPI::PassAttributeToShader(int32_t ShaderID, bool Attribute)
{
	glUniform1i(ShaderID, Attribute);
}

void IGLAPI::PassAttributeToShader(int32_t ShaderID, SVector Attribute)
{
	glUniform3fv(ShaderID, 1, Attribute.ToValuePtr());
}

void IGLAPI::PassAttributeToShader(int32_t ShaderID, SVector4 Attribute)
{
	glUniform4fv(ShaderID, 1, Attribute.ToValuePtr());
}

void IGLAPI::PassAttributeToShader(int32_t ShaderLocation, Matrix4 Attribute)
{
	glUniformMatrix4fv(ShaderLocation, 1, GL_FALSE, value_ptr(Attribute.ToGLM()));
}

void IGLAPI::PassAttributeToShader(int32_t ShaderLocation, TPointer<CTexture> Attribute)
{
	if (!Attribute || !Attribute->IsValid())
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, 0);
		return;
	}
	// Use shader location for index?
	glEnable(GL_BLEND);
	// TODO: Handle multiple
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, Attribute->TextureID);	
}

void IGLAPI::Clear(SVector ClearColour)
{
	glClearColor(ClearColour.R, ClearColour.G, ClearColour.B, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void IGLAPI::SetRenderViewportSize(const SVector2i InViewportSize)
{
	// TODO: Option to expoose/override viewport position
	// Fill whole window with viewport by default
	// Y flipped since gl uses y=0 as bottom of screen instead of top
	glViewport(0, 0, InViewportSize.X, InViewportSize.Y);	
}

void IGLAPI::SetWireframeMode(bool bInWireframeEnabled)
{
	glPolygonMode(GL_FRONT_AND_BACK, bInWireframeEnabled ? GL_LINE : GL_FILL);
}

void IGLAPI::ApplyMaterialFlags(TPointer<CMaterialInterface>InMaterial)
{
	if (!InMaterial->bTwoSided)
	{
		glEnable(GL_CULL_FACE);
	}
	else
	{
		glDisable(GL_CULL_FACE);
	}
	if (InMaterial->bDepthTest)
	{
		glEnable(GL_DEPTH_TEST);
	}
	else
	{
		glDisable(GL_DEPTH_TEST);
	}
}

// GLenum glCheckError_(const char *file, int line)
// {
// 	GLenum errorCode;
// 	while ((errorCode = glGetError()) != GL_NO_ERROR)
// 	{
// 		std::string error;
// 		switch (errorCode)
// 		{
// 			case GL_INVALID_ENUM:                  error = "INVALID_ENUM"; break;
// 			case GL_INVALID_VALUE:                 error = "INVALID_VALUE"; break;
// 			case GL_INVALID_OPERATION:             error = "INVALID_OPERATION"; break;
// 			case GL_STACK_OVERFLOW:                error = "STACK_OVERFLOW"; break;
// 			case GL_STACK_UNDERFLOW:               error = "STACK_UNDERFLOW"; break;
// 			case GL_OUT_OF_MEMORY:                 error = "OUT_OF_MEMORY"; break;
// 			case GL_INVALID_FRAMEBUFFER_OPERATION: error = "INVALID_FRAMEBUFFER_OPERATION"; break;
// 		}
// 		std::cout << error << " | " << file << " (" << line << ")" << std::endl;
// 	}
// 	return errorCode;
// }
// #define glCheckError() glCheckError_(__FILE__, __LINE__)
void GLAPIENTRY MessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
{
	std::string sourceStr, typeStr, severityStr;
	// Convert GLenum parameters to strings

	// printf("%s:%s[%s](%d): %s\n", sourceStr, typeStr, severityStr, id, message);


	const std::string OutputString = std::format("GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
												 ( type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : "" ),
												 type, severity, message);
	CLogManager::GetInstance()->DisplayLogError(OutputString);
	// fprintf( stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
	//          ( type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : "" ),
	//          type, severity, message );
}
