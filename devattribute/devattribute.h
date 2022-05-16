#pragma once
#ifndef _DEVATTRIBUTE_
#define _DEVATTRIBUTE_

#include <yvals_core.h>
#pragma execution_character_set( "utf-8" )

#if _STL_COMPILER_PREPROCESSOR

#if _HAS_CXX17 && defined(_FILESYSTEM_USE_)
#include <filesystem>
#endif // _HAS_CXX17 && _FILE_SYSTEM_USE_

#if !defined(_FILESYSTEM_)
#include <vector>
#endif //!_FILESYSTEM_



/// Include
///
#include <Windows.h>
#include <Psapi.h>
#include <fstream>
#include <string>



/// TYPES
///
typedef struct _SYSTEM_FILECACHE_INFORMATION
{
    SIZE_T CurrentSize;
    SIZE_T PeakSize;
    ULONG PageFaultCount;
    SIZE_T MinimumWorkingSet;
    SIZE_T MaximumWorkingSet;
    SIZE_T CurrentSizeIncludingTransitionInPages;
    SIZE_T PeakSizeIncludingTransitionInPages;
    ULONG TransitionRePurposeCount;
    ULONG Flags;
} SYSTEM_FILECACHE_INFORMATION;

typedef struct _MEMORY_COMBINE_INFORMATION_EX
{
    HANDLE Handle;
    ULONG_PTR PagesCombined;
    ULONG Flags;
} MEMORY_COMBINE_INFORMATION_EX, * PMEMORY_COMBINE_INFORMATION_EX;



/// ENUMS
///
typedef enum _SYSTEM_INFORMATION_CLASS
{
    SystemFileCacheInformation = 21,
    SystemMemoryListInformation = 80,
    SystemCombinePhysicalMemoryInformation = 130
};

typedef enum _SYSTEM_MEMORY_LIST_COMMAND
{
    nul = -1,
    CaptureAccessedBits,
    CaptureAndResetAccessedBits,
    EmptyWorkingSets,
    FlushModifiedList,
    PurgeStandbyList,
    PurgeLowPriorityStandbyList,
    CommandMax
} SYSTEM_MEMORY_LIST_COMMAND;

typedef enum  consoleColor {
    NUL = -1,BLACK, BLUE, GREEN, CYAN, RED, MAGENTA, BROWN, LIGHTGRAY, DARKGRAY, LIGHTBLUE, LIGHTGREEN, LIGHTCYAN, LIGHTRED, LIGHTMAGENTA, YELLOW, WHITE
};



/// FUNCTIONS PROTOTYPES
///
/// 
void pause();

void setConsoleFullscreen();

COORD getConsoleCursorPosition();

void setColor(
    consoleColor foreground,
    consoleColor background
);

void GotoXY(
    short X, 
    short Y, 
    std::string print = "", 
    consoleColor foreground = WHITE, 
    consoleColor background = BLACK
);

const char* WinFileDialog(
    HWND hwnd, 
    const char* mode, 
    const char* lpstrFilters
);


void sheetGenerator(
    int x,
    int y,
    const int sizeX,
    const int sizeY,
    int lengthX,
    int lengthY,
    int boardStyle=0,
    consoleColor text = WHITE,
    consoleColor background = BLACK
);

void cleaning(
    int x, 
    int y, 
    int width, 
    int height, 
    consoleColor background = BLACK
);

void setConsoleSize(
    int columnsX,
    int linesY
);

void getConsoleSize(
    COORD &position
);

void setConsoleNoSelection(
    BOOL status
);

BOOL showConsoleCursor(
    BOOL bShow
);

void setConsoleNoSelection(
    BOOL status
);

BOOL regIsBoot(
    HKEY hkey,
    const char* subkey
);

BOOL addToReg(
    HKEY hkey,
    const char* valueName
);

BOOL delFromReg(
    HKEY hkey,
    const char* valueName
);

void consoleResize(
    BOOL status
);

