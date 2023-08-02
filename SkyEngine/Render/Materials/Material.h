// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once
#include <map>

#include "Render/Lighting.h"
#include "Math/Vector4.h"

class CTexture;
class CShader;
class IGraphicsInstance;

struct ENGINE_API MaterialAttribute
{
	MaterialAttribute(bool InBoolean)
	{
		Boolean = InBoolean;
	}
	MaterialAttribute(int InInteger)
	{
		Integer = InInteger;
	}
	MaterialAttribute(float InNumber)
	{
		Number = InNumber;
	}
	MaterialAttribute(SVector InVector3)
	{
		Vector3 = InVector3;
	}
	MaterialAttribute(SVector4 InVector4)
	{
		Vector4 = InVector4;
	}
	MaterialAttribute(TPointer<CTexture> InTexture)
	{
		Texture = InTexture;
	}
	~MaterialAttribute()
	{
		Texture.reset();
	}
	MaterialAttribute(const MaterialAttribute& Other)
	{
		*this = Other;
	}
	MaterialAttribute(MaterialAttribute& Other)
	{
		*this = Other;
	}
	MaterialAttribute& operator=(const MaterialAttribute& Other)
	{
		Boolean = Other.Boolean;
		Integer = Other.Integer;
		Number = Other.Number;
		Vector3 = Other.Vector3;
		Vector4 = Other.Vector4;
		Texture = Other.Texture;
		return *this;
	}
	union
	{
		// Would read in as boolean if starts with lower case b
		bool Boolean;
		int Integer;
		float Number;
		SVector Vector3;
		SVector4 Vector4;
		TPointer<CTexture> Texture;
	};
};


class ENGINE_API CMaterial : public std::enable_shared_from_this<CMaterial>
{
public:
	// TODO: Load asset from path
	CMaterial(const std::string& InMaterialPath);
	
	void BindMaterial(const TPointer<IGraphicsInstance> InGraphicsInterface);

	// TODO: Remove once from file system setup
	CMaterial(const TPointer<CShader> InShader);

	// const std::map<std::string, MaterialAttribute>& GetMaterialAttributes() { return MaterialAttributes; }
	bool SetMaterialAttribute(const std::string& AttributeName, MaterialAttribute Attribute);
	template<typename T>
	T GetMaterialAttribute(const std::string& AttributeName);
	bool HasMaterialAttribute(const std::string& AttributeName);
	bool HasTexture() const;

	// TODO: Make redundant
	TPointer<CShader> GetShader() const { return Shader; }
	
	// Flags
	bool bTwoSided = false;
	bool bDepthTest = true;
	// TODO: Blend mode (translucent)	
	
protected:
	// TODO: Stores map of keys to values including textures
	// std::map<std::string, MaterialAttribute> MaterialAttributes;
	
	CMaterial(std::string ShaderName);
	// const CTexture& GetTextureData() const { return Texture; }

	// Stencil
	bool bStencil = false;
	glm::vec4 StencilColour = {0.5, 0.7, 0.8, 1.0f};
	
	// TODO: live in Texture?
	bool bAntiAliasing = true;

protected:
	TPointer<CShader> Shader;
	// CTexture Texture;

	std::string MaterialAssetPath; 
};

template <typename T>
T CMaterial::GetMaterialAttribute(const std::string& AttributeName)
{
	return T();
	// return MaterialAttributes[AttributeName];
}
