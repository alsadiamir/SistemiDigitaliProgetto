#include "test_func.h"


unsigned int pseudo_random(unsigned int seed, int load) {
  static ap_uint<32> lfsr;

  if (load ==1 )
    lfsr = seed;
  bool b_32 = lfsr.get_bit(32-32);
  bool b_22 = lfsr.get_bit(32-22);
  bool b_2 = lfsr.get_bit(32-2);
  bool b_1 = lfsr.get_bit(32-1);
  bool new_bit = b_32 ^ b_22 ^ b_2 ^ b_1;
  lfsr = lfsr >> 1;
  lfsr.set_bit(31, new_bit);

  return lfsr.to_uint();

}


unsigned int PRNG()
{

    // Take the seed and return a value between 0 and 32767

	static int load=1;

	static unsigned int seed=0;

	if(!seed) seed=5353;
	else {
		seed=pseudo_random(seed,load);
		if(load==1) load=0;
	}

    int quoz=seed/32767;

    return seed-quoz*32767;

}

/* Get random double: */
double RandNum (double min, double max)
{
   return min + (max - min) * ((double)PRNG()
                            / (double)32767);
}

/* Get random int: */
int RandInt (int min, int max)
{
   return (int)RandNum((double)min + 0.000001,
                       (double)max + 0.999999);
}
