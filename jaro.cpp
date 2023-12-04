// References:
// https://www.youtube.com/watch?v=s0YSKiFdj8Q&t=916s
// https://www.geeksforgeeks.org/jaro-and-jaro-winkler-similarity/#
// https://stackoverflow.com/questions/32439453/is-there-an-existing-algorithm-in-checking-password-strength-or-re-invent-the-w
#include <string>
#include <iostream>
#include <algorithm>
#include <unordered_set>
#include <fstream>
#include <cctype>
using namespace std;

// This class is made to generate statistics about the relation between two passwords
class passwordRater {

	public:
		passwordRater(string mainPassword, string mostSimilar, int mostSimilarPosition);
		float jaroDist();
		float getQuirkScore();

	private:
		// Used for Jaro/Quirk Distance
		int numTranspositions = 0, numMatches = 0;
		
		// Security factors, Used for generating the QuirkScore
		string leastUsedCharType;
		int setScore = 0, positionScore = 0, quirkoScore = 0, lengthScore = 0, mostSimilarPosition = 0;;

		// Password and most common similar string
		string mainPassword, mostSimilar;
		
		// Private Methods
		float quirkDist();
		double scoreLength();
		float scoreSet();
		double scorePosition();
};

// The quirk score represents how unique and secure a password is
float passwordRater::getQuirkScore() {
	double quirkScore = 0;
	quirkScore += setScore;
	quirkScore += positionScore;
	quirkScore += quirkoScore;
	quirkScore += lengthScore;
	if (jaroDist() >= 1)
		quirkScore /= 100;
	return 100 / 19 * quirkScore;
}

// This password runs the Jaro algorithm and computes each security factor's score
passwordRater::passwordRater(string mainPassword, string mostSimilar, int mostSimilarPosition) {
	this->mainPassword = mainPassword;
	this->mostSimilar = mostSimilar;
	this->mostSimilarPosition = mostSimilarPosition;
	int passwordLength = mainPassword.length();
	// m is the number of matching characters
	// t is the number of transpositions
	// searchableDistance is the distance within which a character is considered matching
	int searchableDistance;
	// s1 will always be longer than s2
	string s1 = mainPassword, s2 = mostSimilar;
	if (mainPassword.length() < mostSimilar.length())
		swap(s1, s2);
	searchableDistance = (s1.length() / 2) - 1;
	// Any characters counted in a match can only be counted once, so we keep track of them in a set
	unordered_set<int> matchedIndices;
	int greatestIndex = min(s1.length(), s2.length() + searchableDistance);
	// This is the execution of the algorithm
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
	positionScore = scorePosition();
	setScore = scoreSet();
	quirkoScore = 4 * (1 - quirkDist());
	lengthScore = scoreLength();
	
}

// The longer the password, the greater the length score
double passwordRater::scoreLength() {
	return mainPassword.length() * 0.25 - 1;
}

// This returns the Jaro algorithm's distance, or the similarity score
float passwordRater::passwordRater::jaroDist() {
	// Calculate and return the Jaro Distance
	float jaro = 0;
	if (numMatches != 0) {
		jaro = float(numMatches) / mainPassword.length();
		jaro += float(numMatches) / mostSimilar.length();
		jaro += float(numMatches - numTranspositions / 2.0) / numMatches;
		jaro /= 3;
	}
	return jaro;
}

// This method returns a modified version of the jaro distance made for passwords
float passwordRater::quirkDist() {
	// Calculate and return the Quirky distance
	float dist = 0;
	if(numMatches != 0) {
		dist = 3 * float(numMatches) / mainPassword.length();
		dist += float(numMatches - numTranspositions) / numMatches;
		dist /= 4;
	}
	return dist;
}

// This method returns a score based on the variance of the character sets used in the password
float passwordRater::scoreSet() {
	int setScore = 0, numLower = 0, numUpper = 0, numSpecial = 0, numNumber = 0;
	for(char c : mainPassword) {
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
	float variance = float(leastOccurringSet) / mainPassword.length();
	if (variance >= 0.05)
		setScore++;
	if(variance >= 0.1)
		setScore++;
	return float(setScore) * 3/2;
}

// This method returns a score based on how common the most similar password is
double passwordRater::scorePosition() {
	int score = 0;
	if(mostSimilarPosition > 25000)
		score += 1;
	if(mostSimilarPosition > 50000)
		score += 1;
	return score;
}
