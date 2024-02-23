///////////////////////////
// WRITTEN BY TUCKER DAY //
///////////////////////////

// standard libraries
#include <fstream>

// header files
#include "Leaderboard.h"

using namespace std;

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
		// the format for leaderboard entries when they are saved is NAME1?SCORE1@NAME2?SCORE2...
		// ? and @ are used due to their ascii value being convinent for verifying valid input

		bool onName = true;						// for tracking if currently reading a name
		short index = 0;						// current entry being writen to
		short letTrack = 0;						// tracking how many letters have been entered
		char numAscii[MAX_SCORE_SIZE] = "";		// char entry for the score

		// loop through the entire file or until the leaderboard is full and copy it into the entries
		while (!input.eof() && index < MAX_ENTRIES)
		{
			char currLetter = input.get();

			// check if end of name entry
			if (currLetter == '?')
			{
				// add terminating char to name
				Entries[index].name[letTrack] = '\0';

				onName = false;
				letTrack = 0;
				currLetter = input.get();
			}
			// check if end of score entry
			else if (currLetter == '@')
			{
				// add \0 to the end of numAscii
				numAscii[letTrack] = '\0';
				
				// turn numAscii into an int and put into entry
				Entries[index].score = atoi(numAscii);
				
				onName = true;
				letTrack = 0;
				index++;
				currLetter = input.get();
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
					Entries[index].name[letTrack] = currLetter;
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
	}
	input.close();
}

LBEntry Leaderboard::GetEntry(int index)
{
	return Entries[index];
}

bool Leaderboard::IsWorthy(int score)
{
	// compare the value passed in to see if it's in the top scores
	return score > Entries[MAX_ENTRIES - 1].score;
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
		// loop through entries and write them to the save file
		for (int i = 0; i < MAX_ENTRIES; i++)
		{
			// copy over the name
			for (int let = 0; let < MAX_NAME_SIZE; let++)
			{
				if (Entries[i].name[let] == '\0')
					break;

				output << Entries[i].name[let];
			}

			// add a '?'
			output << '?';

			// copy over the score
			char temp[MAX_SCORE_SIZE] = "";
			_itoa_s(Entries[i].score, temp, 10);
			output << temp;

			// add a '@'
			output << '@';
		}
	}
}

void Leaderboard::FillDefault()
{
	// fill each leaderboard entry with placeholder data
	for (int e = 0; e < MAX_ENTRIES; e++)
	{
		for (int c = 0; c < MAX_NAME_SIZE; c++) 
		{
			Entries[e].name[c] = 'A' + e;
		}
		Entries[e].name[MAX_NAME_SIZE] = '\0';
		Entries[e].score = MAX_ENTRIES - e;
	}
}