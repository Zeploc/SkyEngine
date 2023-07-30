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
	
	unsigned int CreateBuffer(const char* TextureSource, TextureData& Texture, bool bAA, bool bHasNormals = false) override;
	TextureData GetTexture(const char* TextureSource, bool bAA) override;
	void BindArray(const std::vector<float>& Vertices, const std::vector<uint32_t>& Indices, unsigned& Vao, bool bCreateVAO) override;
	Pointer<IGraphicsInstance> CreateNewInstance() override;
};
