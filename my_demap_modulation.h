#ifndef MY_DEMAP_MODULATION_H
#define MY_DEMAP_MODULATION_H

#include <QObject>
#include "common_ofdm.h"

/*
 * |---------------------------------------------|
 * | MODULATION  |  outdata_size |  indata_size  |
 * |-------------|---------------|---------------|
 * |    BPSK     |      1        |               |
 * |    QPSK     |      2        |               |
 * |    QAM16    |      4        |      8        |
 * |    QAM64    |      6        |               |
 * |   QAM256    |      8        |               |
 * |---------------------------------------------|
*/

void demodulation_f(MyComplex_f *inData, uint8_t *outData, Modulation modulation);

void demodulationBPSK_f(MyComplex_f *inData, uint8_t *outData);
void demodulationQPSK_f(MyComplex_f *inData, uint8_t *outData);
void demodulationQAM16_f(MyComplex_f *inData, uint8_t *outData);
void demodulationQAM64_f(MyComplex_f *inData, uint8_t *outData);
void demodulationQAM256_f(MyComplex_f *inData, uint8_t *outData);

extern uint8_t getBytesInmodulation[];

#endif // MY_DEMAP_MODULATION_H
