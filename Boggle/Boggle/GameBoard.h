#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_set>
#include <thread>
#include <mutex>
#include <deque>
#include <ctime>
#include <algorithm>

struct CompareStrByLength
{
	bool operator()(const std::string &first, const std::string &second)
	{
		return first.size() < second.size();
	}
};

class GameBoard
{
	std::unordered_set<std::string> words;
	std::unordered_set<std::string> wordParts;
	int longestWordLength = 0;
	std::string const wordListFilePath = "wordsList.txt";
	std::vector< std::vector<char> > dice;
	int boardSize = 4;
	std::recursive_mutex traverseMutex;

public:
	GameBoard();
	~GameBoard();
	void GenerateDice(const int &boardSize);
	void ShuffleDice();
	void ShowDice();
	void Solve();

private:
	void ReadWordListFile(const std::string &wordListFilePath);
	int CountGivenLetterIn2dVector(const char & letter, const std::vector<std::vector<char>>& vec);
	void PrewarmWordPartsSet();
	void Traverse(const int &x, const int &y, std::unordered_set<std::string> curWordVisitedNodes, std::string possibleWord, std::vector< std::string > &results);
	void Exec(std::deque<std::function<void()>>& jobs);
	void PrintResultsAndScores(std::vector<std::string>& results);
	int GetWordsScore(std::string &word);
};

