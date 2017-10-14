#include "GameBoard.h"
#include <iostream>
#include <ctime>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>
#include <thread>
#include <deque>

GameBoard::GameBoard()
{
	srand(time(NULL));
	ReadWordListFile(wordListFilePath);
	PrewarmWordPartsSet();
}

GameBoard::~GameBoard()
{
}

void GameBoard::ShowDice()
{
	for (int y = 0; y < boardSize; y++)
	{
		std::cout << "|";

		for (int x = 0; x < boardSize; x++)
		{
			std::cout << dice[y][x] << "|";
		}
		std::cout << std::endl;
	}
}

void GameBoard::Traverse(const int &x, const int &y, std::vector< std::vector<bool> > curWordNodes, std::string possibleWord, std::vector< std::string > &results)
{
	if (x < 0 || x >= boardSize || y < 0 || y >= boardSize || curWordNodes[x][y])
	{
		return;
	}

	curWordNodes[x][y] = true;

	possibleWord += dice[x][y];

	if (possibleWord.length() > longestWordLength || wordParts.find(possibleWord) == wordParts.end())
	{
		return;
	}

	Traverse(x, y + 1, curWordNodes, possibleWord, results);
	Traverse(x - 1, y + 1, curWordNodes, possibleWord, results);
	Traverse(x - 1, y, curWordNodes, possibleWord, results);
	Traverse(x - 1, y - 1, curWordNodes, possibleWord, results);
	Traverse(x, y - 1, curWordNodes, possibleWord, results);
	Traverse(x + 1, y - 1, curWordNodes, possibleWord, results);
	Traverse(x + 1, y, curWordNodes, possibleWord, results);
	Traverse(x + 1, y + 1, curWordNodes, possibleWord, results);

	m.lock();
	auto it = std::find(results.begin(), results.end(), possibleWord);
	
	if (it == results.end() && possibleWord.length() >= 3 && words.find(possibleWord) != words.end())
	{
		results.push_back(possibleWord);
	}
	m.unlock();
}

void GameBoard::Exec(std::deque< std::function<void()> > &jobs)
{
	while (!jobs.empty())
	{
		jobs.front()();
		jobs.pop_front();
	}
}

void GameBoard::Solve()
{
	std::vector<std::thread> workers;
	int processorCount = std::thread::hardware_concurrency();
	std::vector< std::string > results;
	std::vector<std::deque<std::function<void()>>> threadJobPools(processorCount);
	

	for (int y = 0; y < boardSize; y++)
	{
		for (int x = 0; x < boardSize; x++)
		{
			std::string possibleWord;
			std::vector< std::vector<bool> > curWordNodes(boardSize, std::vector<bool>(boardSize, false));
			threadJobPools[x % processorCount].push_front(std::bind(&GameBoard::Traverse, this, x, y, curWordNodes, possibleWord, std::ref(results)));
		}
	}

	for (auto &pool : threadJobPools)
	{
		if (!pool.empty())
		{
			workers.push_back(std::thread(&GameBoard::Exec, this, std::ref(pool)));
		}
	}

	for (auto &w : workers)
	{
		w.join();
	}

	CompareStrByLength comparator;
	std::sort(results.begin(), results.end(), comparator);
	PrintResultsAndScores(results);
}

static inline std::string GetIndent(const std::string &s, const int &longestWordLength)
{
	auto numSpaces = longestWordLength + 5 - s.length();
	std::string indent;

	for (int i = 0; i < numSpaces; i++)
	{
		indent += ' ';
	}

	return indent;
}

void GameBoard::PrintResultsAndScores(std::vector< std::string > &results)
{
	int overallScore = 0;
	int score = 0;

	std::cout << "\n\n************************************\nResults:" << std::endl;

	for each (auto result in results)
	{
		score = GetWordScore(result);
		overallScore += score;
		std::cout << result << GetIndent(result, longestWordLength) << score << std::endl;
	}

	std::cout << "____________________________________" << std::endl;
	std::cout << "Overallscore: " << overallScore << std::endl;
	std::cout << "************************************" << std::endl;
}

int GameBoard::GetWordScore(std::string &word)
{
	int wordLength = word.length();

	if (wordLength == 3 || wordLength == 4)
	{
		return 1;
	}

	if (wordLength == 5)
	{
		return 2;
	}

	if (wordLength == 6)
	{
		return 3;
	}

	if (wordLength == 7)
	{
		return 5;
	}

	if (wordLength > 8)
	{
		return 11;
	}
}

void GameBoard::ReadWordListFile(const std::string &wordListFilePath)
{
 	std::ifstream file(wordListFilePath);
	std::string line;

	while (std::getline(file, line))
	{	
		words.insert(line);
	}
}

int GameBoard::CountGivenLetterIn2dVector(const char &letter, const std::vector< std::vector<char> > &vec)
{
	int quantity = 0;

	for each (auto row in vec)
	{
		quantity += std::count(row.begin(), row.end(), letter);
	}

	return quantity;
}

void GameBoard::GenerateDice(const int &boardSize)
{
	dice.clear();
	this->boardSize = boardSize;
	std::vector<char> tempVector;
	int lettersCount = 0;
	int goalLettersQuantity = boardSize * boardSize;
	int passed = 0;

	while (lettersCount < goalLettersQuantity)
	{
		auto iterator = words.begin();
		std::advance(iterator, GetRandomIntWithinRange(0, words.size()));
		auto word = *iterator;

		for (int i = 0; i < word.length(); i++)
		{
			if (lettersCount >= goalLettersQuantity)
			{
				break;
			}

			auto curQuantityCurLetter = CountGivenLetterIn2dVector(word[i], dice);
			auto goalQuantityCurLetter = std::count(word.begin(), word.end(), word[i]); 

			if (curQuantityCurLetter == 0 || passed < 20 ||
				(curQuantityCurLetter > 0 && curQuantityCurLetter < goalQuantityCurLetter)) // since 20 letters didnt added then add all letters in given words
			{
				tempVector.push_back(word[i]);
				lettersCount++;
			}
			else
			{
				passed++;
			}

			if (tempVector.size() == boardSize)
			{
				dice.push_back(tempVector);
				tempVector.clear();
			}
		}
	}
		
	ShuffleDice();
}

void SwapChars(char &ch1, char &ch2)
{
	auto temp = ch2;
	ch2 = ch1;
	ch1 = temp;
}

void GameBoard::ShuffleDice()
{
	for (int i = 0; i < boardSize * boardSize; i++)
	{
		auto ch1 = &dice[rand() % dice.size()][rand() % dice[0].size()];
		auto ch2 = &dice[rand() % dice.size()][rand() % dice[0].size()];
		SwapChars(*ch1, *ch2);
	}
}

int GameBoard::GetRandomIntWithinRange(const int &min, const int &max)
{
	return rand() % max + min;
}

void GameBoard::PrewarmWordPartsSet()
{
	for each (auto word in words)
	{
		std::string wordPart;
		auto len = word.length();

		if (len > longestWordLength)
		{
			longestWordLength = len;
		}

		for each (auto letter in word)
		{
			wordPart += letter;

			if (wordParts.find(wordPart) == wordParts.end())
			{
				wordParts.insert(wordPart);
			}
		}
	}
}