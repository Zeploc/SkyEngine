#include "GLAPI.h"

#include <soil/SOIL2.h>

#include "GLInstance.h"
#include "Platform/Window/GraphicsWindow.h"
#include "Render/Shader.h"
#include "System/LogManager.h"

IGLAPI::IGLAPI()
{	
}

IGLAPI::~IGLAPI()
{	
	glfwTerminate();
}

std::string IGLAPI::GetGraphicsDisplayName()
{
	return "OpenGL";
}

unsigned IGLAPI::CreateBuffer(const char* TextureSource, TextureData& Texture, bool bAA, bool bHasNormals)
{
	GLuint vao;
	GLuint vbo;
	GLuint ebo;

	Texture = GetTexture(TextureSource, bAA);

	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ebo);

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	if (TextureSource != "")
	{
		if (bHasNormals)
		{
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 12 * sizeof(GLfloat), static_cast<GLvoid*>(0));
			glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 12 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
			glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 12 * sizeof(GLfloat), (GLvoid*)(7 * sizeof(GLfloat)));
			glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 12 * sizeof(GLfloat), (GLvoid*)(9 * sizeof(GLfloat)));
			glEnableVertexAttribArray(0);
			glEnableVertexAttribArray(1);
			glEnableVertexAttribArray(2);
			glEnableVertexAttribArray(3);
		}
		else
		{
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), static_cast<GLvoid*>(0));
			glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
			glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (GLvoid*)(7 * sizeof(GLfloat)));
			glEnableVertexAttribArray(0);
			glEnableVertexAttribArray(1);
			glEnableVertexAttribArray(2);
		}
	}
	else
	{
		if (bHasNormals)
		{
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 10 * sizeof(GLfloat), static_cast<GLvoid*>(0));
			glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 10 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
			glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 10 * sizeof(GLfloat), (GLvoid*)(7 * sizeof(GLfloat)));
			glEnableVertexAttribArray(0);
			glEnableVertexAttribArray(1);
			glEnableVertexAttribArray(3);
		}
		else
		{
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), static_cast<GLvoid*>(0));
			glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
			glEnableVertexAttribArray(0);
			glEnableVertexAttribArray(1);
		}
	}

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBindVertexArray(0);

	return vao;
}

TextureData IGLAPI::GetTexture(const char* TextureSource, bool bAA)
{
	TextureData Texture;

	bool bTextureExists = false;
	for (auto& it : Shader::Textures)
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
			glGenTextures(1, &Texture.TextureID);
			glBindTexture(GL_TEXTURE_2D, Texture.TextureID);

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

			Texture.Path = TextureSource;
			unsigned char* image = SOIL_load_image(TextureSource, &Texture.Width, &Texture.Height, nullptr, SOIL_LOAD_RGBA);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, Texture.Width, Texture.Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);

			glGenerateMipmap(GL_TEXTURE_2D);
			SOIL_free_image_data(image);
			glBindTexture(GL_TEXTURE_2D, 0);

			Shader::Textures.insert(std::pair(TextureSource, Texture));
			LogManager::GetInstance()->DisplayLogMessage("Adding Texture, \"" + std::string(TextureSource) + "\", Total Texture Count : " + std::to_string(Shader::Textures.size()));
		}
		else
		{
			glBindTexture(GL_TEXTURE_2D, Texture.TextureID);
		}
	}

	return Texture;	
}

void IGLAPI::BindArray(const std::vector<float>& Vertices, const std::vector<uint32_t>& Indices, unsigned& Vao, bool bCreateVAO)
{	
	TextureData TempTexture;
	if (bCreateVAO)
	{
		Vao = CreateBuffer("", TempTexture, true);
	}

	glBindVertexArray(Vao);
	glBufferData(GL_ARRAY_BUFFER, Vertices.size() * sizeof(float), Vertices.data(), GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, Indices.size() * sizeof(uint32_t), Indices.data(), GL_STATIC_DRAW);
	glBindVertexArray(0);
}

Pointer<IGraphicsInstance> IGLAPI::CreateNewInstance()
{
	// Create instance for basic setup before window
	return std::make_shared<GLInstance>();
}
