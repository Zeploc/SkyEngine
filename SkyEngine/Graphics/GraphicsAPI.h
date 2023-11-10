// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once

#include "Render/Framebuffer.h"
#include "Core/Core.h"

#include "Math/Vector2.h"
#include "Render/Meshes/MeshData.h"

class Matrix4;
class CEngineWindow;
struct STransform;
class CMeshComponent;
class CMaterialInterface;
class CRenderer;
class UIElement;
class Entity;
class IGraphicsWindow;
class CShader;
class CTexture;

enum class EGraphicsAPI
{
	OPENGL,
	VULKAN,
	DIRECTX
};

class ENGINE_API IGraphicsAPI
{
public:
	virtual ~IGraphicsAPI() = default;
	virtual void Init() = 0;
	
	virtual std::string GetGraphicsDisplayName() = 0;

	static TPointer<IGraphicsAPI> CreateGraphicsAPI(EGraphicsAPI APIType);
	
	virtual void ImGuiInit() = 0;
	
	virtual unsigned int CreateVertexBuffer(const MeshData& MeshData) = 0;
	virtual TPointer<CTexture> GetTexture(const std::string& TextureSource, bool bAA = true) = 0;
	/* Create a new VAO if -1, otherwise will use it  */
	virtual void BindVertexArray(const std::vector<float>& vertices, const std::vector<uint32_t>& indices, unsigned& vao) = 0;
	
	virtual bool CreateShaderProgram(uint32_t& ProgramID, const char* VertexShaderFilename, const char* FragmentShaderFilename, const char* GeometryShaderFilename) = 0;

	virtual bool CreateTessProgram(uint32_t& ProgramID, const char* VertexShaderFilename,
	                               const char* FragmentShaderFilename, const char* TessControlShaderFilename, const char* TessEvalShaderFilename) = 0;

	virtual bool CreateComputeProgram(uint32_t& ProgramID, const char* ComputeShaderFilename) = 0;
	virtual void RenderMesh(TPointer<CMeshComponent> Mesh) = 0;	
	virtual void CleanupMesh(TPointer<CMeshComponent> Mesh) = 0;
	virtual void ApplyMVP(uint32_t Program, Matrix4 View, Matrix4 Projection, STransform Transform) = 0;

	virtual void RenderImGui() = 0;

	virtual TPointer<IFramebuffer> CreateFramebuffer(const SFramebufferSpecification& Specification) = 0;	

	virtual void BindShader(uint32_t ShaderProgramID) = 0;
	virtual void Clear(SVector ClearColour) = 0;
	/* Where Y=0 is the top of the screen */
	virtual void SetRenderViewportSize(const SVector2i InViewportSize) = 0;
	// /* Where Y=0 is the top of the screen */
	// virtual void SetRenderViewport(const SVector2i InViewportPosition, const SVector2i InViewportSize) = 0;
	virtual void SetWireframeMode(bool bInWireframeEnabled) = 0;
	virtual void ApplyMaterialFlags(TPointer<CMaterialInterface> InMaterial) = 0;
	
	// template <typename T>
	// void PassAttributeToShader(int32_t ShaderLocation, T Attribute);

	virtual int32_t GetAttributeLocation(const uint32_t ShaderProgram, std::string AttributeName) = 0;
	virtual void PassAttributeToShader(int32_t ShaderLocation, float Attribute) = 0;
	virtual void PassAttributeToShader(int32_t ShaderLocation, int Attribute) = 0;
	virtual void PassAttributeToShader(int32_t ShaderLocation, bool Attribute) = 0;
	virtual void PassAttributeToShader(int32_t ShaderLocation, SVector Attribute) = 0;
	virtual void PassAttributeToShader(int32_t ShaderLocation, SVector4 Attribute) = 0;
	virtual void PassAttributeToShader(int32_t ShaderLocation, Matrix4 Attribute) = 0;
	virtual void PassAttributeToShader(int32_t ShaderLocation, TPointer<CTexture> Attribute) = 0;
};
