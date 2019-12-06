
#include "test_func.h"
#include "ap_int.h"

#define FL 4
#define IL 12

//int MULT= IL + FL + 8;
//int MATRIX= IL + FL;

typedef ap_int<IL + FL + 8> typemult;
typedef ap_uint<8> type;
typedef ap_int<IL + FL> typematrix;

void convert_and_compute_32(typemult in, type *out, ap_uint<7> rand);
void compute32(typemult in, type *out);

void convert_signed(float out, char *ID, typematrix *matrixpixel);
void compute_signed(float pixelin, char *ID, typematrix *matrixpixel);
