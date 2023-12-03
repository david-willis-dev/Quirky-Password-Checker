// =========================== Project 3 - Quirky Password Checker ===========================
// Aidan Doherty, David Willis, Abigail Lawson | Fall 2023

#include "jaro.cpp" // The Jaro algorithm
#include "red_black_tree.cpp" // The Red-Black Tree
#include "heap.cpp" // The Max Heap
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

//    for(int i = 0; i < 10; i++)
//        cout << "Passsword " << i * 1000 << ": " << passwordArray[i * 1000] << endl;
    return passwordArray;
}

// For both data structures below: output the uniqueness of the input password, and then compare it to passwords in passwordArray (moved to data structure)
// Output the similar passwords, and only exit output if certain escape key is pressed (e?). Output calculation time until the escape was pressed.
void redBlackTree(string* passwordArray, string password)
{
    clock_t t;
    int origin = 0;
    cout << "Red - Black Tree" << endl;
    t = clock();
    RBTree myTree;
    for(int i = 0; i < MAX_SIZE; i++)
        myTree.insert(jaroDistance(password, passwordArray[i]), passwordArray[i], ++origin);
    t = clock() - t;
    float cr = (float)t/CLOCKS_PER_SEC;
    cout << "It took " << t << " clicks at " << CLOCKS_PER_SEC << " cps (" << cr << " seconds)" << endl;
    string mostSimilar;
    int mostSimilarPos;
    myTree.getTop(mostSimilar, mostSimilarPos);
    float quirkScore = getQuirkScore(password, mostSimilar, mostSimilarPos);
    cout << "Quirky Score: " << quirkScore << endl;
    if(quirkScore < 25)
	cout << "Blech, your password is not quirky at all!" << endl;
    else if(quirkScore < 50)
	cout << "uhh your password is kinda quirky, I guess..." << endl;
    else if (quirkScore < 75)
	cout << "Wow uhm, I really like how quirky and unique your password is *blush*" << endl;
    else
	cout << "WOWZA! What a quirky Password! Where'd you get a password like that???" << endl;
    cout << "===========================" << endl;
    //printf ("It took me %d clicks (%f seconds).\n",t,((float)t)/CLOCKS_PER_SEC);
    myTree.reverseInOrder();
}

// Max Heap:
void maxHeap(string* passwordArray, string password)
{
    clock_t t;
    cout << "Max Heap" << endl;
    t = clock();
    Heap myHeap;
    for(int i = 0; i < MAX_SIZE; i++)
        myHeap.insert(jaroDistance(password, passwordArray[i]), passwordArray[i], i);
    t = clock() - t;
    float cr = (float)t/CLOCKS_PER_SEC;
    cout << "It took " << t << " clicks at " << CLOCKS_PER_SEC << " cps (" << cr << " seconds)" << endl;
    float quirkScore = getQuirkScore(password, myHeap.getTopPassword(), myHeap.getTopIndex());
    cout << "Quirky Score: " << quirkScore << endl;
    if(quirkScore < 25)
	cout << "Blech, your password is not quirky at all!" << endl;
    else if(quirkScore < 50)
	cout << "uhh your password is kinda quirky, I guess..." << endl;
    else if (quirkScore < 75)
	cout << "Wow uhm, I really like how quirky and unique your password is *blush*" << endl;
    else
	cout << "WOWZA! What a quirky Password! Where'd you get a password like that???" << endl;
    cout << "===========================" << endl;
    myHeap.printTop();
    // cout << "File Index of top: " << myHeap.getTopIndex() << endl;
    // cout << myHeap.getTopPassword() << endl;
}

// Menu implementation: User input will be handled within here as well:
void menu(string* passwordArray)
{
    int structureChoice;
    int exitOption;
    string password;
    bool exitProgram = false;

    cout << "Quirky Password Checker" << endl;
    cout << "===========================" << endl;

    // Run until the menu is exited
    while (!exitProgram)
    {
        cout << "Pick your data structure (type 1 or 2):" << endl;
        cout << "1. Red-Black Tree" << endl; /// Red-black tree is subject to change to a different structure!
        cout << "2. Max Heap" << endl;
        cout << "Type any other key to exit program." << endl;
        cin >> structureChoice;

        // Check if need to exit:
        if (structureChoice != 1 && structureChoice != 2)
        {
            cout << "Exiting program.";
            exitProgram = true;
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
                maxHeap(passwordArray, password);
                break;
            default:
                cout << "Incorrect input, try again." << endl;
                exitProgram = true;
        }

        cout << "===========================" << endl;
        cout << "Would you like to continue, or exit the program?" << endl;
        cout << "1. Continue" << endl;
        cout << "Type any other key to exit program." << endl;
        cin >> exitOption;

        if(exitOption != 1)
        {
            cout << "Exiting program.";
            exitProgram = true;
        }
    }
}

// Main: Reads input.txt, and calls menu (which calls the structures)
int main()
{
    string* passwordArray = readFile("input.txt");
    // Now that passwords are stored in temporary array via file, show the menu:
    menu(passwordArray);

    return 0;
}
