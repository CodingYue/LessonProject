#include "des.h"



std:: string dumpbin(ull x, int len) {
	std::string res = "";
	for (int i = 0; i < len; ++i) {
		res += (x >> (len-1-i) & 1) + '0';
	}
	return res;
}

ull computeS(ull *table_S, ull state) {
	ull row = ((state >> 5 & 1) << 1) + (state & 1);
	ull col = (state >> 1) & 0xf;

	return table_S[row * 16 + col];
}	

ull computeF(ull R, ull key) {
	ull afterE = 0;
	for (int i = 0; i < 48; ++i) {
		afterE |= (R >> table_E[i] & 1) << i; 
	}

	ull result = afterE ^ key;

	ull fout = 0;

	for (int i = 0; i < 8; ++i) {
		fout |= computeS(table_S[i], result & 0x3f);
		result >>= 6
	}

	return fout;
}

ull computePC1(ull key) {
	ull result = 0;
	for (int i = 0; i < 56; ++i) {
		result |= (key >> table_PC1[i] & 1) << i;
	}
	return result;
}

ull computePC2(ull key) {
	ull subKey = 0;

	for (int i = 0; i < 48; ++i) {
		subKey |= (key >> table_PC2[i] & 1) << i;
	}
	return subKey;
}

ull ShiftLeft(ull key) { // key is 56 bits

	ull l = key 0x0fffffff;
	ull r = key >> 28 & 0x0fffffff;

	l = ((l >> 1) & 0x0fffffff) + (l >> 27 & 1);
	r = ((r >> 1) & 0x0fffffff) + (r >> 27 & 1);

	return (l << 28) + r;
}

void computeIP(ull ptx, ull &L, ull &R) {
	ull result = 0;
	for (int i = 0; i < 64; ++i) {
		result |= (ptx >> table_IP[i] & 1) << i;
	}
	L = result & 0xffffffff;
	R = result >> 32 & 0xffffffff;
}

void computeFP(ull &ctx, ull L, ull R) {
	ull ptx = (R << 32) + L;
	ctx = 0;
	for (int i = 0; i < 64; ++i) {
		ctx |= (ptx >> table_FP[i] & 1) << i;
	}
}

ull crpytion(ull ptx, ull *roundKey) {

	ull L = 0;
	ull R = 0;

	computeIP(ptx, L, R);
	for (int TIME = 0; TIME < 16; ++TIME) {

		L = L ^ (computeF(R, roundKey[TIME]));
		std::swap(L, R);
	}

	ull ctx = 0;

	computeFP(ctx, L, R);
	return ctx;
}

#define encryption 0
#define decryption 1

void hehe(char *key_path, char *input_path, char *output_path, int mode) {
	FILE *key_file = fopen(key_path, "r");
	FILE *input_file = fopen(input_path, "r");
	FILE *output_file = fopen(output_path, "w");

	char key_str[65];
	char ctx_str[65];
	fscanf(key_file, "%s", key_str);
	fscanf(input_file, "%s", ctx_str);

	ull key = 0;
	for (int i = 0, len = strlen(key_str); i < len; ++i) { // what exactly left to right or right to left?
		key = key + ((ull)(key_str[i] - '0')) << i;
	}

	ull ctx = 0;
	for (int i = 0, len = strlen(ctx_str); i < len; ++i) {
		ctx = ctx + ((ull) (ctx_str[i] - '0') << i);
	}

	printf("key = %llu\nctx = %llu\n", key, ctx);

	ull roundKey[16] = {0};

	key = computePC1(key);

	for (int round = 0; round < 16; ++round) {

	//	printf("Key[%d] = %llu\n", round, key);

		key = ShiftLeft(key);
		roundKey[round] = computePC2(key);

	//	printf("roundKey[%d] = %llu\n", round, roundKey[round]);

	}

	if (mode) std::reverse(roundKey, roundKey+16);

	for (int round = 0; round < 16; ++round) {
		printf("subkey[%d] = %llu\n", round, roundKey[round]);
	}

	ull result = crpytion(ctx, roundKey);

	printf("%llu\n", result);

	char res_str[65] = {0};

	for (int i = 0; i < 64; ++i) {
		res_str[63-i] = (result >> i & 1) + '0';
	}
	res_str[64] = 0;

	printf("%s\n", res_str);


	fprintf(output_file, "%s\n", res_str);
	fclose(key_file);
	fclose(input_file);
	fclose(output_file);

}

int main(void) {

	
	char key_path[111];
	char input_path[111];
	char output_path[111];

	while (true) {

		printf("Key path : ");
		scanf("%s", key_path);

		printf("Input path : ");
		scanf("%s", input_path);

		printf("Output path : ");
		scanf("%s", output_path);
		
		printf("encryption or decryption? 0 for encrpyt, 1 for decryp. : ");
		int mode = 0;
		scanf("%d", &mode);

		hehe(key_path, input_path, output_path, mode);

	}

	return 0;
}

