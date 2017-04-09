#pragma once

namespace LepusEngine 
{
	// class to print logs to stdout
	// by default works only in Debug - define LEPUS_ENABLE_LOG in app to force it
	class Logger {
	public:
		enum EventTypes { LEPUS_ERROR, LEPUS_INFO, LEPUS_WARNING };
		static void Log(char* className, char* funcName, int eventType, char* message, char* funcParams);
		static void LogError(char* className, char* funcName, char* message, char* funcParams = "");
		static void LogInfo(char* className, char* funcName, char* message, char* funcParams = "");
		static void LogWarning(char* className, char* funcName, char* message, char* funcParams = "");
	};
}