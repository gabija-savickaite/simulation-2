#include "Application.h"
#include "./Config.h"

#include "./Simulation.h"

#include <iostream>
#include <fstream>
#include "./Native.h"

#ifdef _WIN32
#include <windows.h>
#endif

namespace
{
	void setConsolePosition()
	{
#ifdef _WIN32
		HWND consoleWindow = GetConsoleWindow();
		SetWindowPos(consoleWindow, 0, 0, 0, 500, 500, SWP_NOSIZE | SWP_NOZORDER);
#endif
	}

	constexpr int NUM_OPTIONS = 2;
	Config config;

	void loadConfig()
	{
		std::ifstream inFile("config.txt");
		if (!inFile.is_open()) {
			std::cout << "Unable to load config, making default settings\n";
			config =
			{ { 1000, 1000 }, 4 };
			return;
		}
		std::string line;
		while (std::getline(inFile, line)) {
			if (line == "fps") inFile >> config.fps;
			else if (line == "winx") inFile >> config.windowSize.x;
			else if (line == "winy") inFile >> config.windowSize.y;
			else if (line == "cellsize") inFile >> config.cellSize;
		}

		config.init();
	}
}

int main()
{
	loadConfig();
	Application app(config);
	app.init<Simulation>();
	app.run();
}
