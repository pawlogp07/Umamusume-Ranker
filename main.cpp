#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include <ctime>
#include <limits>
#include <string>
#include <vector>
using namespace std;
struct Character
{
	string name;
	int keepCutState = 0;
};
vector<Character> characters;
void init(unsigned short &characterCount)
{
	srand((int)time(NULL));
	ifstream file("characters.txt");
	if (!file.is_open()) return;
	if (!(file >> characterCount)) return;
	characters.clear();
	characters.resize(characterCount);
	for (int i = 0; i < characterCount; i++)
		if (!(file >> characters[i].name)) break;
	file.close();
}
short handle_input() 
{
	short input;
	while (!(cin >> input)) 
	{
		cout << "\nInvalid input, enter a short value\n";
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
	}
	return input;
}
void randomize(short start, short count, unsigned short characterCount)
{
	for (int i = start; i < count; i++)
	{
		Character temp = characters[i];
		unsigned short randIndex = rand() % characterCount;
		characters[i] = characters[randIndex];
		characters[randIndex] = temp;
	}
}
void blindRanking(unsigned short characterCount)
{
	short input, current = 5, filled = 0, results[5] = { 0, 0, 0, 0, 0 };
	bool running = true, position_taken[5] = { false, false, false, false, false };
	randomize(5, 10, characterCount);
	cout << "\n";
	while (running)
	{
		if (filled == 5)
		{
			cout << "\nYour final ranking:" << "\n";
			for (int i = 0; i < 5; i++)
			{
				string name = position_taken[i] ? characters[results[i]].name : "---";
				cout << i + 1 << ". " << name << "\n";
			}
			running = false;
			break;
		}
		cout << "\n";
		for (int i = 0; i < 5; i++)
		{
			string name = position_taken[i] ? characters[results[i]].name : "---";
			cout << i + 1 << ". " << name << "\n";
		}
		cout << "\nRank " << characters[current].name << "\n";
		input = handle_input();
		if (input >= 1 && input <= 5 && !position_taken[input - 1])
		{
			results[input - 1] = current;
			position_taken[input - 1] = true;
			current++;
			filled++;
		}
		else
			cout << "\nPosition is already taken" << "\n";
	}
}
void draw44table(short kCount, short cCount, short current)
{
	cout << "\nKEEP:";
	for (int i = 0; i < 8; i++)
		if (characters[i].keepCutState == 1)
			cout << "\n" << kCount++ << ". " << characters[i].name;
	for (int i = kCount; i <= 4; i++)
		cout << "\n" << i << ". ---";
	cout << "\n\nCUT:";
	for (int i = 0; i < 8; i++)
		if (characters[i].keepCutState == -1)
			cout << "\n" << cCount++ << ". " << characters[i].name;
	for (int i = cCount; i <= 4; i++)
		cout << "\n" << i << ". ---";
}
void keep4cut4(unsigned short characterCount)
{
	short input, current = 0, filled = 0, keepslots = 4, cutslots = 4;
	bool running = true;
	randomize(0, 8, characterCount);
	cout << "\n";
	while (running)
	{
		short kCount = 1, cCount = 1;
		if (filled == 8)
		{
			cout << "\nYour final ranking:" << "\n";
			draw44table(kCount, cCount, current);
			cout << "\n";
			running = false;
			break;
		}
		draw44table(kCount, cCount, current);
		cout << "\n\nKeep or cut: " << characters[current].name << " [Keep - 1 / Cut - 2]" << "\n";
		input = handle_input();
		if (input == 1 && keepslots > 0)
		{
			characters[current].keepCutState = 1;
			keepslots--;
			current++;
			filled++;
		}
		else if (input == 2 && cutslots > 0)
		{
			characters[current].keepCutState = -1;
			cutslots--;
			current++;
			filled++;
		}
		else if ((input == 1 && keepslots == 0) || (input == 2 && cutslots == 0))
			cout << "\nNo slots remaining" << "\n";
		else
			cout << "\nInvalid input" << "\n";
	}
}
int main()
{
	ios_base::sync_with_stdio(false);
	cin.tie(NULL);
	cout.tie(NULL);
	unsigned short choice, characterCount = 0;
	init(characterCount);
	bool menu = true;
	while (menu)
	{
		cout << "[1] BLIND RANKING" << "\n";
		cout << "[2] KEEP 4 CUT 4" << "\n";
		choice = handle_input();
		if (choice == 1 && characterCount >= 10)
		{
			menu = false;
			blindRanking(characterCount);
		}
		else if (choice == 2 && characterCount >= 8)
		{
			menu = false;
			keep4cut4(characterCount);
		}
		else if ((choice == 2 && characterCount < 8) || (choice == 1 && characterCount < 10))
			cout << "\nAdd more characters to characters.txt to play" << "\n\n";
		else
			cout << "\nInvalid input" << "\n\n";
	}
	cin.ignore(numeric_limits<streamsize>::max(), '\n');
	cin.get();
	return 0;
}