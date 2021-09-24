#ifndef MY_OFDM_PAYLOAD_GEN_H
#define MY_OFDM_PAYLOAD_GEN_H

#include <QObject>
#include "common_ofdm.h"
#include "my_fft.h"

//return size data modulate
//if (sizeData < must be) return 0
//NOTE only OFDM256
int genPayloadData(uint8_t *inData, uint32_t sizeData, OFDM_Settings settings, MyComplex *outData);//генерирование спектра
int genPayload(uint8_t *inData, uint32_t sizeData, OFDM_Settings settings, MyComplex *outData);//генерирование символа FFT
int addCP(MyComplex *inData, MyComplex *outData, OFDM_Settings settings);

#endif // MY_OFDM_PAYLOAD_GEN_H
