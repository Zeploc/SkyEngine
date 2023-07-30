// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once
#include "GLFWIncludes.h"

#include "Graphics/GraphicsAPI.h"


class IGLFWAPI : public IGraphicsAPI
{
public:
	IGLFWAPI();
	~IGLFWAPI() override;
	std::string GetGraphicsDisplayName() override;
	
	Pointer<IGraphicsWindow> CreateNewWindow(const std::string& InWindowName, Vector2 InWindowSize, bool bFullScreen) override;
	unsigned int CreateBuffer(const char* TextureSource, TextureData& Texture, bool bAA, bool bHasNormals = false) override;
	TextureData GetTexture(const char* TextureSource, bool bAA) override;
	void BindArray(const std::vector<float>& Vertices, const std::vector<uint32_t>& Indices, unsigned& Vao, bool bCreateVAO) override;
};
