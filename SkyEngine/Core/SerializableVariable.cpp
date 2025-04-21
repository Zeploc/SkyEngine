// Copyright Skyward Studios, Inc. All Rights Reserved.

#include "SEPCH.h"
#include "SerializableVariable.h"
#include <sstream>

#include "Application.h"
#include "Asset/Asset.h"
#include "Core/Object.h"
#include "System/LogManager.h"

std::string SSerializableVariable::GetMetaTag(std::string Key)
{
	if (!HasMetaTag(Key))
	{
		return {};
	}
	return MetaTags[Key];
}

bool SSerializableVariable::HasMetaTag(std::string Key) const
{
	return MetaTags.contains(Key);
}

void SSerializableVariable::SerializeVariable(std::ostream& os) const
{
	os << VariableName << std::string(" ") << GetSerializedVariable() << std::string(" ");
}

void SSerializableVariable::DeserializeVariable(std::istream& is)
{
	is >> VariableName;
	SetDeserializedVariable(is);
}

std::string SSerializableVariable::GetSerializedVariable() const
{
	switch (Type)
	{
	case EVariableType::None:
		break;
	case EVariableType::Boolean:
		return *Boolean ? std::string("true") : std::string("false");
	case EVariableType::String:
		return *String;
	case EVariableType::Number:
		return std::to_string(*Number);
	case EVariableType::Integer:
		return std::to_string(*Integer);
	case EVariableType::Vector2:
		{
			std::stringstream ss("");
			ss << *Vector2;
			return ss.str();
		}
	case EVariableType::Vector4:
		{
			std::stringstream ss("");
			ss << *Vector4;
			return ss.str();
		}
	case EVariableType::Transform:
		{
			std::stringstream ss("");
			ss << *Transform;
			return ss.str();
		}
	case EVariableType::Object:
		{
			TAssetObjectPointer<CAssetObject> AssetPointer = *AssetObject;
			std::stringstream ss("None");
			if (AssetPointer)
			{
				ss << AssetPointer->Asset->FilePath;
			}
			return ss.str();
		}
	default: ;
	}
	return std::string();
}

void SSerializableVariable::SetDeserializedVariable(std::istream& is)
{
	std::string string;
		
	switch (Type)
	{
	case EVariableType::None:
		break;
	case EVariableType::Boolean:
		{
			is >> string;
			*Boolean = string == "true";
		}
		break;
	case EVariableType::String:
		{				
			is >> *String;
		}
		break;
	case EVariableType::Number:
		{
			is >> string;
			*Number = std::stof(string);
		}
		break;
	case EVariableType::Integer:
		{
			is >> string;
			*Integer = std::stoi(string);
		}
		break;
	case EVariableType::Vector2:
		is >> *Vector2;
		break;
	case EVariableType::Vector4:
		is >> *Vector4;
		break;
	case EVariableType::Transform:
		is >> *Transform;
		break;
	case EVariableType::Object:
		{
			std::string AssetPath;
			is >> AssetPath;
			if (AssetPath != "None")
			{
				if (GetAssetManager()->DoesAssetExist(AssetPath))
				{
					*AssetObject = GetAssetManager()->LoadAsset(AssetPath);
				}
				else
				{
					// Fallback for if asset exists but not in file (ie currently used for engine meshes)
					for (TObjectPointer<CAsset> Asset : GetAssetManager()->GetAssets())
					{
						if (AssetPath == Asset->FilePath)
						{
							*AssetObject = GetAssetManager()->LoadAsset(AssetPath);
						}
					}
				}
				if (!AssetObject->IsValid())
				{
					LOG_ERROR("Failed to load asset by path {}", AssetPath);
				}
			}
			else
			{
				*AssetObject = nullptr;
			}
		}
		break;
	default: ;
	}
}
