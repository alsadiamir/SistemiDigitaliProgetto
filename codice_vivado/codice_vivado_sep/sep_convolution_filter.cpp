#include "sep_convolution_filter.h"
#include "test_func.h"
#include "convert_library.h"
#include "stdio.h"

pixel pixel_weighted_average_separable
(
		s_int kernel_h[KERNEL_WIDTH],
		s_int kernel_v[KERNEL_HEIGHT],
		s_int kern_sum,
		s_int kern_off,
		pixel window[KERNEL_HEIGHT],
		ap_uint<7> rand
)
{
	#pragma HLS INLINE

	// buffer nel quale vengono salvati i risultati del primo prodotto di convoluzione : I temp
	static ap_int<CONV_BUFFER_BITS> conv_buffer [KERNEL_WIDTH];

	ap_int<CONV_BUFFER_BITS> temp_v=0;
	ap_int<RESULT_BITS> temp_h=0;

	ap_int<RESULT_BITS> out_pixel=0;


	// convoluzione verticale : 1° prodotto

	for(int i = 0; i < KERNEL_HEIGHT; i++)
		{
			temp_v = temp_v + kernel_v[i] * window[i];
		}

	//Ciclo con il quale mi aggiorno i valori dei prodotti intermedi
	for(int j = 0; j < KERNEL_WIDTH-1; j++)
		{
			conv_buffer[j] = conv_buffer[j+1];
			temp_h = temp_h + conv_buffer[j]*kernel_h[j];
		}

	// eseguo il secondo prodotto e vado ad aggiornare l'ultimo valore  del buffer intermedio
		temp_h = temp_h + temp_v*kernel_h[KERNEL_WIDTH-1];
		conv_buffer[KERNEL_WIDTH-1] = temp_v;

		printf("Temp h %d and temp v %d \n",temp_h.to_int(),temp_v.to_int());

		out_pixel = ((temp_h /kern_sum)+ kern_off);


		return convert_and_compute_32(out_pixel, rand);
}

#if defined(CONFIGURABLE)
void sep_convolution_filter
(
		s_int kernel_config[CONFIG_LEN],
		pixel in_img[IMG_HEIGHT*IMG_WIDTH],
		pixel out_img[IMG_HEIGHT*IMG_WIDTH]
)
{
//#pragma HLS RESOURCE variable=kernel_config core=RAM_S2P_LUTRAM
#pragma HLS INTERFACE ap_ovld port=kernel_config
#else
void sep_convolution_filter
(
		pixel in_img[IMG_HEIGHT*IMG_WIDTH],
		pixel out_img[IMG_HEIGHT*IMG_WIDTH]
)
{
#endif

#pragma HLS INTERFACE axis port=out_img
#pragma HLS INTERFACE axis port=in_img

	//line buffer
	static pixel line_buffer[KERNEL_HEIGHT - 1][IMG_WIDTH];
#pragma HLS ARRAY_PARTITION variable=line_buffer complete dim=1

	//processing window
	static pixel window[KERNEL_HEIGHT];
#pragma HLS ARRAY_PARTITION variable=window complete dim=0

#if defined(CONFIGURABLE)
	//kernel_config
	static s_int kernel_v[KERNEL_HEIGHT];
#pragma HLS ARRAY_PARTITION variable=kernel_v complete dim=0
	static s_int kernel_h[KERNEL_WIDTH];
#pragma HLS ARRAY_PARTITION variable=kernel_h complete dim=0

	static s_int kernel_sum = 1;
	static s_int kernel_off = 0;

	static int i = 0;
	static int j = 0;
	static int iteration = 0;

	iteration = 0;
	i = 0;
	j = 0;
#else
s_int kernel_h[KERNEL_WIDTH] = { 30, 39, 48, 1, 10, 19, 28 };
s_int kernel_v[KERNEL_HEIGHT] = { 30, 38, 46, 5, 13, 21, 22 };
s_int kernel_sum = 1;
s_int kernel_off = 0;
#endif

	for(int i=0;i<KERNEL_WIDTH;i++){
	#pragma HLS PIPELINE II=1
		float conv = kernel_h[i]/kernel_sum;
		kernel_h[i]=compute_signed(conv);
	}

	for(int i=0;i<KERNEL_HEIGHT;i++){
	#pragma HLS PIPELINE II=1
		float conv = kernel_v[i]/kernel_sum;
		kernel_v[i]=compute_signed(conv);
	}

	Loop_row: for(int row = 0; row < IMG_HEIGHT + KERNEL_MID_HEIGHT; row++)
		Loop_col: for(int col = 0; col < IMG_WIDTH + KERNEL_MID_WIDTH; col++)
		{
#pragma HLS PIPELINE II=1

#if defined(CONFIGURABLE)
			//kernel_config setup
			if (iteration < KERNEL_HEIGHT * KERNEL_WIDTH)
			{
				if (j >= KERNEL_WIDTH)
				{
					j = 0;
					i++;
				}

				if (i == 0)
				{
					kernel_h[j] = kernel_config[iteration];
					if (j == 0)
						kernel_v[0] = kernel_h[0];
				}
				else if (j == 0)
					kernel_v[i] = kernel_config[iteration];

				j++;
			}
			else if (iteration == KERNEL_SUM_INDEX)
				kernel_sum = kernel_config[KERNEL_SUM_INDEX];
			else if (iteration == KERNEL_OFF_INDEX)
				kernel_off = kernel_config[KERNEL_OFF_INDEX];
#endif

			//copy KERN_H - 1 values from line_buffer to processing window
			if(col < IMG_WIDTH)
			{
				for(int i = 0; i < KERNEL_HEIGHT - 1; i++)
				{
					window[i] = line_buffer[i][col];
					if (i < KERNEL_HEIGHT - 2)
						line_buffer[i][col] = line_buffer[i + 1][col];
				}
			}

			//input value
			if(col < IMG_WIDTH & row < IMG_HEIGHT)
			{
				pixel in_temp = in_img[row*IMG_WIDTH+col];
				window[KERNEL_HEIGHT-1]= in_temp;
				line_buffer[KERNEL_HEIGHT-2][col] = in_temp;
			}

			//output value
			if (row >= KERNEL_MID_WIDTH  && col > KERNEL_MID_HEIGHT){
				ap_uint<7> rand = RandInt(1,100);
#if defined(CONFIGURABLE)
				pixel out = pixel_weighted_average_separable
						(
								kernel_h,
								kernel_v,
								kernel_sum,
								kernel_off,
								window,
								rand
						);
#else
				pixel out = pixel_weighted_average_separable
						(
								kernel_h,
								kernel_v,
								kernel_sum,
								kernel_off,
								window,
								rand
						);
#endif
				out_img[(row - KERNEL_MID_HEIGHT) * IMG_WIDTH + (col - KERNEL_MID_WIDTH)] = out;
			}

#if defined(CONFIGURABLE)
			iteration++;
#endif

		} //col loop

}
