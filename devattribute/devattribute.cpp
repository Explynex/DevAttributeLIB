#include "devattribute.h"
#include <iostream>


static HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
static HANDLE hStdIn = GetStdHandle(STD_INPUT_HANDLE);
static CONSOLE_SCREEN_BUFFER_INFO scrBufferInfo;

void pause()
{
	system("pause > nul");
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
	if (print != "") {
		std::cin.clear();
		std::cout << print;
	}
	if (foreground != WHITE || background != BLACK) setColor(WHITE, BLACK);
}

void sheetGenerator(int x, int y, const int sizeX, const int sizeY, int lengthX, int lengthY,int boardStyle,consoleColor text,consoleColor background) 
{
	std::string style[7][11] = {
		{ "║","═","╬","╠","╣","╦","╩","╚","╗","╝","╔" },
		{"│","─","┼","├","┤","┬","┴","╰","╮","╯","╭"},
		{ "│","─","┼","├","┤","┬","┴","└","┐","┘","┌" },
		{ "┃","━","╋","┣","┫","┳","┻","┗","┓","┛","┏" },
		{ "┋","╍","╋","┣","┫","┳","┻","┗","┓","┛","┏" },
		{ "█","▄","█","█","█","▄","█","█","▄","█","▄" },
		{ "█","█","█","█","█","█","█","█","█","█","█" }
	};
	lengthY += 1;
	int buffsizeY = sizeY * lengthY + 1, buffX = x;
	setColor(text, background);
	for (int j = 0; j < sizeX; j++) {
		for (int i = 1; i <= buffsizeY; i++) {
			GotoXY(buffX, y + i - 1, style[boardStyle][0]);
			for (int k = 0; k < lengthX; k++)
				std::cout << " ";
			std::cout << style[boardStyle][0];
			if (i % lengthY == 1) {
				GotoXY(buffX, y + i - 1);
				for (int t = 0; t < lengthX + 1; t++)
					std::cout << style[boardStyle][1];
				for (int p = 0; p < lengthX + 1; p++)
					GotoXY(buffX, y + i - 1, style[boardStyle][2]);
				if (i != 1 && i != buffsizeY && j == 0)
					GotoXY(buffX, y + i - 1, style[boardStyle][3]);
				if (i != 1 && i != buffsizeY && j == sizeX - 1)
					GotoXY(buffX + lengthX + 1, y + i - 1, style[boardStyle][4]);
			}
			if (i == 1) {
				x = buffX + lengthX + 1;
				GotoXY(buffX, y + i - 1, style[boardStyle][5]);
			}
			if (i == buffsizeY && x == buffX + lengthX + 1)
				GotoXY(buffX, y + i - 1, style[boardStyle][6]);
		}
		buffX += lengthX + 1;
	}
	GotoXY(x - sizeX * (lengthX + 1), y + sizeY * lengthY, style[boardStyle][7]);
	GotoXY(x, y, style[boardStyle][8]);
	GotoXY(x, y + sizeY * lengthY, style[boardStyle][9]);
	GotoXY(x - sizeX * (lengthX + 1), y, style[boardStyle][10]);
	setColor(WHITE, BLACK);
}

void cleaning(int x, int y, int width, int height, consoleColor background) 
{
	int counter = 0, buff = y;
	if (background != BLACK) setColor(BLACK, background);
	for (int i = 0; i < height; i++) {
		GotoXY(x, buff++);
		for (int j = 0; j < width; j++)
			std::cout << " ";
	}
	setColor(WHITE, BLACK);
	GotoXY(x, y);
}

void setConsoleSize(int columnsX, int linesY)
{
	std::string command = "mode con cols=" + std::to_string(columnsX) + " lines=" + std::to_string(linesY);
	system(command.c_str());
}

void getConsoleSize(COORD &position) 
{
	GetConsoleScreenBufferInfo(hStdOut, &scrBufferInfo);
	short winWidth = scrBufferInfo.srWindow.Right - scrBufferInfo.srWindow.Left + 1;
	short winHeight = scrBufferInfo.srWindow.Bottom - scrBufferInfo.srWindow.Top + 1;
	position.X = winWidth;
	position.Y = winHeight;
}

