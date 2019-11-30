#include "convert_library.h"
//#include "ap_int.h"
#include "stochastic_rounding_module.h"

type compute32(typemult in){
	#pragma HLS INLINE

	printf("IN: %d\n",in.to_int());

	float pixelin=(((float)in.to_int())/pow(2,FL));

	printf("pixel prima della convoluzione a 32 bit: %f\n", pixelin);

	printf("pixel dopo gli if: %f\n", pixelin);

	int pixelinteg=(int)pixelin;

	printf("Parte intera: %d\n",pixelinteg);

	type pixel=pixelinteg;

	printf("Uscita: %d\n",pixel.to_uint());

	return pixel;
}

type convert_and_compute_32(typemult in){

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

			float out=zstoc;

			if(RandInt(1,100)>p)
				pixelin+=err;
		}

	printf("pixel dopo gli if: %f\n", pixelin);

	int pixelinteg=(int)pixelin;

	printf("Parte intera: %d\n",pixelinteg);

	type pixel=pixelinteg;

	printf("Uscita: %d\n",pixel.to_uint());

	return pixel;
}

typematrix convert_signed(float out, char *ID){

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

typematrix compute_signed(float pixelin, char *ID){

//	#pragma HLS INLINE

	int N = (int)(pixelin/err);

	float zstoc=N*err;

	int p= (int)((1 - (pixelin-zstoc)/err)*100);

	float out=zstoc;

	if(RandInt(1,100)>p)
			out+=err;

	//printf("Prima di conversione, dato di %s = %f\n",ID,out);

	return convert_signed(out,ID);
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
