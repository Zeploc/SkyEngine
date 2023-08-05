#include "UndefinedShader.h"


CUndefinedShader::CUndefinedShader(const std::string& InShaderName, const std::string& InVertexShaderPath, const std::string& InFragmentShaderPath)
	:CShader(InShaderName, InVertexShaderPath, InFragmentShaderPath)
{
}

CUndefinedShader::~CUndefinedShader()
{
}

bool CUndefinedShader::HasTexture(const ShaderParameters& InParams)
{
	return false;
}

void CUndefinedShader::UploadMaterialParameters(const TPointer<IGraphicsInstance>& InGraphicsInstance, const ShaderParameters& InParams)
{
}