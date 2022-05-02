#include "devattribute.h"


static HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
static HANDLE hStdIn = GetStdHandle(STD_INPUT_HANDLE);
CONSOLE_SCREEN_BUFFER_INFO scrBufferInfo;

void winPause()
{
	system("pause > nul");
}

void pause()
{
	_getch();
}

void setColor(consoleColor foreground, consoleColor background)
{
	SetConsoleTextAttribute(hStdOut, (WORD)((background << 4) | foreground));
}

const char* WinFileDialog(HWND hwnd,const char* mode,const char* lpstrFilters){
#if defined(_UNICODE)
	OPENFILENAMEA ofn{};
#elif defined(_MBCS)
	OPENFILENAME ofn{};
#endif
    char pathFile[255]{};
    HANDLE hf;

    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = hwnd;
    ofn.lpstrFile = pathFile;
    ofn.lpstrFile[0] = '\0';
    ofn.nMaxFile = sizeof(pathFile);
    ofn.lpstrFilter = lpstrFilters;
    ofn.nFilterIndex = 1;
    ofn.lpstrFileTitle = NULL;
    ofn.nMaxFileTitle = 0;
    ofn.lpstrInitialDir = NULL;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
#if defined(_UNICODE)
	if (GetSaveFileNameA(&ofn) == TRUE)
		hf = CreateFileA(ofn.lpstrFile,
#elif defined(_MBCS)
	if (GetSaveFileName(&ofn) == TRUE)
		hf = CreateFile(ofn.lpstrFile,
#endif
            GENERIC_READ,
            0,
            (LPSECURITY_ATTRIBUTES)NULL,
            OPEN_EXISTING,
            FILE_ATTRIBUTE_NORMAL,
            (HANDLE)NULL);
    return pathFile;
}

void GotoXY(short X, short Y, std::string print, consoleColor foreground, consoleColor background) 
{
	COORD cord = { X, Y };
	SetConsoleCursorPosition(hStdOut, cord);
	if (foreground != WHITE || background != BLACK) setColor(foreground, background);
	if (print != "") printf(print.c_str());
	if (foreground != WHITE || background != BLACK) setColor(WHITE, BLACK);
}

void sheetGenerator(int x, int y, const int sizeX, const int sizeY, int lengthX, int lengthY) 
{
	utf.setUtfLocale();
	lengthY += 1;
	int buffsizeY = sizeY * lengthY + 1, buffX = x;
	for (int j = 0; j < sizeX; j++) {
		for (int i = 1; i <= buffsizeY; i++) {
			GotoXY(buffX, y + i - 1, "║");
			for (int k = 0; k < lengthX; k++)
				printf(" ");
			printf("║");
			if (i % lengthY == 1) {
				GotoXY(buffX, y + i - 1, "");
				for (int t = 0; t < lengthX + 1; t++)
					printf("═");
				for (int p = 0; p < lengthX + 1; p++)
					GotoXY(buffX, y + i - 1, "╬");
				if (i != 1 && i != buffsizeY && j == 0)
					GotoXY(buffX, y + i - 1, "╠");
				if (i != 1 && i != buffsizeY && j == sizeX - 1)
					GotoXY(buffX + lengthX + 1, y + i - 1, "╣");
			}
			if (i == 1) {
				x = buffX + lengthX + 1;
				GotoXY(buffX, y + i - 1, "╦");
			}
			if (i == buffsizeY && x == buffX + lengthX + 1)
				GotoXY(buffX, y + i - 1, "╩");
		}
		buffX += lengthX + 1;
	}
	GotoXY(x - sizeX * (lengthX + 1), y + sizeY * lengthY, "╚");
	GotoXY(x, y, "╗");
	GotoXY(x, y + sizeY * lengthY, "╝");
	GotoXY(x - sizeX * (lengthX + 1), y, "╔");
}

void cleaning(int x, int y, int width, int height, consoleColor background) 
{
	int counter = 0, buff = y;
	if (background != BLACK) setColor(BLACK, background);
	for (int i = 0; i < height; i++) {
		GotoXY(x, buff++);
		for (int j = 0; j < width; j++)
			printf(" ");
	}
	setColor(WHITE, BLACK);
	GotoXY(x, y);
}

void setConsoleSize(int columnsX, int linesY) 
{
	std::string command = "mode con cols=" + std::to_string(columnsX) + " lines=" + std::to_string(linesY);
	system(command.c_str());
}

int getConsoleSize(std::string position) 
{
	GetConsoleScreenBufferInfo(hStdOut, &scrBufferInfo);
	short winWidth = scrBufferInfo.srWindow.Right - scrBufferInfo.srWindow.Left + 1;
	short winHeight = scrBufferInfo.srWindow.Bottom - scrBufferInfo.srWindow.Top + 1;
	if (position == "width") return winWidth;
	if (position == "height") return winHeight;
	return 0;
}

void setConsoleFullscreen() 
{
	COORD newSize;
	short bufferHeight = getConsoleSize("height");
	short bufferWidth = scrBufferInfo.dwSize.X;
	SetConsoleDisplayMode(GetStdHandle(STD_OUTPUT_HANDLE), CONSOLE_FULLSCREEN_MODE, &newSize);
	keybd_event(VK_MENU, 0x38, 0, 0);
	keybd_event(VK_RETURN, 0x1c, 0, 0);
	keybd_event(VK_RETURN, 0x1c, KEYEVENTF_KEYUP, 0);
	keybd_event(VK_MENU, 0x38, KEYEVENTF_KEYUP, 0);
	newSize.X = bufferWidth;
	newSize.Y = bufferHeight;
	SetConsoleScreenBufferSize(hStdOut, newSize);
}

COORD getConsoleCursorPosition()
{
	CONSOLE_SCREEN_BUFFER_INFO cbsi;
	if (GetConsoleScreenBufferInfo(hStdOut, &cbsi))
		return cbsi.dwCursorPosition;
}

BOOL showConsoleCursor(BOOL bShow)
{
	CONSOLE_CURSOR_INFO cci;
	if (!GetConsoleCursorInfo(hStdOut, &cci)) return 0;
	cci.bVisible = bShow;
	if (!SetConsoleCursorInfo(hStdOut, &cci)) return 0;
	return 1;
}

BOOL regIsBoot(HKEY hkey, const char* subkey)
{
	HKEY key;
	BOOL status = FALSE;
	DWORD dwTemp;
	TCHAR path[MAX_PATH]{};
	static TCHAR regPath[] = TEXT("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run");
	if ((RegOpenKeyEx(hkey, regPath, 0, KEY_READ, &key)) != ERROR_SUCCESS) {}
	else {
		dwTemp = sizeof(path);
#if defined(_UNICODE)
		if ((RegGetValueA(key, NULL, subkey, RRF_RT_REG_SZ, 0, path, &dwTemp)) != ERROR_SUCCESS) {}
#elif defined(_MBCS)
		if ((RegGetValue(key, NULL, subkey, RRF_RT_REG_SZ, 0, path, &dwTemp)) != ERROR_SUCCESS) {}
#endif
		else {
			status = TRUE;
		}
		RegCloseKey(key);
	}
	return status;
}

BOOL addToReg(HKEY hkey , const char* valueName) 
{
		HKEY key;
		RegOpenKeyEx(hkey, TEXT("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run"), 0, KEY_WRITE, &key);
#if defined(_UNICODE)
		char path[MAX_PATH]{};
		GetModuleFileNameA(NULL, path, MAX_PATH);
		if ((RegSetValueExA(key, valueName, 0, REG_SZ, (LPBYTE)path, (strlen(path) + 1) * sizeof(wchar_t))) != ERROR_SUCCESS)
#elif defined(_MBCS)
		TCHAR path[MAX_PATH]{};
		GetModuleFileName(NULL, path, MAX_PATH);
		if ((RegSetValueEx(key, valueName, 0, REG_SZ, (LPBYTE)path, (strlen(path) + 1) * sizeof(wchar_t))) != ERROR_SUCCESS)
#endif
			RegCloseKey(key);
		else return 0;
		return 1;
}

BOOL delFromReg(HKEY hkey, const char* valueName)
{
	HKEY key;
	RegOpenKeyEx(hkey, TEXT("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run"), 0, KEY_WRITE, &key);
#if defined(_UNICODE)
	if (RegDeleteValueA(key, valueName) != ERROR_SUCCESS)
#elif defined(_MBCS)
	if (RegDeleteValue(key, valueName) != ERROR_SUCCESS)
#endif
		RegCloseKey(key);
	else return 0;
	return 1;
}

void consoleResize(BOOL status) 
{
	HWND hwnd = GetConsoleWindow();
	HMENU hm{};
	hm = GetSystemMenu(hwnd, status);
	RemoveMenu(hm, SC_SIZE, MF_BYCOMMAND | MF_REMOVE);
	RemoveMenu(hm, SC_MAXIMIZE, MF_BYCOMMAND | MF_REMOVE);
	DrawMenuBar(hwnd);
	GetMenu(hwnd);
}

bool removeLineFromFile(std::string filename, int index) {
	std::vector<std::string> vec;
	std::ifstream file(filename);
	if (file.is_open())
	{
		std::string str;
		while (std::getline(file, str))
			vec.push_back(str);
		file.close();
		if (vec.size() < index - 1)
			return false;
		vec.erase(vec.begin() + index - 1);
		std::ofstream outfile(filename);
		if (outfile.is_open())
		{
			std::copy(vec.begin(), vec.end(),
				std::ostream_iterator<std::string>(outfile, "\n"));
			outfile.close();
			return true;
		}
		return false;
	}
	return false;
}

BOOL checkPrivilege(HANDLE hToken, LPCTSTR lpszPrivilege, BOOL bEnablePrivilege)
{
	TOKEN_PRIVILEGES tp;
	LUID luid;
	if (!LookupPrivilegeValue(NULL, lpszPrivilege, &luid))
		return FALSE;
	tp.PrivilegeCount = 1;
	tp.Privileges[0].Luid = luid;
	tp.Privileges[0].Attributes = bEnablePrivilege ? SE_PRIVILEGE_ENABLED : 0;
	if (!AdjustTokenPrivileges(hToken, FALSE, &tp, sizeof(TOKEN_PRIVILEGES), (PTOKEN_PRIVILEGES)NULL, (PDWORD)NULL))
		return FALSE;
	if (GetLastError() == ERROR_NOT_ALL_ASSIGNED)
		return FALSE;
	return TRUE;
}

LONG calc_percentof64(_In_ LONG64 length, _In_ LONG64 total_length)
{
	LONG value;
	value = (LONG)(((DOUBLE)length / (DOUBLE)total_length) * 100.0);
	return value;
}

void setConsoleNoSelection(BOOL status) 
{
	DWORD prev_mode{};
	if (status == 1) {
		GetConsoleMode(hStdIn, &prev_mode);
		SetConsoleMode(hStdIn, prev_mode & ~ENABLE_QUICK_EDIT_MODE);
		return;
	}
	if (status == 0) {
		SetConsoleMode(hStdIn, prev_mode);
		return;
	}
}

void mTestButtonGenerate(int rangeXmin,int rangeXmax,int rangeYmin,int rangeYmax,int button) 
{
	RECT r;
	POINT p;
	HWND wh = GetConsoleWindow();
	cleaning(rangeXmin, rangeYmin - 1, 16, 4, DARKGRAY);
	int posX, posY,buttonActive=0;
	while (1) {
		Sleep(70);
		GetWindowRect(wh, &r);
		GetCursorPos(&p);
		posX = ((p.x - r.left) / 8);
		posY = ((p.y - r.top) / 16);
		if (posX <= rangeXmax && posX >= rangeXmin && posY <= rangeYmax && posY >= rangeYmin) {
			if(GetKeyState(button) < 0) break;
			Sleep(50);
			cleaning(rangeXmin, rangeYmin - 1, 16, 4, WHITE);
			buttonActive = 1;
		}
		else if (buttonActive == 1) {
			cleaning(rangeXmin, rangeYmin - 1, 16, 4, DARKGRAY);
			buttonActive = 0;
		}
	}
}

std::string printFilter(int length, int x, int y, std::string mode, std::string min, std::string max, int minDig, int maxDig)
{
	if (mode.length() > 3 && min < "0" || mode.length() >3 && max > "9") {
		min = "0"; max = "9";
	}
	std::wstring wmin = utf.stows(min), wmax = utf.stows(max);
	int factive = 0;
	std::wstring buff;
	for (int i = 0; i <= length + 1; i++) {
	m: std::wstring key;
		key += _getwch();
		if (i == length + 1) {
			i--;
			goto m;
		}
		if (GetAsyncKeyState(VK_DOWN) != 0 || GetAsyncKeyState(VK_UP) != 0 ||
			GetAsyncKeyState(VK_LEFT) != 0 || GetAsyncKeyState(VK_RIGHT) != 0) goto m;
		if (GetAsyncKeyState(VK_ESCAPE) != 0) return "0";
		if (GetAsyncKeyState(VK_BACK) != 0) {
			if (i != 0) {
				if (i == factive)
					factive = 0;
				i--;
				buff.erase(buff.length() - 1, 1);
				GotoXY(x + i, y, utf.wstos(key));
				GotoXY(x + i, y, " ");
				GotoXY(x + i, y);
			}
			goto m;
		}
		if (GetAsyncKeyState(VK_RETURN) != 0 && i != 0) {
			if (mode.length() > 3 && stof(buff) < minDig || mode.length() > 3 && stof(buff) > maxDig)
				goto m;
			return utf.wstos(buff);
		}
		if (mode == "str" && key < wmin || key > wmax || mode.length() > 4 && key < wmin || key > wmax)
			goto m;
		if (mode == "calc") {
			if (key < wmin && key != L"-" && key != L"." || key > wmax && key != L"-" && key != L".")
				goto m;
			if (key == L"-" && i != 0) goto m;
			if (key == L".") {
				if (factive != 0 || i == 0) goto m;
				factive = i;
			}
		}
		if (i != length) {
			buff += key;
			GotoXY(x + i, y, utf.wstos(key));
		}
	}
}


// STRTOOLS CLASS 

std::string strtools::toupper(std::string str,std::string flag) 
{
	std::wstring wstr = stows(str);
	if (flag == "") {
		for (int i = 0; i < wstr.length(); i++) {
			if (wstr[i] >= 'a' && wstr[i] <= 'z') {
				wstr[i] = wstr[i] - 32;
			}
			else {
				for (int j = 0; j < RUdown.length(); j++) {
					if (wstr[i] == RUdown[j])
						wstr[i] = RUup[j];
					else {
						for (int u = 0; u < UAdown.length(); u++) {
							if (wstr[i] == UAdown[u])
								wstr[i] = UAup[u];
						}
					}

				}
			}
		}
	}
	if (flag == "el_GR") {
		for (int i = 0; i < wstr.length(); i++) {
			for (int j = 0; j < GRdown.length(); j++) {
				if (wstr[i] == GRdown[j])
					wstr[i] = GRup[j];
			}
		}
	}
	return str = wstos(wstr);
}

std::string strtools::tolower(std::string str, std::string flag) 
{
	std::wstring wstr = stows(str);
	if (flag == "") {
		for (int i = 0; i < wstr.length(); i++) {
			if (wstr[i] >= 'A' && wstr[i] <= 'Z') {
				wstr[i] = wstr[i] + 32;
			}
			else {
				for (int j = 0; j < RUup.length(); j++) {
					if (wstr[i] == RUup[j])
						wstr[i] = RUdown[j];
					else {
						for (int u = 0; u < UAup.length(); u++) {
							if (wstr[i] == UAup[u])
								wstr[i] = UAdown[u];
						}
					}

				}
			}
		}
	}
	if (flag == "el_GR") {
		for (int i = 0; i < wstr.length(); i++) {
			for (int j = 0; j < GRup.length(); j++) {
				if (wstr[i] == GRup[j])
					wstr[i] = GRdown[j];
			}
		}
	}
	return str = wstos(wstr);
}

std::string strtools::strexplus(std::string str1, std::string str2) 
{
	std::wstring wstr1 = utf.stows(str1), wstr2 = utf.stows(str2);
	for (int i = wstr1.length() - 1; 0 <= i; i--)
		for (int j = 0; j < wstr2.length(); j++)
			if (wstr1[i] == wstr2[j])
				wstr1.erase(i, 1);
	return utf.wstos(wstr1);
}

int strtools::strfind(std::string str1, std::string str2) 
{
	int length1 = str1.length(), length2 = str2.length();
	std::string strbuff;
	for (int i = 0; i < length1; i++) {
		for (int j = 0; j < length2; j++) {
			if (length2 <= length1) {
				if (str1[i] == str2[j] && str1[i + 1] == str2[j + 1] || length2 == 1)
					strbuff += str2[j];
				else if (i == length1 - 1 && j == length2 - 1)
					strbuff += str2[j];
			}
		}
		if (strbuff == str2) return 1;
	}
	return 0;
}

// END STRTOOLS CLASS 


// MEMORY TOOLS

namespace memory 
{
	BOOL _d_memoryUtil(_SYSTEM_INFORMATION_CLASS sysInfo, _SYSTEM_MEMORY_LIST_COMMAND command)
	{
		HMODULE ntdll = LoadLibrary(TEXT("ntdll.dll"));
		HANDLE processToken{};
		SYSTEM_FILECACHE_INFORMATION info{};
		MEMORY_COMBINE_INFORMATION_EX combine_info{};
		NTSTATUS(WINAPI * NtSetSystemInformation)(INT, PVOID, ULONG) = (NTSTATUS(WINAPI*)(INT, PVOID, ULONG))GetProcAddress(ntdll, "NtSetSystemInformation");
		NTSTATUS status{};
		OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &processToken);
		if (sysInfo == SystemFileCacheInformation) {
			if (checkPrivilege(processToken, TEXT("SeIncreaseQuotaPrivilege"), TRUE)) {
				RtlZeroMemory(&info, sizeof(info));
				info.MinimumWorkingSet = -1;
				info.MaximumWorkingSet = -1;
				status = NtSetSystemInformation(sysInfo, &info, sizeof(info));
				if (status >= 0) return 1;
				else return 0;
			}
			else return 0;
		}
		if (sysInfo == SystemMemoryListInformation) {
			if (checkPrivilege(processToken, TEXT("SeProfileSingleProcessPrivilege"), TRUE)) {
				SYSTEM_MEMORY_LIST_COMMAND command = command;
				status = NtSetSystemInformation(sysInfo, &command, sizeof(command));
				if (status >= 0) return 1;
				else return 0;
			}
		}
	}


	BOOL _d_sysCacheClean() // need increase quota
	{
		if (_d_memoryUtil(SystemFileCacheInformation, nul) == 1)
			return 1;
		return 0;
	}

	BOOL _d_standbyListClean()
	{
		if (_d_memoryUtil(SystemMemoryListInformation, PurgeStandbyList) == 1)
			return 1;
		return 0;
	}

	BOOL _d_standbyListNoPriority()
	{
		if (_d_memoryUtil(SystemMemoryListInformation, PurgeLowPriorityStandbyList) == 1)
			return 1;
		return 0;
	}

	BOOL _d_quickClean()
	{
		SIZE_T empty = 0;
		SIZE_T ret = 0;
		for (DWORD pid = 0x0; pid < 0x4E1C; pid = pid + sizeof(DWORD)) {
			HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
			if (NULL == hProcess)
				hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_QUERY_LIMITED_INFORMATION | PROCESS_SET_QUOTA | PROCESS_VM_READ, FALSE, pid);
			if (NULL != hProcess) {
				PROCESS_MEMORY_COUNTERS pmc = { 0 };
				pmc.cb = sizeof(PROCESS_MEMORY_COUNTERS) * sizeof(BYTE);
				GetProcessMemoryInfo(hProcess, &pmc, sizeof(pmc));
				empty = pmc.WorkingSetSize;
				if (EmptyWorkingSet(hProcess) == TRUE) {
					ZeroMemory(&pmc, sizeof(PROCESS_MEMORY_COUNTERS) * sizeof(BYTE));
					GetProcessMemoryInfo(hProcess, &pmc, sizeof(pmc));
					empty = empty - pmc.WorkingSetSize;
					ret = ret + empty;
				}
			}
		}
		if (ret == 0) return 0;
		return 1;
	}
}

// END MEMORY TOOLS