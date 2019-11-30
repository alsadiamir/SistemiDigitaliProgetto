#include "stochastic_rounding_module.h"
#include "convert_library.h"
#include "test_func.h"
#include "stdio.h"


//in questo test:
// FORMATO: X		XXXXX		XXXXXXXXXXXX
//			sign	IL			FL
//
// nei primi IL bit <-- (int) z
// nei rimanenti FL bit <-- (frac) z

//// kernel function
type convolute (typematrix MATRIX[K_H][K_W],type WINDOW[K_H][K_W])
{
	#pragma HLS INLINE

	typemult out_pixel=0;

	//senza separazione

	printf("Elementi matriciali per prodotto: \n");

	for(int j=0; j < K_H; j++)
		for(int i = 0; i < K_W; i++){
			//printf("W: %d\nM: %d\n",WINDOW[j][i].to_uint(),MATRIX[j][i].to_int());
			//non divido: sumF=1
			out_pixel += WINDOW[j][i] * MATRIX[j][i];
		}


	//printf("Uscita: %d\n",out_pixel.to_uint());


	return convert_and_compute_32(out_pixel);

}
////

//ingressi a 8 bit

void calcolo_uscita_stoc( type DATA_IN[HEIGHT_IMG*WIDTH_IMG], type DATA_OUT[HEIGHT_IMG*WIDTH_IMG]){

	#pragma HLS INTERFACE axis port=DATA_IN
	#pragma HLS INTERFACE axis port=DATA_OUT

	//line buffer
	static float line_buffer[K_H - 1][WIDTH_IMG];
	#pragma HLS ARRAY_PARTITION variable=line_buffer complete dim=1
	static float line_buffer_temp [K_H - 1];
	#pragma HLS ARRAY_PARTITION variable=line_buffer_temp complete dim=0

	//processing window
	static type window[K_H][K_W];
	#pragma HLS ARRAY_PARTITION variable=window complete dim=0


	//matrice dei pesi
	typematrix MATRIX[K_H][K_W];
	for(int i=0; i < K_H; i++)
			for(int j = 0; j < K_W; j++)
				MATRIX[i][j]=compute_signed(M[i][j],"MATRIX");


	for(int row = 0; row < HEIGHT_IMG + Km_H; row++){
		for(int col = 0; col < WIDTH_IMG + Km_W; col++){

		#pragma HLS PIPELINE II=1
			// shift columns of processing window
			for(int ii = 0; ii < K_H; ii++)
				for(int jj = 0; jj < K_W-1; jj++)
					window[ii][jj] = window[ii][jj+1];


			//line_buffer_temp
			if(col < WIDTH_IMG)
				for(int ii = 0; ii < K_H - 1; ii++)
					line_buffer_temp[ii] = line_buffer[ii][col];



			// copy K_H - 1 values from line_buffer to processing window
			if(col < WIDTH_IMG)
				for(int ii = 0; ii < K_H - 1; ii++)
					window[ii][K_W - 1] = line_buffer_temp[ii];


			//shift row of line buffer
			if(col < WIDTH_IMG)
				for(int ii = 0; ii < K_H-2; ii++)
					line_buffer[ii][col]= line_buffer_temp[ii+1];


			//input value
			if(col < WIDTH_IMG & row < HEIGHT_IMG)
			{
				int in_temp = DATA_IN[row*WIDTH_IMG+col];
				window[K_H-1][K_W-1]  = in_temp;
				line_buffer[K_H-2][col] = in_temp;
			}

			//output value
			if (row - Km_H >= 0 && col - Km_W >= 0)
				DATA_OUT[(row - Km_H)*WIDTH_IMG+(col - Km_W)] = convolute(MATRIX,window);

		} //loop columns
	}//loop rows
}
