// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once
#include <map>

#include "Lighting.h"
#include "Texture.h"
#include "Math/Vector4.h"

class ENGINE_API Material
{
public:
	Material(std::string ShaderName);
	void SetTexture(const std::string& Path, bool bUseAA = true);
	bool HasTexture() const;
	const TextureData& GetTextureData() const { return Texture; }

	// TODO: Make redundant
	uint32_t GetShaderProgram() const { return ShaderProgram; }

	Vector4 Colour;
	LightInfo LightProperties;
	bool bIsLit = true;
	bool bReflect = false;
	// Stencil
	bool bStencil = false;
	glm::vec4 StencilColour = {0.5, 0.7, 0.8, 1.0f};

	bool bFog = true;
	glm::vec4 UVCoords;
	int m_iIndicies = 0;

	// Flags
	bool bTwoSided = false;
	bool bDepthTest = true;
	// TODO: Blend mode (translucent)
	
	// TODO: live in Texture?
	bool bAntiAliasing = true;

protected:
	uint32_t ShaderProgram;
	TextureData Texture;
	std::map<std::string, int32_t> UniformLocations;
};
