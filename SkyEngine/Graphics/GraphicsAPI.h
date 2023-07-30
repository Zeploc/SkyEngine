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
	GLFW,
	VULKAN
};

class ENGINE_API IGraphicsAPI
{
public:
	virtual ~IGraphicsAPI() = default;
	
	virtual std::string GetGraphicsDisplayName() = 0;
	
	virtual Pointer<IGraphicsWindow> CreateNewWindow(const std::string& InWindowName, Vector2 InWindowSize, bool bFullScreen) = 0;

	static Pointer<IGraphicsAPI> CreateGraphicsAPI(EGraphicsAPI APIType);
	
	virtual unsigned int CreateBuffer(const char* TextureSource, TextureData& Texture, bool bAA, bool bHasNormals = false) = 0;
	virtual TextureData GetTexture(const char* TextureSource, bool bAA) = 0;
	/* Create a new VAO if -1, otherwise will use it  */
	virtual void BindArray(const std::vector<float>& vertices, const std::vector<uint32_t>& indices, unsigned& vao, bool bCreateVAO) = 0;
};
