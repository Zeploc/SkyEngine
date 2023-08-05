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

	ShaderParameters Params;

	void BindShader(const TPointer<IGraphicsInstance>& InGraphicsInterface) override;
	void UploadMaterialParameters(const TPointer<IGraphicsInstance>& InGraphicsInstance, const ShaderParameters& InParams);
	
	// TODO: place holder, remove
	bool HasTexture(const ShaderParameters& InParams);
};

using CMaterial_PBR = TMaterial<CPBRShader>;