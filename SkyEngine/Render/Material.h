// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once
#include "Lighting.h"
#include "Texture.h"
#include "Math/Vector4.h"

class Material
{
public:
	bool bDepthTest = true;
	Vector4 Colour;
	uint32_t ShaderProgram;
	TextureData Texture;
	LightInfo LightProperties;
	bool bCullFace = true;
	bool bIsLit = true;
	bool bReflect = false;
	// Stencil
	bool bStencil = false;
	glm::vec4 StencilColour = {0.5, 0.7, 0.8, 1.0f};

	bool bFog = true;
	glm::vec4 UVCoords;
	int m_iIndicies;
};
