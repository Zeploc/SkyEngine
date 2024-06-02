// Copyright Skyward Studios, Inc. All Rights Reserved.

#include "SEPCH.h"
#include "Config.h"

#include "Core/Application.h"
#include "Platform/File/FileManager.h"

TArray<TPointer<CConfig>> CConfig::Configs;

CConfig::CConfig(std::string Name, std::string RelativePath)
	: ConfigName(Name), ConfigPath(RelativePath)
{
}

std::string CConfig::GetAbsoluteConfigPath() const
{
	return GetApplication()->GetProjectDirectory() + "\\Config\\" + ConfigPath + ConfigName + ".ini";
}

void CConfig::SerializeVariables(std::ostream& os) const
{
	std::string Section = "Default";
	for (ConfigVariable Variable : Variables)
	{
		std::stringstream ss;
		Variable.SerializeVariable(ss);
		WritePrivateProfileStringA(Section.c_str(), Variable.VariableName.c_str(), ss.str().c_str(), GetAbsoluteConfigPath().c_str());

		Variable.SerializeVariable(os);
	}
}

void CConfig::DeserializeVariables(std::istream& is)
{
	std::string Section = "Default";
	for (ConfigVariable Variable : Variables)
	{
		char sResult[256] = {};
		DWORD ResultSize = GetPrivateProfileStringA(Section.c_str(), Variable.VariableName.c_str(), std::string().c_str(), sResult, 255, GetAbsoluteConfigPath().c_str());
		// If a value is found (a value of size 0 will not count as found)
		if (ResultSize > 0)
		{
			std::stringstream ss(sResult);
			Variable.DeserializeVariable(ss);
		}
	}
}

bool CConfig::LoadConfig()
{
	std::string FileContents;
	if (!CFileManager::ReadFile(GetAbsoluteConfigPath(), FileContents))
	{
		return false;
	}
	std::stringstream FileStream(FileContents);
	DeserializeVariables(FileStream);
	return true;
}

bool CConfig::DoesConfigExist() const
{
	return CFileManager::FileExists(GetAbsoluteConfigPath());
}

bool CConfig::SaveConfig()
{
	std::stringstream Serialized;
	SerializeVariables(Serialized);
	
	// if (!CFileManager::SaveFile(GetAbsoluteConfigPath(), Serialized.str()))
	// {
	// 	return false;
	// }
	return true;
}
