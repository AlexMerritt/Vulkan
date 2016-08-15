#pragma once

#include <vector>
#include <string>
#include <Windows.h>

namespace Log
{
	static std::vector<std::string> LOGS;

	static void Output(std::string s)
	{
		std::string ss = s + "\n";
		OutputDebugStringA(ss.c_str());
	}

	static void Info(std::string s)
	{
		Output(s);
	}

	static void Error(std::string s)
	{
		Output("Error: " + s);
	}

	static void Print()
	{
		for (unsigned int i = 0; i < LOGS.size(); ++i)
		{
			Output(LOGS[i]);
		}
	}
}