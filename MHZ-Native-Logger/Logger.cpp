#include "Logger.hpp"

int Logger::LoggerMain()
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
	

	while (true)
	{
		Sleep(1000);
	}
	return 0;
}

void Logger::SetupLogfile()
{
	if (!logSetup)
	{
		log_out.open("MHZLogger.log", 'r');
		log_out << "[INIT] MHZLogger" << std::endl;
		log_out.flush();
		logSetup = true;
	}
}

void Logger::Log(std::string fmt_str, ...)
{
	if (logSetup)
	{
		int final_n, n = ((int)fmt_str.size()) * 2; /* Reserve two times as much as the length of the fmt_str */
		std::unique_ptr<char[]> formatted;
		va_list ap;
		while (1) {
			formatted.reset(new char[n]); /* Wrap the plain char array into the unique_ptr */
			strcpy(&formatted[0], fmt_str.c_str());
			va_start(ap, fmt_str);
			final_n = vsnprintf(&formatted[0], n, fmt_str.c_str(), ap);
			va_end(ap);
			if (final_n < 0 || final_n >= n)
				n += abs(final_n - n + 1);
			else
				break;
		}

		log_out << std::string(formatted.get()).c_str() << std::endl;
		log_out.flush();
	}
	else
	{
		MessageBox(nullptr, "Log called before setup?!", "uwu", MB_OK);
	}
}
