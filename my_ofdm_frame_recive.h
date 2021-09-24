#ifndef MY_OFDM_FRAME_RECIVE_H
#define MY_OFDM_FRAME_RECIVE_H

#include <QObject>
#include "common_ofdm.h"

typedef enum{
    WaytFerstPreamble = 0,
    WaytSecondPreamble = 1,
    WaytFCH = 2,
    WaytData = 3
}StateRx;

int OFDM_recive(MyComplex *inputData, uint32_t sizeInputData, uint8_t *outputData, OFDM_Settings settings, MyComplex *outDataModulate = NULL);
void resetRx();


#endif // MY_OFDM_FRAME_RECIVE_H
