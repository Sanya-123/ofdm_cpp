#include "my_demap_modulation.h"

void demodulation_f(MyComplex_f *inData, uint8_t *outData, Modulation modulation)
{
    switch (modulation) {
    case BPSK:
        demodulationBPSK_f(inData, outData);
        break;
    case QPSK:
        demodulationQPSK_f(inData, outData);
        break;
    case QAM16:
        demodulationQAM16_f(inData, outData);
        break;
    case QAM64:
        demodulationQAM64_f(inData, outData);
        break;
    case QAM256:
        demodulationQAM256_f(inData, outData);
        break;
    default:
        break;
    }
}

void demodulationBPSK_f(MyComplex_f *inData, uint8_t *outData)//+
{
    outData[0] = 0;
    for(int i = 0; i < 8; i++)
    {
        if(inData[i].real > 0)
            outData[0] |= 1 << i;
    }
}

void demodulationQPSK_f(MyComplex_f *inData, uint8_t *outData)//+
{
    outData[0] = 0;
    outData[1] = 0;
    for(int i = 0; i < 4; i++)
    {
        if(inData[i].real > 0)
            outData[0] |= 1 << (i*2);
        if(inData[i].image > 0)
            outData[0] |= 1 << (i*2+1);

        if(inData[i+4].real > 0)
            outData[1] |= 1 << (i*2);
        if(inData[i+4].image > 0)
            outData[1] |= 1 << (i*2+1);
    }
}

inline uint8_t demap16(float inData)
{
    //TODO MAP
    if(inData < DMP_QAM16__3)
        return 0b00;
    if(inData < DMP_QAM16__1)
        return 0b01;
    if(inData < DMP_QAM16_1)
        return 0b11;


    return 0b10;
}

void demodulationQAM16_f(MyComplex_f *inData, uint8_t *outData)//+
{
    outData[0] = 0;
    outData[1] = 0;
    outData[2] = 0;
    outData[3] = 0;
    for(int i = 0; i < 2; i++)
    {
        outData[0] |= demap16(inData[i].real) << 4*i;
        outData[0] |= demap16(inData[i].image) << (4*i + 2);

        outData[1] |= demap16(inData[i+2].real) << 4*i;
        outData[1] |= demap16(inData[i+2].image) << (4*i + 2);

        outData[2] |= demap16(inData[i+4].real) << 4*i;
        outData[2] |= demap16(inData[i+4].image) << (4*i + 2);

        outData[3] |= demap16(inData[i+6].real) << 4*i;
        outData[3] |= demap16(inData[i+6].image) << (4*i + 2);
    }
}

inline uint8_t demap64(float inData)
{
    //TODO MAP
    if(inData < DMP_QAM64__7)
        return 0b000;
    if(inData < DMP_QAM64__5)
        return 0b001;
    if(inData < DMP_QAM64__3)
        return 0b011;
    if(inData < DMP_QAM64__1)
        return 0b010;
    if(inData < DMP_QAM64_1)
        return 0b110;
    if(inData < DMP_QAM64_3)
        return 0b111;
    if(inData < DMP_QAM64_5)
        return 0b101;

    return 0b100;
}

void demodulationQAM64_f(MyComplex_f *inData, uint8_t *outData)//+
{
    uint32_t dataTmp;
    for(int i = 0; i < 2; i++)
    {
        dataTmp = 0;
        dataTmp |= demap64(inData[i*4].real) << 0;
        dataTmp |= demap64(inData[i*4].image) << 3;

        dataTmp |= demap64(inData[i*4+1].real) << 6;
        dataTmp |= demap64(inData[i*4+1].image) << 9;

        dataTmp |= demap64(inData[i*4+2].real) << 12;
        dataTmp |= demap64(inData[i*4+2].image) << 15;

        dataTmp |= demap64(inData[i*4+3].real) << 18;
        dataTmp |= demap64(inData[i*4+3].image) << 21;

        outData[i*3+0] =  dataTmp & 0xFF;
        outData[i*3+1] = (dataTmp >> 8) & 0xFF;
        outData[i*3+2] = (dataTmp >> 16) & 0xFF;
    }
}

inline uint8_t demap256(float inData)
{
    //TODO MAP
    if(inData < DMP_QAM256__15)
        return 0b0001;
    if(inData < DMP_QAM256__13)
        return 0b0101;
    if(inData < DMP_QAM256__11)
        return 0b0111;
    if(inData < DMP_QAM256__9)
        return 0b0011;
    if(inData < DMP_QAM256__7)
        return 0b0010;
    if(inData < DMP_QAM256__5)
        return 0b0110;
    if(inData < DMP_QAM256__3)
        return 0b0100;
    if(inData < DMP_QAM256__1)
        return 0b0000;
    if(inData < DMP_QAM256_1)
        return 0b1000;
    if(inData < DMP_QAM256_3)
        return 0b1100;
    if(inData < DMP_QAM256_5)
        return 0b1110;
    if(inData < DMP_QAM256_7)
        return 0b1010;
    if(inData < DMP_QAM256_9)
        return 0b1011;
    if(inData < DMP_QAM256_11)
        return 0b1111;
    if(inData < DMP_QAM256_13)
        return 0b1101;


    return 0b1001;
}

void demodulationQAM256_f(MyComplex_f *inData, uint8_t *outData)//+
{
    for(int i = 0; i < 8; i++)
    {
        outData[i] = (demap256(inData[i].image) << 4) | demap256(inData[i].real);
    }
}
