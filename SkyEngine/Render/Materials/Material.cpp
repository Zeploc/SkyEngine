#include "Material.h"

#include "Render/Shaders/ShaderManager.h"
#include "Core/Application.h"
#include "Graphics/GraphicsInstance.h"
#include "Render/Shaders/Shader.h"

CMaterial::CMaterial(std::string ShaderName)
{
	// TODO: Check loaded? Ideally all materials would be determined and appropriate shaders loaded? Or all loaded at startup
	Shader = ShaderManager::GetShader(ShaderName);
	// Set material defaults to shader defaults
	// MaterialAttributes = Shader->GetShaderAttributes();
}

CMaterial::CMaterial(const std::string& InMaterialPath)
{
	MaterialAssetPath = InMaterialPath;
	// TODO: Read from material path
	// Convert to values and store
	// Get shader from material file and store pointer
}

void CMaterial::BindMaterial(const TPointer<IGraphicsInstance> InGraphicsInterface)
{
	ensure(InGraphicsInterface->ActiveShader == Shader, "Attempted to bind material when shader wasn't active!");
	Shader->BindMaterial(InGraphicsInterface, shared_from_this());
}

CMaterial::CMaterial(const TPointer<CShader> InShader)
{
	Shader = InShader;
	// MaterialAttributes = Shader->GetShaderAttributes();
}

bool CMaterial::SetMaterialAttribute(const std::string& AttributeName, MaterialAttribute Attribute)
{
	// if (!MaterialAttributes.contains(AttributeName))
	// {
	// 	ensure(false, "Attempted to set attribute of missing name!");
	// 	return false;
	// }
	// MaterialAttributes[AttributeName] = Attribute;
	return true;
}

bool CMaterial::HasMaterialAttribute(const std::string& AttributeName)
{
	// return MaterialAttributes.contains(AttributeName);
	return false;
}

bool CMaterial::HasTexture() const
{
	// for (auto MaterialAttribute : MaterialAttributes)
	// {
	// 	if (MaterialAttribute.second.Texture)
	// 	{
	// 		return true;
	// 	}
	// }
	return false;
}
