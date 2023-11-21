#pragma once

namespace lepus
{
	namespace engine
	{
		class ILogger
		{
			public:
			enum LogEventTypes { LEPUS_ERROR, LEPUS_INFO, LEPUS_WARNING };

			virtual void Log(char* className, char* funcName, int eventType, char* message, char* funcParams) = 0;
			virtual void LogError(char* className, char* funcName, char* message, char* funcParams = "") = 0;
			virtual void LogInfo(char* className, char* funcName, char* message, char* funcParams = "") = 0;
			virtual void LogWarning(char* className, char* funcName, char* message, char* funcParams = "") = 0;
		};
	}
}