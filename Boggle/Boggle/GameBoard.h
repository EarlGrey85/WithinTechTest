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
    Coord(const Coord& other) {
        X = other.X;
        Y = other.Y;
    };

    bool operator == (const Coord& other) const
    {
        return X == other.X && Y == other.Y;
    };

    bool operator < (const Coord& other) const
    {
        if (X < other.X)
        {
            return true;
        }

        if (X == other.X && Y == other.Y)
        {
            return true;
        }

        return false;
    }

    size_t operator()(const Coord& pointToHash) const noexcept 
    {
        size_t hash = pointToHash.X + 10 * pointToHash.Y;
        return hash;
    };
};

namespace std {
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
	std::unordered_set<std::string> words;
	std::unordered_set<std::string> wordParts;
	int longestWordLength = 0;
	std::string const wordListFilePath = "wordsList.txt";
	std::vector< std::vector<char> > grid;
	int boardSize = 4;
	std::recursive_mutex traverseMutex;

public:
	GameBoard();
	~GameBoard();
	void Generate(const int &boardSize);
	void Show();
	void Solve();

private:
	void ReadWordListFile(const std::string &wordListFilePath);
    static int CountGivenLetterIn2dVector(const char & letter, const std::vector<std::vector<char>>& vec);
	void Traverse(const int &x, const int &y, std::unordered_set<Coord> visited, std::string possibleWord, std::unordered_set< std::string > &results);
	void Exec(std::deque<std::function<void()>>& jobs);
	void PrintResultsAndScores(std::unordered_set<std::string>& results);
	int GetWordsScore(std::string &word);
};

