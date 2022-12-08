#pragma once

#include "ILogger.h"

namespace LepusEngine 
{
	// An stdout-based ILogger implementation.
	// LEPUS_ALLOW_STDOUT needs to be defined and LepusEngine::ConsoleLogger::Enabled needs to be true for the logs to appear.
	class ConsoleLogger : ILogger {
	public:
		static bool Enabled;
		static void Log(char* className, char* funcName, int eventType, char* message, char* funcParams);
		static void LogError(char* className, char* funcName, char* message, char* funcParams = "");
		static void LogInfo(char* className, char* funcName, char* message, char* funcParams = "");
		static void LogWarning(char* className, char* funcName, char* message, char* funcParams = "");
	};
}