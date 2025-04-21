// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once
#include <vulkan/vulkan_core.h>

#include "imgui_impl_vulkan.h"
#include "Graphics/GraphicsAPI.h"

class CVulkanAPI : public IGraphicsAPI
{
public:
	~CVulkanAPI() override;

	void Init() override;
	void ImGuiInit() override;
	
	std::string GetGraphicsDisplayName() override;
	bool GenerateTexture(TAssetObjectPointer<CTexture> Texture, bool bAA) override;
	bool CreateShaderProgram(uint32_t& ProgramID, const char* VertexShaderFilename, const char* FragmentShaderFilename,
		const char* GeometryShaderFilename) override;
	bool CreateTessProgram(uint32_t& ProgramID, const char* VertexShaderFilename, const char* FragmentShaderFilename,
		const char* TessControlShaderFilename, const char* TessEvalShaderFilename) override;
	bool CreateComputeProgram(uint32_t& ProgramID, const char* ComputeShaderFilename) override;
	THardPointer<IFramebuffer> CreateFramebuffer(const SFramebufferSpecification& Specification) override;
	unsigned CreateVertexBuffer(const CMesh& MeshData) override;
	void BindVertexArray(const std::vector<float>& vertices, const std::vector<uint32_t>& indices,
		unsigned& vao) override;
	void RenderMesh(ISceneVisual* SceneVisual) override;
	void CleanupMesh(ISceneVisual* SceneVisual) override;
	
	void RenderImGui() override;
	void ImGuiNewFrame() override;
	void SwapBuffers() override;
	
	void RenderLines(ISceneVisual* SceneVisual, float Thickness) override;
	void ApplyMVP(uint32_t Program, Matrix4 View, Matrix4 Projection, STransform Transform) override;
	void BindShader(uint32_t ShaderProgramID) override;
	void Clear(SVector ClearColour) override;
	void SetRenderViewportSize(const SVector2i InViewportSize) override;
	void SetWireframeMode(bool bInWireframeEnabled) override;
	void ApplyMaterialFlags(THardPointer<CMaterialInterface> InMaterial) override;
	int32_t GetAttributeLocation(const uint32_t ShaderProgram, std::string AttributeName) override;
	void PassAttributeToShader(int32_t ShaderLocation, float Attribute) override;
	void PassAttributeToShader(int32_t ShaderLocation, int Attribute) override;
	void PassAttributeToShader(int32_t ShaderLocation, bool Attribute) override;
	void PassAttributeToShader(int32_t ShaderLocation, SVector Attribute) override;
	void PassAttributeToShader(int32_t ShaderLocation, SVector4 Attribute) override;
	void PassAttributeToShader(int32_t ShaderLocation, Matrix4 Attribute) override;
	void PassAttributeToShader(int32_t ShaderLocation, TAssetObjectPointer<CTexture> Attribute) override;

private:
	VkPhysicalDevice SetupVulkan_SelectPhysicalDevice() const;
	void SetupVulkanWindow(VkSurfaceKHR Surface, int Width, int Height);
	
	// Data
	VkAllocationCallbacks*		Allocator = nullptr;
	VkInstance					Instance = VK_NULL_HANDLE;
	VkPhysicalDevice			PhysicalDevice = VK_NULL_HANDLE;
	VkDevice					Device = VK_NULL_HANDLE;
	uint32_t					QueueFamily = static_cast<uint32_t>(-1);
	VkQueue						Queue = VK_NULL_HANDLE;
	VkDebugReportCallbackEXT	DebugReport = VK_NULL_HANDLE;
	VkPipelineCache				PipelineCache = VK_NULL_HANDLE;
	VkDescriptorPool			DescriptorPool = VK_NULL_HANDLE;

	ImGui_ImplVulkanH_Window	ImGuiWindow;
	int32_t						MinImageCount = 2;
	bool						SwapChainRebuild = false;
};
