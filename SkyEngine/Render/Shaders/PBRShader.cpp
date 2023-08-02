#include "PBRShader.h"

std::string CPBRShader::DiffuseColour = "DiffuseColour";
std::string CPBRShader::DiffuseTexture = "DiffuseTexture";
std::string CPBRShader::SpecularStrength = "SpecularStrength";

CPBRShader::CPBRShader()
: CShader("PBR","Resources/Shaders/PBRVertexShader.vs", "Resources/Shaders/PBRFragmentShader.vs")
{
	// TODO: Setup shader uniforms
	DefineAttribute(DiffuseColour, SVector4(1.0f));
	DefineAttribute(DiffuseTexture, TPointer<CTexture>());
	DefineAttribute(SpecularStrength, 1.0f);
	DefineAttribute("Shininess", 32.0f);

	bool bReflect = false;

	// TODO:
	// Diffuse Texture
	// Metallic Texture and/or strength
	// Specular Texture and/or strength
	// Roughness Texture and/or strength
	// Translucency/Mask and/or strength
}

void CPBRShader::BindShader(const TPointer<IGraphicsInstance> InGraphicsInterface)
{
	CShader::BindShader(InGraphicsInterface);

	// TODO: Improve with light objects
	Lighting::PassLightingToShader(InGraphicsInterface);
}
