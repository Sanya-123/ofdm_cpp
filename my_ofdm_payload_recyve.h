#ifndef MY_OFDM_PAYLOAD_RECYVE_H
#define MY_OFDM_PAYLOAD_RECYVE_H

#include <QObject>
#include "common_ofdm.h"
#include "my_fft.h"

//NOTE only OFDM256
int recyvePayloadData(MyComplex *inData, OFDM_Settings settings, uint8_t *outData);//вытаскивание данных из спектра
//int recyvePayload(MyComplex *inData, OFDM_Settings settings, uint8_t *outData);


#endif // MY_OFDM_PAYLOAD_RECYVE_H
