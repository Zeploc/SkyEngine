// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once

#include <memory>

#define SE_PLATFORM_WINDOWS
#define SE_BUILD_DLL
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
using Scope = std::unique_ptr<T>;


#ifndef NDEBUG
#   define ensure(Expr, Msg) \
__M_Assert(#Expr, Expr, __FILE__, __LINE__, Msg); \
_ASSERTE(Expr)
#else
#   define M_Assert(Expr, Msg) ;
#endif

ENGINE_API void __M_Assert(const char* expr_str, bool expr, const char* file, int line, const char* msg);