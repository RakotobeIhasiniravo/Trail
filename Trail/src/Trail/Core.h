#pragma once

//this if def is here to define to help us not have to think about writing __declspec(dllexport) or __declspec(dllimport)
//if we're in the dll, we export, if we're not in the dll we import
#ifdef TRL_PLATFORM_WINDOWS
	
#else
	#error Trail only supports windows
#endif


#ifdef TRL_ENABLE_ASSERTS
	#define TRL_ASSERT(x, ...) { if(!(x)) {TRL_ERROR("Assertion failed: {0}", __VA_ARGS__);__debugbreak();} }
	#define TRL_CORE_ASSERT(x, ...) { if(!(x)) {TRL_CORE_ERROR("Assertion failed: {0}", __VA_ARGS__);__debugbreak();} }
#else
	#define  TRL_ASSERT(x, ...)
	#define  TRL_CORE_ASSERT(x, ...)
#endif

#define BIT(x) (1 << x)
#define TRL_BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)
