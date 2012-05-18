//============================================================================
// Name        : Main.cpp
// Author      : Kan Xiao
// Version     : ver 1.0
// Copyright   :
// Description : Diaosi Encoding Encoder/Decoder Selector
//============================================================================

#include "DSDecoder.h"
#include "DSEncoder.h"
#include <string>
using namespace std;

int main() {
	string base = "/home/kxiao/Desktop/integer_encoding_library/bigbig";
//	const char * en[] = { "", (base + ".dat").c_str(),
//			(base + "Compressed").c_str() };
//
//	enMain(3, en);
	const char * de[] = { "", (base + "Compressed").c_str(), (base
			+ "Uncompressed.dat").c_str() };
	deMain(3, de);
	return 0;
}
