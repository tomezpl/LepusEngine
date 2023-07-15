#pragma once

#include "ILogger.h"

namespace LepusEngine 
{
	// An stdout-based ILogger implementation.
	// LEPUS_ALLOW_STDOUT needs to be defined and LepusEngine::ConsoleLogger::Enabled needs to be true for the logs to appear.
	class ConsoleLoggerImpl : ILogger {
	private:
		void LogInternal(char* className, char* funcName, int eventType, char* message, char* funcParams);
	public:
		bool Enabled;

		ConsoleLoggerImpl() {
			Enabled = false;
		}

		inline void Log(char* className, char* funcName, int eventType, char* message, char* funcParams)
		{
#ifdef LEPUS_ALLOW_STDOUT
			LogInternal(className, funcName, eventType, message, funcParams);
#endif
		}

		void LogError(char* className, char* funcName, char* message, char* funcParams = "") { Log(className, funcName, LogEventTypes::LEPUS_ERROR, message, funcParams); }
		void LogInfo(char* className, char* funcName, char* message, char* funcParams = "") { Log(className, funcName, LogEventTypes::LEPUS_INFO, message, funcParams); }
		void LogWarning(char* className, char* funcName, char* message, char* funcParams = "") { Log(className, funcName, LogEventTypes::LEPUS_WARNING, message, funcParams); }
	};

	ConsoleLoggerImpl ConsoleLogger;
}