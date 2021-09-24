#include "my_ofdm_payload_recyve.h"
#include "my_demap_modulation.h"

int recyvePayloadData(MyComplex *inData, OFDM_Settings settings, uint8_t *outData)//+
{
    uint8_t bytsInModulations = getBytesInmodulation[settings.modulation];
    uint32_t bytesMustBe = bytsInModulations*DATA_CARRIER_IN_OFDM256 >> 3/*/8*/;
    MyComplex_f vectodDataMOD[DATA_CARRIER_IN_OFDM256];
    //demap harmonic to modulate data
    for(int i = 0, j = 0; i < FFT_SIZE_OFDM256; i++)
    {
        switch (i) {
        case 0://zero
        case 13://pilot
        case 38:
        case 63:
        case 88:
        case 168:
        case 193:
        case 218:
        case 243:
            break;
        default://data and ZI
            if((i > 100) && (i < 156))//ZI
            {
            }
            else//data
            {
                INT_COMPLEX_TO_FLOAT_COMPLEX(inData[i], vectodDataMOD[j]);
                j++;
            }
            break;
        }
    }

    //demodulate data
    for(int i = 0,j = 0; i < DATA_CARRIER_IN_OFDM256; i += 8, j += bytsInModulations)
    {
        demodulation_f(&vectodDataMOD[i], &outData[j], settings.modulation);
    }

    return bytesMustBe;
}

int recyvePayload(MyComplex *inData, OFDM_Settings settings, uint8_t *outData)//TODO эквалозирование
{
    doFFT(inData, inData, FFT_SIZE_OFDM256, false/*forvard FFT*/, settings.comp);

    return recyvePayloadData(inData, settings, outData);
}
