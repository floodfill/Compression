/*
 * DSEncoder.h
 *
 *  Created on: 2012-5-13
 *      Author: kxiao
 */

#ifndef DSENCODER_H_
#define DSENCODER_H_


#include <iostream>
#include <compress/Simple16.hpp>
#include <cstring>


void showMsg();
int numOfBits(uint32_t);
void write2disk(uint32_t);
int enMain(int, const char*[]);

#endif /* DSENCODER_H_ */
