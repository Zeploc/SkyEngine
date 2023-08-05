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
	
	unsigned int CreateBuffer(const MeshData& MeshData) override;
	TPointer<CTexture> GetTexture(const std::string& TextureSource, bool bAA) override;
	void BindArray(const std::vector<float>& Vertices, const std::vector<uint32_t>& Indices, unsigned& Vao) override;
	TPointer<IGraphicsInstance> CreateNewInstance() override;
	bool CreateShaderProgram(uint32_t& ProgramID, const char* VertexShaderFilename, const char* FragmentShaderFilename, const char* GeometryShaderFilename) override;
	bool CreateTessProgram(uint32_t& ProgramID, const char* VertexShaderFilename, const char* FragmentShaderFilename, const char* TessControlShaderFilename, const char* TessEvalShaderFilename) override;
	bool CreateComputeProgram(uint32_t& ProgramID, const char* ComputeShaderFilename) override;
	
private:
	std::string ReadShader(const char* filename);

	bool CreateShader(uint32_t& ShaderID, GLenum shaderType, std::string source, const char* shaderName);
};
