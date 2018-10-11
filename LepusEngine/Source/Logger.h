#pragma once

namespace LepusEngine 
{
	// class to print logs to stdout
	// by default it's disabled - set the Enabled variable to true
	class Logger {
	public:
		static bool Enabled;
		enum EventTypes { LEPUS_ERROR, LEPUS_INFO, LEPUS_WARNING };
		static void Log(char* className, char* funcName, int eventType, char* message, char* funcParams);
		static void LogError(char* className, char* funcName, char* message, char* funcParams = "");
		static void LogInfo(char* className, char* funcName, char* message, char* funcParams = "");
		static void LogWarning(char* className, char* funcName, char* message, char* funcParams = "");
	};
}