#include "Filter_convolution.h"

//// kernel function
pixel mediaPixel (weight v_kern[N],weight h_kern[M],weight sum,weight offset,pixel wind[N])
{
#pragma HLS INLINE

// buffer nel quale vengono salvati i risultati del primo prodotto di convoluzione : I temp
static ap_int<16> conv_buffer [M];

ap_int<16> temp_v=0;
ap_int<24> temp_h=0;

pixel res=0;


// convoluzione verticale : 1° prodotto

for(int i = 0; i < N; i++)
	{
		temp_v = temp_v + v_kern[i] * wind[i];
	}

//Ciclo con il quale mi aggiorno i valori dei prodotti intermedi
for(int j = 0; j < M-1; j++)
	{
		conv_buffer[j] = conv_buffer[j+1];
		temp_h = temp_h + conv_buffer[j]*h_kern[j];
	}

// eseguo il secondo prodotto e vado ad aggiornare l'ultimo valore  del buffer intermedio
	temp_h = temp_h + temp_v*h_kern[M-1];
	conv_buffer[M-1] = temp_v;

	res = ((temp_h / sum) + offset)(7,0);

	return res;
















}
////


void Separable_Filter_Convolution (pixel   in_img[HEIGHT_IMG*WIDTH_IMG],
		          pixel   out_img[HEIGHT_IMG*WIDTH_IMG])
{

#pragma HLS INTERFACE axis port=out_img
#pragma HLS INTERFACE axis port=in_img


//line buffer
static pixel line_buffer[N-1][WIDTH_IMG];
#pragma HLS ARRAY_PARTITION variable=line_buffer complete dim=1

//processing window
static pixel window[N];
#pragma HLS ARRAY_PARTITION variable=window complete dim=0

// define values of vertical and horizontal kernels
static weight kernel_v[N]={1,2,1};
static weight kernel_h[N]={1,2,1};
#pragma HLS ARRAY_PARTITION variable=kernel_v complete dim=0
#pragma HLS ARRAY_PARTITION variable=kernel_h complete dim=0


static weight k_sum=10;
static weight k_off=0;


// Ciclo sulla tela con una iterazione aggiuntiva in lunghezza e in larghezza dovuta al kernel

Loop_row: for(int row = 0; row < HEIGHT_IMG + V_MID_HEIGHT; row++){
	Loop_col: for(int col = 0; col < WIDTH_IMG +H_MID_HEIGHT ; col++)

	{
#pragma HLS PIPELINE II=1

		//copy KERN_H - 1 values from line_buffer to processing window
		if(col < WIDTH_IMG)
		{
			for(int i = 0; i < N - 1; i++)
			{
				window[i] = line_buffer[i][col];
				if (i < N - 2)
					line_buffer[i][col] = line_buffer[i + 1][col];
			}
		}

		//input value
		if(col < WIDTH_IMG & row < HEIGHT_IMG)
		{
			pixel in_temp = in_img[row*WIDTH_IMG+col];
			window[N-1]= in_temp;
			line_buffer[N-2][col] = in_temp;
		}

		//output value
		if (row >= V_MID_HEIGHT  && col > H_MID_HEIGHT)
		{   pixel out_temp=0;
			out_temp=mediaPixel (kernel_v,kernel_h,k_sum,k_off, window);
			out_img[(row-V_MID_HEIGHT)*WIDTH_IMG+(col - H_MID_HEIGHT)] = out_temp;
		}

    } //loop columns
}//loop rows

}
