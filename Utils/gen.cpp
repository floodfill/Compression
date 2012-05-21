/** 
this is to generate dummy test data for testing the compression algo.

compile:
g++ gen.cpp -o gen

usage:
./gen [output file] [# of list] [maximum gap between number]

neil liang

**/

#include<bitset>
#include<stdio.h>
#include<time.h>
#include<iostream>
#include<stdlib.h>
#include<string>
using namespace std;

#define BITS_L 32
#define MAX_START  1000

typedef unsigned long BYTE;

void writeBinary(const bitset<BITS_L>&);

FILE *f;

int main(int argc, char *argv[]){

	BYTE count;
	if(argc != 4){
		cout<<"usage: ./gen [output file] [# of list] [maximum gap between number]"<<endl;
		return 0; 
	}
	f = fopen(argv[1],"a+");

	int times = atoi(argv[2]);
	int gap = atoi(argv[3]);
	int sum = 0;
	srand(time(NULL));

	while(times>0){
		
		cout<<count<<endl;
		count = rand() % MAX_START;
		sum += count;
		BYTE start = rand() % MAX_START;

		bitset<BITS_L>bits((count));

		writeBinary(bits);

		for(int i=0;i<count;i++){
			start = start + rand() % gap;
      			bitset<BITS_L>bits(start);
        		writeBinary(bits);

		}
		times--;
	}	
	fclose(f);
	cout<<"dummy file: " <<argv[1]<<endl;
	cout<<"total ints generated: "<<sum<<endl;
	return 0;
}


void writeBinary(const bitset<BITS_L>& bits ){
	size_t size = bits.size();
		
	BYTE result = bits.to_ulong();	

	fwrite(&result,sizeof(BYTE),1,f);	
}

