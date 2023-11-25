#include <string>
#include <iostream>
#include <algorithm>
#include <unordered_set>
#include <fstream>
#include <stdexcept>
using namespace std;

float jaroDistance(string s1, string s2) {
	// m is the number of matching characters
	// t is the number of transpositions
	// searchableDistance is the distance within which a character is considered matching
	int m = 0, t = 0, searchableDistance;
	// s1 will always be longer than s2
	if (s1.length() < s2.length())
		swap(s1, s2);
	searchableDistance = (s1.length() / 2) - 1;
	// Any characters counted in a match can only be counted once, so wer keep track of them in a set
	unordered_set<int> matchedIndices;
	//This is the execution of the algorithm
	for(int i = 0; i < min(s1.length(), s2.length() + searchableDistance); i++) {
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
	return  (1.0 / 3) * (float(m) / s1.length() + float(m) / s2.length() + (float(m) - t/2.0) / m);
	
}

int main() {
	fstream inputFile("input.txt", fstream::in);
	if(!inputFile.is_open())
		throw runtime_error("File not opened correctly");
		
	//Get the strings from the input file
	string s1, s2;
	getline(inputFile, s1);
	getline(inputFile, s2);
	cout << "Distance between the strings: " << jaroDistance(s1, s2);
}