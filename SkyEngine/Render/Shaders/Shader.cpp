// Copyright Skyward Studios, Inc. All Rights Reserved.

#include "SEPCH.h"
#include "Shader.h"

#include "Core/Application.h"
#include "Core/Asset/Asset.h"
#include "Render/Materials/Material.h"
#include "Render/Textures/Texture.h"

void SShaderParameter::SetDeserializedVariable(std::istream& is)
{
	SSerializableVariable::SetDeserializedVariable(is);
	if (Type == EVariableType::Asset && *Asset)
	{
		LoadedTexture = (*Asset)->Load<CTexture>();
	}
}

void BaseShaderParameters::UploadMaterialParameters()
{
	for (const SShaderParameter& ShaderParameter : ShaderVariables)
	{
		const int32_t UniformLocation = ShaderParameter.Location;
		switch (ShaderParameter.Type)
		{
		case EVariableType::Boolean:
			GetGraphicsAPI()->PassAttributeToShader(UniformLocation, *ShaderParameter.Boolean);
			break;
		// case EVariableType::String:
		// 	GetGraphicsAPI()->PassAttributeToShader(UniformLocation, *ShaderParameter.String);
			break;
		case EVariableType::Number:
			GetGraphicsAPI()->PassAttributeToShader(UniformLocation, *ShaderParameter.Number);
			break;
		case EVariableType::Integer:
			GetGraphicsAPI()->PassAttributeToShader(UniformLocation, *ShaderParameter.Integer);
			break;
		// case EVariableType::Vector2:
		// 	GetGraphicsAPI()->PassAttributeToShader(UniformLocation, *ShaderParameter.Vector2);
			break;
		case EVariableType::Vector4:
			GetGraphicsAPI()->PassAttributeToShader(UniformLocation, *ShaderParameter.Vector4);
			break;
		case EVariableType::Asset:
			{
				GetGraphicsAPI()->PassAttributeToShader(UniformLocation, ShaderParameter.LoadedTexture);
				const bool bHasTexture = ShaderParameter.LoadedTexture != nullptr;
				const int32_t Location = Shader->GetHasTextureUniformLocation(ShaderParameter.VariableName);
				GetGraphicsAPI()->PassAttributeToShader(Location, bHasTexture);
			}
			break;
		default: ;
		}
	}
}

CShader::CShader(const std::string& InShaderName, const std::string& InVertexShaderPath, const std::string& InFragmentShaderPath)
{
	ShaderName = InShaderName;
	VertexShaderPath = InVertexShaderPath;
	FragmentShaderPath = InFragmentShaderPath;
}

CShader::CShader(const std::string& InShaderName, const std::string& InComputeShaderPath)
: ShaderProgram(0)
{
	ShaderName = InShaderName;
	ComputeShaderPath = InComputeShaderPath;
}

void CShader::SetGeometryShader(const std::string& InGeometryShaderPath)
{
	GeometryShaderPath = InGeometryShaderPath;
}

void CShader::SetTessShader(const std::string& InTessControlShaderPath, const std::string& InTessEvalShaderPath)
{
	TessControlShaderPath = InTessControlShaderPath;
	TessEvalShaderPath = InTessEvalShaderPath;
}

int32_t CShader::GetUniformLocation(const std::string& VariableName)
{
	const TArray<SShaderParameter>& ShaderParameters = GetShaderVariables();
	for (const SShaderParameter& ShaderParameter : ShaderParameters)
	{
		if (ShaderParameter.VariableName == VariableName)
		{
			return ShaderParameter.Location;
		}
	}
	// If fails to find location and not setting up, means a material failed to get an expected uniform
	ensure(bSettingUpUniformLocations, "Failed to find shader uniform for %s in %s", VariableName, GetShaderName());
	return -1;
}

int32_t CShader::GetHasTextureUniformLocation(const std::string& VariableName)
{
	if (!HasTextureUniformLocations.contains(VariableName))
	{
		return -1;
	}
	return HasTextureUniformLocations[VariableName];
}

bool CShader::CompileShader()
{
	bool bProgramCreationSuccessful;
	if (!ComputeShaderPath.empty())
	{
		 bProgramCreationSuccessful = GetGraphicsAPI()->CreateComputeProgram(ShaderProgram,ComputeShaderPath.c_str());
	}
	else if (!TessControlShaderPath.empty())
	{
		 bProgramCreationSuccessful = GetGraphicsAPI()->CreateTessProgram(ShaderProgram, VertexShaderPath.c_str(), FragmentShaderPath.c_str(), TessControlShaderPath.c_str(),TessEvalShaderPath.c_str());
	}
	else
	{
		bProgramCreationSuccessful=  GetGraphicsAPI()->CreateShaderProgram(ShaderProgram, VertexShaderPath.c_str(), FragmentShaderPath.c_str(),GeometryShaderPath.c_str());
	}
	if (!bProgramCreationSuccessful)
	{
		return false;
	}

	bSettingUpUniformLocations = true;
	InitializeShaderVariables();
	TArray<SShaderParameter>& ShaderParameters = GetShaderVariables();
	for (SShaderParameter& ShaderParameter : ShaderParameters)
	{
		ShaderParameter.Location = GetAttributeLocation(ShaderParameter.VariableName);
		if (ShaderParameter.Type == EVariableType::Asset)
		{			
			std::string LocationName = std::string("bHas") + ShaderParameter.VariableName;
			int32_t HasTextureLocation = GetAttributeLocation(LocationName);
			HasTextureUniformLocations.insert(std::pair(ShaderParameter.VariableName, HasTextureLocation));
		}
	}
	bSettingUpUniformLocations = false;
	return true;
}

std::ostream& operator<<(std::ostream& os, const BaseShaderParameters& InMaterialParameters)
{
	for (const SShaderParameter& MaterialParameter : InMaterialParameters.ShaderVariables)
	{
		MaterialParameter.SerializeVariable(os);
	}
	return os;
}

std::istream& operator>>(std::istream& is, BaseShaderParameters& InMaterialParameters)
{
	for (SShaderParameter& MaterialParameter : InMaterialParameters.ShaderVariables)
	{
		// Will overwrite name that is already set from InitializeMaterialParameters() - If this is unwanted DeserializeVariable can be overrridden
		MaterialParameter.DeserializeVariable(is);
	}
	return is;
}

void CShader::BindMaterial(TPointer<CMaterialInterface> InMaterial)
{
	ensure(InMaterial != nullptr, "Missing material to set shader attributes!");

	GetGraphicsAPI()->ApplyMaterialFlags(InMaterial);
}

void CShader::BindShader()
{
	GetGraphicsAPI()->BindShader(ShaderProgram);
}

int CShader::GetAttributeLocation(std::string AttributeName) const
{
	return GetGraphicsAPI()->GetAttributeLocation(ShaderProgram, AttributeName);
}
