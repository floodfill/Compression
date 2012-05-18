//============================================================================
// Name        : DSEncoder.cpp
// Author      : Kan Xiao
// Version     : ver 1.0
// Copyright   :
// Description : Diaosi Encoding Encoder
//============================================================================

/**
 * The correctness of this program is not verified @ May 13 3:19am
 *
 * And verified @ May 13 6:18pm
 */
#include "DSEncoder.h"

using namespace std;
using namespace opc;

const uint32_t MAXSIZE = 1024 * 1024 * 10; //Write buffer: 10M
FILE *finp, *foutp, *infout;
uint32_t* wBuf, cnt = 0, rem = 0;
uint32_t *buff, *sell;
long cntOfSel = 0;

void showMsg() {
	printf("DiaoSiEncoding Encoder ver1.0 by Kan Xiao & Xinzhao Liang\n");
	printf("Usage: DSEncoder in out\n");
	printf("    In: input data filename\n");
	printf("    Out: output filename -- out.dat & out.inf will be generated");
}

int numOfBits(uint32_t num) {
	int count = 0;
	while (num > 0) {
		num >>= 1;
		count++;
	}
	return count;
}

void write2disk(uint32_t num) {
	int bN = numOfBits(num);
	int bR = numOfBits(rem);
	if (bN + bR > 32) {
		int dif = 32 - bR;
		int temp = num % (1 << dif);
		temp <<= bR;
		temp += rem;
		rem = num >> dif;
		//buffer is full, write out to disk
		if (cnt >= MAXSIZE) {
			fwrite(wBuf, 4, cnt, foutp);
			cnt = 0;
		}
		wBuf[cnt++] = temp;
	} else {
		num <<= bR;
		rem += num;
	}

}

int enMain(int argc, const char *argv[]) {
	if (argc < 3) {
		showMsg();
		exit(0);
	}
	finp = fopen(argv[1], "rb");
	string prefix = argv[2];
	foutp = fopen((prefix + ".data").c_str(), "wb");
	infout = fopen((prefix + ".inf").c_str(), "wb");
	uint64_t fileSize;
	fseek(finp, 0, SEEK_END);
	fileSize = ftell(finp) / 4ul + 28ul;
	fseek(finp, 0, SEEK_SET);
	uint64_t cntONumber = 0;
	int size = 0;
	buff = new uint32_t[fileSize];
	sell = new uint32_t[fileSize];
	wBuf = new uint32_t[MAXSIZE];
	memset(buff, 0, fileSize);
	memset(sell, 0, fileSize);
	cntONumber = fread(buff, 4, fileSize, finp);
	uint64_t p = 0;
//	for (int i = 0; i < fileSize; i++) {
//		printf("%d ", buf[i]);
//	}
	//printf("\n");
	while (p < cntONumber) {
		uint32_t N = buff[p];
		sell[size++] = N;
		//printf("%d\n", sell[size - 1]);
		long last = -1;
		for (uint32_t i = 1; i <= N; i++) {
			int n2write = 0;
			if (last == -1)
				n2write = buff[p + i];
			else
				n2write = buff[p + i] - last;
			sell[size++] = numOfBits(n2write);
			write2disk(n2write);
			//printf("%d %d\n", buff[p + i], n2write);
			last = buff[p + i];
		}
		p += N + 1;
	}
	//flush buffer
	if (cnt > 0 || rem > 0) {
		if (cnt < MAXSIZE) {
			if (rem > 0)
				wBuf[cnt++] = rem;
			fwrite(wBuf, 4, cnt, foutp);
		} else {
			fwrite(wBuf, 4, cnt, foutp);
			cnt = 0;
			if (rem > 0) {
				wBuf[cnt++] = rem;
				fwrite(wBuf, 4, cnt, foutp);
			}
		}
		fflush(foutp);
	}
	memset(buff, 0, fileSize);
	Simple16 s;
	uint32_t n;
//	for (uint32_t i = 0; i < cntONumber; i++)
//		printf("%d ", sell[i]);
	s.encodeArray(sell, cntONumber, buff, n);
//	printf("\n%d\n", n);
	fwrite(buff, 4, n, infout);
	fflush(infout);
	fclose(finp);
	fclose(foutp);
	fclose(infout);
	delete[] buff;
	delete[] sell;
	delete[] wBuf;

	return 0;

}
