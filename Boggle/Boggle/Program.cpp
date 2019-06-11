#include <iostream>
#include <string>
#include <ctime>
#include "GameBoard.h"
#include <chrono>
#include <windows.h>

static inline void PrepareConsole()
{
	CONSOLE_FONT_INFOEX cfi;
	cfi.cbSize = sizeof(cfi);
	cfi.nFont = 0;
	cfi.dwFontSize.X = 0;                   
	cfi.dwFontSize.Y = 14;                  
	cfi.FontFamily = FF_DONTCARE;
	cfi.FontWeight = FW_NORMAL;
	auto handle = GetStdHandle(STD_OUTPUT_HANDLE);
	SetCurrentConsoleFontEx(handle, FALSE, &cfi);
}

int main()
{
	PrepareConsole();
	GameBoard gameBoard;

	while (true)
	{
		printf("enter Boggle board size: ");
		
		int boardSize;
		std::cin >> boardSize;
		printf("\n");
		
		gameBoard.Generate(boardSize);
		gameBoard.Show();

		std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();

		gameBoard.Solve();

		std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();

		auto solvedIn = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();

		printf("solved in: %d milliseconds\n\n", solvedIn);
		printf("I`m not sure about some words correctness. I found them on some word generator sites and put them into wordList.txt file.So whether it is a correct english word or not, it is correct for current Boggle solver.\n\n", solvedIn);

		getchar();
	}

	
	return 0;
}