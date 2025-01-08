#include <iostream>
#include <gmpxx.h>
#include <string>
#include <fstream>

using namespace std;

// Simple recursive function to get greatest common divisor
mpz_class gcd(mpz_class i, mpz_class j) {
	if (i==0) return j;
	if (j==0) return i;
	if (i==j) return i;
	if (i>j) return gcd(i-j, j);
	return gcd(i, j-i);
}

// Extended Euclidean algorithm to get modular inverse
mpz_class ext(mpz_class i, mpz_class j, mpz_class& x, mpz_class& y) {
	if (j==0) {
		x = 1;
		y = 0;
		return i;
	}
	mpz_class x1, y1;
	mpz_class d = ext(j, i%j, x1, y1);
	x = y1;
	y = x1 - y1 * (i/j);
	return d;
}

// Find random e which is coprime to phi (argument name: i)
mpz_class findE(mpz_class i) {
	srand((unsigned) time(NULL));
	while (true) {
		// If random number has gcd of 1 with phi return it
		mpz_class random = 1 + (rand() % i);
		if (gcd(random, i)==1) return random;
	}
}

// Check potential prime against array of first primes
bool divisionTest(mpz_class p) {
	// Array to hold first primes
	int firstPrimes[] = {2,3,5,7,11,13,17,19,23,29,31,37,41,43,47,53,59,61,67,
						71,73,79,83,89,97,101,103,107,109,113,127,131,137,
						139,149,151,157,163,167,173,179,181,191,193,197};
	// Loop through primes, if p divisible by any return false
	for (int i : firstPrimes) if (p % i == 0) return false;

	// Return true else
	return true;
}

// Fermat test of primality
bool Fermat(mpz_class p, int iterations) {
	// Corner case
	if (p == 1) return false;

	// Set up gmp random number generator
	gmp_randclass g(gmp_randinit_default);
	g.seed(time(NULL));

	// Loop for number of iterations
	for (int i = 0; i<iterations; i++) {
		mpz_class a = (g.get_z_range(p-4) + 2);
		mpz_class b;
		mpz_class temp = p-1;
		mpz_powm(b.get_mpz_t(), a.get_mpz_t(), temp.get_mpz_t(), p.get_mpz_t());
		// If modular exponent not 1, prime = false
		if (b != 1) return false;
	}

	// Return true if no iteration finds the number nonprime
	return true;
}

// Miller-Rabin test of primality
bool Miller(mpz_class p, int iterations) {
	// Corner cases
	if (p < 2) return false;
	if (p != 2 && p % 2 == 0) return false;

	// Set up b (divide p-1 until it is odd)
	mpz_class b = p-1;
	while (b % 2 == 0) b /= 2;

	// Set up gmp random number generator
	gmp_randclass g(gmp_randinit_default);
	g.seed(time(NULL));

	// Loop for number of iterations
	for (int i = 0; i < iterations; i++) {
		mpz_class a = g.get_z_range(p-4) + 2;
		mpz_class temp = b;
		mpz_class mod;
		mpz_powm(mod.get_mpz_t(), a.get_mpz_t(), temp.get_mpz_t(), p.get_mpz_t());
		// Check two conditions: (a**temp) % n == 1 ----and---- (a**((2**r) * d)) == n-1
		// Loop to test for successive values of r in second condition
		while ((temp != p-1) && (mod != 1) && (mod != p-1)) {
			mod = (mod * mod) % p;
			temp *= 2;
		}
		if ((mod != p-1) && (temp%2 == 0)) return false;
	}

	// Return true if no iteration finds the number nonprime
	return true;
}

// Function which generates a large number and calls the check functions in order
mpz_class genByBits(mpz_class b) {
	// Set up variables and gmp random number generator
	mpz_class retVal;
	bool exit = false;
	gmp_randclass g(gmp_randinit_default);
	g.seed(time(NULL));

	// Loop until high probability prime is found
	while (exit == false) {
		// Get random
		retVal = g.get_z_bits(b);

		// Try again and save computation if nonprime found easily
		if (divisionTest(retVal)) exit = true;

		// Else call Fermat
		if (exit == true && Fermat(retVal, 30))
			;
		else exit = false;

		// If still possible prime call Miller
		if (exit == true && Miller(retVal, 40)) 
			;
		else exit = false;
	}

	// Return high probability prime
	return retVal;
}

int main() {
	// Generate primes
	mpz_class p = genByBits(2048), q = genByBits(2048);

	// Perform computations
	mpz_class n = p*q;
	mpz_class phi = ((p-1)*(q-1));
	mpz_class e = findE(phi);
	mpz_class d, x;
	ext(e, phi, d, x);

	// For negative d values
	d = (d % phi + phi) % phi;

	// Write to files
	ofstream pub, priv;
	pub.open("Jacob-Ingraham.publickey", ios::out);
	priv.open("Jacob-Ingraham.privatekey", ios::out);
	pub << e << ',' << n;
	priv << d << ',' << n;
	pub.close();
	priv.close();

	return 0;
}
