#include <stdio.h>
#include <errno.h>
#define BUF_SIZE 256

int main(int argc, char* argv[]) {

	FILE* inFile, * outFile;
	char rec[BUF_SIZE];
	size_t bytesIn, bytesOut;

	if (argc != 3) {
		printf("Usage: cp file1 file2 \n");
		return 1;
	}

	inFile = fopen(argv[1], "rb");
	if (inFile == NULL) {
		perror(argv[1]);
		return 2;
	}

	outFile = fopen(argv[2], "wb");
	if (outFile == NULL) {
		perror(argv[2]);
		return 3;
	}

	while ((bytesIn = fread(rec, 1, BUF_SIZE, inFile)) > 0) {
		
		bytesOut = fwrite(rec, 1, bytesIn, outFile);
		if (bytesOut != bytesIn) {

			perror("Fatal write error.");
			return 4;
		}
	}

	fclose(inFile);
	fclose(outFile);

	return 0;
}