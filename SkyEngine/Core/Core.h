#pragma once

#include <memory>

#define SE_PLATFORM_WINDOWS
#define SE_BUILD_DLL

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
using Pointer = std::shared_ptr<T>;

template<typename T>
using Scope = std::unique_ptr<T>;

