// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once

#include "Core/Core.h"

#include "Math/Vector4.h"
#include "Math/Internal/Vector2.decl.h"

class CTexture;
class CAsset;

enum class EVariableType : int
{
	None = -1,
	Boolean,
	String,
	Number,
	Integer,
	Vector2,
	Vector4,
	Asset
};

struct ENGINE_API SSerializableVariable
{
	std::string VariableName;
	EVariableType Type = EVariableType::None;

	SSerializableVariable(const std::string& InName, bool* InBoolean) : VariableName(InName)
	{
		Type = EVariableType::Boolean;
		Boolean = InBoolean;
	}
	SSerializableVariable(const std::string& InName, std::string* InString) : VariableName(InName)
	{
		Type = EVariableType::String;
		String = InString;
	}
	SSerializableVariable(const std::string& InName, float* InFloat) : VariableName(InName)
	{
		Type = EVariableType::Number;
		Number = InFloat;
	}
	SSerializableVariable(const std::string& InName, int* InInteger) : VariableName(InName)
	{
		Type = EVariableType::Integer;
		Integer = InInteger;
	}
	SSerializableVariable(const std::string& InName, SVector2* InVector2) : VariableName(InName)
	{
		Type = EVariableType::Vector2;
		Vector2 = InVector2;
	}
	SSerializableVariable(const std::string& InName, SVector4* InVector4) : VariableName(InName)
	{
		Type = EVariableType::Vector4;
		Vector4 = InVector4;
	}
	SSerializableVariable(const std::string& InName, TPointer<CAsset>* InAsset) : VariableName(InName)
	{
		Type = EVariableType::Asset;
		Asset = InAsset;
	}

	union
	{
		// Would read in as boolean if starts with lower case b
		bool* Boolean;
		std::string* String;
		float* Number;
		int* Integer;
		SVector2* Vector2;
		SVector4* Vector4;
		TPointer<CAsset>* Asset;
	};
	
	void SerializeVariable(std::ostream& os) const;
	void DeserializeVariable(std::istream& is);
	
	virtual std::string GetSerializedVariable() const;
	virtual void SetDeserializedVariable(std::istream& is);
};
