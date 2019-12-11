#include "ap_int.h"
//#include "sep_convolution_filter.h"

#define FL 6
#define IL 10
#define RESULT_BITS 24

typedef ap_int<IL + FL + 8> typemult;
typedef ap_uint<8> type;
typedef ap_int<IL + FL> typematrix;

//Default 8
#define PIXEL_BITS 8
//Default 8
#define WEIGHT_BITS 16
//Set this to at least
//ceil(log_2(2^(PIXEL_BITS + WEIGHT_BITS) * KERNEL_HEIGHT * KERNEL_WIDTH))
#define MAC_BITS 24
//Set this to at least PIXEL_BITS + WEIGHT_BITS
#define MUL_BITS 24

typedef ap_uint<PIXEL_BITS> pixel;
typedef ap_int<WEIGHT_BITS> s_int;

type convert_and_compute_32(ap_int<RESULT_BITS> in, ap_uint<7> rand);
void compute32(typemult in, type *out);

s_int convert_signed(float out);
s_int compute_signed(float pixelin);
