///////////////////////////
// WRITTEN BY TUCKER DAY //
///////////////////////////

#pragma once

namespace LeaderboardConst {
	const int MAX_ENTRIES = 10;
	const int MAX_NAME_SIZE = 9;
	const int MAX_SCORE_SIZE = 11;

	const char NAME_TERMINATE = '_';
	const char SCORE_TERMINATE = ';';
}

using namespace LeaderboardConst;

struct LBEntry {
	char name[MAX_NAME_SIZE + 1];
	int score;
};

class Leaderboard {
public:
	Leaderboard(const char* fn);
	~Leaderboard() {};

	LBEntry GetEntry(int index);
	short AddEntry(const char* name, int score);
	bool IsWorthy(int score);
	void Save();

private:
	std::string fileName = "";
	LBEntry Entries[MAX_ENTRIES] = {};

	void FillDefault();
};