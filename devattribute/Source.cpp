#include "devattribute.h"
#include <iostream>





int main()
{
	setConsoleNoSelection(true);
	Sleep(1000);
	setConsoleNoSelection(false);

	pause();

}



/*	MEMORYSTATUSEX memInfo = { 0 };
	memInfo.dwLength = sizeof(memInfo);
	GlobalMemoryStatusEx(&memInfo);
	long double oldMem = memInfo.ullAvailPhys;
	memory::_d_quickClean();
	ZeroMemory(&memInfo, sizeof(memInfo));
	memInfo.dwLength = sizeof(memInfo);
	GlobalMemoryStatusEx(&memInfo);
	long double newMem = memInfo.ullAvailPhys - oldMem;
	std::string mem = std::to_string(newMem / pow(1024, 2));
	std::cout << "\nPhysical memory cleared with result: " << mem.erase(mem.length() - 5, 5) << " mb";
	system("pause > nul");*/