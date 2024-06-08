// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once
#include "Shader.h"
#include "Math/Vector4.h"
#include "Render/Textures/Texture.h"

template<class S>
class TMaterial;

class ENGINE_API CUnlitShader : public CShader
{
public:
	CUnlitShader();
	static std::string GetStaticName();
	
	BeginShaderParams()
		DefineShaderParameter(DiffuseColour);
		DefineShaderParameter(DiffuseTexture);
	ListShaderVariables()	
		SVector4 DiffuseColour = SVector4(1.0f);
		TPointer<CTexture> DiffuseTexture = nullptr;
	EndShaderParams()

	bool HasTexture(const ShaderParameters& InParams);
};

using CMaterial_Unlit = TMaterial<CUnlitShader>;