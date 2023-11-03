// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once
#include "Graphics/GraphicsAPI.h"

class VulkanAPI : public IGraphicsAPI
{
public:
	~VulkanAPI() override;
	std::string GetGraphicsDisplayName() override;
	TPointer<IGraphicsInstance> CreateNewInstance() override;
	void ImGuiInit() override;
	unsigned CreateBuffer(const MeshData& MeshData) override;
	TPointer<CTexture> GetTexture(const std::string& TextureSource, bool bAA) override;
	void BindArray(const std::vector<float>& vertices, const std::vector<uint32_t>& indices, unsigned& vao) override;
	bool CreateShaderProgram(uint32_t& ProgramID, const char* VertexShaderFilename, const char* FragmentShaderFilename,
		const char* GeometryShaderFilename) override;
	bool CreateTessProgram(uint32_t& ProgramID, const char* VertexShaderFilename, const char* FragmentShaderFilename,
		const char* TessControlShaderFilename, const char* TessEvalShaderFilename) override;
	bool CreateComputeProgram(uint32_t& ProgramID, const char* ComputeShaderFilename) override;
	TPointer<IFramebuffer> CreateFramebuffer(const SFramebufferSpecification& Specification) override;
};
