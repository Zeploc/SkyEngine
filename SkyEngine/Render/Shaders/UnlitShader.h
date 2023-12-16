// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once
#include "Shader.h"
#include "Math/Vector4.h"

class CTexture;
template<class S>
class TMaterial;

class ENGINE_API CUnlitShader : public CShader
{
public:
	CUnlitShader();
	static std::string GetStaticName();
	bool CompileShader() override;
	
	struct ShaderParameters
	{
		DefineShaderVector4Param(DiffuseColour, SVector4(1.0f))
		DefineShaderTextureParam(DiffuseTexture, nullptr)
	};
	
	ShaderParameters Params;
	void BindShader() override;
	void UploadMaterialParameters(const ShaderParameters& InParams);

	bool HasTexture(const ShaderParameters& InParams);
	
	ENGINE_API friend std::ostream& operator<<(std::ostream& os, const ShaderParameters& InShaderParameters);
	ENGINE_API friend std::istream& operator>>(std::istream& is, ShaderParameters& InShaderParameters);
};

using CMaterial_Unlit = TMaterial<CUnlitShader>;