// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once
#include "GLIncludes.h"

#include "Graphics/GraphicsAPI.h"
#include "Render/Framebuffer.h"

class CRenderer;

class IGLAPI : public IGraphicsAPI
{
public:
	IGLAPI();
	~IGLAPI() override;
	virtual void Init() override;
	std::string GetGraphicsDisplayName() override;
	
	unsigned int CreateVertexBuffer(const MeshData& MeshData) override;
	TPointer<CTexture> GetTexture(const std::string& TextureSource, bool bAA) override;
	void BindVertexArray(const std::vector<float>& Vertices, const std::vector<uint32_t>& Indices, unsigned& Vao) override;
	TPointer<CRenderer> CreateNewRenderer() override;
	bool CreateShaderProgram(uint32_t& ProgramID, const char* VertexShaderFilename, const char* FragmentShaderFilename, const char* GeometryShaderFilename) override;
	bool CreateTessProgram(uint32_t& ProgramID, const char* VertexShaderFilename, const char* FragmentShaderFilename, const char* TessControlShaderFilename, const char* TessEvalShaderFilename) override;
	bool CreateComputeProgram(uint32_t& ProgramID, const char* ComputeShaderFilename) override;
	TPointer<IFramebuffer> CreateFramebuffer(const SFramebufferSpecification& Specification) override;
	
	void BindShader(uint32_t ShaderProgramID) override;
	void PassAttributeToShader(int32_t ShaderID, float Attribute) override;
	void PassAttributeToShader(int32_t ShaderID, int Attribute) override;
	void PassAttributeToShader(int32_t ShaderID, bool Attribute) override;
	void PassAttributeToShader(int32_t ShaderID, SVector Attribute) override;
	void PassAttributeToShader(int32_t ShaderID, SVector4 Attribute) override;
	void PassAttributeToShader(int32_t ShaderLocation, Matrix4 Attribute) override;
	void PassAttributeToShader(int32_t ShaderLocation, TPointer<CTexture> Attribute) override;

	void Clear(SVector ClearColour) override;
	void SetRenderViewportSize(const SVector2i InViewportSize) override;
	
private:
	std::string ReadShader(const char* filename);

	bool CreateShader(uint32_t& ShaderID, GLenum shaderType, std::string source, const char* shaderName);
};
