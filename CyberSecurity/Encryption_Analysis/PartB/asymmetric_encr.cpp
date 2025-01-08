#include <iostream>
#include <gmpxx.h>
#include <string>
#include <sstream>
#include <fstream>
//#include <cmath>
#include <vector>

using namespace std;

// Function to split message
void splitMessage(string message, mpz_class size, vector<mpz_class>& retVal) {
	// Temporary variables
	mpz_class tempI;
	stringstream buffer;

	// Loop through string and add hex values to tempI
	for (char c : message) buffer << hex << int(c);
	buffer >> tempI;

	// Base case, find when less than n and add to vector
	if (tempI < size) {
		retVal.push_back(tempI);
		return;
	}

	// Split string in two for recursive calls
	string firstHalf, secondHalf;
	firstHalf = message.substr(0, message.size()/2);
	secondHalf = message.substr(message.size()/2);

	// Recursive calls
	splitMessage(firstHalf, size, retVal);
	splitMessage(secondHalf, size, retVal);
}

int main(int argc, char *argv[]) {
	// Check for two files as input
	if (argc != 3) {
		cout << "Incorrect number of arguments\n";
		return 1;
	}

	// Open files
	ifstream in1, in2;
	in1.open(argv[1], ios::in);
	in2.open(argv[2], ios::in);
	if (!in1) {
		cout << "Cannot open key file for reading, exiting program\n";
		return 1;
	}
	if (!in2) {
		cout << "Cannot open message file for reading, exiting program\n";
		return 1;
	}

	// Read from files
	mpz_class eKey, nKey;
	string read;
	getline(in1, read, ',');
	eKey = read;
	getline(in1, read);
	nKey = read;
	getline(in2, read, '.');

	// Clean up files
	in1.close();
	in2.close();

	// Break up message if larger than n
	vector<mpz_class> fullMessage;
	splitMessage(read, nKey, fullMessage);

	// Perform encryption on each section in vector
	for (int i = 0; i<fullMessage.size(); i++) {
		mpz_class c;
		mpz_powm(c.get_mpz_t(), fullMessage[i].get_mpz_t(), eKey.get_mpz_t(), nKey.get_mpz_t());
		fullMessage[i] = c;
	}

	// Pad blocks of encrypted text if necessary
	string s = "";
	for (auto a : fullMessage) {
		if (fullMessage.size() > 1) for (int i = (a.get_str().size()); i < (nKey.get_str().size()); i++) s += '0';
		s += a.get_str();
	}

	// Write to file
	ofstream out;
	out.open("Jacob-Ingraham.ciphertext", ios::out);
	out << s;
	out.close();

	return 0;
}
