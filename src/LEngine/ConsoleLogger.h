#pragma once

#include "ILogger.h"

namespace LepusEngine 
{
	// An stdout-based ILogger implementation.
	// LEPUS_ALLOW_STDOUT needs to be defined and LepusEngine::ConsoleLogger::Enabled needs to be true for the logs to appear.
	class ConsoleLogger : ILogger {
	private:
		static void LogInternal(char* className, char* funcName, int eventType, char* message, char* funcParams);
	public:
		static bool Enabled;

		static inline void Log(char* className, char* funcName, int eventType, char* message, char* funcParams)
		{
#ifdef LEPUS_ALLOW_STDOUT
			LogInternal(className, funcName, eventType, message, funcParams);
#endif
		}

		static void LogError(char* className, char* funcName, char* message, char* funcParams = "") { Log(className, funcName, LogEventTypes::LEPUS_ERROR, message, funcParams); }
		static void LogInfo(char* className, char* funcName, char* message, char* funcParams = "") { Log(className, funcName, LogEventTypes::LEPUS_INFO, message, funcParams); }
		static void LogWarning(char* className, char* funcName, char* message, char* funcParams = "") { Log(className, funcName, LogEventTypes::LEPUS_WARNING, message, funcParams); }
	};
}