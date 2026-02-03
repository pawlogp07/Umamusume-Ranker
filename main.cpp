#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <stdio.h>
#include <time.h>
#include <limits>
using namespace std;
struct Character
{
	char name[64]{};
	int keepCutState = 0;
};
Character* character = nullptr;
int characterCount = 0;
bool running = true;
static void init()
{
	srand((int)time(NULL));
	FILE* file = fopen("characters.txt", "r");
	if (file == NULL)
		return;
	(void)fscanf(file, "%d", &characterCount);
	character = new Character[characterCount];
	for (int i = 0; i < characterCount; i++)
		(void)fscanf(file, "%s", character[i].name);
	fclose(file);
}
static int handle_input() 
{
	int input;
	while (!(cin >> input)) 
	{
		cout << "\nInvalid input, enter an int value\n";
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
	}
	return input;
}
static void randomize(int start, int count)
{
	for (int i = start; i < count; i++)
	{
		Character temp = character[i];
		int randIndex = rand() % characterCount;
		character[i] = character[randIndex];
		character[randIndex] = temp;
	}
}
static void blindRanking()
{
	int input, current = 5, filled = 0;
	int results[5] = { -1, -1, -1, -1, -1 };
	bool position_taken[5] = { false };
	randomize(5, 10);
	cout << "\n";
	while (running)
	{
		if (filled == 5)
		{
			cout << "\nYour final ranking:" << endl;
			for (int i = 0; i < 5; i++)
			{
				string name = position_taken[i] ? character[results[i]].name : "---";
				cout << i + 1 << ". " << name << endl;
			}
			running = false;
			break;
		}
		cout << "\n";
		for (int i = 0; i < 5; i++)
		{
			string name = position_taken[i] ? character[results[i]].name : "---";
			cout << i + 1 << ". " << name << endl;
		}
		cout << "\nRank " << character[current].name << endl;
		input = handle_input();
		if (input >= 1 && input <= 5 && !position_taken[input - 1])
		{
			results[input - 1] = current;
			position_taken[input - 1] = true;
			current++;
			filled++;
		}
		else
			cout << "\nPosition is already taken" << endl;
	}
}
static void draw44table(int kCount, int cCount, int current)
{
	cout << "\nKEEP:";
	for (int i = 0; i < 8; i++)
	{
		if (character[i].keepCutState == 1)
			cout << "\n" << kCount++ << ". " << character[i].name;
	}
	for (int i = kCount; i <= 4; i++)
		cout << "\n" << i << ". ---";
	cout << "\n\nCUT:";
	for (int i = 0; i < 8; i++)
	{
		if (character[i].keepCutState == -1)
			cout << "\n" << cCount++ << ". " << character[i].name;
	}
	for (int i = cCount; i <= 4; i++)
		cout << "\n" << i << ". ---";
}
static void keep4cut4()
{
	int input, current = 0, filled = 0;
	int keepslots = 4, cutslots = 4;
	randomize(0, 8);
	cout << "\n";
	while (running)
	{
		int kCount = 1, cCount = 1;
		if (filled == 8)
		{
			cout << "\nYour final ranking:" << endl;
			draw44table(kCount, cCount, current);
			cout << "\n";
			running = false;
			break;
		}
		draw44table(kCount, cCount, current);
		cout << "\n\nKeep or cut: " << character[current].name << " [Keep - 1/Cut - 2]" << endl;
		input = handle_input();
		if (input == 1 && keepslots > 0)
		{
			character[current].keepCutState = 1;
			keepslots--;
			current++;
			filled++;
		}
		else if (input == 2 && cutslots > 0)
		{
			character[current].keepCutState = -1;
			cutslots--;
			current++;
			filled++;
		}
		else if ((input == 1 && keepslots == 0) || (input == 2 && cutslots == 0))
			cout << "\nNo slots remaining" << endl;
		else
			cout << "\nInvalid input" << endl;
	}
}
int main()
{
	init();
	int choice = 0;
	bool menu = true;
	while (menu)
	{
		cout << "[1] BLIND RANKING" << endl;
		cout << "[2] KEEP 4 CUT 4" << endl;
		choice = handle_input();
		if (choice == 1 && characterCount >= 10)
		{
			menu = false;
			blindRanking();
		}
		else if (choice == 2 && characterCount >= 8)
		{
			menu = false;
			keep4cut4();
		}
		else if ((choice == 2 && characterCount < 8) || (choice == 1 && characterCount < 10))
			cout << "\nAdd more characters to characters.txt to play" << endl << endl;
		else
			cout << "\nInvalid input" << endl << endl;
	}
	delete[] character;
	return 0;
}