#include "ap_int.h"

#define BIT_ACCURATE

//image dim
#define WIDTH_IMG  640
#define HEIGHT_IMG 480


//filter window dim

#define N 3
#define M 3
/*
//filter window
const int M[K_H][K_W] = { // Gaussian (16)
						     {{1, 2, 1},
		                      {2, 4, 2},
							  {1, 2, 1}},
						};
*/
const int V_MID_HEIGHT=(N-1)/2;
const int H_MID_HEIGHT=(M-1)/2;


typedef ap_uint<8> pixel;
typedef ap_int<8> weight;

void Separable_Filter_Convolution(pixel in_img[HEIGHT_IMG*WIDTH_IMG], pixel out_img[HEIGHT_IMG*WIDTH_IMG]);
