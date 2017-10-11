#include <iostream>
#include <string>
#include <ctime>
#include "GameBoard.h"
#include <chrono>

int main()
{
	GameBoard gameBoard;

	while (true)
	{
		printf("enter Boggle board size: ");
		
		int boardSize;
		std::cin >> boardSize;
		printf("\n");
		
		gameBoard.GenerateDice(boardSize);
		gameBoard.ShowDice();

		std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();

		gameBoard.Solve();

		std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();

		auto solvedIn = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();

		printf("solved in: %d microseconds\n\n", solvedIn);
		printf("I`m not sure about some words correctness. I found them on some word generator sites and put them into wordList.txt file.So whether it is a correct english word or not, it is correct for current Boggle solver.\n\n", solvedIn);

		getchar();
	}

	
	return 0;
}