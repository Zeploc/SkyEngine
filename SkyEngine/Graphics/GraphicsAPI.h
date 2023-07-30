// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once
#include <string>
#include <vector>

#include "Core/Core.h"

#include "Math/Vector2.h"
#include "Render/Texture.h"

class IGraphicsInstance;
class UIElement;
class Entity;
class IGraphicsWindow;

enum class EGraphicsAPI
{
	OPENGL,
	VULKAN
};

class ENGINE_API IGraphicsAPI
{
public:
	virtual ~IGraphicsAPI() = default;
	
	virtual std::string GetGraphicsDisplayName() = 0;

	static Pointer<IGraphicsAPI> CreateGraphicsAPI(EGraphicsAPI APIType);

	virtual Pointer<IGraphicsInstance> CreateNewInstance() = 0;
	
	virtual unsigned int CreateBuffer(const char* TextureSource, TextureData& Texture, bool bAA, bool bHasNormals = false) = 0;
	virtual TextureData GetTexture(const char* TextureSource, bool bAA) = 0;
	/* Create a new VAO if -1, otherwise will use it  */
	virtual void BindArray(const std::vector<float>& vertices, const std::vector<uint32_t>& indices, unsigned& vao, bool bCreateVAO) = 0;
	
	virtual unsigned int CreateProgram(const char* VertexShaderFilename, const char* FragmentShaderFilename, const char* GeometryShaderFilename) = 0;

	virtual unsigned int CreateTessProgram(const char* VertexShaderFilename, const char* FragmentShaderFilename,
								const char* TessControlShaderFilename, const char* TessEvalShaderFilename) = 0;

	virtual unsigned int CreateComputeProgram(const char* ComputeShaderFilename) = 0;
};
