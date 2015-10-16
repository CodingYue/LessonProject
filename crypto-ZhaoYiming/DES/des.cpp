/*
 *	Author : Yang Yue
 *	DES algorithm, CBC mode.
 */

#include "des.h"

const int table_S[8][64] = {
/*
 *	S Box Table
 *	(row, col) values is located in (row * 16 + col)
 *	It's a magical table, I cannot figure out why it makes whole process inversible.
 */

	// S0
	{14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7,
		0, 15, 7, 4, 14, 2, 13, 1, 10, 6, 12, 11, 9, 5, 3, 8,
		4, 1, 14, 8, 13, 6, 2, 11, 15, 12, 9, 7, 3, 10, 5, 0,
		15, 12, 8, 2, 4, 9, 1, 7, 5, 11, 3, 14, 10, 0, 6, 13},

	// S1
	{15, 1, 8, 14, 6, 11, 3, 4, 9, 7, 2, 13, 12, 0, 5, 10,
		3, 13, 4, 7, 15, 2, 8, 14, 12, 0, 1, 10, 6, 9, 11, 5,
		0, 14, 7, 11, 10, 4, 13, 1, 5, 8, 12, 6, 9, 3, 2, 15,
		13, 8, 10, 1, 3, 15, 4, 2, 11, 6, 7, 12, 0, 5, 14, 9},

	// S2
	{10, 0, 9, 14, 6, 3, 15, 5, 1, 13, 12, 7, 11, 4, 2, 8,
		13, 7, 0, 9, 3, 4, 6, 10, 2, 8, 5, 14, 12, 11, 15, 1,
		13, 6, 4, 9, 8, 15, 3, 0, 11, 1, 2, 12, 5, 10, 14, 7,
		1, 10, 13, 0, 6, 9, 8, 7, 4, 15, 14, 3, 11, 5, 2, 12},

	// S3
	{7, 13, 14, 3, 0, 6, 9, 10, 1, 2, 8, 5, 11, 12, 4, 15,
		13, 8, 11, 5, 6, 15, 0, 3, 4, 7, 2, 12, 1, 10, 14, 9,
		10, 6, 9, 0, 12, 11, 7, 13, 15, 1, 3, 14, 5, 2, 8, 4,
		3, 15, 0, 6, 10, 1, 13, 8, 9, 4, 5, 11, 12, 7, 2, 14},

	// S4
	{2, 12, 4, 1, 7, 10, 11, 6, 8, 5, 3, 15, 13, 0, 14, 9,
		14, 11, 2, 12, 4, 7, 13, 1, 5, 0, 15, 10, 3, 9, 8, 6,
		4, 2, 1, 11, 10, 13, 7, 8, 15, 9, 12, 5, 6, 3, 0, 14,
		11, 8, 12, 7, 1, 14, 2, 13, 6, 15, 0, 9, 10, 4, 5, 3},

	// S5
	{12, 1, 10, 15, 9, 2, 6, 8, 0, 13, 3, 4, 14, 7, 5, 11,
		10, 15, 4, 2, 7, 12, 9, 5, 6, 1, 13, 14, 0, 11, 3, 8,
		9, 14, 15, 5, 2, 8, 12, 3, 7, 0, 4, 10, 1, 13, 11, 6,
		4, 3, 2, 12, 9, 5, 15, 10, 11, 14, 1, 7, 6, 0, 8, 13},

	// S6
	{4, 11, 2, 14, 15, 0, 8, 13, 3, 12, 9, 7, 5, 10, 6, 1,
		13, 0, 11, 7, 4, 9, 1, 10, 14, 3, 5, 12, 2, 15, 8, 6,
		1, 4, 11, 13, 12, 3, 7, 14, 10, 15, 6, 8, 0, 5, 9, 2,
		6, 11, 13, 8, 1, 4, 10, 7, 9, 5, 0, 15, 14, 2, 3, 12},

	// S7
	{13, 2, 8, 4, 6, 15, 11, 1, 10, 9, 3, 14, 5, 0, 12, 7,
		1, 15, 13, 8, 10, 3, 7, 4, 12, 5, 6, 11, 0, 14, 9, 2,
		7, 11, 4, 1, 9, 12, 14, 2, 0, 6, 10, 13, 15, 3, 5, 8,
		2, 1, 14, 7, 4, 10, 8, 13, 15, 12, 9, 0, 3, 5, 6, 11},
};


const int table_FP[64] = {
/*
 * FP: output bits i equals to input table_FP i
 */
    39,  7, 47, 15, 55, 23, 63, 31,
    38,  6, 46, 14, 54, 22, 62, 30,
    37,  5, 45, 13, 53, 21, 61, 29,
    36,  4, 44, 12, 52, 20, 60, 28,
    35,  3, 43, 11, 51, 19, 59, 27,
    34,  2, 42, 10, 50, 18, 58, 26,
    33,  1, 41,  9, 49, 17, 57, 25,
    32,  0, 40,  8, 48, 16, 56, 24
};

