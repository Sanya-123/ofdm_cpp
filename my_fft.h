#ifndef MY_FFT_H
#define MY_FFT_H

#include <iostream>
#include <cmath>
#include "common_ofdm.h"

#define USE_SCALE       0

//MyComplex multComplex(MyComplex a, MyComplex b);
//MyComplex summComplex(MyComplex a, MyComplex b);
//MyComplex multEComplex(MyComplex a, float fi_deg);
//void multComplexEFFT(MyComplex a, int sizeFFT, int fi_deg, MyComplex *minus, MyComplex *plus, bool invers = false, CompensFP comp = None);

void doFFT(MyComplex *inputData, MyComplex *outputData, int size, bool invers, CompensFP comp = None);
void doFFT_f(MyComplex_f *inputData, MyComplex_f *outputData, int size, bool invers);

void doFFTR4_f(MyComplex_f *inputData, MyComplex_f *outputData, int size, bool invers);



#endif // MY_FFT_H
