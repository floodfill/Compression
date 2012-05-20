//============================================================================
// Name        : DSDncoder.cpp
// Author      : Kan Xiao
// Version     : ver 1.0
// Copyright   :
// Description : Diaosi Encoding Decoder
//============================================================================

/**
 *
 * Bug #1@May14_0256: The decodeArray has a bug that some selectors
 * couldn't be extracted therefore segment error raised(visiting somewhere
 * beyond the array bound.
 *
 * Patch #1@May14_2339: Bug #1 already fixed
 *
 */

#include "DSDecoder.h"

using namespace std;
using namespace opc;

const uint32_t BUFSIZE = 1024 * 1024 * 64; //Buffer size is 64M;
const uint32_t MAXNUM = 30 * BUFSIZE;
FILE *selIn, *datIn, *fout;
//char *bs, *bd;
string prefix;
string outfile;

uint32_t *rBuf;
uint32_t cntOfBuf = 0, sizeOfFilled = 0;
uint32_t *numbers;

uint32_t *sBuf;

uint32_t *wBuf;
uint32_t offset = 0;
uint32_t remain = 0;
uint32_t rBits = 0;

unsigned onStart;
unsigned onEnd;

Simple16 s;

int deMain(int argc, const char *argv[]) {
	checkIllegalPara(argc, argv);
	init();
	decompress();
	dispose();
	showTime();
	return 0;
}

void showTime(void) {
	onEnd = clock();
	double CLOCKS_PER_MILLISEC = CLOCKS_PER_SEC / 1000.0;
	printf("Diaosi Decoder ends in %0.2f seconds\n",
			(onEnd - onStart) / (CLOCKS_PER_MILLISEC * 1000.0));

}

void checkIllegalPara(int argc, const char *argv[]) {
	if (argc < 3) {
		msg();
		exit(0);
	}
	prefix = argv[1];
	outfile = argv[2];
	onStart = clock();
}

inline int _numOfBits(uint32_t num) {
	int count = 0;
	while (num > 0) {
		num >>= 1;
		count++;
	}
	return count;
}

inline uint32_t getNextInt(uint32_t numOBits) {
	uint32_t ret = 0;
	//uint32_t rBits = _numOfBits(remain);
	if (numOBits <= rBits) {
		ret = remain % (1 << numOBits);
		remain >>= numOBits;
		rBits -= numOBits;
	} else {
		ret = remain;
		uint32_t next;
		if (cntOfBuf < sizeOfFilled) {
			next = rBuf[cntOfBuf++];
		} else {
			sizeOfFilled = fread(rBuf, 4, BUFSIZE, datIn);
			cntOfBuf = 0;
			if (sizeOfFilled <= 0) {
				printf("Input data error!");
				exit(-1);
			}
			next = rBuf[cntOfBuf++];
		}
		int nBits = numOBits - rBits;
		uint32_t temp = next % (1 << nBits);
		temp <<= rBits;
		ret = temp + remain;
		remain = next >> nBits;
		rBits = 32 - nBits;
	}
	return ret;
}

inline void _write2disk(uint32_t num) {
	if (offset < BUFSIZE) {
		wBuf[offset++] = num;
	} else {
		uint32_t temp = fwrite(wBuf, 4, offset, fout);
		if (temp != offset)
			throw exception();
		offset = 0;
		wBuf[offset++] = num;
	}
}

inline void refill(uint32_t &count, uint32_t &inc) {
	count = fread(sBuf, 4, BUFSIZE, selIn);
	s.decodeArrayFixed(sBuf, count, numbers, inc);
//	for (int i = 0; i < inc; i++)
//		if (numbers[i] != 0) {
//			printf("%d ", numbers[i]);
//		}
}

void decompress() {
	//decompress all selectors first;
	memset(rBuf, 0, BUFSIZE);
	memset(wBuf, 0, BUFSIZE);
	memset(sBuf, 0, BUFSIZE);
	memset(numbers, 0, MAXNUM);

	sizeOfFilled = fread(rBuf, 4, BUFSIZE, datIn);

	uint32_t count = 0;
	uint32_t inc = 0;
	refill(count, inc);
	if (count <= 0 || inc <= 0)
		return;

	uint64_t p = 0;

	while (true) {
		while (numbers[p] == 0 && p < inc)
			p++;
		if (p >= inc) {
			refill(count, inc);
			if (count <= 0)
				break;
			p = 0;
		}

		uint32_t N = numbers[p++];
		_write2disk(N);

		if (p >= inc) {
			refill(count, inc);
			if (count <= 0)
				break;
			p = 0;
		}

		long last = 0;
		for (uint32_t i = 1; i <= N; i++) {
			while (numbers[p] == 0 && p < inc)
				p++;

			if (p >= inc) {
				refill(count, inc);
				if (count <= 0)
					break;
				p = 0;
			}

			uint32_t sel = numbers[p++];
			uint32_t now = getNextInt(sel);
			_write2disk(last + now);
			last += now;
		}
	}
	//ensure numbers in buffer written
	if (offset > 0)
		fwrite(wBuf, 4, offset, fout);

}

void init() {
	selIn = fopen((prefix + ".inf").c_str(), "rb");
	datIn = fopen((prefix + ".data").c_str(), "rb");
	fout = fopen(outfile.c_str(), "wb");
	if (NULL == selIn || NULL == datIn) {
		msg();
		exit(-1);
	}
	rBuf = new uint32_t[BUFSIZE];
	wBuf = new uint32_t[BUFSIZE];
	sBuf = new uint32_t[BUFSIZE];
	numbers = new uint32_t[MAXNUM];
//
//	bs = new char[BUFSIZE];
//	bd = new char[BUFSIZE];
//	setbuf(selIn, bs);
//	setbuf(datIn, bd);
}

void dispose() {
	fclose(selIn);
	fclose(datIn);
	fflush(fout);
	fclose(fout);
	delete[] rBuf;
	delete[] wBuf;
	delete[] sBuf;
	delete[] numbers;
////	delete[] bs;
}

void msg() {
	printf("DiaoSiEncoding Decoder ver1.0 by Kan Xiao & Xinzhao Liang\n");
	printf("Usage: DSDecoder infilePrefix outfile\n");
	printf(
			"InfilePrefix: prefix of compressed files eg. simple16 ->(simple16.dat & simple16.inf)\n");
	printf("Outfile: output filename\n");
}
