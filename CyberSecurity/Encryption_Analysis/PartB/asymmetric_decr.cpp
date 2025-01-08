#include <iostream>
#include <gmpxx.h>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>

using namespace std;

// Function to split message
void splitMessage(string message, mpz_class size, vector<mpz_class>& retVal) {
	// Loop through message, split into blocks of size n if necessary
	for (int i = 0; i<message.size(); i+=((size.get_str()).size())) {
		string temp = message.substr(i, (size.get_str()).size());

		// Remove leading zeros (padding)
		while (temp[0] == '0') temp.erase(0, 1);

		// Add to vector
		mpz_class tempI(temp);
		retVal.push_back(tempI);
	}
}

int main(int argc, char *argv[]) {
	// Check for two files as input
	if (argc != 3) {
		cout << "Incorrect number of arguments\n";
		return 1;
	}
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
	mpz_class dKey, nKey;
	string read;
	getline(in1, read, ',');
	dKey = read;
	getline(in1, read);
	nKey = read;
	getline(in2, read, '.');

	// Clean up files
	in1.close();
	in2.close();

	// Break up message into blocks
	vector<mpz_class> fullMessage;
	splitMessage(read, nKey, fullMessage);

	// Perform decryption
	for (int i = 0; i<fullMessage.size(); i++) {
		mpz_class m;
		mpz_powm(m.get_mpz_t(), fullMessage[i].get_mpz_t(), dKey.get_mpz_t(), nKey.get_mpz_t());
		fullMessage[i] = m;
	}

	// Convert message back to string
	// String to hold hexadecimal digits
	string digits = "0123456789abcdef";
	string dMessage;

	// Loop through blocks in vector
	for (auto m : fullMessage) {
		string hexString;

		// Convert hex to string of hex characters until block is <= 0
		while (m > 0) {
			mpz_class digit = m % 16;
			hexString += digits[digit.get_ui()];
			m = m / 16;
		}

		// Reverse string before reading hex
		reverse(hexString.begin(), hexString.end());

		// Loop through hexString and get characters from hex values
		for (int i = 0; i<hexString.size(); i+=2) {
			string temp = hexString.substr(i, 2);
			char c = stoul(temp, nullptr, 16);
			dMessage += c;
		}
	}

	// Write to file
	ofstream out;
	out.open("Jacob-Ingraham.plaintext", ios::out);
	out << dMessage << '.';
	out.close();

	return 0;
}
