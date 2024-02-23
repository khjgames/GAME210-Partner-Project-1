#pragma once

const int MAX_ENTRIES = 10;
const int MAX_NAME_SIZE = 5;
const int MAX_SCORE_SIZE = 11;

struct LBEntry {
	char name[MAX_NAME_SIZE + 1];
	int score;
};

class Leaderboard {
public:
	Leaderboard(const char* fn);
	~Leaderboard() {};

	LBEntry GetEntry(int index); 
	bool IsWorthy(int score);
	void Save();

private:
	std::string fileName = "";
	LBEntry Entries[MAX_ENTRIES] = {};

	void FillDefault();
};