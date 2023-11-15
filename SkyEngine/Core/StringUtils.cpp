#include "SEPCH.h"
#include "StringUtils.h"

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

// // trim from start (in place)
// static inline void ltrim(std::string &s) {
// 	s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
// 		return !std::isspace(ch);
// 	}));
// }
//
// // trim from end (in place)
// static inline void rtrim(std::string &s) {
// 	s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
// 		return !std::isspace(ch);
// 	}).base(), s.end());
// }
//
// // trim from both ends (in place)
// static inline void trim(std::string &s) {
// 	rtrim(s);
// 	ltrim(s);
// }
//
// // trim from start (copying)
// static inline std::string ltrim_copy(std::string s) {
// 	ltrim(s);
// 	return s;
// }
//
// // trim from end (copying)
// static inline std::string rtrim_copy(std::string s) {
// 	rtrim(s);
// 	return s;
// }
//
// // trim from both ends (copying)
// static inline std::string trim_copy(std::string s) {
// 	trim(s);
// 	return s;
// }