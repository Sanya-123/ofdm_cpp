#include "my_map_modulation.h"

uint8_t getBytesInmodulation[5] = {1, 2, 4, 6, 8};

void modulation_f(uint8_t *inData, MyComplex_f *outData, Modulation modulation)//+
{
    switch (modulation) {
    case BPSK:
        modulationBPSK_f(inData, outData);
        break;
    case QPSK:
        modulationQPSK_f(inData, outData);
        break;
    case QAM16:
        modulationQAM16_f(inData, outData);
        break;
    case QAM64:
        modulationQAM64_f(inData, outData);
        break;
    case QAM256:
        modulationQAM256_f(inData, outData);
        break;
    default:
        break;
    }
}

//TODO MAP modulation

void modulationBPSK_f(uint8_t *inData, MyComplex_f *outData)//+
{
    for(int i = 0; i < 8; i++)
    {
        //TODO MAP data
        outData[i].real = (((inData[0]) >> i) & 0x01) ? BPSK_1 : BPSK__1;
        outData[i].image = 0;
    }
}

void modulationQPSK_f(uint8_t *inData, MyComplex_f *outData)//+
{
    for(int i = 0; i < 4; i++)
    {
        //TODO MAP data
        outData[i].real =  (((inData[0]) >> i*2) & 0x01) ? QPSK_1 : QPSK__1;
        outData[i].image = (((inData[0]) >> (i*2+1)) & 0x01) ? QPSK_1 : QPSK__1;

        outData[i+4].real =  (((inData[1]) >> i*2) & 0x01) ? QPSK_1 : QPSK__1;
        outData[i+4].image = (((inData[1]) >> (i*2+1)) & 0x01) ? QPSK_1 : QPSK__1;
    }
}

void modulationQAM16_f(uint8_t *inData, MyComplex_f *outData)//+
{
    uint8_t tmp;
    for(int i = 0; i < 2; i++)
    {
        //TODO MAP data
        tmp = ((inData[0]) >> i*4) & 0x03;
        outData[i].real =  tmp == 0 ? QAM16__3 : tmp == 1 ? QAM16__1: tmp == 3 ? QAM16_1 : QAM16_3;
        tmp = ((inData[0]) >> (i*4 + 2)) & 0x03;
        outData[i].image = tmp == 0 ? QAM16__3 : tmp == 1 ? QAM16__1: tmp == 3 ? QAM16_1 : QAM16_3;

        tmp = ((inData[1]) >> i*4) & 0x03;
        outData[i+2].real =  tmp == 0 ? QAM16__3 : tmp == 1 ? QAM16__1: tmp == 3 ? QAM16_1 : QAM16_3;
        tmp = ((inData[1]) >> (i*4 + 2)) & 0x03;
        outData[i+2].image = tmp == 0 ? QAM16__3 : tmp == 1 ? QAM16__1: tmp == 3 ? QAM16_1 : QAM16_3;

        tmp = ((inData[2]) >> i*4) & 0x03;
        outData[i+4].real =  tmp == 0 ? QAM16__3 : tmp == 1 ? QAM16__1: tmp == 3 ? QAM16_1 : QAM16_3;
        tmp = ((inData[2]) >> (i*4 + 2)) & 0x03;
        outData[i+4].image = tmp == 0 ? QAM16__3 : tmp == 1 ? QAM16__1: tmp == 3 ? QAM16_1 : QAM16_3;

        tmp = ((inData[3]) >> i*4) & 0x03;
        outData[i+6].real =  tmp == 0 ? QAM16__3 : tmp == 1 ? QAM16__1: tmp == 3 ? QAM16_1 : QAM16_3;
        tmp = ((inData[3]) >> (i*4 + 2)) & 0x03;
        outData[i+6].image = tmp == 0 ? QAM16__3 : tmp == 1 ? QAM16__1: tmp == 3 ? QAM16_1 : QAM16_3;
    }
}

//битовое поля для перобразования зи 6 8 битный элементов в 8 6 битныйх элементов для QAM64
#pragma pack(push, 1)