void setConsoleFullscreen() 
{
	COORD newSize;
	short bufferHeight = scrBufferInfo.dwSize.Y;
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

COORD setConsoleButton(
	int x, int y, int width, int height, int countButtonsX, int countButtonsY,
	int returnButton, consoleColor inactiveButton, consoleColor activeButton,
	int YCorrect, int XCorrect, bool boarder, int boarder_style,
	consoleColor boarderColor, consoleColor backBoarderColor, std::string titles[], consoleColor titlesColor
)
{
	RECT r{};
	POINT p{};
	COORD** buttonCord = new COORD * [countButtonsY], numButton{};
	HWND wh = GetConsoleWindow();
	showConsoleCursor(FALSE);
	for (int i = 0; i < countButtonsY; i++) buttonCord[i] = new COORD[countButtonsX];
	if (boarder) sheetGenerator(x - 1, y - 1, countButtonsX, countButtonsY, width, height, boarder_style, boarderColor, backBoarderColor);
	for (int i = 0, tempY = y; i < countButtonsY; i++) {
		for (int j = 0, tempX = x; j < countButtonsX; j++) {
			if (inactiveButton != -1 || activeButton != -1)
				cleaning(tempX, tempY, width, height, inactiveButton);
			buttonCord[i][j].X = tempX;
			buttonCord[i][j].Y = tempY;
			if (titles != 0 && countButtonsX == 1) {
				GotoXY(tempX, tempY + (height / 2), titles[i], titlesColor, inactiveButton);
			}
			else if (titles != 0 && countButtonsY == 1) {
				GotoXY(tempX, tempY + (height / 2), titles[j], titlesColor, inactiveButton);
			}

			if (!boarder) tempX += width + 2;
			else tempX += width + 1;
		}

		tempY += height + 1;
	}
	int posX = 0, posY = 0, buttonActive = 0, i = 0, j = 0, tempCordX = 0, tempCordY = 0,
		mCord = 0, buttonPointX = 0, buttonPointY = 0; // переменные для проверки находится или нет курсор на конкретной кнопке
	while (true) {
		GetWindowRect(wh, &r);
		GetCursorPos(&p);
		posX = ((p.x - r.left) / (8 - XCorrect));
		posY = ((p.y - r.top) / (16 - YCorrect));
		for (i = 0; i < countButtonsY; i++) {
			for (j = 0; j < countButtonsX; j++) {
				if (countButtonsX <= countButtonsY) mCord = i;
				else mCord = j;
				if (posX >= buttonCord[i][j].X && posX <= buttonCord[i][j].X + width && posY >= buttonCord[i][j].Y && posY <= buttonCord[i][j].Y + height) {
					if (tempCordX != buttonCord[i][j].X || tempCordY != buttonCord[i][j].Y) {
						if (inactiveButton != NUL && activeButton != NUL) {
							cleaning(buttonCord[i][j].X, buttonCord[i][j].Y, width, height, activeButton);
							if (titles != 0)
								GotoXY(buttonCord[i][j].X, buttonCord[i][j].Y + (height / 2), titles[mCord], titlesColor, activeButton);
							if (tempCordX != 0 || tempCordY != 0) {
								cleaning(tempCordX, tempCordY, width, height, inactiveButton);
								if (titles != 0)
									GotoXY(tempCordX, tempCordY + (height / 2), titles[mCord + 1], titlesColor, inactiveButton);
							}
						}
						tempCordX = buttonCord[i][j].X;
						tempCordY = buttonCord[i][j].Y;

					}
					buttonPointY = i; buttonPointX = j;
					if (GetKeyState(returnButton) < 0) {
						for (int i = 0; i < countButtonsY; i++) // очистка памяти под массив координат кнопок
							delete[] buttonCord[i];
						delete[] buttonCord;
						if (countButtonsY != 1 || countButtonsX != 1) {
							numButton.X = j + 1;
							numButton.Y = i + 1;
						}
						else {
							numButton.X = 1;
							numButton.Y = 1;
						}
						return numButton;
					}
				}
				else if (inactiveButton != NUL && i == buttonPointY && buttonPointX == j && (tempCordX != 0 || tempCordY != 0)) {
					cleaning(tempCordX, tempCordY, width, height, inactiveButton);
					if (titles != 0)
						GotoXY(tempCordX, tempCordY + (height / 2), titles[mCord], titlesColor, inactiveButton);
					tempCordX = 0;
					tempCordY = 0;
				}
			}

		}
		if (GetKeyState(VK_ESCAPE) < 0) {
			for (int i = 0; i < countButtonsY; i++)
				delete[] buttonCord[i];
			delete[] buttonCord;
			numButton.X = 0;
			numButton.Y = 0;
			return numButton;
		}
		Sleep(60);
	}
}


void setConsoleTrayIcon(const wchar_t* trayInfo, LPCWSTR pathIcon, WNDPROC msgCallback, UINT flags)
{
	HINSTANCE hinstance{};
	NOTIFYICONDATA icon{};
	WNDCLASSEX window{};
	window.cbSize = sizeof(WNDCLASSEX);
	window.hInstance = hinstance;
	window.lpszClassName = TEXT("OnlyMessageWindow");
	window.lpfnWndProc = msgCallback;
	RegisterClassEx(&window);
	HWND windowHwnd = CreateWindowEx(0, TEXT("OnlyMessageWindow"), NULL, 0, 0, 0, 0, 0, HWND_MESSAGE, NULL, NULL, NULL);

	icon.cbSize = sizeof(NOTIFYICONDATA);
	icon.hWnd = windowHwnd;
	icon.uCallbackMessage = WM_USER;
#if defined(_UNICODE)
	icon.hIcon = (HICON)LoadImage(NULL, pathIcon, IMAGE_ICON, 0, 0, LR_LOADFROMFILE | LR_SHARED);
	wcscpy(icon.szTip, trayInfo);
#else defined (_MBCS)
	char* str{};
	wcstombs(str, trayInfo, wcslen(trayInfo));
	icon.hIcon = (HICON)LoadImageW(NULL, pathIcon, IMAGE_ICON, 0, 0, LR_LOADFROMFILE | LR_SHARED);
	strcpy(icon.szTip, str);
#endif
	icon.uFlags = flags;
	Shell_NotifyIcon(NIM_ADD, &icon);

	MSG message;
	while (GetMessage(&message, NULL, 0, 0)) {
		TranslateMessage(&message);
		DispatchMessage(&message);
	}
	Shell_NotifyIcon(NIM_DELETE, &icon);
}

int fact(int val)
{
	if (val ? 0 : 1) return 1;
	return val *= fact(val - 1);
}


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
				SYSTEM_MEMORY_LIST_COMMAND command{};
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