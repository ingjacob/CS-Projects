-- README.txt --
-- The contents of this directory are listed below --
-- All .cpp files also include directions for compiling and running --

PartA/encrypt.cpp - Code for symmetric encryption
    Compile with:   g++ encrypt.cpp -o encrypt
    Run with:       ./encrypt
                    The program will request the keys as input, they must be at least 10 characters (excluding duplicates). Extra characters will be ignored.
                    The program will then request the name of the binary file to be encrypted. If the input is not a valid binary file the program will terminate.
                    Resulting ciphertext will be written to Jacob-Ingraham-encrypted-str

PartA/decrypt.cpp - Code for symmetric decryption
    Compile with:   g++ decrypt.cpp -o decrypt
    Run with:       ./decrypt
                    The program will request the keys in the same way as encrypt.cpp. They must match those given to encrypt.cpp exactly.
                    The program will read ciphertext from the binary file Jacob-Ingraham-encrypted-str. Program will terminate if the file cannot be read.
                    Resulting plaintext will be written to Jacob-Ingraham-decrypted-str

PartB/makeKey.cpp - Code to generate RSA keys (currently set up to create size 4096-bit n)
    Compile with:   g++ makeKey.cpp -lgmp -lgmpxx -o makeKey
    Run with:       ./makeKey

PartB/asymmetrickey_encr.cpp - Code for RSA encryption
    Compile with:   g++ asymmetrickey_encr.cpp -lgmp -lgmpxx -o asymmetrickey_encr
    Run with:       ./asymmetricKey_encr <filename1> <filename2>
                    <filename1> is the public key of form e,n. If generated by makeKey.cpp this will be Jacob-Ingraham.publickey
                    <filename2> is the plaintext message to be encrypted. It is in the form of one English sentence terminated by a period. (Program only reads up to '.').
                    Resulting ciphertext will be written to Jacob-Ingraham.ciphertext

PartB/asymmetrickey_decr.cpp - Code for RSA decryption
    Compile with:   g++ asymmetrickey_decr.cpp -lgmp -lgmpxx -o asymmetrickey_decr
    Run with:       ./asymmetricKey_decr <filename1> <filename2>
                    <filename1> is the private key of form d,n. If generated by makeKey.cpp this will be Jacob-Ingraham.privatekey
                    <filename2> is the ciphertext generated by encryption. When generated by asymmetrickey_encr this will be Jacob-Ingraham.ciphertext

PartB/Jacob-Ingraham.publickey - Pre-calculated public key of form e,n (n is 4096 bits)

PartB/Jacob-Ingraham.privatekey - Pre-calculated private key of form d,n (n is 4096 bits)

README.txt - This file