struct StructData
{
    //NOTE здесть если мспользовать 8 битныйе данные то он их неправильно оптимизурует
    //видемо потомучто некоторые элементы заходт в обы байта
    uint32_t real_0 : 3;
    uint32_t image_0 : 3;
    uint32_t real_1 : 3;
    uint32_t image_1 : 3;
    uint32_t real_2 : 3;
    uint32_t image_2 : 3;
    uint32_t real_3 : 3;
    uint32_t image_3 : 3;


    uint32_t unused : 8;
};

typedef union {
    struct StructData structData [2];
    struct {
        uint8_t data[3];
        uint8_t unused[1];
    }mas[2];
}_6x8byset_to_8x3bytes;
#pragma pack(pop)

inline float localMap64(quint8 inData/*0-7*/)
{
    //TODO MAP data
    float res = 0;
    switch (inData) {
    case 0b000: res = QAM64__7; break;
    case 0b001: res = QAM64__5; break;
    case 0b011: res = QAM64__3; break;
    case 0b010: res = QAM64__1; break;
    case 0b110: res =  QAM64_1; break;
    case 0b111: res =  QAM64_3; break;
    case 0b101: res =  QAM64_5; break;
    case 0b100: res =  QAM64_7; break;
    default:    res =  0; break;
    }

    return res;
}

void modulationQAM64_f(uint8_t *inData, MyComplex_f *outData)//+
{

    _6x8byset_to_8x3bytes tmp;
    for(int i = 0; i < 3; i ++)
    {
//        tmp.data.massData[i] = inData[i];
        tmp.mas[0].data[i] = inData[i];
        tmp.mas[1].data[i] = inData[i+3];
    }

    outData[0].real = localMap64(tmp.structData[0].real_0);
    outData[1].real = localMap64(tmp.structData[0].real_1);
    outData[2].real = localMap64(tmp.structData[0].real_2);
    outData[3].real = localMap64(tmp.structData[0].real_3);

    outData[4].real = localMap64(tmp.structData[1].real_0);
    outData[5].real = localMap64(tmp.structData[1].real_1);
    outData[6].real = localMap64(tmp.structData[1].real_2);
    outData[7].real = localMap64(tmp.structData[1].real_3);

    outData[0].image = localMap64(tmp.structData[0].image_0);
    outData[1].image = localMap64(tmp.structData[0].image_1);
    outData[2].image = localMap64(tmp.structData[0].image_2);
    outData[3].image = localMap64(tmp.structData[0].image_3);

    outData[4].image = localMap64(tmp.structData[1].image_0);
    outData[5].image = localMap64(tmp.structData[1].image_1);
    outData[6].image = localMap64(tmp.structData[1].image_2);
    outData[7].image = localMap64(tmp.structData[1].image_3);

//    for(int i = 0; i < 8; i++)
//    {
//        outData[i].real = localMap64(tmp.data.structData[i].real_0);
//        outData[i].image = localMap64(tmp.data.structData[i].image_0);
//    }
}

inline float localMap256(quint8 inData/*0-15*/)
{
    //TODO MAP data
    float res = 0;
    inData = inData & 0x0F;
    switch (inData) {
    case 0b0001: res = QAM256__15; break;
    case 0b0101: res = QAM256__13; break;
    case 0b0111: res = QAM256__11; break;
    case 0b0011: res = QAM256__9;  break;
    case 0b0010: res = QAM256__7;  break;
    case 0b0110: res = QAM256__5;  break;
    case 0b0100: res = QAM256__3;  break;
    case 0b0000: res = QAM256__1;  break;
    case 0b1000: res =  QAM256_1;  break;
    case 0b1100: res =  QAM256_3;  break;
    case 0b1110: res =  QAM256_5;  break;
    case 0b1010: res =  QAM256_7;  break;
    case 0b1011: res =  QAM256_9;  break;
    case 0b1111: res =  QAM256_11; break;
    case 0b1101: res =  QAM256_13; break;
    case 0b1001: res =  QAM256_15; break;
    default:     res =  0;  break;
    }

    return res;
}

void modulationQAM256_f(uint8_t *inData, MyComplex_f *outData)//+
{
    for(int i = 0; i < 8; i++)
    {
        outData[i].real = localMap256(inData[i] & 0x0F);
        outData[i].image = localMap256((inData[i] >> 4) & 0x0F);
    }
}
