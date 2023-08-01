// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once
#include "GLIncludes.h"

#include "Graphics/GraphicsAPI.h"

class IGLAPI : public IGraphicsAPI
{
public:
	IGLAPI();
	~IGLAPI() override;
	std::string GetGraphicsDisplayName() override;
	
	unsigned int CreateBuffer(Pointer<Material> Material) override;
	TextureData GetTexture(const char* TextureSource, bool bAA) override;
	void BindArray(const std::vector<float>& Vertices, const std::vector<uint32_t>& Indices, unsigned& Vao) override;
	Pointer<IGraphicsInstance> CreateNewInstance() override;
	unsigned CreateProgram(const char* VertexShaderFilename, const char* FragmentShaderFilename, const char* GeometryShaderFilename) override;
	unsigned CreateTessProgram(const char* VertexShaderFilename, const char* FragmentShaderFilename, const char* TessControlShaderFilename, const char* TessEvalShaderFilename) override;
	unsigned CreateComputeProgram(const char* ComputeShaderFilename) override;
	
private:
	std::string ReadShader(const char* filename);

	GLuint CreateShader(GLenum shaderType, std::string source, const char* shaderName);
};
