// Copyright Skyward Studios, Inc. All Rights Reserved.

#include "SEPCH.h"
#include "Shader.h"

#include "Core/Application.h"
#include "Render/Materials/Material.h"

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

bool CShader::CompileShader()
{
	if (!ComputeShaderPath.empty())
	{
		 return GetGraphicsAPI()->CreateComputeProgram(ShaderProgram,ComputeShaderPath.c_str());
	}
	if (!TessControlShaderPath.empty())
	{
		 return GetGraphicsAPI()->CreateTessProgram(ShaderProgram, VertexShaderPath.c_str(), FragmentShaderPath.c_str(), TessControlShaderPath.c_str(),TessEvalShaderPath.c_str());
	}

	return GetGraphicsAPI()->CreateShaderProgram(ShaderProgram, VertexShaderPath.c_str(), FragmentShaderPath.c_str(),GeometryShaderPath.c_str());
}

void CShader::BindMaterial(TPointer<CMaterialInterface> InMaterial)
{
	ensure(InMaterial != nullptr, "Missing material to set shader attributes!");

	GetRenderer()->ApplyMaterialFlags(InMaterial);
}

void CShader::BindShader()
{
	GetGraphicsAPI()->BindShader(ShaderProgram);
}
