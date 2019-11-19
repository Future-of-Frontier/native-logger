#include "Logger.hpp"


extern "C" {

	int WINAPI LoggerMain(LPVOID lpParameter)
	{
		Logger::LoggerMainCPP();
		return 0;
	}
}

void Logger::LoggerMainCPP()
{
	SetupLogfile();
	if (MH_Initialize() != MH_OK)
	{
		Log("Minhook failed to init!");
		return;
	}
	
	// Todo here:
	// 1. Locate crypt / decrypt
	// 2. Minhook them
	// 3. Log packets!

	return;
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
