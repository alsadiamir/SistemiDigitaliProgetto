#include "ap_int.h"
#include <math.h>

#define FL 16
#define IL 16

typedef ap_int<IL + FL + 8> typemult;
typedef ap_uint<8> type;
typedef ap_int<IL + FL> typematrix;

//image dim
#define WIDTH_IMG  640
#define HEIGHT_IMG 480

//filter window dim

#define K_H 3
#define K_W 3

//#define K_H 7
//#define K_W 7

const int Km_H = (K_H - 1 ) / 2;
const int Km_W = (K_W - 1 ) / 2;

const float err = pow(2,-FL);
const float min=-1*pow(2,-FL);
const float max=pow(2,IL-1)-err;

//filter window
const float M[K_H][K_W] = {
			 // Gaussian blur
			  {-1,0,1},
			  {-2,0,2},
			  {-1,0,1} };

const int divF1=1;
const int sumF1=0;

//const float M[K_H][K_W] = {
//			// 7x7
//		   {1,6,15,20,15,6,1},
//		   {6,36,90,120,90,36,6},
//		   {15,90,225,300,225,90,15},
//		   {20,120,300,400,300,120,20},
//		   {15,90,225,300,225,90,15},
//		   {6,36,90,120,90,36,6},
//		   {1,6,15,20,15,6,1} };
//
//
//const int divF1=4096;
//const int sumF1=1;


const int offset1=128;

const int V_MID_HEIGHT=(K_H-1)/2;
const int H_MID_HEIGHT=(K_W-1)/2;

void set_matrix_converted_final(typematrix MATRIX_CONVERTED[K_H][K_W]);

void set_random_array(ap_uint<7> random_array[HEIGHT_IMG*WIDTH_IMG]);

void calcolo_uscita_stoc(type DATA_IN[WIDTH_IMG*HEIGHT_IMG], type DATA_OUT[WIDTH_IMG*HEIGHT_IMG]);

void calcolo_uscita_stoc_con_separabilita( type DATA_IN[HEIGHT_IMG*WIDTH_IMG], type DATA_OUT[HEIGHT_IMG*WIDTH_IMG]);
