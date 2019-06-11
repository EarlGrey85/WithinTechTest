#include "GameBoard.h"

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
= default;

void GameBoard::Show()
{
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

void GameBoard::Traverse(const int &x, const int &y, std::unordered_set<Coord> visited, std::string possibleWord, std::unordered_set< std::string > &results)
{
	if (x < 0 || x >= boardSize || y < 0 || y >= boardSize)
	{
		return;
	}

    auto currentPos = Coord(x, y);

    if(visited.find(currentPos) != visited.end())
    {
        return;
    }

    visited.insert(currentPos);
    possibleWord += grid[y][x];

	if (wordParts.find(possibleWord) == wordParts.end())
	{
		return;
	}

    traverseMutex.lock();

    if (possibleWord.length() >= 3 && words.find(possibleWord) != words.end())
    {
        results.insert(possibleWord);
    }

    traverseMutex.unlock();

	Traverse(x, y + 1, visited, possibleWord, results);
	Traverse(x - 1, y + 1, visited, possibleWord, results);
	Traverse(x - 1, y, visited, possibleWord, results);
	Traverse(x - 1, y - 1, visited, possibleWord, results);
	Traverse(x, y - 1, visited, possibleWord, results);
	Traverse(x + 1, y - 1, visited, possibleWord, results);
	Traverse(x + 1, y, visited, possibleWord, results);
	Traverse(x + 1, y + 1, visited, possibleWord, results);
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
	std::unordered_set< std::string > results;
	std::vector<std::deque<std::function<void()>>> threadJobPools(processorCount);


	for (int y = 0; y < boardSize; y++)
	{
		for (int x = 0; x < boardSize; x++)
		{
			std::string possibleWord;
            std::unordered_set<Coord> visited;
			threadJobPools[(boardSize * y + x) % processorCount].emplace_back(std::bind(&GameBoard::Traverse, this, x, y, visited, possibleWord, std::ref(results)));
		}
	}

	for (auto &pool : threadJobPools)
	{
		if (!pool.empty())
		{
			workers.emplace_back(&GameBoard::Exec, this, std::ref(pool));
		}
	}

	for (auto &w : workers)
	{
		w.join();
	}

	PrintResultsAndScores(results);
}

void GameBoard::PrintResultsAndScores(std::unordered_set< std::string > &results)
{
	int overallScore = 0;
	int score = 0;

	std::cout << "\n\n************************************\nResults:" << std::endl;

	for each (auto result in results)
	{
		score = GetWordsScore(result);
		overallScore += score;
		std::cout << result << GetIndent(result, longestWordLength) << score << std::endl;
	}

	std::cout << "____________________________________" << std::endl;
	std::cout << "Overallscore: " << overallScore << std::endl;
	std::cout << "************************************" << std::endl;
}

int GameBoard::GetWordsScore(std::string &word)
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
	std::string word;
    std::stringstream ss;

	while (std::getline(file, word))
	{	
		words.insert(word);

        ss.str(std::string());
        auto len = word.length();

        if (len > longestWordLength)
        {
            longestWordLength = len;
        }

        for each (auto letter in word)
        {
            ss << letter;
            wordParts.insert(ss.str());
        }
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

void GameBoard::Generate(const int &boardSize)
{
	grid.clear();
	this->boardSize = boardSize;
    std::vector<char> tempVector;
	
    for (int y = 0; y < boardSize; y++)
    {
        for (int x = 0; x < boardSize; x++)
        {
            tempVector.push_back(static_cast<char>('a' + GetRandomIntWithinRange(0, 26)));
        }

        grid.push_back(tempVector);
        tempVector.clear();
    }
}