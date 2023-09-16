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
template<typename T, typename ... Args>
constexpr TPointer<T> CreatePointer(Args&& ... args)
{
	return std::make_shared<T>(std::forward<Args>(args)...);
}

template<typename T>
using TScope = std::unique_ptr<T>;
template<typename T, typename ... Args>
constexpr TScope<T> CreateScope(Args&& ... args)
{
	return std::make_unique<T>(std::forward<Args>(args)...);
}

#define SE_BIND_EVENT_FN(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }

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
__M_Ensure(#Expr, Expr, __FILE__, __LINE__, Msg); \
_ASSERTE(Expr)
#else
#   define M_Assert(Expr, Msg) ;
#endif

ENGINE_API bool __M_Ensure(const char* expr_str, bool expr, const char* file, int line, const char* msg);
ENGINE_API void __M_Assert(const char* expr_str, bool expr, const char* file, int line, const char* msg);