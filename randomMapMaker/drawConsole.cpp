#include <Windows.h>
#include <string>
#include "drawConsole.h"
#include "MapMaker.h"



void DrawStr(int x, int y, char* str)
{
	COORD coord = {(short)x * 2, (short)y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
	printf("%s", str);

	HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO info;
	info.dwSize = 10;
	info.bVisible = 0;
	SetConsoleCursorInfo(consoleHandle, &info);
}

void DrawStr(int x, int y, int _int)
{
	COORD coord = { (short)x * 2, (short)y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
	printf("%3d", _int);

	HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO info;
	info.dwSize = 10;
	info.bVisible = 0;
	SetConsoleCursorInfo(consoleHandle, &info);
}

void MoveCurser(int x, int y) {
	COORD coord = { (short)x * 2, (short)y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);

	HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO info;
	info.dwSize = 10;
	info.bVisible = 0;
	SetConsoleCursorInfo(consoleHandle, &info);

};

