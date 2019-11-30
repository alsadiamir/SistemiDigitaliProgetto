
#include "stochastic_rounding_module.h"
#include <fstream>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>


using namespace std;

ifstream inFile;

int main(){

	int x=0;

	inFile.open("D:/imgnew.txt");

	if (!inFile) {
	    cerr << "Unable to open file datafile.txt";
	    exit(1);   // call system to stop
	}

	type *DATA_IN   = (type*)malloc(sizeof(type)*HEIGHT_IMG*WIDTH_IMG);

//	for(long j=0;j<4*HEIGHT_IMG*WIDTH_IMG;j++)
//		DATA_IN[j]=0;

    for(long j=0;j<HEIGHT_IMG*WIDTH_IMG;j++){
//    	unsigned long temp = 0;
//
//    	unsigned int firstbyte;
//    	unsigned int secondbyte;
//    	unsigned int thirdbyte;
//    	unsigned int fourthbyte;
//
//    	inFile >> firstbyte;
//    	inFile >> secondbyte;
//    	inFile >> thirdbyte;
//    	inFile >> fourthbyte;
//
//    	temp = ((unsigned long)firstbyte) << 24;
//    	temp += ((unsigned long)secondbyte) << 16;
//    	temp += ((unsigned long)thirdbyte) << 8;
//    	temp += ((unsigned long)fourthbyte);


    	inFile>>DATA_IN[j];
    }

    type *DATA_OUT   = (type*)malloc(sizeof(type)*HEIGHT_IMG*WIDTH_IMG);


    cout<<"++++++++++++++++++++++++++++++++++++++OUT: \n\n\n\n\n";

    calcolo_uscita_stoc(DATA_IN,DATA_OUT);

    FILE * pFile = fopen ("D:/myfile.txt" , "w");

	for(int j=0;j<HEIGHT_IMG*WIDTH_IMG;j++)
		fprintf(pFile,"%d \n",DATA_OUT[j].to_uint());

	fclose(pFile);

//    for(long j=0;j<HEIGHT_IMG*WIDTH_IMG;j++)
//   	cout<<"Valore: "<< (float)(DATA_OUT[j]/(pow(2,10)-1))<<"\n";


	return 0;
}
