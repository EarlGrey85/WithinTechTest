#include "GameBoard.h"
#include <unordered_map>
#include <functional>
#include <ctime>
#include <fstream>
#include <sstream>
#include <iostream>
#include <windows.h>

static int GetRandomIntWithinRange(const int &min, const int &max)
{
	return rand() % max + min;
}

static std::string GetIndent(const std::string &s, const int &longestWordLength)
{
	auto numSpaces = longestWordLength + 5 - s.length();
	std::stringstream ss;

	for (int i = 0; i < numSpaces; i++)
	{
		ss << ' ';
	}

	return ss.str();
}

GameBoard::GameBoard()
{
	srand(time(NULL));
	ReadWordListFile(wordListFilePath);
}

GameBoard::~GameBoard()
{
	CleanGrid();
}

void GameBoard::CleanGrid()
{
	for (int i = 0; i < boardSize; ++i)
	{
		delete[] grid[i];
	}

	delete[] grid;
}

void GameBoard::Show() const
{
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);

	if (boardSize > csbi.srWindow.Right / 2)
	{
		std::cout << "board is too wide to be shown" << std::endl;
		return;
	}

	for (int y = 0; y < boardSize; y++)
	{
		std::cout << "|";

		for (int x = 0; x < boardSize; x++)
		{
			std::cout << grid[y][x] << "|";
		}
		std::cout << std::endl;
	}
}

void GameBoard::Traverse(const int &x, const int &y, std::unordered_set<Coord> visited, PrefixTreeNode *possibleWordNode, std::unordered_set< std::string > &results)
{
	if (x < 0 || x >= boardSize || y < 0 || y >= boardSize)
	{
		return;
	}

	auto currentPos = Coord(x, y);

	if (visited.find(currentPos) != visited.end() || possibleWordNode->children == nullptr)
	{
		return;
	}

	possibleWordNode = possibleWordNode->children[grid[y][x] - 'a'];

	if (possibleWordNode == nullptr)
	{
		return;
	}

	traverseMutex.lock();

	if (!possibleWordNode->word.empty() && possibleWordNode->word.length() >= 3)
	{
		results.insert(possibleWordNode->word);
	}

	traverseMutex.unlock();

	visited.insert(currentPos);

	Traverse(x, y + 1, visited, possibleWordNode, results);
	Traverse(x - 1, y + 1, visited, possibleWordNode, results);
	Traverse(x - 1, y, visited, possibleWordNode, results);
	Traverse(x - 1, y - 1, visited, possibleWordNode, results);
	Traverse(x, y - 1, visited, possibleWordNode, results);
	Traverse(x + 1, y - 1, visited, possibleWordNode, results);
	Traverse(x + 1, y, visited, possibleWordNode, results);
	Traverse(x + 1, y + 1, visited, possibleWordNode, results);

	visited.erase(currentPos);
}

void GameBoard::Exec(std::queue< std::function<void()> >& jobs)
{
	while (!jobs.empty())
	{
		jobs.front()();
		jobs.pop();
	}
}

void GameBoard::Solve()
{
	int processorCount = std::thread::hardware_concurrency();
	std::unordered_set< std::string > results;

	for (int y = 0; y < boardSize; y++)
	{
		std::vector<std::queue<std::function<void()>>> threadJobPools(processorCount);
		std::vector<std::thread> workers;

		for (int x = 0; x < boardSize; x++)
		{
			std::unordered_set<Coord> visited;
			//Traverse(x, y, visited, prefixTree.root, results); //single threaded
			threadJobPools[(boardSize * y + x) % processorCount].push(std::bind(&GameBoard::Traverse, this, x, y, visited, prefixTree.root, std::ref(results)));
		}

		for (auto& pool : threadJobPools)
		{
			//std::cout << pool.size() << std::endl;
			if (!pool.empty())
			{
				workers.emplace_back(&GameBoard::Exec, this, std::ref(pool));
			}
		}

		for (auto& w : workers)
		{
			w.join();
		}
	}

	PrintResultsAndScores(results);
	
}

void GameBoard::PrintResultsAndScores(std::unordered_set< std::string > &results) const
{
	int overallScore = 0;
	int score = 0;
	std::unordered_map<int, int> scoresTable{ {3, 1}, {4, 1}, {5, 2}, {6, 4}, {7, 5} };

	std::cout << "\n\n************************************\nResults:" << std::endl;

	for each (auto word in results)
	{
		score = GetWordScore(word, scoresTable);
		overallScore += score;
		std::cout << word << GetIndent(word, longestWordLength) << score << std::endl;
	}

	std::cout << "____________________________________" << std::endl;
	std::cout << "Overallscore: " << overallScore << std::endl;
	std::cout << "************************************" << std::endl;

	results.clear();
}

int GameBoard::GetWordScore(std::string &word, std::unordered_map<int, int> &scoresTable)
{
	if (word.length() >= 8)
	{
		return 11;
	}

	if (scoresTable.find(word.length()) == scoresTable.end())
	{
		return 0;
	}

	return scoresTable[word.length()];
}

void GameBoard::ReadWordListFile(const std::string &wordListFilePath)
{
	std::ifstream file(wordListFilePath);
	std::string word;

	while (std::getline(file, word))
	{
		if (word.size() > longestWordLength)
		{
			longestWordLength = word.size();
		}

		prefixTree.Insert(word);
	}
}

void GameBoard::Generate(const int &boardSize)
{
	grid = new char*[boardSize];
	this->boardSize = boardSize;

	for (int y = 0; y < boardSize; y++)
	{
		grid[y] = new char[boardSize];
		for (int x = 0; x < boardSize; x++)
		{
			grid[y][x] = 'a' + GetRandomIntWithinRange(0, 26);
		}
	}
}