#pragma once

namespace lepus
{
	namespace engine
	{
		enum LogEventTypes { LEPUS_ERROR, LEPUS_INFO, LEPUS_WARNING };
		class ILogger
		{
			public:

			virtual void Log(const char* className, const char* funcName, LogEventTypes eventType, const char* message, const char* funcParams) = 0;
			virtual void LogError(const char* className, const char* funcName, const char* message, const char* funcParams = "") = 0;
			virtual void LogInfo(const char* className, const char* funcName, const char* message, const char* funcParams = "") = 0;
			virtual void LogWarning(const char* className, const char* funcName, const char* message, const char* funcParams = "") = 0;

			/// @brief Dummy virtual destructor. Unlikely it'll ever be needed but the compiler wants one anyway.
			virtual ~ILogger() {}
		};
	}
}