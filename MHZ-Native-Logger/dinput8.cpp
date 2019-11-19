#include "Logger.hpp"

HINSTANCE hLThis = 0;
FARPROC p[6];
HINSTANCE hL = 0;
bool running = false;

BOOL WINAPI DllMain(HINSTANCE hInst, DWORD reason,LPVOID v)
{
	if (reason == DLL_PROCESS_ATTACH)
	{
		hLThis = hInst;
		hL = LoadLibrary("C:\\Windows\\SysWOW64\\dinput8.dll");
		if (!hL)
		{
			MessageBox(nullptr, "Unable to load dinput8_.dll for proxying!", "Error", MB_OK);
			return false;
		}
		char buf[64];
		GetModuleFileName(nullptr, (LPSTR)&buf, 64);
		if (strcmp(buf, "mhf.exe") && !running)
		{
			CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)&Logger::LoggerMain, NULL, 0, NULL);
			running = true;
		}
	}
	

	p[0] = GetProcAddress(hL, "DirectInput8Create");
	p[1] = GetProcAddress(hL, "DllCanUnloadNow");
	p[2] = GetProcAddress(hL, "DllGetClassObject");
	p[3] = GetProcAddress(hL, "DllRegisterServer");
	p[4] = GetProcAddress(hL, "DllUnregisterServer");
	p[5] = GetProcAddress(hL, "GetdfDIJoystick");

	if (reason == DLL_PROCESS_DETACH)
	{
		FreeLibrary(hL);
		return 1;
	}

	return TRUE;
}

extern "C"
{
	void PROXY_DirectInput8Create() {
		__asm
		{			jmp p[1 * 4]
		}
	}
	void PROXY_DllCanUnloadNow() {
		__asm
		{			jmp p[2 * 4]
		}
	}
	void PROXY_DllGetClassObject() {
		__asm
		{			jmp p[3 * 4]
		}
	}
	void PROXY_DllRegisterServer() {
		__asm
		{			jmp p[4 * 4]
		}
	}
	void PROXY_DllUnregisterServer() {
		__asm
		{			jmp p[5 * 4]
		}
	}
	void PROXY_GetdfDIJoystick() {
		__asm
		{			jmp p[6 * 4]
		}
	}
}
