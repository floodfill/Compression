/*
 * DSDecoder.h
 *
 *  Created on: 2012-5-13
 *      Author: kxiao
 */

#ifndef DSDECODER_H_
#define DSDECODER_H_

#include <iostream>
#include <fstream>
#include <cstring>
#include <vector>
#include "compress/Simple16.hpp"
#include <exception>
#include <ctime>

void msg(void);
void init(void);
void dispose(void);
void decompress(void);
void _write2disk(uint32_t);
int deMain(int,const char*[]);
void checkIllegalPara(int,const char*[]);
void showTime(void);

#endif /* DSDECODER_H_ */
