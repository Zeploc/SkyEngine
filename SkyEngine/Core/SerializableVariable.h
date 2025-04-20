// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once

#include "Core/Core.h"
#include "Object.h"
#include "Math/Transform.h"

#include "Math/Vector4.h"
#include "Math/Internal/Vector2.decl.h"

class CTexture;
class CAssetObject;

enum class EVariableType : int
{
	None = -1,
	Boolean,
	String,
	Number,
	Integer,
	Vector2,
	Vector4,
	Transform,
	Object
};

#define MetaTag_Class "Class"
#define MetaTag_HideInEditor "HideInEditor"
#define MetaTag_ReadOnlyEditor "ReadOnlyEditor"

struct ENGINE_API SSerializableVariable
{
	std::string VariableName;
	EVariableType Type = EVariableType::None;
	TMap<std::string, std::string> MetaTags;

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
	SSerializableVariable(const std::string& InName, STransform* InTransform) : VariableName(InName)
	{
		Type = EVariableType::Transform;
		Transform = InTransform;
	}

	// template<class T, std::enable_if_t<std::is_base_of_v<CAssetObject, T>, bool> = true>
	// SSerializableVariable(const std::string& InName, TWeakPointer<T>* InObject) : VariableName(InName)
	// {		
	// 	Type = EVariableType::Object;
	// 	AssetObject = (TAssetObjectPointer<CAssetObject>*)(InObject);
	// 	// MetaTags.Add(MetaTag_Class, T::GetStaticName());
	// 	MetaTags.insert(std::pair(MetaTag_Class, T::GetStaticName()));
	// 	// MetaTags[MetaTag_Class] = T::GetStaticName();
	// }
	template<class T, std::enable_if_t<std::is_base_of_v<CAssetObject, T>, bool> = true>
	SSerializableVariable(const std::string& InName, TAssetObjectPointer<T>* InObject) : VariableName(InName)
	{		
		Type = EVariableType::Object;
		AssetObject = (TAssetObjectPointer<CAssetObject>*)(InObject);
		// MetaTags.Add(MetaTag_Class, T::GetStaticName());
		MetaTags.insert(std::pair(MetaTag_Class, T::GetStaticName()));
		// MetaTags[MetaTag_Class] = T::GetStaticName();
	}

	std::string GetMetaTag(std::string Key);
	bool HasMetaTag(std::string Key) const;

	union
	{
		// Would read in as boolean if starts with lower case b
		bool* Boolean;
		std::string* String;
		float* Number;
		int* Integer;
		SVector2* Vector2;
		SVector4* Vector4;
		STransform* Transform;
		TAssetObjectPointer<CAssetObject>* AssetObject;
	};
	
	void SerializeVariable(std::ostream& os) const;
	void DeserializeVariable(std::istream& is);
	
	virtual std::string GetSerializedVariable() const;
	virtual void SetDeserializedVariable(std::istream& is);
	virtual void OnValueChanged() {}
};

#define SetSerializeVariable(VariableName) SerializeVariableWithTags(VariableName)

#define SerializeVariableWithTags(VariableName, MetaTags) CustomSerializeVariable(SSerializableVariable, SerializeVariables, VariableName)

#define CustomSerializeVariable(SerializeStructType, SerializeVariables, VariableName)\
/*static_assert(!std::is_class<typeid(VariableName)>() || typeid(VariableName) == typeid(CAssetObject), "Can only serialize asset object classes");*/\
SerializeStructType Serialize##VariableName = SerializeStructType(std::string(#VariableName), &VariableName);\
SerializeVariables.push_back(Serialize##VariableName)

#define AddMetaTag(VariableName, Tag) AddMetaTagValue(VariableName, Tag, "")
	
#define AddMetaTagValue(Name, Tag, Value)\
for (SSerializableVariable& Serializable : SerializeVariables)\
{\
	if (Serializable.VariableName == std::string(#Name))\
	{\
		Serializable.MetaTags.insert(std::pair(Tag, Value));\
	}\
}

// #define AddMetaTag(VariableName, Tag, Value)\
// Serialize##VariableName.MetaTags.Add(Tag, Value)