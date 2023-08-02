// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once
#include "Shader.h"
#include "Math/Vector4.h"

class CUnlitShader : public CShader
{
public:
	CUnlitShader();
	
	static std::string DiffuseColour;
	static std::string DiffuseTexture;	
};
