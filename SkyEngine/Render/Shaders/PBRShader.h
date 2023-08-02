// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once

#include "Shader.h"
#include "Math/Vector4.h"

class CPBRShader : public CShader
{
public:
	CPBRShader();
	~CPBRShader() override;

	void BindShader(const TPointer<IGraphicsInstance> InGraphicsInterface) override;

	static std::string DiffuseColour;
	static std::string DiffuseTexture;
	static std::string SpecularStrength;

	
	bool bIsLit = true;
};
