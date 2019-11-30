#include "ap_int.h"
#include <math.h>


#define FL 8
#define IL 8


typedef ap_int<24> typemult;
typedef ap_uint<8> type;
typedef ap_int<16> typematrix;

#define BIT_OUT 16;

//image dim
#define WIDTH_IMG  640
#define HEIGHT_IMG 480

//filter window dim

#define NUMBER_OF_FILTER 8
#define K_H 3
#define K_W 3
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
			  {0.0625, 0.125, 0.0625}	};

typedef struct custom_out{
	ap_uint<FL> fractional;
	ap_int<IL> integer;
} custom_out;

const int sumF=1;

void calcolo_uscita_stoc( type DATA_IN[WIDTH_IMG*HEIGHT_IMG], type DATA_OUT[WIDTH_IMG*HEIGHT_IMG]);
