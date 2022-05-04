#include "devattribute.h"
#include <iostream>
#include <thread>





/*posY -= y;
posX -= x;

if (posY % (height + 1) == 0) GotoXY(10, 1, "BORDER");*/


int main() {

	Sleep(100);
	utf.setUtfLocale();
	std::string titles[3] = { "Главное меню","Поебота","Выйти в санузел" };
	COORD key = setConsoleButton(40, 10, 20, 1, 1, 3, VK_RBUTTON, BROWN, WHITE, 1, false, 0, NUL, NUL, titles, 3, BLACK);
	if (key.X == 0 && key.Y == 0) {
		std::cout << "Выйшло пойти нахуй";
	}
	
	std::string  ass = "Выйти в главное меню";
	GotoXY(1, 2, ass, YELLOW);
	winPause();
}

