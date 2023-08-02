#include "UnlitShader.h"

std::string CUnlitShader::DiffuseColour = "DiffuseColour";
std::string CUnlitShader::DiffuseTexture = "DiffuseTexture";

CUnlitShader::CUnlitShader()
: CShader("Unlit","Resources/Shaders/UnlitVertexShader.vs", "Resources/Shaders/UnlitFragmentShader.vs")
{
	// TODO: Setup shader uniforms
	DefineAttribute(DiffuseColour, SVector4(1.0f));
	DefineAttribute(DiffuseTexture, TPointer<CTexture>());
}
