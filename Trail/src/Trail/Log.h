#pragma once

#include <memory>
#include "Core.h"
#include "spdlog/spdlog.h"
#include <spdlog/fmt/fmt.h> // Ensure this is included
namespace Trail {
	class   Log
	{
	public:
		static void Init();

		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger;  }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }
	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
	};
}


//CORE LOG MACROS 
//scope resolution operator is put in front to make the compiler always search for Trail namespace from the globals scope and not any other nested namespace
#define TRL_CORE_TRACE(...)   ::Trail::Log::GetCoreLogger()->trace(__VA_ARGS__); 
#define TRL_CORE_INFO(...)    ::Trail::Log::GetCoreLogger()->info(__VA_ARGS__); 
#define TRL_CORE_WARN(...)    ::Trail::Log::GetCoreLogger()->warn(__VA_ARGS__); 
#define TRL_CORE_ERROR(...)   ::Trail::Log::GetCoreLogger()->error(__VA_ARGS__);
#define TRL_CORE_FATAL(...)   ::Trail::Log::GetCoreLogger()->fatal(__VA_ARGS__); 

//CLIENT LOG MACROS 
#define TRL_TRACE(...)        ::Trail::Log::GetClientLogger()->trace(__VA_ARGS__); 
#define TRL_INFO(...)         ::Trail::Log::GetClientLogger()->info(__VA_ARGS__); 
#define TRL_WARN(...)         ::Trail::Log::GetClientLogger()->warn(__VA_ARGS__); 
#define TRL_ERROR(...)        ::Trail::Log::GetClientLogger()->error(__VA_ARGS__);
#define TRL_FATAL(...)        ::Trail::Log::GetClientLogger()->fatal(__VA_ARGS__); 