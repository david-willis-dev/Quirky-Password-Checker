#include <string>
#include <iostream>
#include <algorithm>
#include <unordered_set>
#include <fstream>
#include <stdexcept>
#include <vector>
#include <cctype>
using namespace std;

class stringComparison {
	public:
		int numTranspositions = 0, numMatches = 0;
		string headString, secondaryString;
		stringComparison(string headString, string secondaryString);
		float jaroDist();
		float quirkDist();
		
};

double scoreLength(string password);
double scoreJaro(string password, string secondary);
float scoreSet(string password);
double scorePosition(int position);
float jaroDistance(string s1, string s2);
float getQuirkScore(string password, string mostSimilar, int mostSimilarPosition);

float getQuirkScore(string password, string mostSimilar, int mostSimilarPosition) {
	double positionScore = scorePosition(mostSimilarPosition);
	int setScore = scoreSet(password);
	stringComparison passCompare(password, mostSimilar);
	float jaroScore = scoreJaro(password, mostSimilar);
	double lengthScore = scoreLength(password);
	double quirkScore = 0;
	quirkScore += setScore;
	quirkScore += positionScore;
	quirkScore += jaroScore;
	quirkScore += lengthScore;
	return quirkScore;
}

double scoreLength(string password) {
	return password.length() / 4 - 1;
}

double scoreJaro(string password, string s2) {
	stringComparison compare(password, s2);
	return 4 * (1 - compare.quirkDist());
}

stringComparison::stringComparison(string headString, string secondaryString) {
	this->headString = headString;
	this->secondaryString = secondaryString;
	int passwordLength = headString.length();
	// m is the number of matching characters
	// t is the number of transpositions
	// searchableDistance is the distance within which a character is considered matching
	int searchableDistance;
	// s1 will always be longer than s2
	string s1 = headString, s2 = secondaryString;
	if (headString.length() < secondaryString.length())
		swap(s1, s2);
	searchableDistance = (s1.length() / 2) - 1;
	// Any characters counted in a match can only be counted once, so we keep track of them in a set
	unordered_set<int> matchedIndices;
	int greatestIndex = min(s1.length(), s2.length() + searchableDistance);
	// This is the execution of the algorithm
	cout << "greatestIndex: " << greatestIndex << endl;
	for(int i = 0; i < greatestIndex; i++) {
		// Test to see if there is a match
		// Find the first unique match
		for(int j = max(0, i - searchableDistance); j < min(int(s2.length()), i + searchableDistance + 1); j++) {
			if(s1[i] == s2[j] && matchedIndices.find(j) == matchedIndices.end()) {
				// If it is not an exact match, its both a match and a transposition
				if(i != j)
					numTranspositions++;
				numMatches++;
				matchedIndices.insert(j);
				break;
			}
		}
	}
	cout << "Num matches: " << numMatches << endl;
	cout << "Num transpositions: " << numTranspositions << endl;
}

float stringComparison::jaroDist() {
	// Calculate and return the Jaro Distance
	float jaro = 0;
	if (numMatches != 0) {
		jaro = float(numMatches) / headString.length();
		jaro += float(numMatches) / secondaryString.length();
		jaro += float(numMatches - numTranspositions / 2.0) / numMatches;
		jaro /= 3;
	}
	return jaro;
}

float stringComparison::quirkDist() {
	// Calculate and return the Quirky distance
	float dist = 0;
	if(numMatches != 0) {
		dist = 3 * float(numMatches) / headString.length();
		dist += float(numMatches - numTranspositions) / numMatches;
		dist /= 4;
	}
	return dist;
}


float scoreSet(string password) {
	int setScore = 0, numLower = 0, numUpper = 0, numSpecial = 0, numNumber = 0;
	for(char c : password) {
		if(islower(c))
			numLower++;
		else if(isupper(c))
			numUpper++;
		else if(isdigit(c))
			numNumber++;
		else
			numSpecial++;
	}
	if (numUpper > 0)
		setScore++;
	if(numNumber > 0)
		setScore++;
	if(numSpecial > 0)
		setScore += 2;
	int leastOccurringSet = min(numLower, numUpper);
	leastOccurringSet = min(leastOccurringSet, numSpecial);
	leastOccurringSet = min(leastOccurringSet, numNumber);
	float variance = float(leastOccurringSet) / password.length();
	if (variance >= 0.05)
		setScore++;
	if(variance >= 0.1)
		setScore++;
	cout << "Variance: " << variance << endl;
	return float(setScore) * 3/2;
}

double scorePosition(int position) {
	int score = 0;
	if(position > 25000)
		score += 1;
	if(position > 50000)
		score += 1;
	return score;
}

// s1 is the user password, s2 is the password you're comparing it to
float jaroDistance(string s1, string s2) {
	stringComparison comparison(s1, s2);
	return comparison.jaroDist();
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
	double positionScore = scorePosition(26000);
	int setScore = scoreSet(s1);
	stringComparison passCompare(s1, s2);
	float jaroScore = scoreJaro(s1, s2);
	double lengthScore = scoreLength(s1);
	cout << "Distance between the strings: " << passCompare.jaroDist() << endl;
	cout << "Quirk Distance: " << passCompare.quirkDist() << endl;
	cout << "Position Score: " << positionScore << "/2" << endl;
	cout << "Set Score: " << setScore << "/9" << endl;
	cout << "Jaro Score: " << jaroScore << "/4" << endl;
	cout << "Length Score: " << lengthScore << "/4" << endl;
	double quirkScore =0;
	//quirkScore += scoreLength(s1);
	//quirkScore += scoreJaro(s1);
	quirkScore += setScore;
	quirkScore += positionScore;
	quirkScore += jaroScore;
	quirkScore += lengthScore;
	cout << "Score is: " << quirkScore << endl;
}
*/
