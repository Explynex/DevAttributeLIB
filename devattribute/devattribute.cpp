#include "devattribute.h"
HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);

void setColor(consoleColor foreground, consoleColor background) {
	SetConsoleTextAttribute(hStdOut, (WORD)((background << 4) | foreground));
}

void GotoXY(short X, short Y, std::string print, consoleColor foreground, consoleColor background) {
	COORD cord = { X, Y };
	SetConsoleCursorPosition(hStdOut, cord);
	if (foreground != WHITE || background != BLACK) setColor(foreground, background);
	if (print != "") std::cout << print;
	if (foreground != WHITE || background != BLACK) setColor(WHITE, BLACK);
}

void sheetGenerator(int x, int y, const int sizeX, const int sizeY, int lengthX, int lengthY){
	lengthY += 1;
	int buffsizeY = sizeY * lengthY + 1, buffX = x;
	for (int j = 0; j < sizeX; j++) {
		for (int i = 1; i <= buffsizeY; i++) {
			GotoXY(buffX, y + i - 1, "║");
			for (int k = 0; k < lengthX; k++)
				std::cout << " ";
			std::cout << "║";
			if (i % lengthY == 1) {
				GotoXY(buffX, y + i - 1, "");
				for (int t = 0; t < lengthX + 1; t++)
					std::cout << "═";
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

void cleaning(int x, int y, int width, int height) {
	int counter = 0, buff = y;
	for (int i = 0; i < width; i++) {
		GotoXY(x, buff++);
		for (int j = 0; j < height; j++)
			std::cout << " ";
	}
}

void pause() {
	setColor(BLACK, BLACK);
	GotoXY(50, 0, "");
	system("pause");
	setColor(WHITE, BLACK);
}

std::string printFilter(int x, int y, int length, std::string mode, std::string min,
	std::string max, int minDig, int maxDig)
{
	if (strlen(mode.c_str()) > 3 && "calc" && min < "0" || strlen(mode.c_str())>3 && max > "9") {
		min = "0"; max = "9";
	}
	int factive = 0;
	std::string buff;
	for (int j = 0; j <= length + 1; j++) {
	m: std::string key;
		if (j == length + 1) {
			buff.erase(strlen(buff.c_str()), 1);
			j--;
			goto m;
		}
		for (int i = 0; i < 2; i++) {
			key += _getch();
			if (GetAsyncKeyState(VK_ESCAPE) != 0) {
				if (mode == "str") return "0";
				if (mode == "digit" || mode == "calc") return 0;
			}
			if (GetAsyncKeyState(VK_BACK) != 0) {
				if (j != 0) {
					if (j == factive)
						factive = 0;
					j--;
					if (buff[j] >= *" " && buff[j] <= *"~" && GetAsyncKeyState(VK_BACK) != 0)
						buff.erase(strlen(buff.c_str())-1, 1);
					else buff.erase(strlen(buff.c_str())-2, -2);
					GotoXY(x + j, y, key);
					GotoXY(x + j, y, " ");
					GotoXY(x + j, y);
				}
				goto m;
			}
			if (GetAsyncKeyState(VK_RETURN) != 0 && j!=0) {
				if (strlen(mode.c_str()) > 3 && stof(buff) < minDig || strlen(mode.c_str()) > 3 && stof(buff) > maxDig)
					goto m;
				return buff;
			}
			if (key >= " " && key <= "~") break;
		}
		if (mode == "str" && key < min || key > max || strlen(mode.c_str()) > 4 && key < min || key > max)
			goto m;
		if (mode == "calc") {
			if (key < min && key != "-" && key != "." || key > max && key != "-" && key != ".")
				goto m;
			if (key == "-" && j != 0) goto m;
			if (key == ".") {
				if (factive != 0 || j == 0) goto m;
				factive = j;
			}
		}
		if (j != length) {
			buff += key;
			GotoXY(x + j, y, key);
		}
	}
}
