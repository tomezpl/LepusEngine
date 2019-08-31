#include "../Logger.h"
#include <string>
#include <iostream>

using namespace LepusEngine;
using namespace std;

bool Logger::Enabled = false;

void Logger::Log(char* cN, char* fN, int eT, char* msg, char* fP)
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
	case EventTypes::LEPUS_ERROR:
		output.append("error");
		break;
	case EventTypes::LEPUS_INFO:
		output.append("info");
		break;
	case EventTypes::LEPUS_WARNING:
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

void Logger::LogError(char* cN, char* fN, char* msg, char* fP) { Log(cN, fN, EventTypes::LEPUS_ERROR, msg, fP); }
void Logger::LogInfo(char* cN, char* fN, char* msg, char* fP) { Log(cN, fN, EventTypes::LEPUS_INFO, msg, fP); }
void Logger::LogWarning(char* cN, char* fN, char* msg, char* fP) { Log(cN, fN, EventTypes::LEPUS_WARNING, msg, fP); }