// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once
#include "Graphics/GraphicsAPI.h"

class VulkanAPI : public IGraphicsAPI
{
public:
	~VulkanAPI() override;
	std::string GetGraphicsDisplayName() override;
	void ImGuiInit() override;
	TPointer<CTexture> GetTexture(const std::string& TextureSource, bool bAA) override;
	bool CreateShaderProgram(uint32_t& ProgramID, const char* VertexShaderFilename, const char* FragmentShaderFilename,
		const char* GeometryShaderFilename) override;
	bool CreateTessProgram(uint32_t& ProgramID, const char* VertexShaderFilename, const char* FragmentShaderFilename,
		const char* TessControlShaderFilename, const char* TessEvalShaderFilename) override;
	bool CreateComputeProgram(uint32_t& ProgramID, const char* ComputeShaderFilename) override;
	TPointer<IFramebuffer> CreateFramebuffer(const SFramebufferSpecification& Specification) override;
	unsigned CreateVertexBuffer(const MeshData& MeshData) override;
	void BindVertexArray(const std::vector<float>& vertices, const std::vector<uint32_t>& indices,
		unsigned& vao) override;
	void RenderMesh(TPointer<CMeshComponent> Mesh) override;
	void CleanupMesh(TPointer<CMeshComponent> Mesh) override;
	void ApplyMVP(uint32_t Program, Matrix4 View, Matrix4 Projection, STransform Transform) override;
	void RenderImGui() override;
	void BindShader(uint32_t ShaderProgramID) override;
	void Clear(SVector ClearColour) override;
	void SetRenderViewportSize(const SVector2i InViewportSize) override;
	void SetWireframeMode(bool bInWireframeEnabled) override;
	void ApplyMaterialFlags(TPointer<CMaterialInterface> InMaterial) override;
	int32_t GetAttributeLocation(const uint32_t ShaderProgram, std::string AttributeName) override;
	void PassAttributeToShader(int32_t ShaderLocation, float Attribute) override;
	void PassAttributeToShader(int32_t ShaderLocation, int Attribute) override;
	void PassAttributeToShader(int32_t ShaderLocation, bool Attribute) override;
	void PassAttributeToShader(int32_t ShaderLocation, SVector Attribute) override;
	void PassAttributeToShader(int32_t ShaderLocation, SVector4 Attribute) override;
	void PassAttributeToShader(int32_t ShaderLocation, Matrix4 Attribute) override;
	void PassAttributeToShader(int32_t ShaderLocation, TPointer<CTexture> Attribute) override;
};
