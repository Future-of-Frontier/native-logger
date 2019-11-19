#include "Logger.hpp"

int Logger::LoggerMain(LPVOID lpParamater)
{
	SetupLogfile();
	if (MH_Initialize() != MH_OK)
	{
		Log("Minhook failed to init!");
		return -1;
	}
	
	// Todo here:
	// 1. Locate crypt / decrypt
	// 2. Minhook them
	// 3. Log packets!
	bool gameLoaded = false;
	HMODULE game_dll = nullptr;
	Log("[INFO] Waiting for game dll to load...");
	while (!gameLoaded)
	{
		auto sd_module = GetModuleHandle("mhfo.dll");
		auto hd_module = GetModuleHandle("mhfo-hd.dll");
		if (sd_module)
		{
			game_dll = sd_module;
			gameLoaded = true;
		}
		if (hd_module)
		{
			game_dll = hd_module;
			gameLoaded = true;
		}
		Sleep(1000);
	}
	Log("[INFO] Module found! Handle @ %X", game_dll);

	memory::pattern_batch crypt_batch;
	crypt_batch.add("Encrypt Function", "E8 ? ? ? ? 8B 97 ? ? ? ? 8B 84 97 ? ? ? ?", [](memory::handle ptr)
	{
		enc = ptr.add(1).rip().as<PacketFn>();
	});

	crypt_batch.add("Decrypt Function", "E8 ? ? ? ? 66 8B 57 1A 66 3B 95 ? ? ? ? ", [](memory::handle ptr)
	{
		dec = ptr.add(1).rip().as<PacketFn>();
	});

	crypt_batch.run(memory::module(game_dll));

	if (!enc || !dec)
	{
		Log("[ERROR] Finding enc or dec routines failed!");
		Log("[ERROR] Found enc %X; Found dec %X", enc, dec);
		return -1;
	}
	else
	{
		Log("[DEBUG] Found enc: %X; Found dec: %X", enc, dec);
	}
	
	return 0;
}

void Logger::SetupLogfile()
{
	if (!logSetup)
	{
		log_out.open("MHZLogger.log", 'r');
		log_out << "[INIT] MHZLogger";
	}
}

void Logger::Log(std::string fmt, ...)
{
	if (logSetup)
	{
		int n, size = 100;
		std::string str;
		va_list ap;

		while (1) {
			str.resize(size);
			va_start(ap, fmt);
			int n = vsnprintf(&str[0], size, fmt.c_str(), ap);
			va_end(ap);

			if (n > -1 && n < size)
				Logger::log_out << str.c_str();
			if (n > -1)
				size = n + 1;
			else
				size *= 2;
		}
	}
}
