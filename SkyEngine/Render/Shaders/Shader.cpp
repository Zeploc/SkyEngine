#include "Shader.h"

#include "Core/Application.h"
#include "Graphics/GraphicsInstance.h"
#include "Render/Materials/Material.h"

CShader::CShader(const std::string& ShaderName, const std::string& InVertexShaderPath, const std::string& InFragmentShaderPath)
{
	VertexShaderPath = InVertexShaderPath;
	FragmentShaderPath = InFragmentShaderPath;
}

CShader::CShader(const std::string& ShaderName, const std::string& InComputeShaderPath)
: ShaderProgram(0)
{
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
	// TODO: Don't return above, and store all uniform locations
	// UniformLocations
}

void CShader::BindMaterial(const TPointer<IGraphicsInstance> InGraphicsInstance, TPointer<CMaterial> InMaterial)
{
	ensure(InMaterial != nullptr, "Missing material to set shader attributes!");

	const std::map<std::string, MaterialAttribute> MaterialAttributes;// = InMaterial->GetMaterialAttributes();
	for (std::pair<const std::string, MaterialAttribute> Attribute : MaterialAttributes)
	{
		if (Attribute.second.Texture)
		{
			// TODO: Handle multiple
			InGraphicsInstance->BindTexture(Attribute.second.Texture);
		}
		else
		{
			const int UniformLocation = UniformLocations[Attribute.first];
			//TODO: Need to mark which index is used
			InGraphicsInstance->PassAttributeToShader(UniformLocation, Attribute.second);
		}
	}
	InGraphicsInstance->ApplyMaterialFlags(InMaterial);
}

void CShader::BindShader(const TPointer<IGraphicsInstance> InGraphicsInterface)
{
	InGraphicsInterface->BindShader(ShaderProgram);
}

void CShader::DefineAttribute(const std::string& AttributeName, float Number)
{
	DefineAttribute(AttributeName, MaterialAttribute(Number));
}

void CShader::DefineAttribute(const std::string& AttributeName, int Integer)
{
	DefineAttribute(AttributeName, MaterialAttribute(Integer));
}

void CShader::DefineAttribute(const std::string& AttributeName, bool Boolean)
{
	DefineAttribute(AttributeName, MaterialAttribute(Boolean));
}

void CShader::DefineAttribute(const std::string& AttributeName, SVector Vector3)
{
	DefineAttribute(AttributeName, MaterialAttribute(Vector3));
}

void CShader::DefineAttribute(const std::string& AttributeName, TPointer<CTexture> Texture)
{
	DefineAttribute(AttributeName, MaterialAttribute(Texture));
}

void CShader::DefineAttribute(const std::string& AttributeName, SVector4 Vector4)
{
	DefineAttribute(AttributeName, MaterialAttribute(Vector4));
}

void CShader::DefineAttribute(const std::string& AttributeName, MaterialAttribute Attribute)
{
	// ShaderAttributes.insert(std::pair(AttributeName, Attribute));
}
