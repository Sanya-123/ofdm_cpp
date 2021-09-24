#ifndef MY_MAP_MODULATION_H
#define MY_MAP_MODULATION_H

#include <QObject>
#include "common_ofdm.h"


/*
 * |---------------------------------------------|
 * | MODULATION  |  indata_size  | outdata_size  |
 * |-------------|---------------|---------------|
 * |    BPSK     |      1        |               |
 * |    QPSK     |      2        |               |
 * |    QAM16    |      4        |      8        |
 * |    QAM64    |      6        |               |
 * |   QAM256    |      8        |               |
 * |---------------------------------------------|
*/


void modulation_f(uint8_t *inData, MyComplex_f *outData, Modulation modulation);

void modulationBPSK_f(uint8_t *inData, MyComplex_f *outData);
void modulationQPSK_f(uint8_t *inData, MyComplex_f *outData);
void modulationQAM16_f(uint8_t *inData, MyComplex_f *outData);
void modulationQAM64_f(uint8_t *inData, MyComplex_f *outData);
void modulationQAM256_f(uint8_t *inData, MyComplex_f *outData);

extern uint8_t getBytesInmodulation[];

#endif // MY_MAP_MODULATION_H
