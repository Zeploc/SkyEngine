// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once

#include "Core/SerializableVariable.h"


struct SConfigVariable : public SSerializableVariable
{
	using SSerializableVariable::SSerializableVariable;
};

class ENGINE_API CConfig
{
public:
	virtual ~CConfig() = default;
	CConfig(std::string Name, std::string RelativePath = "");

	bool LoadConfig();
	bool DoesConfigExist() const;
	bool SaveConfig();

	template<class T>
	static bool RegisterConfig(bool bCreateIfMissing = true);
	
	template<class T>
	static TSharedPointer<T> GetConfig();
	
	std::string GetName() const { return ConfigName; }
	
	const TArray<SConfigVariable>& GetVariables() const { return Variables; }
	
protected:
	std::string ConfigName;
	/* Relative to the configs directory */
	std::string ConfigPath;
	
	TArray<SConfigVariable> Variables;

	std::string GetAbsoluteConfigPath() const;
	virtual void SerializeVariables(std::ostream& os) const;
	virtual void DeserializeVariables(std::istream& is);

	static TArray<TSharedPointer<CConfig>> Configs;
};

template <class T>
bool CConfig::RegisterConfig(bool bCreateIfMissing)
{
	TSharedPointer<CConfig> Config = CreatePointer<T>();
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
TSharedPointer<T> CConfig::GetConfig()
{
	for (TSharedPointer<CConfig> Config : Configs)
	{
		if (TSharedPointer<T> CheckedConfig = Cast<T>(Config))
		{
			return CheckedConfig;
		}	
	}
	return nullptr;
}

#define DefineConfigVariable(VariableName)\
	Variables.push_back(SConfigVariable(std::string(#VariableName), &VariableName))