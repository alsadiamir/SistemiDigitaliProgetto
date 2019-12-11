#include "convert_library.h"
#include <stdio.h>
#include "sep_convolution_filter.h"
#include "test_func.h"

const float err = pow(2,-FL);
const float min=-1*pow(2,-FL);
const float max=pow(2,IL-1)-err;

void compute32(typemult in, type *out){
	#pragma HLS INLINE

	printf("IN: %d\n",in.to_int());

	float pixelin=(((float)in.to_int())/pow(2,FL));

	printf("pixel prima della convoluzione a 32 bit: %f\n", pixelin);

	printf("pixel dopo gli if: %f\n", pixelin);

	int pixelinteg=(int)pixelin;

	printf("Parte intera: %d\n",pixelinteg);

	type pixel=pixelinteg;

	printf("Uscita: %d\n",pixel.to_uint());

	*out=pixel;
}

pixel convert_and_compute_32(ap_int<RESULT_BITS> in, ap_uint<7> rand){

	#pragma HLS INLINE

	printf("IN: %d\n",in.to_int());

	float pixelin=(((float)in.to_int())/pow(2,FL));

	printf("pixel prima della convoluzione a 32 bit: %f\n", pixelin);

	if(pixelin<min){
		pixelin = min;
	}

	else
		//RICONTROLLA
		if(pixelin>max){
			pixelin = max;
		}
		else{
			int N = (int)(pixelin/err);

			float zstoc=N*err;

			int p= (int)((1 - (pixelin-zstoc)/err)*100);

			pixelin=zstoc;

			if(rand>p)
				pixelin+=err;

//			if(pixelin-zstoc < err/2) pixelin=zstoc;
//			else pixelin=zstoc+err;
		}

	printf("pixel dopo gli if: %f\n", pixelin);

	int pixelinteg=(int)pixelin;

	printf("Parte intera: %d\n",pixelinteg);

	pixel out=pixelinteg;

	printf("Uscita: %d\n",out.to_uint());

	return out;
}

s_int convert_signed(float out){

	if(out<min){
		out = min;
	}

	if(out>max){
		out = max;
	}

	typematrix pixelout=(out)*(pow(2,FL));

	//printf("Pixel moltiplicato a destra: %d\n",pixelout);

	//printf("Dopo conversione, dato di %s = %f\n", ID,(((float)pixelout)/(pow(2,16)-1)));

	return pixelout;
}

s_int compute_signed(float pixelin){

	#pragma HLS INLINE

	int N = (int)(pixelin/err);

	float zstoc=N*err;

	int p= (int)((1 - (pixelin-zstoc)/err)*100);

	float out=zstoc;

	if(RandInt(1,100)>p)
			out+=err;

	//printf("Prima di conversione, dato di %s = %f\n",ID,out);

	return convert_signed(out);
}

//unsigned int convert_unsigned(float out, char *ID){
//
//
//	if(out<min){
//		out = min;
//	}
//
//	if(out>max){
//		out = max;
//	}
//
//	unsigned int pixelout=(out)*(pow(2,FL));
//
//	//printf("Pixel moltiplicato a destra: %d\n",pixelout);
//
//	//printf("Dopo conversione, dato di %s = %f\n", ID,(((float)pixelout)/(pow(2,16)-1)));
//
//	return pixelout;
//}
//
//unsigned int compute_unsigned(float pixelin, char *ID){
//
//	#pragma HLS INLINE
//
//	int N = (int)(pixelin/err);
//
//	float zstoc=N*err;
//
//	int p= (int)((1 - (pixelin-zstoc)/err)*100);
//
//	float out=zstoc;
//
//	if(RandInt(1,100)>p)
//		out+=err;
//
//	//printf("Prima di conversione, dato di %s = %f\n",ID,out);
//
//	return convert_unsigned(out,ID);
//}
