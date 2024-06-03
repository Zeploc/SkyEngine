// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once

#include "Shader.h"
#include "Math/Vector4.h"

class CTexture;
template<class S>
class TMaterial;

class ENGINE_API CPBRShader : public CShader
{
public:
	CPBRShader();
	~CPBRShader() override;
	static std::string GetStaticName();

	BeginShaderParams()
		DefineShaderParameter(DiffuseColour);
		DefineShaderParameter(DiffuseTexture);
		DefineShaderParameter(SpecularStrength);
		DefineShaderParameter(Shininess);
	ListShaderVariables()
		SVector4 DiffuseColour = SVector4(1.0f);
		TPointer<CTexture> DiffuseTexture = nullptr;
		float SpecularStrength = 1.0f;
		float Shininess = 32.0f;
	EndShaderParams()
	
	// TODO: Setup shader uniforms
	// TODO:
	// Metallic Texture and/or strength
	// Specular Texture and/or strength
	// Roughness Texture and/or strength
	// Translucency/Mask and/or strength
	// Reflection

	void BindShader() override;
	
	// TODO: place holder, remove
	bool HasTexture(const ShaderParameters& InMaterialParams);
};

using CMaterial_PBR = TMaterial<CPBRShader>;