const int table_IP[64] = {
/*
 *  IP: output bits i equals input table_IP i
 */
    57, 49, 41, 33, 25, 17,  9,  1,
    59, 51, 43, 35, 27, 19, 11,  3,
    61, 53, 45, 37, 29, 21, 13,  5,
    63, 55, 47, 39, 31, 23, 15,  7,
    56, 48, 40, 32, 24, 16,  8,  0,
    58, 50, 42, 34, 26, 18, 10,  2,
    60, 52, 44, 36, 28, 20, 12,  4,
    62, 54, 46, 38, 30, 22, 14,  6
};

const int table_PC1[] = {
/*
 *  PC1: Permutation choice 1, used to pre-process the key
 */
	56, 48, 40, 32, 24, 16,  8,
	0, 57, 49, 41, 33, 25, 17,
	9,  1, 58, 50, 42, 34, 26,
	18, 10,  2, 59, 51, 43, 35
};

/*
 *  PC2: Map 56-bit round key to a 48-bit subkey
 */
const int table_PC2[] = {
	62, 54, 46, 38, 30, 22, 14,
	6, 61, 53, 45, 37, 29, 21,
	13,  5, 60, 52, 44, 36, 28,
	20, 12,  4, 27, 19, 11,  3
};


/*
 *  E: Expand 32-bit R to 48 bits.
 */
const int table_E[48] = {
    31,  0,  1,  2,  3,  4,  3,  4,
     5,  6,  7,  8,  7,  8,  9, 10,
    11, 12, 11, 12, 13, 14, 15, 16,
    15, 16, 17, 18, 19, 20, 19, 20,
    21, 22, 23, 24, 23, 24, 25, 26,
    27, 28, 27, 28, 29, 30, 31,  0
};


/*
 *  P: Permutation of S table outputs
 */
const int table_P[32] = {
	15, 6, 19, 20, 28, 11, 27, 16,
	0, 14, 22, 25, 4, 17, 30, 9,
	1, 7, 23, 13, 31, 26, 2, 8,
	18,12, 29, 5, 21, 10, 3, 24
};

ull boolstr2ull(bool *input, int len) { 

	/*
	 *	convert bool string to unsigned long long
	 *	lower index means higher power
	 */

	ull res = 0;
	for (int i = 0; i < len; ++i) {
		res = res * 2 + input[i];
	}
	return res;
}

void ull2boolstr(ull mask, bool *output, int len) {

	/*
	 *	convert unsigned long long to bool string
	 *	higher power mapping to lower index
	 */

	for (int i = 0; i < len; ++i) {
		output[i] = mask >> (len-i-1) & 1;
	}
}

ull compute_s_box(const int *s, bool *input) {

	/*
	 *	get the value of coordinate(row, col);
	 *	row = input[0]input[1]
	 *	col = input[1-4]
	 */

	int row = (input[0] << 1) + input[5];
	int col = boolstr2ull(input+1, 4);

	return s[row * 16 + col];
}

ull computeF(ull R, ull subKey) { 

	/*
	 *	function F(Ri, Ki)
	 *	Ri 32 bits, Ki 48 bits
	 */

	bool input[32] = {0};
	bool key[48] = {0};
	ull2boolstr(R, input, 32);
	ull2boolstr(subKey, key, 48);

	bool mask[48] = {0};

	for (int i = 0; i < 48; ++i) {
		mask[i] = input[table_E[i]] ^ key[i]; 
	}

	bool sout[32] = {0};
	for (int i = 0; i < 8; ++i) {
		
		/*
		 *	compute S box
		 */

		ull2boolstr(compute_s_box(table_S[i], mask + 6*i), sout+4*i, 4);
	}

	/*
	 *	Permutaion bit string after S Box
	 */
	bool fout[32] = {0};

	for (int i = 0; i < 32; ++i) {
		fout[i] = sout[table_P[i]];
	}
	return boolstr2ull(fout, 32);
}

ull computePC1(ull key) {

	/*
	 *	select 56 bits subkey from 64 bits key
	 */

	bool input[64] = {0};
	bool output[56] = {0};

	ull2boolstr(key, input, 64);

	for (int i = 0; i < 56; ++i) {
		output[i] = input[table_PC1[i]];
	}
	return boolstr2ull(output, 56);
}

ull computePC2(ull key) { 

	/*
	 *	select 48 bits with permutating from original key
	 */

	bool input[56] = {0};
	bool output[48] = {0};

	ull2boolstr(key, input, 56);
	
	for (int i = 0; i < 48; ++i) {
		output[i] = input[table_PC2[i]];
	}
	return boolstr2ull(output, 48);
}

