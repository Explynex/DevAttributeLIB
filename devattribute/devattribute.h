#pragma once
#ifndef _DEVATTRIBUTE_
#define _DEVATTRIBUTE_
//std::to_string() - integer to str conversion func

#include <yvals_core.h>

#if _STL_COMPILER_PREPROCESSOR

#if _HAS_CXX17 && defined(_FILESYSTEM_USE_)
#include <filesystem>
#endif // _HAS_CXX17 && _FILE_SYSTEM_USE_

#if !defined(_FILESYSTEM_)
#include <vector>
#endif //!_FILESYSTEM_

#include <Windows.h>
#include <iostream>
#include <conio.h>
#include <string>

enum consoleColor {
    BLACK, BLUE, GREEN, CYAN, RED, MAGENTA, BROWN, LIGHTGRAY, DARKGRAY, LIGHTBLUE, LIGHTGREEN, LIGHTCYAN, LIGHTRED, LIGHTMAGENTA, YELLOW, WHITE
};

void setColor(consoleColor foreground, consoleColor background);
void GotoXY(short X, short Y, std::string print = "", consoleColor foreground = WHITE, consoleColor background = BLACK);
void sheetGenerator(int x, int y, const int sizeX, const int sizeY, int lengthX, int lengthY);
void cleaning(int x, int y, int width, int height);
void pause();
std::string printFilter(
    int x, int y, int length, std::string mode = "str", std::string min = " ",
    std::string max = "ї", int minDig = INT_MIN, int maxDig = INT_MAX);

/* Information how to use printFilter function
 *
 * +-----------------------+-----+-----+
 * |      *Partition*      | MIN | MAX |
 * +-----------------------+-----+-----+
 * |          ALL          | " " | "ї" |
 * +-----------------------+-----+- ---+
 * |        UA + RU        |А(ru)| "ї" |
 * +-----------------------+-----+-----+   +--------+----------------------+
 * |        EN + RU        |A(en)| "я" |   |  MODE  |         INFO         |
 * +-----------------------+-----+-----+   +--------+----------------------+
 * |          RU           |A(ru)| "я" |   |   --   |     All symbols      |
 * +-----------------------+-----+-----+   +--------+----------------------+------------------------------+
 * |          EN           |A(en)| "z" |   | "calc" |  "0"-"9","." && "-"  | For conversion std::string   |
 * +-----------------------+-----+-----+   +--------+----------------------| stoi,f() - to int,float      |
 * |    DIG + OPERATIONS   | "*" | "9" |   | "digit"|         "0"-"9"      | .c_str() - to pointer char[] |
 * +-----------------------+-----+-----+   +--------+----------------------+------------------------------+
 */



#if defined(_FILESYSTEM_)
class path {
public:
    std::string getPath(std::string mode = "path") {
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

    std::vector < std::string> fileNameList(char only_names = 'n', std::string path_to_dir = "auto") {
        if (path_to_dir == "auto") path_to_dir = getPath("path");
        if (std::filesystem::is_directory(path_to_dir)) {
            std::vector < std::string> file_names;
            int counter = 0;
            for (const auto& entry : std::filesystem::directory_iterator(path_to_dir))
                if (entry.is_regular_file()) {
                    file_names.push_back(std::filesystem::absolute(entry.path()).string());
                    if (only_names == 'y') {
                        file_names[counter].erase(0, path_to_dir.length());
                        counter++;
                    }
                }
            return file_names;
        }
    }
};
#endif // defined(_FILESYSTEM_)

#if ( __cplusplus >= 201103L || _MSC_VER)
#define SWITCH_DECLTYPE decltype
#else
#define SWITCH_DECLTYPE typeof
#endif //( __cplusplus >= 201103L || _MSC_VER)

namespace switch_data {
    template<class T>
    struct SwitchData {
        bool bEnterFall;
        bool bEnterDefault;
        bool bDone;
        T strPtrThrSw;
        SwitchData(T arg) : strPtrThrSw(arg) {}
        inline bool transition(bool fall, const T& cnst, bool ndeflt) {
            if (bDone)
                return false;
            if (fall && bEnterFall)
                return 1;
            if (!fall && bEnterFall) {
                bDone = 1;
                return 0;
            }
            if (ndeflt) {
                bEnterFall = strPtrThrSw == cnst;
                if (bEnterFall) bEnterDefault = false;
            }
            return ndeflt ?
                bEnterFall :
                bEnterDefault;
        }
        void cpp97() {};
    };
} // namespace switch_data

#define SWITCH(arg) if(1){switch_data::SwitchData< SWITCH_DECLTYPE(arg) > \
    switch__d_a_t_a(arg); \
  switch__d_a_t_a.bEnterDefault=true;switch__d_a_t_a.bEnterFall=false; \
  switch__d_a_t_a.bDone=false;if(switch__d_a_t_a.transition(false,

#define CASE(cnst)  cnst,true)){

#define BREAK       switch__d_a_t_a.bDone=true; \
                    ;} if(switch__d_a_t_a.transition(false,

#define FALL        ;} if(switch__d_a_t_a.transition(true,

#define DEFAULT     switch__d_a_t_a.strPtrThrSw,false)){

#define END         ;}};
#endif // _STL_COMPILER_PREPROCESSOR
#endif // _DEVATTRIBUTE_
