// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once

#include "Core/Core.h"
#include "Math/Internal/Vector2.decl.h"

enum class EVariableType : int
{
	None = -1,
	Boolean,
	String,
	Number,
	Integer,
	Vector2
};

struct ConfigVariable
{
	std::string VariableName;
	EVariableType Type = EVariableType::None;

	ConfigVariable(const std::string& InName, bool* InBoolean) : VariableName(InName)
	{
		Type = EVariableType::Boolean;
		Boolean = InBoolean;
	}
	ConfigVariable(const std::string& InName, std::string* InString) : VariableName(InName)
	{
		Type = EVariableType::String;
		String = InString;
	}
	ConfigVariable(const std::string& InName, float* InFloat) : VariableName(InName)
	{
		Type = EVariableType::Number;
		Number = InFloat;
	}
	ConfigVariable(const std::string& InName, int* InInteger) : VariableName(InName)
	{
		Type = EVariableType::Integer;
		Integer = InInteger;
	}
	ConfigVariable(const std::string& InName, SVector2* InVector2) : VariableName(InName)
	{
		Type = EVariableType::Vector2;
		Vector2 = InVector2;
	}

	union
	{
		// Would read in as boolean if starts with lower case b
		bool* Boolean;
		std::string* String;
		float* Number;
		int* Integer;
		SVector2* Vector2;
	};
	
	void SerializeVariable(std::ostream& os) const
	{
		switch (Type)
		{
		case EVariableType::None:
			break;
		case EVariableType::Boolean:
			{
				const std::string BoolValue = *Boolean ? std::string("true") : std::string("false");
				os << BoolValue;// VariableName << std::string(" ") << BoolValue << std::string(" ");
			}
			break;
		case EVariableType::String:
			os << *String;//VariableName << std::string(" ") << *String << std::string(" ");
			break;
		case EVariableType::Number:
			os << std::to_string(*Number);//VariableName << std::string(" ") << std::to_string(*Number) << std::string(" ");
			break;
		case EVariableType::Integer:
			os << std::to_string(*Integer);//VariableName << std::string(" ") << std::to_string(*Number) << std::string(" ");
			break;
		case EVariableType::Vector2:
			os << *Vector2;//VariableName << std::string(" ") << *Vector2 << std::string(" ");
			break;
		default: ;
		}
	}
	void DeserializeVariable(std::istream& is)
	{
		switch (Type)
		{
		case EVariableType::None:
			break;
		case EVariableType::Boolean:
			{
				std::string FoundBool;
				is >> /*VariableName >>*/ FoundBool;
				*Boolean = FoundBool == "true";
			}
			break;
		case EVariableType::String:
			is >> /*VariableName >>*/ *String;
			break;
		case EVariableType::Number:
			{
				std::string FoundFloat;
				is >> /*VariableName >>*/ FoundFloat;
				*Number = std::stof(FoundFloat);
			}
			break;
		case EVariableType::Integer:
			{
				std::string FoundInteger;
				is >> /*VariableName >>*/ FoundInteger;
				*Integer = std::stoi(FoundInteger);
			}
			break;
		case EVariableType::Vector2:
    		is >> /*VariableName >>*/ *Vector2;\
			break;
		default: ;
		}
	}
};

class ENGINE_API CConfig
{
public:
	virtual ~CConfig() = default;
	CConfig(std::string RelativePath);

	bool LoadConfig();
	bool DoesConfigExist() const;
	bool SaveConfig();

	template<class T>
	static bool RegisterConfig(bool bCreateIfMissing = true);
	
	template<class T>
	static TPointer<T> GetConfig();
	
protected:
	/* Relative to the configs directory */
	std::string ConfigPath;
	
	TArray<ConfigVariable> Variables;

	std::string GetAbsoluteConfigPath() const;
	virtual void SerializeVariables(std::ostream& os) const;
	virtual void DeserializeVariables(std::istream& is);

	static TArray<TPointer<CConfig>> Configs;
};

template <class T>
bool CConfig::RegisterConfig(bool bCreateIfMissing)
{
	TPointer<CConfig> Config = CreatePointer<T>();
	if (!Config->DoesConfigExist() || !Config->LoadConfig())
	{
		if (!bCreateIfMissing || !Config->SaveConfig())
		{
			Config.reset();
			return false;
		}
	}
	
	Configs.push_back(Config);
	return true;
}

template <class T>
TPointer<T> CConfig::GetConfig()
{
	for (TPointer<CConfig> Config : Configs)
	{
		if (TPointer<T> CheckedConfig = std::dynamic_pointer_cast<T>(Config))
		{
			return CheckedConfig;
		}	
	}
	return nullptr;
}

#define DefineConfigVariable(VariableName)\
	Variables.push_back(ConfigVariable(VariableName##Name, &VariableName))

#define DeclareConfigBoolean(VariableName, DefaultValue)\
	bool VariableName = DefaultValue;\
	std::string VariableName##Name = #VariableName;

#define DeclareConfigString(VariableName, DefaultValue)\
	std::string VariableName = DefaultValue;\
	std::string VariableName##Name = #VariableName;

#define DeclareConfigFloat(VariableName, DefaultValue)\
	float VariableName = DefaultValue;\
	std::string VariableName##Name = #VariableName;

#define DeclareConfigInteger(VariableName, DefaultValue)\
	int VariableName = DefaultValue;\
	std::string VariableName##Name = #VariableName;

#define DeclareConfigVector2(VariableName, DefaultValue)\
	SVector2 VariableName = DefaultValue;\
	std::string VariableName##Name = #VariableName;