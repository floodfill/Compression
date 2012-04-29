/** this is to check whether the input data is in the right format

Neil Liang

**/


#include<iostream>
#include<stdio.h>
#include<stdlib.h>
using namespace std;
#define BITS_L 32
typedef unsigned long BYTES;

int main(){
	FILE *f = fopen("INVERTED_INDEX_FILE.DAT","r");
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
	fclose(f);
	delete []buf;
	return 0;
	
}
