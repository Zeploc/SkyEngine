#include "SEPCH.h"
#include "StringUtils.h"

#include <filesystem>

bool StringUtils::ReplaceFirst(std::string& InString, const std::string& From, const std::string& To) {
	const size_t Start_Pos = InString.find(From);
	if(Start_Pos == std::string::npos)
		return false;
	InString.replace(Start_Pos, From.length(), To);
	return true;
}

void StringUtils::Replace(std::string& InString, const std::string& From, const std::string& To) {
	if(From.empty())
		return;
	size_t Start_Pos = 0;
	while((Start_Pos = InString.find(From, Start_Pos)) != std::string::npos) {
		InString.replace(Start_Pos, From.length(), To);
		Start_Pos += To.length(); // In case 'to' contains 'from', like replacing 'x' with 'yx'
	}
}

std::wstring StringUtils::ToWString(const std::string& InString)
{
	return std::wstring(InString.begin(), InString.end());
}

std::string StringUtils::NormalizePath(const std::string& MessyPath)
{
	std::filesystem::path Path(MessyPath);
	std::string NPath = Path.make_preferred().string();
	return NPath;
}

// trim from start (in place)
inline void StringUtils::LTrim(std::string &s) {
	s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
		return !std::isspace(ch);
	}));
}

// trim from end (in place)
inline void StringUtils::RTrim(std::string &s) {
	s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
		return !std::isspace(ch);
	}).base(), s.end());
}

// trim from both ends (in place)
inline void StringUtils::Trim(std::string &s) {
	RTrim(s);
	LTrim(s);
}

// trim from start (copying)
inline std::string StringUtils::LTrim_Copy(std::string s) {
	LTrim(s);
	return s;
}

// trim from end (copying)
inline std::string StringUtils::RTrim_Copy(std::string s) {
	RTrim(s);
	return s;
}

// trim from both ends (copying)
inline std::string StringUtils::Trim_Copy(std::string s) {
	Trim(s);
	return s;
}

TArray<std::string> StringUtils::Split(std::string String, const std::string& Delimniator)
{
	TArray<std::string> Tokens;
	size_t pos;
	std::string token;
	while ((pos = String.find(Delimniator)) != std::string::npos) {
		std::string Token = String.substr(0, pos);
		Tokens.push_back(Token);
		String.erase(0, pos + Delimniator.length());
	}
	Tokens.push_back(String);
	
	return Tokens;
}
