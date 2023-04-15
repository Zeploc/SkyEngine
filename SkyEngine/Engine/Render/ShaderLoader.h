// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once

// Library Includes //
#include <iostream>

// OpenGL Library Includes //
#include "glew.h"

class ShaderLoader
{
private:
	std::string ReadShader(const char* filename);

	GLuint CreateShader(GLenum shaderType,
	                    std::string source,
	                    const char* shaderName);

public:
	ShaderLoader(void);

	~ShaderLoader(void);

	GLuint CreateProgram(const char* VertexShaderFilename,
	                     const char* FragmentShaderFilename, const char* GeometryShaderFilename);

	GLuint CreateTessProgram(const char* VertexShaderFilename,
	                         const char* FragmentShaderFilename, const char* TessControlShaderFilename,
	                         const char* TessEvalShaderFilename);

	GLuint CreateComputeProgram(const char* ComputeShaderFilename);
};
