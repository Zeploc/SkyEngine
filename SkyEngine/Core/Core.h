// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once

#include <memory>
#include <vector>
#include <string>
#include <map>
#include "CoreTypes.h"


// TODO: Warnings with exporting class containing STDL
#pragma warning (disable : 4251)

// #define NDEBUG

// If define fails on compile?
#ifdef SE_PLATFORM_WINDOWS
	#ifdef SE_BUILD_DLL
		#define ENGINE_API __declspec(dllexport)
	#else
		#define ENGINE_API __declspec(dllimport)
	#endif
#else
	#error SkyEngine only supports Windows!
#endif

#define SE_BIND_EVENT_FN(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }

#define DISABLE_OPTIMISATION
	#pragma optimize("", off)
#define ENABLE_OPTIMISATION
	#pragma optimize("", on)

#ifdef SKYENGINE_DEBUG
	#define SE_ENABLE_ASSERTS
#endif

#ifdef SE_ENABLE_ASSERTS
	#define ASSERT(x, ...)\
	 __M_Assert(#x, x, __FILE__, __LINE__, __VA_ARGS__)
	#define ASSERT_CORE(x, ...)\
	 __M_Assert(#x, x, __FILE__, __LINE__, __VA_ARGS__)
	#define ENSURE(x, ...)\
	 __M_Ensure(#x, x, __FILE__, __LINE__, __VA_ARGS__)
	#define ENSURE_CORE(x, ...)\
	__M_Ensure(#x, x, __FILE__, __LINE__, __VA_ARGS__)
#else
	#define ASSERT(x, ...)
	#define ASSERT_CORE(x, ...)
	#define ENSURE(x, ...) x
	#define ENSURE_CORE(x, ...) x
#endif

#ifndef NDEBUG
#   define ensure(Expr, Msg) \
__M_Ensure(#Expr, Expr, __FILE__, __LINE__, Msg)
#else
#   define M_Assert(Expr, Msg)
#endif

#define LOG_MESSAGE(Text, ...)\
CLogManager::Get()->Log(ELogMessageType::Message, std::format(Text, __VA_ARGS__))
#define LOG_WARNING(Text, ...)\
CLogManager::Get()->Log(ELogMessageType::Warning, std::format(Text, __VA_ARGS__))
#define LOG_ERROR(Text, ...)\
CLogManager::Get()->Log(ELogMessageType::Error, std::format(Text, __VA_ARGS__))

ENGINE_API bool __M_Ensure(const char* expr_str, bool expr, const char* file, int line, const char* msg);
template<class T>
bool __M_Ensure(const char* expr_str, T* ptr, const char* file, int line, const char* msg = "No information given")
{
	return __M_Ensure(expr_str, ptr != nullptr, file, line, msg);
}
template<class T>
bool __M_Ensure(const char* expr_str, std::shared_ptr<T> ptr, const char* file, int line, const char* msg = "No information given")
{
	return __M_Ensure(expr_str, ptr.get(), file, line, msg);
}
ENGINE_API void __M_Assert(const char* expr_str, bool expr, const char* file, int line, const char* msg = "No information given");