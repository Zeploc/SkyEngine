
#include "Core.h"

#include <format>
#include <iostream>

#include "Input/CXBOXController.h"

void __M_Assert(const char* expr_str, bool expr, const char* file, int line, const char* msg)
{
	if (!expr)
	{
		std::string Message = std::format("Assert failed:\t {}\nExpected:\t{}\nSource:\t\t{}:{}\n", msg, expr_str, file, line);
		std::cerr << Message;
		// int Outcome = MessageBoxA(NULL, Message.c_str(), "Assert failed!", MB_ABORTRETRYIGNORE);
		// assert(expr);
		//std::terminate();
		// _ASSERTE(expr);
		// __debugbreak();
		// abort();
	}
	//##__VA_ARGS__
}
