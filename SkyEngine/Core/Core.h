// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once

#include <memory>

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

template<typename T>
using TPointer = std::shared_ptr<T>;
template<typename T>
using TWeakPointer = std::weak_ptr<T>;

template<typename T>
using TScope = std::unique_ptr<T>;

// TODO: Move to premake preprocessor macros
#define SE_ENABLE_ASSERTS

#ifdef SE_ENABLE_ASSERTS
	#define ASSERT(x, ...)\
	{ if (!x) { /* TODO: Error*/__M_Assert(#x, x, __FILE__, __LINE__, __VA_ARGS__); __debugbreak();}}
	#define ASSERT_CORE(x, ...)\
	{ if (!x) { /* TODO: Error*/__M_Assert(#x, x, __FILE__, __LINE__, __VA_ARGS__); __debugbreak();}}
	#define ENSURE(x, ...)\
	 __M_Assert(#x, x, __FILE__, __LINE__, __VA_ARGS__)
	#define ENSURE_CORE(x, ...)\
	__M_Assert(#x, x, __FILE__, __LINE__, __VA_ARGS__)
#else
	#define ASSERT(x, ...)
	#define ASSERT_CORE(x, ...)
	#define ENSURE(x, ...) x
	#define ENSURE_CORE(x, ...) x
#endif

#ifndef NDEBUG
#   define ensure(Expr, Msg) \
__M_Assert(#Expr, Expr, __FILE__, __LINE__, Msg); \
_ASSERTE(Expr)
#else
#   define M_Assert(Expr, Msg) ;
#endif

ENGINE_API bool __M_Assert(const char* expr_str, bool expr, const char* file, int line, const char* msg);