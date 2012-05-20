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

const uint32_t MAXSIZE = 1024 * 1024 * 256; //Write buffer: 256M
FILE *finp, *foutp, *infout;
uint32_t* _wBuf, cnt = 0, rem = 0;
uint32_t *buff;
uint32_t *selectors;
long cntOfSel = 0;
Simple16 ss;

void showMsg() {
	printf("DiaoSiEncoding Encoder ver1.0 by Kan Xiao & Xinzhao Liang\n");
	printf("Usage: DSEncoder in out\n");
	printf("    In: input data filename\n");
	printf("    Out: output filename -- out.dat & out.inf will be generated\n");
}

inline int ___numOfBits(uint32_t num) {
	int cc = 0;
	while (num > 0) {
		num >>= 1;
		cc++;
	}
	return cc;
}

inline void write2disk(uint32_t num) {
	int bN = ___numOfBits(num);
	int bR = ___numOfBits(rem);
	if (bN + bR > 32) {
		int dif = 32 - bR;
		int temp = num % (1 << dif);
		temp <<= bR;
		temp += rem;
		rem = num >> dif;
		//buffer is full, write out to disk
		if (cnt >= MAXSIZE) {
			fwrite(_wBuf, 4, cnt, foutp);
			cnt = 0;
		}
		_wBuf[cnt++] = temp;
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
	buff = new uint32_t[MAXSIZE];
	_wBuf = new uint32_t[MAXSIZE];
	selectors = new uint32_t[MAXSIZE];
	memset(buff, 0, MAXSIZE);
	uint32_t count;
	while (!feof(finp)) {
		count = fread(buff, 4, 1, finp);
		if (count <= 0)
			break;
//		printf("#: %d\n", count);
		uint32_t N = buff[0];
		fread(buff + 1, 4, N, finp);
//		cout << ftell(finp) << endl;
		uint32_t last = 0, n2write = 0;
		for (uint32_t i = 1; i <= N; i++) {
			n2write = buff[i] - last;
			last = buff[i];
			buff[i] = ___numOfBits(n2write);
			write2disk(n2write);
			//printf("%d %d\n", buff[p + i], n2write);
		}
		ss.encodeArray(buff, N + 1, selectors, count);
		fwrite(selectors, 4, count, infout);
	}
	//flush buffer
	if (cnt > 0 || rem > 0) {
		if (cnt < MAXSIZE) {
			if (rem > 0)
				_wBuf[cnt++] = rem;
			fwrite(_wBuf, 4, cnt, foutp);
		} else {
			fwrite(_wBuf, 4, cnt, foutp);
			cnt = 0;
			if (rem > 0) {
				_wBuf[cnt++] = rem;
				fwrite(_wBuf, 4, cnt, foutp);
			}
		}
	}
	fflush(infout);
	fflush(foutp);
	fclose(finp);
	fclose(foutp);
	fclose(infout);
	delete[] buff;
	delete[] _wBuf;

	return 0;

}
