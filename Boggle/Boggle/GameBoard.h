#pragma once
#include <string>
#include <vector>
#include <unordered_set>
#include <thread>
#include <mutex>
#include <queue>
#include "PrefixTree.h"
#include <unordered_map>
#include <functional>

struct CompareStrByLength
{
	bool operator()(const std::string &first, const std::string &second) const
	{
		return first.size() < second.size();
	}
};

struct Coord
{
	int X;
	int Y;

	Coord() : X(0), Y(0) {};
	Coord(const int& x, const int& y) : X(x), Y(y) {};
	Coord(const Coord& other)
	{
		X = other.X;
		Y = other.Y;
	};

	bool operator == (const Coord& other) const
	{
		return X == other.X && Y == other.Y;
	};

	size_t operator()(const Coord& pointToHash) const noexcept
	{
		size_t hash = pointToHash.X + pointToHash.Y * 10;
		return hash;
	};
};

namespace std
{
	template<> struct hash<Coord>
	{
		std::size_t operator()(const Coord& p) const noexcept
		{
			return p(p);
		}
	};
}

class GameBoard
{
	PrefixTree prefixTree;
	int longestWordLength = 0;
	std::string const wordListFilePath = "wordsList.txt";
	char** grid;
	int boardSize = 4;
	std::recursive_mutex traverseMutex;

public:
	GameBoard();
	~GameBoard();
	void Generate(const int &boardSize);
	void Show() const;
	void Solve();
	void CleanGrid();

private:
	void ReadWordListFile(const std::string &wordListFilePath);
	void Traverse(const int &x, const int &y, std::unordered_set<Coord> visited, PrefixTreeNode *possibleWordNode, std::unordered_set< std::string > &results);
	void Exec(std::queue<std::function<void()>> &jobs);
	void PrintResultsAndScores(std::unordered_set<std::string>& results) const;
	static int GetWordScore(std::string &word, std::unordered_map<int, int> &scoresTable);
};