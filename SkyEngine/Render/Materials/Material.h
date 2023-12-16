// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once

#include "InternalMaterial.h"
#include "Core/Application.h"
#include "Core/Object.h"
#include "Core/Asset/AssetInterface.h"
#include "Render/Renderer.h"
#include "Render/Lighting.h"
#include "Render/Shaders/ShaderManager.h"

class CTexture;
class CShader;

enum class EAttributeType
{
	None,
	Boolean,
	Integer,
	Number,
	Vector3,
	Vector4,
	Texture
};

template <class S = CShader>
class TMaterial : public CMaterialInterface, public IAssetObjectInterface
{
	// TODO: Once all shaders switched over, also make base CShader not allowed
	static_assert(std::is_base_of<CShader, S>::value, "Template S must inherit from Shader");
	
public:
	TMaterial(std::string InMaterialName);
	
	// TODO: Remove once from file system setup
	TMaterial(std::string InMaterialName, const TPointer<S> InShader);

	// TODO: Make redundant
	TPointer<S> GetShader() const;
	
	typename S::ShaderParameters Params;
	
	TPointer<CShader> GetBaseShader() override;
	void BindMaterial() override;
	bool HasTexture() override;
	virtual std::string GetMaterialName() const override { return MaterialName; }
	
	void Serialize(std::ostream& os) override;
	void Deserialize(std::istream& is) override;
	std::string GetAssetClassName() override;
	void OnLoaded() override;

	TPointer<S> Shader;
protected:
	// CMaterial(std::string ShaderName);
	std::string MaterialName;

	// Stencil
	bool bStencil = false;
	glm::vec4 StencilColour = {0.5, 0.7, 0.8, 1.0f};
	
	// TODO: live in Texture?
	bool bAntiAliasing = true;

protected:

};

template <class S>
TPointer<CShader> TMaterial<S>::GetBaseShader()
{
	return Shader;
}

template <class S>
void TMaterial<S>::BindMaterial()
{
	CMaterialInterface::BindMaterial();
	TPointer<S> TypedShader = std::static_pointer_cast<S>(GetRenderer()->ActiveShader);
	TypedShader->UploadMaterialParameters(Params);
}

template <class S>
bool TMaterial<S>::HasTexture()
{
	return Shader->HasTexture(Params);
}

template <class S>
void TMaterial<S>::Serialize(std::ostream& os)
{
	os << MaterialName << std::string("\n");
	os << Params;
}

template <class S>
void TMaterial<S>::Deserialize(std::istream& is)
{
	is >> MaterialName;
	is >> Params;
}

template <class S>
std::string TMaterial<S>::GetAssetClassName()
{
	return GetStaticName() + std::string(":") + Shader->GetShaderName();
}

template <class S>
void TMaterial<S>::OnLoaded()
{
	GetMaterialManager()->AddMaterial(shared_from_this());
}

template <class S>
TMaterial<S>::TMaterial(std::string InMaterialName)
{
	MaterialName = InMaterialName;
	Shader = ShaderManager::GetShader<S>();
	// TODO: Read from material path
	// Convert to values and store
	// Get shader from material file and store pointer
}

// template <class S>
// CMaterial::CMaterial(std::string ShaderName)
// {
// 	// TODO: Check loaded? Ideally all materials would be determined and appropriate shaders loaded? Or all loaded at startup
// 	Shader = ShaderManager::GetShader(ShaderName);
// 	// Set material defaults to shader defaults
// 	// MaterialAttributes = Shader->GetShaderAttributes();
// }

template <class S>
TMaterial<S>::TMaterial(std::string InMaterialName, const TPointer<S> InShader)
{
	MaterialName = InMaterialName;
	Shader = InShader;
	// MaterialAttributes = Shader->GetShaderAttributes();
}

template <class S>
TPointer<S> TMaterial<S>::GetShader() const
{ return Shader; }

// template <typename T>
// T CMaterial<S>::GetMaterialAttribute(const std::string& AttributeName)
// {
// 	return MaterialAttributes[AttributeName].GetActiveMember<T>();
// 	return T();
// }


// struct ENGINE_API MaterialAttribute
// {
// 	EAttributeType Type = EAttributeType::None;
// 	MaterialAttribute(bool InBoolean)
// 	{
// 		Boolean = InBoolean;
// 		Type = EAttributeType::Boolean;
// 	}
// 	MaterialAttribute(int InInteger)
// 	{
// 		Integer = InInteger;
// 		Type = EAttributeType::Integer;
// 	}
// 	MaterialAttribute(float InNumber)
// 	{
// 		Number = InNumber;
// 		Type = EAttributeType::Number;
// 	}
// 	MaterialAttribute(SVector InVector3)
// 	{
// 		Vector3 = InVector3;
// 		Type = EAttributeType::Vector3;
// 	}
// 	MaterialAttribute(SVector4 InVector4)
// 	{
// 		Vector4 = InVector4;
// 		Type = EAttributeType::Vector4;
// 	}
// 	MaterialAttribute(TPointer<CTexture> InTexture)
// 	{
// 		Texture = InTexture;
// 		Type = EAttributeType::Texture;
// 	}
// 	// TODO: Want something like this
// 	// template<typename T>
// 	// T GetActiveMember()
// 	// {
// 	// 	switch (Type)
// 	// 	{
// 	// 	case EAttributeType::Boolean:
// 	// 		return Boolean;
// 	// 	case EAttributeType::Integer:
// 	// 		return Integer;
// 	// 	case EAttributeType::Number:
// 	// 		return Number;
// 	// 	case EAttributeType::Vector3:
// 	// 		return Vector3;
// 	// 	case EAttributeType::Vector4:
// 	// 		return Vector4;
// 	// 	case EAttributeType::Texture:
// 	// 		return Texture;
// 	// 	}
// 	// 	return nullptr;
// 	// }
// 	bool GetBoolean() const { return Boolean; }
// 	int GetInteger() const { return Integer; }
// 	float GetNumber() const { return Number; }
// 	SVector3 GetVector3() const { return Vector3; }
// 	SVector4 GetVector4() const { return Vector4; }
// 	TPointer<CTexture> GetTexture() const { return Texture; }
// 	~MaterialAttribute()
// 	{
// 		Texture.reset();
// 	}
// 	MaterialAttribute(const MaterialAttribute& Other)
// 	{
// 		*this = Other;
// 	}
// 	MaterialAttribute(MaterialAttribute& Other)
// 	{
// 		*this = Other;
// 	}
// 	MaterialAttribute& operator=(const MaterialAttribute& Other)
// 	{
// 		Boolean = Other.Boolean;
// 		Integer = Other.Integer;
// 		Number = Other.Number;
// 		Vector3 = Other.Vector3;
// 		Vector4 = Other.Vector4;
// 		Texture = Other.Texture;
// 		return *this;
// 	}
// 	union
// 	{
// 		// Would read in as boolean if starts with lower case b
// 		bool Boolean;
// 		int Integer;
// 		float Number;
// 		SVector Vector3;
// 		SVector4 Vector4;
// 		TPointer<CTexture> Texture;
// 	};
// };