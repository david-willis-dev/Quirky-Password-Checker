// Project 3 - Main 
#include "jaro.cpp" // The Jaro algorithm
#include <queue>
#include <string>
#include <vector>
using namespace std;


/// NOTE: temporary functions below, would be easier to separate into two classes! Just laying out the ideas here:
// For both data structures below: output the uniqueness of the input password, and then compare it to passwords in passwordArray (moved to data structure)
// Output the similar passwords, and only exit output if certain escape key is pressed (e?). Output calculation time until the escape was pressed.
void redBlackTree(vector<string>passwordArray, string password)
{
	cout << "Red - Black Tree" << endl;
}
void hashMap(vector<string> passwordArray, string password)
{
	cout << "Hash Map" << endl;
}

// Menu implementation: User input will be handled within here as well:
void menu(vector<string> passwordArray)
{
	int structureChoice;
	string password;

	cout << "Quirky Password Checker" << endl;
	cout << "===========================" << endl;

	// Run until the menu is exited
	while (!exit)
	{
		cout << "Pick your data structure (type 1 or 2):" << endl;
		cout << "1. Red-Black Tree" << endl; /// Red-black tree is subject to change to a different structure!
		cout << "2. Hash Map" << endl;
		cout << "3. Exit Program" << endl;
		cin >> structureChoice;

		// Check if need to exit:
		if (structureChoice == 3)
			exit;

		// Otherwise prompt for password:
		cout << "Input your password: ";
		cin >> password;

		switch (structureChoice)
		{
		case 1:
			redBlackTree(passwordArray, password);
			break;
		case 2:
			hashMap(passwordArray, password);
			break;
		default:
			cout << "Incorrect input, try again." << endl;
			exit;
		}
	}
}

// Main: Reads input.txt, and calls menu (which calls the structures)
int main()
{
	// Read in input file (all the passwords) and store into an array (0 = first in txt, and array.size-1 = last)
	string password;
	vector <string> passwordArray;

	ifstream file("input.txt");

	if (file.is_open())
	{
		// Only loop while there are lines (passwords) to be read:
		while (getline(file, password))
			// Add to the array of passwords
			passwordArray.push_back(password);
		file.close();
	}

	for (int i = 0; i < passwordArray.size(); i++)
	{
		cout << password.at(i) << endl;
	}

	// Now that passwords are stored in temporary array via file, show the menu:
	menu(passwordArray);

	return 0;
}
