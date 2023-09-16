// Copyright Skyward Studios, Inc. All Rights Reserved.

#include "SEPCH.h"
#include "Core.h"

#include <format>
#include <iostream>

#include "Input/CXBOXController.h"

bool __M_Ensure(const char* expr_str, bool expr, const char* file, int line, const char* msg)
{
	if (!expr)
	{
		std::string Message = std::format("Assert failed:\t {}\nExpected:\t{}\nSource:\t\t{}:{}\n", msg, expr_str, file, line);
		std::cerr << Message;
		// TODO: Continue/retry popup 
		__debugbreak();
		/* TODO: Error through macro that logs */
		// int Outcome = MessageBoxA(NULL, Message.c_str(), "Assert failed!", MB_ABORTRETRYIGNORE);
		// assert(expr);
		//std::terminate();
		// _ASSERTE(expr);
		// __debugbreak();
		// abort();
	}
	//##__VA_ARGS__
	return expr;
}

void __M_Assert(const char* expr_str, bool expr, const char* file, int line, const char* msg)
{
	if (!__M_Ensure(expr_str, expr, file, line, msg))
	{
		/* TODO: Error*/
	}
}
