#include <string>
#include <iostream>
#include <algorithm>
#include <unordered_set>
#include <fstream>
#include <stdexcept>
using namespace std;

// s1 is the user password, s2 is the password you're comparing it to
float jaroDistance(string s1, string s2) {
	int passwordLength = s1.length();
	// m is the number of matching characters
	// t is the number of transpositions
	// searchableDistance is the distance within which a character is considered matching
	int m = 0, t = 0, searchableDistance;
	// s1 will always be longer than s2
	if (s1.length() < s2.length())
		swap(s1, s2);
	searchableDistance = (s1.length() / 2) - 1;
	// Any characters counted in a match can only be counted once, so we keep track of them in a set
	unordered_set<int> matchedIndices;
	//This is the execution of the algorithm
	int greatestIndex = min(s1.length(), s2.length() + searchableDistance);
	for(int i = 0; i < greatestIndex; i++) {
		// Test to see if there is a match
		// Find the first unique match
		for(int j = max(0, i - searchableDistance); j < min(int(s2.length()), i + searchableDistance + 1); j++) {
			if(s1[i] == s2[j] && matchedIndices.find(j) == matchedIndices.end()) {
				// If it is not an exact match, its both a match and a transposition
				if(i != j)
					t++;
				m++;
				matchedIndices.insert(j);
				break;
			}
		}
	}
	// Calculate and return the Jaro Distance
	float jaro = 0;
	if (m != 0)
		jaro = (1.0 / 2) * (float(m) / passwordLength + float(m - t / 2.0) / m);
	return jaro;
}

/*
int main() {
	fstream inputFile("input2.txt", fstream::in);
	if(!inputFile.is_open())
		throw runtime_error("File not opened correctly");
		
	//Get the strings from the input file
	string s1, s2;
	getline(inputFile, s1);
	getline(inputFile, s2);
	cout << "Distance between the strings: " << jaroDistance(s1, s2);
}
*/