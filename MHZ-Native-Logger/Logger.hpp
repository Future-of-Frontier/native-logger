#pragma once
#include <Windows.h>
#include <iostream>
#include <fstream>
#include <MinHook.h>
#include "memory/all.hpp"
#pragma comment(lib, "libMinHook.lib")

typedef int(__fastcall *PacketFn)(int, char*, int, char*, int, unsigned short*);

namespace Logger
{
	int LoggerMain(LPVOID);
	void SetupLogfile();
	void Log(std::string fmt, ...);

	static bool logSetup = false;
	static std::ofstream log_out;
	static PacketFn orig_enc;
	static PacketFn orig_dec;
	static PacketFn enc;
	static PacketFn dec;
}