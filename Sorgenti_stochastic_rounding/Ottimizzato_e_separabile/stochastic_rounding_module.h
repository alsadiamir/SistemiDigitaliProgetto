#include "ap_int.h"
#include <math.h>


#define FL 6
#define IL 10


typedef ap_int<IL + FL + 8> typemult;
typedef ap_uint<8> type;
typedef ap_int<IL + FL> typematrix;

#define BIT_OUT 16;

//image dim
#define WIDTH_IMG  640
#define HEIGHT_IMG 480

//filter window dim


#define K_H 3
#define K_W 3
//
//#define K_H 5
//#define K_W 5

const int Km_H = (K_H - 1 ) / 2;
const int Km_W = (K_W - 1 ) / 2;

const float err = pow(2,-FL);
const float min=-1*pow(2,-FL);
const float max=pow(2,IL-1)-err;

//filter window
const float M[K_H][K_W] = {
			 // Gaussian blur
			  {0.0625, 0.125, 0.0625},
			  {0.125, 0.250, 0.125},
			  {0.0625, 0.125, 0.0625} };
//
//const float M[K_H][K_W] = {
//			// 5x5
//		   {0.0036630036630037,0.0146520146520147,0.0256410256410256,0.0146520146520147,0.0036630036630037},
//		   {0.0146520146520147,0.0586080586080586,0.1025641025641026,0.0586080586080586,0.0146520146520147},
//		   {0.0256410256410256,0.1025641025641026,0.1794871794871795,0.10256410256410266,0.0256410256410256},
//		   {0.0146520146520147,0.0586080586080586,0.1025641025641026,0.0586080586080586,0.0146520146520147},
//		   {0.0036630036630037,0.0146520146520147,0.0256410256410256,0.0146520146520147,0.0036630036630037} };

const int sumF1=1;
const float sumF2=1.058608058608059;

const int offset1=0;
const int offset2=0;

const int V_MID_HEIGHT=(K_H-1)/2;
const int H_MID_HEIGHT=(K_W-1)/2;

void set_matrix_converted_final(typematrix MATRIX_CONVERTED[K_H][K_W]);

void set_random_array(ap_uint<7> random_array[HEIGHT_IMG*WIDTH_IMG]);

void calcolo_uscita_stoc(type DATA_IN[WIDTH_IMG*HEIGHT_IMG], type DATA_OUT[WIDTH_IMG*HEIGHT_IMG]);
