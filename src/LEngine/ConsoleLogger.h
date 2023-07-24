#ifndef LENGINE_CONSOLE_LOGGER_INSTANCE
#define LENGINE_CONSOLE_LOGGER_INSTANCE

#include "ILogger.h"

namespace LepusEngine
{
	// An stdout-based ILogger implementation.
	// LEPUS_ALLOW_STDOUT needs to be defined and LepusEngine::ConsoleLogger::Enabled needs to be true for the logs to appear.
	class ConsoleLogger : ILogger
	{
		private:
		void LogInternal(char* className, char* funcName, int eventType, char* message, char* funcParams);
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

			return *reinterpret_cast<ConsoleLogger*>(m_Instance);
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
#endif