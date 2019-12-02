
#include "stochastic_rounding_module.h"
#include "test_func.h"
#include "convert_library.h"
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

	ap_uint<7> RANDOM_ARRAY[HEIGHT_IMG*WIDTH_IMG];

	typematrix MATRIX_CONVERTED[K_H][K_W];

	for(int i=0;i<K_H;i++)
		for(int j=0;j<K_W;j++)
			compute_signed(M[i][j],"MATRIX", &MATRIX_CONVERTED[i][j]);

	for(int i=0;i<K_H;i++)
			for(int j=0;j<K_W;j++)
				printf("MATRIX[%d,%d]: %d\n",i,j,MATRIX_CONVERTED[i][j].to_uint());


    for(long j=0;j<HEIGHT_IMG*WIDTH_IMG;j++){

    	RANDOM_ARRAY[j]=RandInt(1,100);

    	inFile>>DATA_IN[j];
    }

    type *DATA_OUT   = (type*)malloc(sizeof(type)*HEIGHT_IMG*WIDTH_IMG);

    set_random_array(RANDOM_ARRAY);
    set_matrix_converted_final(MATRIX_CONVERTED);

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
