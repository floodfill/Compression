//============================================================================
// Name        : DSDncoder.cpp
// Author      : Kan Xiao
// Version     : ver 1.0
// Copyright   :
// Description : Diaosi Encoding Decoder
//============================================================================

/**
 * Time: May 14, 0256am
 * Bug #1: Simple16.cpp 的decoderArray存在问题，不能全部被解压selectors，导致segment error(vector 越界 ）
 *
 */

#include "DSDecoder.h"

using namespace std;
using namespace opc;

const uint32_t BUFSIZE = 100;
const uint32_t MAXNUM = 30 * BUFSIZE;
FILE *selIn, *datIn, *fout;
//char *bs, *bd;
string prefix;
string outfile;

uint32_t *buf;
uint32_t cntOfBuf = 0, sizeOfFilled = 0;
vector<uint32_t> sels;
uint32_t numbers[MAXNUM];
uint32_t offset = 0;
uint32_t remain = 0;
uint32_t rBits = 0;
time_t onStart, onEnd;

int deMain(int argc, const char *argv[]) {
	checkIllegalPara(argc, argv);
	init();
	decompress();
	dispose();
	showTime();
	return 0;
}

void showTime(void) {
	time(&onEnd);
	double dif = difftime(onEnd, onStart);
	printf("Diaosi Coding Decoder ends in %0.2f seconds\n", dif);

}

void checkIllegalPara(int argc, const char *argv[]) {
	cout << sels.max_size() << endl;
	if (argc < 3) {
		msg();
		exit(0);
	}
	prefix = argv[1];
	outfile = argv[2];
	time(&onStart);
}

int _numOfBits(uint32_t num) {
	int count = 0;
	while (num > 0) {
		num >>= 1;
		count++;
	}
	return count;
}

uint32_t getNextInt(uint32_t numOBits) {
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
			next = buf[cntOfBuf++];
		} else {
			sizeOfFilled = fread(buf, 4, BUFSIZE, datIn);
			cntOfBuf = 0;
			if (sizeOfFilled <= 0) {
				printf("Input data error!");
				exit(-1);
			}
			next = buf[cntOfBuf++];
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

void _write2disk(uint32_t num) {
	if (offset < MAXNUM) {
		numbers[offset++] = num;
	} else {
		uint32_t temp = fwrite(numbers, 4, offset, fout);
		if (temp != offset)
			throw exception();
		offset = 0;
		numbers[offset++] = num;
	}
}

void decompress() {
	//decompress all selectors first;
	Simple16 s;
	while (!feof(selIn)) {
		memset(numbers, 0, MAXNUM);
		uint32_t count = fread(buf, 4, BUFSIZE, selIn);
		s.decodeArray(buf, count, numbers, count);
		for (uint32_t i = 0; i < BUFSIZE; i++) {
			//uint32_t tt = numbers[i];
			if (numbers[i] != 0) {
				sels.push_back(numbers[i]);
			} else
				break;
		}
	}
	for (int i = 0; i < BUFSIZE; i++)
		if (numbers[i] != 0)
			printf("%d\n", numbers[i]);
		else
			break;
	memset(buf, 0, BUFSIZE);
	sizeOfFilled = fread(buf, 4, BUFSIZE, datIn);
	memset(numbers, 0, MAXNUM);

	uint64_t p = 0;
	while (p < sels.size()) {
		uint32_t N = sels.at(p);
		_write2disk(N);
		long last = -1;
		for (uint32_t i = 1; i <= N; i++) {
			uint32_t sel = sels.at(p + i);
			uint32_t now = getNextInt(sel);
			if (last == -1) {
				_write2disk(now);
				last = now;
			} else {
				_write2disk(last + now);
				last += now;
			}
		}
		p += N + 1;
	}
	//ensure numbers in buffer written
	if (offset > 0)
		fwrite(numbers, 4, offset, fout);

}

void init() {
	selIn = fopen((prefix + ".inf").c_str(), "rb");
	datIn = fopen((prefix + ".data").c_str(), "rb");
	fout = fopen(outfile.c_str(), "wb");
	if (NULL == selIn || NULL == datIn) {
		msg();
		exit(-1);
	}
	buf = new uint32_t[BUFSIZE];
	//numbers = new uint32_t[MAXNUM];
//
//	bs = new char[BUFSIZE];
//	bd = new char[BUFSIZE];
//	setbuf(selIn, bs);
//	setbuf(datIn, bd);
}

void dispose() {
	fclose(selIn);
	fclose(datIn);
	fclose(fout);
	delete[] buf;
	//delete[] numbers;
////	delete[] bs;
////	delete[] bd;
//	delete sels;
}

void msg() {
	printf("DiaoSiEncoding Decoder ver1.0 by Kan Xiao & Xinzhao Liang\n");
	printf("Usage: DSDecoder infilePrefix outfile\n");
	printf(
			"InfilePrefix: prefix of compressed files eg. simple16 ->(simple16.dat & simple16.inf)\n");
	printf("Outfile: output filename");
}
