#include "stochastic_rounding_module.h"
#include "convert_library.h"
#include "stdio.h"


//in questo test:
// FORMATO: X		XXXXX		XXXXXXXXXXXX
//			sign	IL			FL
//
// nei primi IL bit <-- (int) z
// nei rimanenti FL bit <-- (frac) z

typematrix MATRIX_CONVERTED_FINAL[K_H][K_W];
typematrix MATRIX_CONVERTED_H[K_H];
typematrix MATRIX_CONVERTED_W[K_W];

void set_matrix_converted_final(typematrix MATRIX_CONVERTED[K_H][K_W]){
	for(int i=0;i<K_H;i++)
		for(int j=0;j<K_W;j++){
			MATRIX_CONVERTED_FINAL[i][j] = MATRIX_CONVERTED[i][j];
		}

	//y
	for(int i=0;i<K_H;i++)
		MATRIX_CONVERTED_H[i]=MATRIX_CONVERTED_FINAL[0][i];
	//x
	for(int j=0;j<K_W;j++)
			MATRIX_CONVERTED_W[j]=MATRIX_CONVERTED_FINAL[j][0];
}


ap_uint<7> RANDOM_ARRAY[HEIGHT_IMG*WIDTH_IMG];

void set_random_array(ap_uint<7> random_array[HEIGHT_IMG*WIDTH_IMG]){
	for(int i=0;i<HEIGHT_IMG*WIDTH_IMG;i++)
		RANDOM_ARRAY[i]=random_array[i];
}

//// kernel function
void convolute (type WINDOW[K_H][K_W], type *out, ap_uint<7> rand)
{
	#pragma HLS INLINE

	typemult out_pixel=0;

	//senza separazione

	printf("Elementi matriciali per prodotto: \n");

	Loop_convolution_rows: for(int j=0; j < K_H; j++)
		Loop_convolution_cols:	for(int i = 0; i < K_W; i++)
									out_pixel += WINDOW[j][i] * MATRIX_CONVERTED_FINAL[j][i];


	//printf("Uscita: %d\n",out_pixel.to_uint());

	convert_and_compute_32(out_pixel, out, rand);

}

void calcolo_uscita_stoc( type DATA_IN[HEIGHT_IMG*WIDTH_IMG], type DATA_OUT[HEIGHT_IMG*WIDTH_IMG]){

	#pragma HLS INTERFACE axis port=DATA_IN
	#pragma HLS INTERFACE axis port=DATA_OUT
	#pragma HLS INTERFACE axis port=RANDOM_ARRAY

	//line buffer
	static float line_buffer[K_H - 1][WIDTH_IMG];
	#pragma HLS ARRAY_PARTITION variable=line_buffer complete dim=1
	static float line_buffer_temp [K_H - 1];
	#pragma HLS ARRAY_PARTITION variable=line_buffer_temp complete dim=0

	//processing window
	static type window[K_H][K_W];
	#pragma HLS ARRAY_PARTITION variable=window complete dim=0

	int index=0;

	Loop_rows: for(int row = 0; row < HEIGHT_IMG + Km_H; row++){
		Loop_cols: for(int col = 0; col < WIDTH_IMG + Km_W; col++){

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
						if (row - Km_H >= 0 && col - Km_W >= 0){
							ap_uint<7> rand = RANDOM_ARRAY[index];
							convolute(window,&DATA_OUT[(row - Km_H)*WIDTH_IMG+(col - Km_W)], rand);
							index++;
						}


					} //loop columns
				}//loop rows
}

////

void convolute_with_separability( type WINDOW_H[K_H], type *out, ap_uint<7> rand){
#pragma HLS INLINE

	// buffer nel quale vengono salvati i risultati del primo prodotto di convoluzione : I temp
	static typemult conv_buffer [K_W];

	typematrix temp_v=0;
	typemult temp_h=0;

	type out_pixel=0;


	// convoluzione verticale : 1° prodotto

	for(int i = 0; i < K_H; i++)
		{
			temp_v = temp_v + MATRIX_CONVERTED_H[i] * WINDOW_H[i];
		}

	//Ciclo con il quale mi aggiorno i valori dei prodotti intermedi
	for(int j = 0; j < K_W-1; j++)
		{
			conv_buffer[j] = conv_buffer[j+1];
			temp_h = temp_h + conv_buffer[j]*MATRIX_CONVERTED_W[j];
		}

	// eseguo il secondo prodotto e vado ad aggiornare l'ultimo valore  del buffer intermedio
		temp_h = temp_h + temp_v*MATRIX_CONVERTED_H[K_W-1];
		conv_buffer[K_W-1] = temp_v;

		out_pixel = (int)((temp_h / sumF1) + offset1);

		convert_and_compute_32(out_pixel, out, rand);
}

void calcolo_uscita_stoc_con_separabilita( type DATA_IN[HEIGHT_IMG*WIDTH_IMG], type DATA_OUT[HEIGHT_IMG*WIDTH_IMG]){

	#pragma HLS INTERFACE axis port=DATA_IN
	#pragma HLS INTERFACE axis port=DATA_OUT
	#pragma HLS INTERFACE axis port=RANDOM_ARRAY

	//line buffer
	static type line_buffer[K_H-1][WIDTH_IMG];
	#pragma HLS ARRAY_PARTITION variable=line_buffer complete dim=1

	//processing window
	static type window[K_H];
	#pragma HLS ARRAY_PARTITION variable=window complete dim=0

	int index=0;

	Loop_row: for(int row = 0; row < HEIGHT_IMG + V_MID_HEIGHT; row++){
		Loop_col: for(int col = 0; col < WIDTH_IMG +H_MID_HEIGHT ; col++)

		{
	#pragma HLS PIPELINE II=1

			//copy KERN_H - 1 values from line_buffer to processing window
			if(col < WIDTH_IMG)
			{
				for(int i = 0; i < K_H - 1; i++)
				{
					window[i] = line_buffer[i][col];
					if (i < K_H - 2)
						line_buffer[i][col] = line_buffer[i + 1][col];
				}
			}

			//input value
			if(col < WIDTH_IMG & row < HEIGHT_IMG)
			{
				type in_temp = DATA_IN[row*WIDTH_IMG+col];
				window[K_H-1]= in_temp;
				line_buffer[K_H-2][col] = in_temp;
			}

			//output value
			if (row >= V_MID_HEIGHT  && col > H_MID_HEIGHT){
				ap_uint<7> rand = RANDOM_ARRAY[index];
				index++;
				convolute_with_separability(window,&DATA_OUT[(row-V_MID_HEIGHT)*WIDTH_IMG+(col - H_MID_HEIGHT)],rand);
			}

	    } //loop columns
	}//loop rows
}


