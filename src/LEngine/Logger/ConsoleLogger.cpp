#include "../ConsoleLogger.h"
#include <string>
#include <iostream>

using namespace LepusEngine;
using namespace std;

ILogger* ConsoleLogger::m_Instance = nullptr;

void ConsoleLogger::LogInternal(char* cN, char* fN, int eT, char* msg, char* fP)
{
	if (Enabled)
	{

		string output = cN;
		output.append("::");
		output.append(fN);
		output.append("(");
		output.append(fP);
		output.append(") - (event: ");

		switch (eT)
		{
			case LogEventTypes::LEPUS_ERROR:
				output.append("error");
				break;
			case LogEventTypes::LEPUS_INFO:
				output.append("info");
				break;
			case LogEventTypes::LEPUS_WARNING:
				output.append("warning");
				break;
			default:
				output.append("unknown, type ");
				output.append(to_string(eT));
				break;
		}

		output.append("): ");
		output.append(msg);

		cout << output << endl;
	}
}