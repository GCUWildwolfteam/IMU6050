#ifndef __FILTER_H
#define __FILTER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
//#include <arm_math.h>

//#include "stm32f4xx.h"
#include "typedefs.h"


void sensorsAccIIRLPFilter(vector_t *in,vector_t *out);
void Filter_IIRLPF(float *in,float *out, float LpfAttFactor);




#endif /* __FILTER_H */



