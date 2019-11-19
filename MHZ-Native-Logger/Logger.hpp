#pragma once
#include <Windows.h>
#include <iostream>
#include <fstream>
#include <MinHook.h>
#pragma comment(lib, "libMinHook.lib")


namespace Logger
{
	void LoggerMainCPP();
	void SetupLogfile();
	void Log(std::string fmt, ...);

	static bool logSetup = false;
	static std::ofstream log_out;
}