// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once

#include "Core/Core.h"

#include "Math/Vector2.h"
#include "Render/Meshes/MeshData.h"

class IGraphicsInstance;
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
	
	virtual std::string GetGraphicsDisplayName() = 0;

	static TPointer<IGraphicsAPI> CreateGraphicsAPI(EGraphicsAPI APIType);

	virtual TPointer<IGraphicsInstance> CreateNewInstance() = 0;
	
	virtual unsigned int CreateBuffer(const MeshData& MeshData) = 0;
	virtual TPointer<CTexture> GetTexture(const std::string& TextureSource, bool bAA = true) = 0;
	/* Create a new VAO if -1, otherwise will use it  */
	virtual void BindArray(const std::vector<float>& vertices, const std::vector<uint32_t>& indices, unsigned& vao) = 0;
	
	virtual bool CreateShaderProgram(uint32_t& ProgramID, const char* VertexShaderFilename, const char* FragmentShaderFilename, const char* GeometryShaderFilename) = 0;

	virtual bool CreateTessProgram(uint32_t& ProgramID, const char* VertexShaderFilename,
	                               const char* FragmentShaderFilename, const char* TessControlShaderFilename, const char* TessEvalShaderFilename) = 0;

	virtual bool CreateComputeProgram(uint32_t& ProgramID, const char* ComputeShaderFilename) = 0;
};