ull shift_left(ull key) { 
	/*
	 *	cyclic shift left first 28 bits.
	 *	cyclic shift left last 28 bits.
	 *	because higher power means lower index, 
	 *	'<<' is needed.
	 *	& 0x0fffffff means take only last 28 bits.
	 *  x >> 27 & 1 means get the 27-bit value from x.
	 */
	ull l = key >> 28 & 0x0fffffff;
	ull r = key & 0x0fffffff;

	l = (l << 1 & 0x0fffffff) + (l >> 27 & 1);
	r = (r << 1 & 0x0fffffff) + (r >> 27 & 1);
	return (l << 28) + r;
}

void computeIP(ull context, ull &L, ull &R) {

	/*
	 *	Perform Initital Permutation to inblock
	 *	L: first 32bits
	 *	R: last 32bits.
	 */

	bool input[64] = {0};
	ull2boolstr(context, input, 64);

	bool output[64] = {0};
	for (int i = 0; i < 64; ++i) {
		output[i] = input[table_IP[i]];
	}

	L = boolstr2ull(output, 32);
	R = boolstr2ull(output+32, 32);
}

void computeFP(ull &cipher, ull L, ull R) {
	
	/*
	 *	Perform Final Permutation of input LR
	 */

	bool input[64] = {0};
	ull2boolstr(L, input, 32);
	ull2boolstr(R, input+32, 32);

	bool output[64] = {0};
	for (int i = 0; i < 64; ++i) {
		output[i] = input[table_FP[i]];
	}
	cipher = boolstr2ull(output, 64);
}

ull cryption(ull inblock, ull *roundKey) {

	/*
	 *	calculate outblock with roundKey[] and inblock
	 */

	ull L = 0;
	ull R = 0;
	computeIP(inblock, L, R);
	for (int TIME = 0; TIME < maxRound; ++TIME) {
		L ^= computeF(R, roundKey[TIME]);
		std::swap(L, R);
	}

	/*
	 *	for Inversible progress, canceling last swap process is needed.
	 */

	std::swap(L, R);

	ull outblock = 0;
	computeFP(outblock, L, R);
	return outblock;
}


ull block_cipher(ull inblock, ull key, int mode) {

	/*
	 *	Main process to generate outblock with inblock, key, and mode
	 *	mode switch to encryption / decryption which were defined in des.h
	 *	Return outblock, block_cipher is over.
	 */

	ull roundKey[maxRound] = {0};
	key = computePC1(key);

	/*
	 *	Perform the process maxRound times.
	 *	maxRound was defined in des.h
	 */

	for (int TIME = 0; TIME < maxRound; ++TIME) {
		key = shift_left(key);
		roundKey[TIME] = computePC2(key);
	}

	if (mode == decryption) {
		/*
		 *	if mode is decryption, reversing the roundKey to make process inverse is necessary.
		 */
		std::reverse(roundKey, roundKey + maxRound);
	}
	//for (int TIME = 0; TIME < maxRound; ++TIME) printf("%llu\n", roundKey[TIME]);

	return cryption(inblock, roundKey);
}

int getblocks(char *input, ull *block) {
	int len = strlen(input);
	assert(len % 8 == 0);
	for (int i = 0; i < len; i += 8) {
		block[i/8] = 0;
		for (int j = i; j < i+8; ++j) {
			block[i/8] = (block[i/8] << 8) + input[j];
		}
	}
	return len / 8;
}

const ull IV = 4071733403362795336ull;

void CBC_encryption(ull *inblock, ull *outblock, ull key, int block_size) {
	ull now = IV;
	for (int i = 0; i < block_size; ++i) {
		outblock[i] = block_cipher(inblock[i] ^ now, key, encryption);
		now = outblock[i];
	}
}

void CBC_decryption(ull *inblock, ull *outblock, ull key, int block_size) {
	for (int i = block_size-1; i >= 0; --i) {
		ull now = 0;
		if (i) {
			now = outblock[i-1]; 
		} else {
			now = IV;
		}
		inblock[i] = block_cipher(outblock[i], key, decryption) ^ now;
	}
}

char input[MAX_CONTEXT_LEN];
char output[MAX_CONTEXT_LEN];
ull blocks[2][MAX_CONTEXT_LEN/8];

int main(void) {


	ull *in = blocks[0], *ou = blocks[1];;

	ull key = 0;
	int mode = 0;
	gets(input);
	scanf(" %llu %d", &key, &mode);

	int block_size = getblocks(input, in);

	if (mode == encryption) {
		CBC_encryption(in, ou, key, block_size);
	} else {
		CBC_decryption(ou, in, key, block_size);
	}

	for (int i = 0; i < block_size; ++i) {
		for (int j = 0; j < 8; ++j) {
			output[i*8+j] = (char) (ou[i] >> (7-j) & 0xff);
		}
	}

	return 0;
}
