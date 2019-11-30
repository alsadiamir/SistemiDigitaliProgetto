
#include "test_func.h"
#include "ap_int.h"

typedef ap_int<24> typemult;
typedef ap_uint<8> type;
typedef ap_int<16> typematrix;

type convert_and_compute_32(typemult in);
type compute32(typemult in);

typematrix convert_signed(float out, char *ID);
typematrix compute_signed(float pixelin, char *ID);
