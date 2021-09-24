#include "my_ofdm_frame_gen.h"
#include "my_map_modulation.h"
#include "my_ofdm_payload_gen.h"
#include "my_preambles.h"

uint32_t sizeOutputDataForFrame(OFDM_Settings settings)
{
    return SIZE_ALL_PREAMBLE + (FFT_SIZE_OFDM256 + settings.CP_lenght)*(1/*FCH*/+settings.sizeSymbolsDataOFDM/*DATA*/);
}

uint32_t sizeInputDataForFrame(OFDM_Settings settings)
{
    uint8_t bytsInModulations = getBytesInmodulation[settings.modulation];
    uint32_t bytesMustBe = bytsInModulations*DATA_CARRIER_IN_OFDM256 >> 3/*/8*/;

    return bytesMustBe*settings.sizeSymbolsDataOFDM;
}

inline void addPreamblesOFDM(MyComplex *outData)
{
    memcpy(outData, PA, SIZE_PREAMBLE_A*sizeof(MyComplex));
    memcpy(&outData[SIZE_PREAMBLE_A], PB, SIZE_PREAMBLE_B*sizeof(MyComplex));
}

inline void addFCH_OFDM(OFDM_Settings settings, MyComplex *symbolsFCH)
{
    uint8_t dataFCH[MAX_DATA_IN_SYMBOL_SIZE] = {0};//TODO
    OFDM_Settings settingsFCH;
    settingsFCH  = settings;
    MyComplex symbolDataFCH[FFT_SIZE_OFDM256];
    settingsFCH.modulation = MODULATION_FCH;
    genPayload(dataFCH, MAX_DATA_IN_SYMBOL_SIZE, settingsFCH, symbolDataFCH);
    addCP(symbolDataFCH, symbolsFCH, settingsFCH);
}

int genFramOFDM(uint8_t *inData, uint32_t sizeInData, OFDM_Settings settings, MyComplex *outData)
{
    uint32_t bytesMustBeInOneSymbol = getBytesInmodulation[settings.modulation]*DATA_CARRIER_IN_OFDM256 >> 3/*/8*/;
    uint32_t bytesMustBe = bytesMustBeInOneSymbol*settings.sizeSymbolsDataOFDM;
    if(sizeInData < bytesMustBe)
        return 0;

    uint32_t pointData = 0;

    addPreamblesOFDM(outData);
    pointData += SIZE_ALL_PREAMBLE;
    addFCH_OFDM(settings, &outData[pointData]);
    pointData += FFT_SIZE_OFDM256 + settings.CP_lenght;

    MyComplex symbolData[FFT_SIZE_OFDM256];
    for(uint32_t i = 0 ; i < settings.sizeSymbolsDataOFDM; i++)
    {
        genPayload(&inData[i*bytesMustBeInOneSymbol], bytesMustBeInOneSymbol, settings, symbolData);
        pointData += addCP(symbolData, &outData[pointData], settings);
    }

    return pointData;
}

int genFramOFDM_realTime(uint8_t *inData, uint8_t sizeInData, OFDM_Settings settings, MyComplex *outData)
{//TODO не доделан и лучьше по другому формировать данные
    if(sizeInData == 0)
        return 0;
    //когда вызываеться этот блок фиксирую входные настройки и записываю данные
    static uint32_t dataComeSize = 0;
    static uint32_t poitInputData = 0;//точка куда записываю входные данные
    static uint8_t dataCome[/*MAX_DATA_IN_FRAME_SIZE**/MAX_DATA_IN_SYMBOL_SIZE];
    static OFDM_Settings settingsSet;
    static bool idle = true;
    static uint32_t poitOutputData = 0;//точка куда записываю выходные данные
    static uint32_t bytesMustBeInOneSymbol = MAX_DATA_IN_SYMBOL_SIZE;
    static uint32_t sizeSymbolsModulate = 0;
//    static uint8_t stateFrameGenerator = 0;
//    /*
//     * 0 - IDLR
//     * 1 - wayt data
//    */

    if(idle)
    {
        idle = false;
        settingsSet = settings;
        if(settingsSet.CP_lenght > MAX_CP_SIZE)
            settingsSet.CP_lenght = MAX_CP_SIZE;
        //
        addPreamblesOFDM(outData);
        poitOutputData = SIZE_ALL_PREAMBLE;
        bytesMustBeInOneSymbol = getBytesInmodulation[settingsSet.modulation]*DATA_CARRIER_IN_OFDM256 >> 3/*/8*/;
        addFCH_OFDM(settingsSet, &outData[poitOutputData]);
        poitOutputData += FFT_SIZE_OFDM256 + settings.CP_lenght;
    }

    dataComeSize += sizeInData;

//    if(dataComeSize <= MAX_DATA_IN_FRAME_SIZE*MAX_DATA_IN_SYMBOL_SIZE)
//    {
//        memcpy(dataCome[poitInputData], inData, sizeInData);
//        poitInputData = dataComeSize;
//    }
//    else
//    {
//        memcpy(dataCome[poitInputData], inData, MAX_DATA_IN_FRAME_SIZE*MAX_DATA_IN_SYMBOL_SIZE - poitInputData);
//        poitInputData = MAX_DATA_IN_FRAME_SIZE*MAX_DATA_IN_SYMBOL_SIZE;
//    }


    uint32_t symboltTrereis = dataComeSize/bytesMustBeInOneSymbol;//количество симводлов которые могу сгенерировать
    uint32_t readSieData = 0;
    MyComplex symbolData[FFT_SIZE_OFDM256];

    while(symboltTrereis)
    {
        uint8_t tmp[MAX_DATA_IN_SYMBOL_SIZE];
        if(poitInputData > 0)//если в буффере еще остались данные
        {
            memcpy(tmp, dataCome, poitInputData);
            memcpy(&tmp[poitInputData], inData, bytesMustBeInOneSymbol - poitInputData);
            poitInputData = 0;
        }
        else
        {
            memcpy(tmp, &inData[readSieData], bytesMustBeInOneSymbol);
            readSieData += bytesMustBeInOneSymbol;
        }
        genPayload(tmp, bytesMustBeInOneSymbol, settingsSet, symbolData);
        poitOutputData += addCP(symbolData, &outData[poitOutputData], settingsSet);

        symboltTrereis--;
        dataComeSize -= bytesMustBeInOneSymbol;
        sizeSymbolsModulate++;

        if(sizeSymbolsModulate == settingsSet.sizeSymbolsDataOFDM)
            break;
    }


    if(sizeSymbolsModulate == settingsSet.sizeSymbolsDataOFDM)
    {

        idle = true;
        //если символ уже смоделирован но осталось немного информации
    }


    return 0;
}