int fact(
    int val
);

bool removeLineFromFile(
    std::string filename,
    int index
);

BOOL checkPrivilege(
    HANDLE hToken,
    LPCTSTR lpszPrivilege,
    BOOL bEnablePrivilege
);

LONG calc_percentof64(
    _In_ LONG64 length,
    _In_ LONG64 total_length
);

COORD setConsoleButton(
    int x,
    int y,
    int width,
    int height,
    int countButtonsX,
    int countButtonsY = 1,
    int returnButton = VK_RBUTTON,
    consoleColor inactiveButton = DARKGRAY,
    consoleColor activeButton = WHITE,
    int YCorrect = 0,
    int XCorrect = 0,
    bool boarder = false,
    int boarder_style = 0,
    consoleColor boarderColor = WHITE,
    consoleColor backBoarderColor = BLACK,
    std::string titles[] = 0,
    consoleColor titlesColor = WHITE
);

void setConsoleTrayIcon(
    const wchar_t* trayInfo,
    LPCWSTR pathIcon,
    WNDPROC msgCallback,
    UINT flags = NIF_MESSAGE | NIF_ICON | NIF_TIP
);

namespace memory {

    BOOL _d_memoryUtil(
        _SYSTEM_INFORMATION_CLASS sysInfo,
        _SYSTEM_MEMORY_LIST_COMMAND command
    );
    BOOL _d_sysCacheClean();
    BOOL _d_quickClean();
    BOOL _d_standbyListNoPriority();
    BOOL _d_standbyListClean();
    BOOL _d_sysCacheClean();

}



#if defined(_FILESYSTEM_)
    static std::string getPath(std::string mode = "path") {
        if (mode == "path")
            return std::filesystem::absolute(std::filesystem::current_path()).string() + "\\";
        if (mode == "username") {
            DWORD userNameBuff = 256 + 1;
#if defined(_UNICODE)
            wchar_t buff[256 + 1];
            GetUserName(buff, &userNameBuff);
            std::wstring ws(buff);
            std::string username(ws.begin(), ws.end());
#elif defined(_MBCS)
            char buff[256 + 1];
            GetUserName(buff, &userNameBuff);
            std::string username;
#endif
            return username;
        }
    }
static std::vector < std::string> fileNameList(bool only_names = 0, std::string path_to_dir = "auto", std::string extension = "") {
        if (path_to_dir == "auto") path_to_dir = getPath("path");
        if (std::filesystem::is_directory(path_to_dir)) {
            std::vector < std::string> file_names;
            int counter = 0,extLen = extension.length();
            for (const auto& entry : std::filesystem::directory_iterator(path_to_dir))
                if (entry.is_regular_file()) {
                    file_names.push_back(std::filesystem::absolute(entry.path()).string());
                    if (only_names == 1) {
                        file_names[counter].erase(0, path_to_dir.length());
                        if (extension != "") file_names[counter].erase(file_names[counter].length() - extLen, extLen);
                        counter++;
                    }
                }
            return file_names;
        }
    }

    void fileRename(std::string old_name, std::string new_name, std::string path_to_dir = "auto") {
        std::filesystem::path p;
        if (path_to_dir == "auto") p = std::filesystem::current_path();
        else  p = path_to_dir;
        std::filesystem::rename(p / old_name, p / new_name);
    }
#endif // defined(_FILESYSTEM_)



/// Templates
///
template <class type>
void _d_arrMemAlloc(type **&arrPtr,int rows,int cols) {
    arrPtr = new type *[rows];
    for (int i = 0; i < rows; i++){
        arrPtr[i] = new type[cols];
    }
}

template <class type>
void _d_arrMemFree(type**& arrPtr, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        delete[] arrPtr[i];
    }
    delete[] arrPtr;
    arrPtr = nullptr;
}



#endif // _STL_COMPILER_PREPROCESSOR
#endif // _DEVATTRIBUTE_
