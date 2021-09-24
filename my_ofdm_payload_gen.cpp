#include "my_ofdm_payload_gen.h"
#include "my_map_modulation.h"
#include <QVector>

uint8_t dataAndZeros[MAX_DATA_IN_SYMBOL_SIZE] = {0};
int pilots_i_OFDM256[PILOT_SYMBOL_SIZE] = {PILOT_P13_I, PILOT_P38_I, PILOT_P63_I, PILOT_P88_I, PILOT_P_P88_I, PILOT_P_P63_I, PILOT_P_P38_I, PILOT_P_P13_I};
int pilots_q_OFDM256[PILOT_SYMBOL_SIZE] = {PILOT_P13_Q, PILOT_P38_Q, PILOT_P63_Q, PILOT_P88_Q, PILOT_P_P88_Q, PILOT_P_P63_Q, PILOT_P_P38_Q, PILOT_P_P13_Q};

int genPayloadData(uint8_t *inData, uint32_t sizeData, OFDM_Settings settings, MyComplex *outData)
{
    uint8_t bytsInModulations = getBytesInmodulation[settings.modulation];
    uint32_t bytesMustBe = bytsInModulations*DATA_CARRIER_IN_OFDM256 >> 3/*/8*/;

    uint8_t *dataFroGen = inData;

    if(sizeData < bytesMustBe)//если информации меньше тообеденяю массив данных и 0
    {
        memset(dataAndZeros, 0, sizeof(dataAndZeros));
        memcpy(dataAndZeros, inData, sizeData);
        dataFroGen = dataAndZeros;
    }


//    QVector<MyComplex> vectodDataMOD(DATA_CARRIER_IN_OFDM256);

    MyComplex_f vectodDataMOD[DATA_CARRIER_IN_OFDM256];
//    modulation_f()

    for(int i = 0,j = 0; i < DATA_CARRIER_IN_OFDM256; i += 8, j += bytsInModulations)
    {
        modulation_f(dataFroGen + j, &vectodDataMOD[i], settings.modulation);
    }


    //map modulate data to harmonic
//    QVector<MyComplex> vectodData(FFT_SIZE_OFDM256);
//    MyComplex vectodData[FFT_SIZE_OFDM256];
    MyComplex tmp;
    for(int i = 0, p = 0, j = 0; i < FFT_SIZE_OFDM256; i++)
    {
        switch (i) {
        case 0://zero
            tmp.real = 0;
            tmp.image = 0;
            break;
        case 13://pilot
        case 38:
        case 63:
        case 88:
        case 168:
        case 193:
        case 218:
        case 243:
            tmp.real = pilots_i_OFDM256[p];
            tmp.image = pilots_q_OFDM256[p];
            p++;
            break;
        default://data and ZI
            if((i > 100) && (i < 156))//ZI
            {
                tmp.real = 0;
                tmp.image = 0;
            }
            else//data
            {
//                tmp = vectodDataMOD[j++];
                FLOAT_COMPLEX_TO_INT_COMPLEX(vectodDataMOD[j], tmp);
                j++;
            }
            break;
        }

        outData[i] = tmp;
    }

    return bytesMustBe;
}

int genPayload(uint8_t *inData, uint32_t sizeData, OFDM_Settings settings, MyComplex *outData)
{
    int dataModulate = genPayloadData(inData, sizeData, settings, outData);

    doFFT(outData, outData, FFT_SIZE_OFDM256, true/*invers FFT*/, settings.comp);

    return dataModulate;
}

int addCP(MyComplex *inData, MyComplex *outData, OFDM_Settings settings)
{
    memcpy(&outData[0], &inData[FFT_SIZE_OFDM256 - settings.CP_lenght], settings.CP_lenght*sizeof(MyComplex));
    memcpy(&outData[settings.CP_lenght], inData, FFT_SIZE_OFDM256*sizeof(MyComplex));

    return FFT_SIZE_OFDM256 + settings.CP_lenght;
}


