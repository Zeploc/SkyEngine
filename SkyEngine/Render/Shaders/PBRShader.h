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
	bool CompileShader() override;
	
	struct ShaderParameters
	{
		DefineShaderVector4Param(DiffuseColour, SVector4(1.0f))
		DefineShaderTextureParam(DiffuseTexture, nullptr)
		DefineShaderFloatParam(SpecularStrength, 1.0f)
		DefineShaderFloatParam(Shininess, 32.0f)
		
		// TODO: Setup shader uniforms
		// TODO:
		// Metallic Texture and/or strength
		// Specular Texture and/or strength
		// Roughness Texture and/or strength
		// Translucency/Mask and/or strength
		// Reflection
	};
	
	ENGINE_API friend std::ostream& operator<<(std::ostream& os, const ShaderParameters& InShaderParameters);
	ENGINE_API friend std::istream& operator>>(std::istream& is, ShaderParameters& InShaderParameters);

	ShaderParameters Params;

	void BindShader() override;
	void UploadMaterialParameters(const ShaderParameters& InParams);
	
	// TODO: place holder, remove
	bool HasTexture(const ShaderParameters& InParams);
};

using CMaterial_PBR = TMaterial<CPBRShader>;