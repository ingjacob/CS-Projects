/*
    encrypt.cpp
    Performs a symmetric encryption on a binary file using two 10 character keys
    Encryption is performed as a bit-wise XOR (using the first key), followed by block transposition (once with each key)
*/

#include <iostream>
#include <string>
#include <fstream>
#include <bitset>
#include <algorithm>

using namespace std;

// Function to get keys
string getKeys(string whichKey) {
    string temp, retVal;

    // Loop until key is large enough
    while (retVal.length() < 10) {
        retVal = "";
        cout << "Enter the " << whichKey << " key (10 characters a-z, no duplicates): ";
        cin >> temp;

        // Loop to remove duplicates
        for (int i = 0; i<temp.length(); i++) {
            bool dup = false;
            for (int j = 0; j<retVal.length(); j++) {
                if (tolower(temp[i]) == retVal[j]) dup = true;
            }
            if (!dup && (retVal.length() < 10)) retVal += tolower(temp[i]);
        }
        if (retVal.length() < 10) cout << "Key too short, re-enter\n";
    }

    return retVal;
}

// Function to convert keys to binary
string stringToBinary(string key) {
    string retVal = "";

    // Loop through key by character (back to front)
    for (int i = 9; i >= 0; i--) {
        int value = int(key[i]);
        while (value > 0) {
            (value % 2)? retVal.insert(0,"1") : retVal.insert(0,"0");
            value /= 2;
        }
        retVal.insert(0,"0");
    }
    return retVal;
}

// Function to implement transposition encryption algorithm
string transposition(int totIndex, string partialEncryption, string key) {
    string matrix[totIndex][10], retVal = "";
    int partialIndex = 0, arr[10] = {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1};

    // Loop through by row and fill matrix with message
    for (int i = 0; i < totIndex; i++) {
        for (int j = 0; j < 10; j++) {
            matrix[i][j] = partialEncryption[partialIndex];
            partialIndex++;
        }
    }

    // Loop through by column and determine alphabetic order of key
    for (int i = 0; i < 10; i++) {
        char min = '{';
        int index = -1;
        for (int j = 0; j < 10; j++) {
            if ((key[j] < min) && ((find(begin(arr), end(arr), j)) == end(arr))) {
                index = j;
                min = key[j];
            }
        }
        arr[i] = index;
    }

    // Loop through by sorted columns and extract coded message
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < totIndex; j++) {
            retVal += matrix[j][arr[i]];
        }
    }
    return retVal;
}

int main() {
    // Create variables
    string k1, k2, filename;
    bitset<10> read;

    // Get keys
    k1 = getKeys("first");
    k2 = getKeys("second");

    // Convert key1 to binary
    bitset<80> bk1(stringToBinary(k1));

    // Get filename
    //cout << "Please enter the name of the binary file to be encrypted: ";
    //cin >> filename;
    filename = "binaryFile.bin";

    // Open binary files for reading and writing
    ifstream in;
    ofstream out;
    in.open(filename, ios::in | ios::binary);
    out.open("Jacob-Ingraham-encrypted-str", ios::out | ios::binary);
    if (!in) {
        cout << "Cannot open file for reading, exiting program\n";
        return 1;
    }
    if (!out) {
        cout << "Cannot open file for writing, exiting program\n";
        return 1;
    }

    // Loop through contents of file and encrypt with Vigenere
    int keyIndex = 0, totIndex = 0;
    string partialEncryption = "";
    while (in >> read) {
        // Perform XOR
        string fileRead = "";
        for (int i = 0; i < 10; i++) {
            if (read[i] ^ bk1[keyIndex+i]) fileRead+="1";
            else fileRead+="0";
        }

        // Reverse read before adding to string (bitset indexes right to left)
        reverse(fileRead.begin(), fileRead.end());
        partialEncryption += fileRead;

        totIndex += 1;
        keyIndex = (totIndex*10) % 80;
    }

    // Call function to create matrix and do transposition
    partialEncryption = transposition(totIndex, partialEncryption, k1);
    partialEncryption = transposition(totIndex, partialEncryption, k2);

    // Write to file
    for (int outIndex = 0; outIndex < partialEncryption.size(); outIndex += 10) {
        string partialOut = partialEncryption.substr(outIndex, 10);
        bitset<10> retVal(partialOut);
        out << retVal;
    }

    // Clean up files
    in.close();
    out.close();

    return 0;
}
