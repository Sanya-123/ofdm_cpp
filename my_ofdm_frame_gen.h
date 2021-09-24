#ifndef MY_OFDM_FRAME_GEN_H
#define MY_OFDM_FRAME_GEN_H

#include <QObject>
#include "common_ofdm.h"

uint32_t sizeOutputDataForFrame(OFDM_Settings settings);//требуемое количество данных на выходе
uint32_t sizeInputDataForFrame(OFDM_Settings settings);//требуемое количество данных на входе

//входные данные копяться и когда накопять на 1 кадр на выход полылаеться весь кадр
int genFramOFDM(uint8_t *inData, uint32_t sizeInData, OFDM_Settings settings, MyComplex *outData);


#endif // MY_OFDM_FRAME_GEN_H
