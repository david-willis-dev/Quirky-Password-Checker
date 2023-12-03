// Project 3 - Main 
#include "jaro.cpp" // The Jaro algorithm
#include "red_black_tree.cpp" // The Red-Black Tree
#include <string>
#include <ctime>
#define MAX_SIZE 100000
using namespace std;

/*
PROGRAM OVERVIEW
Program is made to check peoples passwords
1. First read all of a person's passwords with their similarities into a data structure
2. Print the 100 most similar passwords


*/

string* readFile(string filename)
{
	cout << "here" << endl;
	// Read in input file (all the passwords) and store into an array (0 = first in txt, and array.size-1 = last)
	string password;
	string* passwordArray = new string[100000];
	ifstream file(filename);

	if (file.is_open())
	{
		// Only loop while there are lines (passwords) to be read:
		for(int i = 0; i < 100000; i++)
		{
			getline(file, password);
			// Add to the array of passwords
			passwordArray[i] = password;
		}
		file.close();
	}
	else
		cout << "File was not opened" << endl;
	
	for(int i = 0; i < 10; i++)
		cout << "Passsword " << i * 1000 << ": " << passwordArray[i * 1000] << endl;
	return passwordArray;
}

/// NOTE: temporary functions below, would be easier to separate into two classes! Just laying out the ideas here:
// For both data structures below: output the uniqueness of the input password, and then compare it to passwords in passwordArray (moved to data structure)
// Output the similar passwords, and only exit output if certain escape key is pressed (e?). Output calculation time until the escape was pressed.
void redBlackTree(string* passwordArray, string password)
{
	clock_t t;
	cout << "Red - Black Tree" << endl;
	t = clock();
	RBTree myTree;
	for(int i = 0; i < MAX_SIZE; i++)
		myTree.insert(jaroDistance(password, passwordArray[i]), passwordArray[i]);
	t = clock() - t;
	cout << "Tree Filled" << endl;
	float cr = (float)t/CLOCKS_PER_SEC;
	cout << "It took " << t << " clicks at " << CLOCKS_PER_SEC << " cps (" << cr << " seconds)" << endl;
	//printf ("It took me %d clicks (%f seconds).\n",t,((float)t)/CLOCKS_PER_SEC);
	myTree.reverseInOrder();
}
void hashMap(string* passwordArray, string password)
{
	cout << "Hash Map" << endl;
}

// Menu implementation: User input will be handled within here as well:
void menu(string* passwordArray)
{
	int structureChoice;
	string password;
	bool exitProgram = false;

	cout << "Quirky Password Checker" << endl;
	cout << "===========================" << endl;

	// Run until the menu is exited
	while (!exitProgram)
	{
		cout << "Pick your data structure (type 1 or 2):" << endl;
		cout << "1. Red-Black Tree" << endl; /// Red-black tree is subject to change to a different structure!
		cout << "2. Hash Map" << endl;
		cout << "Type any other key to exit program." << endl;
		cin >> structureChoice;

		// Check if need to exit:
		if (structureChoice != 1 && structureChoice != 2)
		{
			cout << "Exiting program.";
			exitProgram = true;
			continue;
		}
		// Otherwise prompt for password:
		cout << "Input your password: ";
		cin >> password;
		cout << endl;

		switch (structureChoice)
		{
		case 1:
			redBlackTree(passwordArray, password);
			break;
		case 2:
			cout << "TODO: Run program with Hash Map" << endl;
			break;
		default:
			cout << "Incorrect input, try again." << endl;
			exitProgram = true;
		}
	}
}

// Main: Reads input.txt, and calls menu (which calls the structures)
int main()
{
	cout << "In main" << endl;
	string* passwordArray = readFile("input.txt");
	// Now that passwords are stored in temporary array via file, show the menu:
	menu(passwordArray);

	return 0;
}
