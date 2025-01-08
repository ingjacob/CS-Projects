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

    // Loop through key (back to front)
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

// Tranposition decryption
string decrypt(int totIndex, string partialEncryption, string key) {
    string matrix[totIndex][10], retVal = "";
    int partialIndex = 0, arr[10] = {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1};

    // Loop by column and determine alphabetic order
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

    // Loop by sorted columns and fill matrix with coded message
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < totIndex; j++) {
            matrix[j][arr[i]] = partialEncryption[partialIndex];
            partialIndex++;
        }
    }

    // Loop by row and extract decoded message
    for (int i = 0; i < totIndex; i++) {
        for (int j = 0; j < 10; j++) {
            retVal += matrix[i][j];
        }
    }
    return retVal;
}

int main() {
    // Create variables
    string k1, k2, partialEncryption = "";
    bitset<10> read;

    // Get keys
    k1 = getKeys("first");
    k2 = getKeys("second");

    // Convert key1 to binary
    bitset<80> bk1(stringToBinary(k1));

    // Open binary files for reading and writing
    ifstream in;
    ofstream out;
    in.open("Jacob-Ingraham-encrypted-str", ios::in | ios::binary);
    out.open("Jacob-Ingraham-decrypted-str", ios::out | ios::binary);
    if (!in) {
        cout << "Cannot open file for reading, exiting program\n";
        return 1;
    }
    if (!out) {
        cout << "Cannot open file for writing, exiting program\n";
        return 1;
    }

    // Read from file
    int totIndex = 0;
    while (in >> read) {
        string fileRead = "";
        for (int i = 0; i < 10; i++) {
            if (read[i]) fileRead+="1";
            else fileRead+="0";
        }

        // Reverse read before adding to string
        reverse(fileRead.begin(), fileRead.end());
        partialEncryption += fileRead;
        totIndex += 1;
    }

    // Reverse the transposition
    partialEncryption = decrypt(totIndex, partialEncryption, k2);
    partialEncryption = decrypt(totIndex, partialEncryption, k1);

    // XOR Decryption
    int keyIndex = 0;
    string dString = "";
    for (int i = 0; i < partialEncryption.size(); i += 10) {
        string next = partialEncryption.substr(i, 10), temporary;
        bitset<10> temp(next);

        for (int j = 0; j < 10; j++) {
            if (temp[j] ^ bk1[keyIndex+j]) temporary+='1';
            else temporary+='0';
        }

        // Reverse before adding to string
        reverse(temporary.begin(), temporary.end());
        dString+=temporary;
        keyIndex = (keyIndex + 10) % 80;
    }

    // Write to file
    for (int outIndex = 0; outIndex < dString.size(); outIndex += 10) {
        string partialOut = dString.substr(outIndex, 10);
        bitset<10> retVal(partialOut);
        out << retVal;
    }

    // Clean up files
    in.close();
    out.close();

    return 0;
}
