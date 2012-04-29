/**
test whether a file is the right format for the compressor

compile:
g++ checkIncrease.cpp -o checkIncrease

usage:
./checkIncrease [file to be checked]

neilliang

**/
#include<iostream>
#include<stdio.h>
#include<stdlib.h>
using namespace std;
#define BITS_L 32
typedef unsigned long BYTES;

int main(int argc,char *argv[]){
	if(argc !=2){
		cout<<"usage: ./checkIncrease [file to be checked]"<<endl;
	}

	FILE *f = fopen(argv[1],"r");
	size_t size = sizeof(BYTES);
	BYTES *buf = new BYTES[1];
	BYTES temp = 0ul;

	while(fread(buf,size,1,f) > 0){
		temp = buf[0];
		BYTES first = 0ul,second = 0ul;
		for(int i =0; i <temp;i++){
			second = first;
			fread(buf,size,1,f);
			first = buf[0];
			if(second>first){
				cout<<"wrong"<<endl;
			}
		}
	}
	delete []buf;
	fclose(f);
	
}
