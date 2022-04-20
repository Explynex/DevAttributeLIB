#include "devattribute.h"
#include <iostream>
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

void sheetGenerator(int x, int y, const int sizeX, const int sizeY, int lengthX, int lengthY) {
	utf.setUtfLocale();
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
		if (GetAsyncKeyState(VK_ESCAPE) != 0) {
			if (mode == "str") return "0";
		}
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

std::string strtools::toupper(std::string str,std::string flag) {
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

std::string strtools::tolower(std::string str, std::string flag) {
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

std::string strtools::strexplus(std::string str1, std::string str2) {
	std::wstring wstr1 = utf.stows(str1), wstr2 = utf.stows(str2);
	for (int i = wstr1.length() - 1; 0 <= i; i--)
		for (int j = 0; j < wstr2.length(); j++)
			if (wstr1[i] == wstr2[j])
				wstr1.erase(i, 1);
	return utf.wstos(wstr1);
}

