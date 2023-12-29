#ifndef LENGINE_CONSOLE_LOGGER_INSTANCE
#define LENGINE_CONSOLE_LOGGER_INSTANCE

#include "ILogger.h"

namespace lepus
{
	namespace engine
	{
		// An stdout-based ILogger implementation.
		// LEPUS_ALLOW_STDOUT needs to be defined and LepusEngine::ConsoleLogger::Enabled needs to be true for the logs to appear.
		class ConsoleLogger : ILogger
		{
			private:
			void LogInternal(const char* className, const char* funcName, LogEventTypes eventType, const char* message, const char* funcParams);
			static ILogger* m_Instance;

			public:
			bool Enabled;

			ConsoleLogger()
			{
				Enabled = false;
			}

			static ConsoleLogger& Global()
			{
				if (!m_Instance)
				{
					m_Instance = new ConsoleLogger();
				}

				return *(ConsoleLogger*)m_Instance;
			}

			inline void Log(const char* className, const char* funcName, LogEventTypes eventType, const char* message, const char* funcParams)
			{
#ifdef LEPUS_ALLOW_STDOUT
				LogInternal(className, funcName, eventType, message, funcParams);
#endif
			}

			void LogError(const char* className, const char* funcName, const char* message, const char* funcParams = "") { Log(className, funcName, LogEventTypes::LEPUS_ERROR, message, funcParams); }
			void LogInfo(const char* className, const char* funcName, const char* message, const char* funcParams = "") { Log(className, funcName, LogEventTypes::LEPUS_INFO, message, funcParams); }
			void LogWarning(const char* className, const char* funcName, const char* message, const char* funcParams = "") { Log(className, funcName, LogEventTypes::LEPUS_WARNING, message, funcParams); }

			static void Shutdown()
			{
				if (m_Instance)
				{
					delete m_Instance;
					m_Instance = nullptr;
				}
			}
		};
	}
}
#endif