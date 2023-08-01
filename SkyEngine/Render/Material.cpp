#include "Material.h"

#include "Shader.h"
#include "Core/Application.h"

Material::Material(std::string ShaderName)
{
	// TODO: Check loaded? Ideally all materials would be determined and appropriate shaders loaded? Or all loaded at startup
	ShaderProgram = Shader::Programs[ShaderName];
}

void Material::SetTexture(const std::string& Path, bool bUseAA)
{
	Texture = GetGraphicsAPI()->GetTexture(Path.c_str(), bUseAA);
}

bool Material::HasTexture() const
{
	return Texture.IsValid();
}
