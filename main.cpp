// =========================== Project 3 - Quirky Password Checker ===========================
// Aidan Doherty, David Willis, Abigail Lawson | Fall 2023

#include "jaro.cpp" // The Jaro algorithm
#include "red_black_tree.cpp" // The Red-Black Tree
#include "heap.cpp" // The Max Heap
#include <string>
#include <ctime>
#define MAX_SIZE 100000
using namespace std;

// Read in the 100k passwords via the file (input.txt) REF: https://lucidar.me/en/security/list-of-100000-most-common-passwords/ 
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
    
    return passwordArray;
}

// For both data structures below: output the uniqueness of the input password, and then compare it to passwords in passwordArray (moved to data structure)
// Output the similar passwords, and only exit output if certain escape key is pressed (e?). Output calculation time until the escape was pressed.
void redBlackTree(string* passwordArray, string password)
{
    cout << "Red - Black Tree" << endl;
    cout << "---------------------------" << endl;
    
    clock_t t_redBlack; // Time of red-black tree
    clock_t t_maxHeap; // Time of max heap
    int topindex = 0; // topIndex keeps track fo the index of the top Jaro value, in terms of the input.txt file's lines
    t_redBlack = clock();
    RBTree myTree;
    
    // Ensure that we only insert up to max size (100,000) defined at top of main!
    for(int i = 0; i < MAX_SIZE; i++) {
	passwordRater comparison(password, passwordArray[i], i);
        myTree.insert(comparison.jaroDist(), passwordArray[i], ++topindex);
    }
    t_redBlack = clock() - t_redBlack;
    float cr_redBlack = (float)t_redBlack/CLOCKS_PER_SEC; // Click rate of red-black tree
    cout << "Time it took: " << endl;
    cout << "Red-Black: " << t_redBlack << " clicks at " << CLOCKS_PER_SEC << " cps (" << cr_redBlack << " seconds)" << endl;

    // Repeat for the max heap (to show the time difference)
    t_maxHeap = clock();
    Heap myHeap;
    
    for(int i = 0; i < MAX_SIZE; i++) {
	passwordRater comparison(password, passwordArray[i], i);
        myHeap.insert(comparison.jaroDist(), passwordArray[i], i);
    }
    t_maxHeap = clock() - t_maxHeap;
    float cr_maxHeap = (float)t_maxHeap/CLOCKS_PER_SEC; // Click rate of max heap
    cout << "Max Heap: " << t_maxHeap << " clicks at " << CLOCKS_PER_SEC << " cps (" << cr_maxHeap << " seconds)" << endl;

    // Print the comparison of them!
    if(t_redBlack > t_maxHeap)
        cout << "The Max Heap ran faster!" << endl;
    else
        cout << "The Red-Black tree ran faster!" << endl;
    cout << "---------------------------" << endl;

    // Calculate the quirky score:
    string mostSimilar;
    int mostSimilarPos;
    myTree.getTop(mostSimilar, mostSimilarPos);
    passwordRater mainComparison(password, mostSimilar, mostSimilarPos);
    float quirkScore = mainComparison.getQuirkScore();
    cout << "Quirky Score: " << quirkScore << endl;
    if(quirkScore < 25)
        cout << "Blech, your password is not quirky at all!" << endl;
    else if(quirkScore < 50)
        cout << "Uhh your password is kinda quirky, I guess... Could be better..." << endl;
    else if (quirkScore < 75)
        cout << "Wow uhm, I really like how quirky and unique your password is *blush*" << endl;
    else
        cout << "WOWZA!!! What a quirky Password! Where'd you get a password like that???" << endl;
    cout << "===========================" << endl;
    //printf ("It took me %d clicks (%f seconds).\n",t,((float)t)/CLOCKS_PER_SEC);
    myTree.reverseInOrder();
}

// Max Heap:
void maxHeap(string* passwordArray, string password)
{
    // Get the time of the max heap: (in clicks and cps)
    cout << "Max Heap" << endl;
    cout << "---------------------------" << endl;

    clock_t t_maxHeap; // Time of max heap
    clock_t t_redBlack; // Time of red-black tree
    t_maxHeap = clock();
    Heap myHeap;
    
    // Ensure that we only insert up to max size (100,000) defined at top of main!
    for(int i = 0; i < MAX_SIZE; i++) {
	passwordRater comparison(password, passwordArray[i], i);
        myHeap.insert(comparison.jaroDist(), passwordArray[i], i);
    }
    t_maxHeap = clock() - t_maxHeap;
    float cr_maxHeap = (float)t_maxHeap/CLOCKS_PER_SEC; // Click rate of max heap
    cout << "Time it took: " << endl;
    cout << "Max Heap: " << t_maxHeap << " clicks at " << CLOCKS_PER_SEC << " cps (" << cr_maxHeap << " seconds)" << endl;

    // Repeat for the red-black tree (to show the time difference)
    t_redBlack = clock();
    RBTree myTree;
    
    for(int i = 0; i < MAX_SIZE; i++) {
	passwordRater comparison(password, passwordArray[i], i);
        myTree.insert(comparison.jaroDist(), passwordArray[i], 0);
    }
    t_redBlack = clock() - t_redBlack;
    float cr_redBlack = (float)t_redBlack/CLOCKS_PER_SEC; // Click rate of red-black tree
    cout << "Red-Black Tree: " << t_redBlack << " clicks at " << CLOCKS_PER_SEC << " cps (" << cr_redBlack << " seconds)" << endl;

    // Print the comparison of them!
    if(cr_redBlack > cr_maxHeap)
        cout << "The Max Heap ran faster!" << endl;
    else
        cout << "The Red-Black tree ran faster!" << endl;
    cout << "---------------------------" << endl;

    // Calculate the quirky score:
    passwordRater mainComparison(password, myHeap.getTopPassword(), myHeap.getTopIndex());
    float quirkScore = mainComparison.getQuirkScore();
    cout << "Quirky Score: " << quirkScore << endl;
    if(quirkScore < 25)
        cout << "Blech, your password is not quirky at all!" << endl;
    else if(quirkScore < 50)
        cout << "Uhh your password is kinda quirky, I guess... Could be better..." << endl;
    else if (quirkScore < 75)
        cout << "Wow uhm, I really like how quirky and unique your password is *blush*" << endl;
    else
        cout << "WOWZA!!!! What a quirky Password! Where'd you get a password like that???" << endl;
    cout << "===========================" << endl;

    // Print out the actual heap: (print top 10)
    myHeap.printTop();
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
        cout << "1. Red-Black Tree" << endl; 
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
    // Now that passwords are stored in a temporary array via file, show the menu/prompt input
    menu(passwordArray);

    return 0;
}
