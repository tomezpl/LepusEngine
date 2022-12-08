#include "../ConsoleLogger.h"
#include <string>
#include <iostream>

using namespace LepusEngine;
using namespace std;

bool ConsoleLogger::Enabled = false;

void ConsoleLogger::Log(char* cN, char* fN, int eT, char* msg, char* fP)
{
	if(!Enabled)
		return;
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

void ConsoleLogger::LogError(char* cN, char* fN, char* msg, char* fP) { Log(cN, fN, LogEventTypes::LEPUS_ERROR, msg, fP); }
void ConsoleLogger::LogInfo(char* cN, char* fN, char* msg, char* fP) { Log(cN, fN, LogEventTypes::LEPUS_INFO, msg, fP); }
void ConsoleLogger::LogWarning(char* cN, char* fN, char* msg, char* fP) { Log(cN, fN, LogEventTypes::LEPUS_WARNING, msg, fP); }