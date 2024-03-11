///////////////////////////
// WRITTEN BY TUCKER DAY //
///////////////////////////

// standard libraries
#include <fstream>

// header a
#include "Leaderboard.h"

using namespace std;

using namespace LeaderboardConst;

Leaderboard::Leaderboard(const char* fn)
{
	// save the file name for use in saving on close
	fileName = fn;

	ifstream input(fn);

	if (input.fail())
	{
		printf("Save file does not exist. Using default values.\n");
		FillDefault();
	}
	else
	{
		bool onName = true;						// for tracking if currently reading a name
		short index = 0;						// current entry being writen to
		short leaderboardIndex = 0;				// current leaderboard being written to
		short letTrack = 0;						// tracking how many letters have been entered
		char numAscii[MAX_SCORE_SIZE] = "";		// char entry for the score

		// loop through the entire file or until the leaderboard is full and copy it into the entries
		while (!input.eof() && index < MAX_ENTRIES)
		{
			char currLetter = input.get();

			// check if end of name entry
			if (currLetter == NAME_TERMINATE)
			{
				// add terminating char to name
				Entries[leaderboardIndex][index].name[letTrack] = '\0';

				onName = false;
				letTrack = 0;
				currLetter = input.get();
			}
			// check if end of score entry
			else if (currLetter == SCORE_TERMINATE)
			{
				// add \0 to the end of numAscii
				numAscii[letTrack] = '\0';
				
				// turn numAscii into an int and put into entry
				Entries[leaderboardIndex][index].score = atoi(numAscii);
				
				onName = true;
				letTrack = 0;
				index++;
				currLetter = input.get();
			}
			
			// check for leaderboard change
			if (currLetter == LEADERBOARD_INCREMENT)
			{	
				index = 0;
				leaderboardIndex++;
				currLetter = input.get();
				
				// check if there are too many leaderboards in the file
				if (leaderboardIndex > NUM_LEADERBOARDS)
				{
					printf("Save file contains too many leaderboards. Using default values.\n");
					FillDefault();
					break;
				}
			}

			// check for eof
			if (input.eof())
			{
				break;
			}

			// check that the recieved char is one of the possible chars
			if (!((currLetter >= 'A' && currLetter <= 'Z') ||
				(currLetter >= '0' && currLetter <= '9')))
			{
				printf("Save file contains illegal character. Using default values.\n");
				FillDefault();
				break;
			}

			// check what is currently being read
			if (onName)
			{
				// check that name isn't too long
				if (letTrack > MAX_NAME_SIZE - 1)
				{
					printf("Save file contains illegal name. Using default values.\n");
					FillDefault();
					break;
				}
				else
				{
					// copy character into entry
					Entries[leaderboardIndex][index].name[letTrack] = currLetter;
					letTrack++;
				}
			}
			else
			{
				// check that score isn't too long
				if (letTrack > MAX_SCORE_SIZE - 1)
				{
					printf("Save file contains illegal score. Using default values.\n");
					FillDefault();
					break;
				}
				else
				{
					// copy char into numAscii
					numAscii[letTrack] = currLetter;
					letTrack++;
				}
			}
		}

		// check for the correct amount of leaderboards
		if (leaderboardIndex != NUM_LEADERBOARDS)
		{
			printf("Save file dosen't contain enough leaderboards. Using default values.\n");
			FillDefault();
		}
	}
	input.close();
}

LBEntry Leaderboard::GetEntry(int index, int leaderboard)
{
	return Entries[leaderboard][index];
}

short Leaderboard::AddEntry(const char* name, int score, int leaderboard)
{
	// compare the score passed in to the entries in the leaderboard from bottom to top
	int placement = MAX_ENTRIES;
	while (Entries[leaderboard][placement - 1].score < score && placement != 0) { placement--; }

	// check that the score should be added into the leaderboard
	if (placement != MAX_ENTRIES)
	{
		// move other entries down to make room for the new entry
		for (int i = MAX_ENTRIES - 2; i >= placement; i--)
		{
			Entries[leaderboard][i + 1] = Entries[leaderboard][i];
		}

		// check if the name is empty, and fill it if it is
		if (name[0] == '\0')
		{
			name = "NAME";
		}

		// insert name into the new entry
		int c = 0;
		while (name[c] != '\0' && c < MAX_NAME_SIZE)
		{
			Entries[leaderboard][placement].name[c] = name[c];
			c++;
		}
		Entries[leaderboard][placement].name[c] = '\0';

		// insert score into new entry
		Entries[leaderboard][placement].score = score;
	}
	short place = (placement >= MAX_ENTRIES) ? 0 : placement + 1; // return first through tenth place, or 0 if not placed

	return place;
}

void Leaderboard::Save()
{
	// create an output stream
	ofstream output(fileName);

	if (output.fail())
	{
		printf("Save failed");
	}
	else
	{
		// go through all leaderboards
		for (int l = 0; l < NUM_LEADERBOARDS; l++)
		{
			// loop through entries and write them to the save files
			for (int i = 0; i < MAX_ENTRIES; i++)
			{
				// check if the name is empty, and fill it if it is
				if (Entries[l][i].name[0] == '\0')
				{
					Entries[l][i].name[0] = 'N';
					Entries[l][i].name[1] = 'A';
					Entries[l][i].name[2] = 'M';
					Entries[l][i].name[3] = 'E';
				}
				
				// copy over the name
				for (int let = 0; let < MAX_NAME_SIZE; let++)
				{
					if (Entries[l][i].name[let] == '\0')
						break;

					output << Entries[l][i].name[let];
				}

				// add a name terminator
				output << NAME_TERMINATE;

				// copy over the score
				char temp[MAX_SCORE_SIZE] = "";
				_itoa_s(Entries[l][i].score, temp, 10);
				output << temp;

				// add a score terminator
				output << SCORE_TERMINATE;
			}

			// add a leaderboard increment
			output << LEADERBOARD_INCREMENT;
		}
	}
	output.close();
}

void Leaderboard::FillDefault()
{
	// fill each leaderboard entry with placeholder data
	for (int p = 0; p < NUM_LEADERBOARDS; p++)
	{
		for (int e = 0; e < MAX_ENTRIES; e++)
		{
			for (int c = 0; c < MAX_NAME_SIZE; c++)
			{
				Entries[p][e].name[c] = 'A' + e;
			}
			Entries[p][e].name[MAX_NAME_SIZE] = '\0';
			Entries[p][e].score = (MAX_ENTRIES - e) * 100;
		}
	}
}