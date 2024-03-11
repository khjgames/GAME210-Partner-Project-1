///////////////////////////
// WRITTEN BY TUCKER DAY //
///////////////////////////

#pragma once

namespace LeaderboardConst {
	const short MAX_ENTRIES = 10;
	const short MAX_NAME_SIZE = 9;
	const short MAX_SCORE_SIZE = 11;

	const char NAME_TERMINATE = '_';
	const char SCORE_TERMINATE = ';';
	const char LEADERBOARD_INCREMENT = '$';

	enum Leaderboards {
		ONE_PLAYER,
		TWO_PLAYER,

		NUM_LEADERBOARDS
	};
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

	LBEntry GetEntry(int index, int leaderboard = ONE_PLAYER);
	short AddEntry(const char* name, int score, int leaderboard = ONE_PLAYER);
	void Save();

private:
	std::string fileName = "";
	LBEntry Entries[NUM_LEADERBOARDS][MAX_ENTRIES] = {};

	void FillDefault();
};