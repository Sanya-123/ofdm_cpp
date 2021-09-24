#include "my_ofdm_frame_recive.h"
#include <QVector>
#include <QtMath>
#include "my_preambles.h"
#include "my_math_operations.h"
#include "my_fft.h"
#include "my_ofdm_payload_recyve.h"
#include "my_preambles.h"

extern int pilots_i_OFDM256[];
extern int pilots_q_OFDM256[];

//todo initPilotcomples

//#include <QDebug>

void getEqalizingData(MyComplex *inputData, MyComplex *eqSumbolS)
{
    MyComplex pilots[PILOT_SYMBOL_SIZE] = {
        {pilots_i_OFDM256[0], pilots_q_OFDM256[0]},
        {pilots_i_OFDM256[1], pilots_q_OFDM256[1]},
        {pilots_i_OFDM256[2], pilots_q_OFDM256[2]},
        {pilots_i_OFDM256[3], pilots_q_OFDM256[3]},
        {pilots_i_OFDM256[4], pilots_q_OFDM256[4]},
        {pilots_i_OFDM256[5], pilots_q_OFDM256[5]},
        {pilots_i_OFDM256[6], pilots_q_OFDM256[6]},
        {pilots_i_OFDM256[7], pilots_q_OFDM256[7]}
    };

//    pilots[0].real = pilots_i_OFDM256[0];
    //TODO вообще надо поменять местами pilots и inputData но у меня большые значения у inputData и без плавающей точки
    eqSumbolS[13] = divComplex(inputData[13], pilots[0]);
    eqSumbolS[38] = divComplex(inputData[38], pilots[1]);
    eqSumbolS[63] = divComplex(inputData[63], pilots[2]);
    eqSumbolS[88] = divComplex(inputData[88], pilots[3]);
    eqSumbolS[168] = divComplex(inputData[168], pilots[4]);
    eqSumbolS[193] = divComplex(inputData[193], pilots[5]);
    eqSumbolS[218] = divComplex(inputData[218], pilots[6]);
    eqSumbolS[243] = divComplex(inputData[243], pilots[7]);

    int64_t step1_i = (eqSumbolS[38].real - eqSumbolS[13].real)/25;
    int64_t step1_q = (eqSumbolS[38].image - eqSumbolS[13].image)/25;

    int64_t step2_i = (eqSumbolS[63].real - eqSumbolS[38].real)/25;
    int64_t step2_q = (eqSumbolS[63].image - eqSumbolS[38].image)/25;

    int64_t step3_i = (eqSumbolS[88].real - eqSumbolS[63].real)/25;
    int64_t step3_q = (eqSumbolS[88].image - eqSumbolS[63].image)/25;

    int64_t step4_i = (eqSumbolS[193].real - eqSumbolS[168].real)/25;
    int64_t step4_q = (eqSumbolS[193].image - eqSumbolS[168].image)/25;

    int64_t step5_i = (eqSumbolS[218].real - eqSumbolS[193].real)/25;
    int64_t step5_q = (eqSumbolS[218].image - eqSumbolS[193].image)/25;

    int64_t step6_i = (eqSumbolS[243].real - eqSumbolS[218].real)/25;
    int64_t step6_q = (eqSumbolS[243].image - eqSumbolS[218].image)/25;
    for(int i = 0; i < 24; i++)
    {
        eqSumbolS[14+i].real = eqSumbolS[13].real + step1_i * (i + 1);
        eqSumbolS[14+i].image = eqSumbolS[13].image + step1_q * (i + 1);

        eqSumbolS[39+i].real = eqSumbolS[38].real + step2_i * (i + 1);
        eqSumbolS[39+i].image = eqSumbolS[38].image + step2_q * (i + 1);

        eqSumbolS[64+i].real = eqSumbolS[63].real + step3_i * (i + 1);
        eqSumbolS[64+i].image = eqSumbolS[63].image + step3_q * (i + 1);

        eqSumbolS[169+i].real = eqSumbolS[168].real + step4_i * (i + 1);
        eqSumbolS[169+i].image = eqSumbolS[168].image + step4_q * (i + 1);

        eqSumbolS[194+i].real = eqSumbolS[193].real + step5_i * (i + 1);
        eqSumbolS[194+i].image = eqSumbolS[193].image + step5_q * (i + 1);

        eqSumbolS[219+i].real = eqSumbolS[218].real + step6_i * (i + 1);
        eqSumbolS[219+i].image = eqSumbolS[218].image + step6_q * (i + 1);
    }

    eqSumbolS[0].real = eqSumbolS[0].image = 1;

    int64_t step7_i = (eqSumbolS[13].real - eqSumbolS[243].real)/26;
    int64_t step7_q = (eqSumbolS[13].image - eqSumbolS[243].image)/26;

    for(int i = 1; i < 13; i++)
    {
        eqSumbolS[i].real = eqSumbolS[243].real + step7_i*(i + 14);
        eqSumbolS[i].image = eqSumbolS[243].image + step7_q*(i + 14);
    }

    for(int i = 244; i < 256; i++)
    {
        eqSumbolS[i].real = eqSumbolS[243].real + step7_i*(i-243);
        eqSumbolS[i].image = eqSumbolS[243].image + step7_q*(i-243);
    }

//    int64_t step8_i = (eqSumbolS[13].real - eqSumbolS[243].real)/26;
//    int64_t step8_q = (eqSumbolS[13].real - eqSumbolS[243].real)/26;

    for(int i = 89; i < 128; i++)
    {
        eqSumbolS[i].real = eqSumbolS[88].real;
        eqSumbolS[i].image = eqSumbolS[88].image;
    }

    for(int i = 128; i < 168; i++)
    {
        eqSumbolS[i].real = eqSumbolS[168].real;
        eqSumbolS[i].image = eqSumbolS[168].image;
    }

}

void equalizing(MyComplex *inputData, MyComplex *outputData, QVector<MyComplex> *pulotSumbolS)
{
    QVector<MyComplex> inputDataVector(FFT_SIZE_OFDM256);

    memcpy(inputDataVector.data(), inputData, sizeof(MyComplex)*FFT_SIZE_OFDM256);
    memcpy(outputData, divComplexAray(inputDataVector, *pulotSumbolS, FFT_SIZE_OFDM256).data(), sizeof(MyComplex)*FFT_SIZE_OFDM256);
}

#include <QDebug>
StateRx state = WaytFerstPreamble;

extern uint64_t filterScol;
extern QVector<MyComplex> inputOldPreamble;

int OFDM_recive(MyComplex *inputData, uint32_t sizeInputData, uint8_t *outputData, OFDM_Settings settings, MyComplex *outDataModulate)
{
    if(sizeInputData == 0)
        return 0;

//    static StateRx state = WaytFerstPreamble;

    static MyComplex preambleA_val;
    static MyComplex preambleB_val;

    static float beginPhase;
    static float addPhase;
    static int pakcetRecives;

    if(state == WaytFerstPreamble)
    {
//        qDebug() << sizeInputData;
//        qDebug() << (uint32_t)inputData;
//        porogPreamble = 15000000;
        MyComplex *outDataAfterPreamble;
        uint32_t sizeDataAfterPreambke;
        if(findPreamble(preamleA, inputData, sizeInputData, outDataAfterPreamble, &sizeDataAfterPreambke, &preambleA_val, NULL, NULL))
        {
            filterScol = MIN_POROG_PREAMBLE;
//            inputOldPreamble.clear();
            inputOldPreamble.remove(0, FFT_SIZE_OFDM256 - FILTER_PARAM_PREAMBLE);
//            qDebug() << "find preamble A" << sizeDataAfterPreambke << preambleA_val.image << preambleA_val.real;
//            qDebug() << "find preamble A";
            state = WaytSecondPreamble;
            if(sizeDataAfterPreambke == 0)
                return 0;
        }
        else
            return 0;
        inputData = /*outDataAfterPreamble*/&inputData[sizeInputData - sizeDataAfterPreambke];//смещаю указатель
        sizeInputData = sizeDataAfterPreambke;// и размер
    }

    if(state == WaytSecondPreamble)
    {
//        porogPreamble = 50000000;
        MyComplex *outDataAfterPreamble;
        uint32_t sizeDataAfterPreambke;
//        qDebug() << sizeInputData;
//        qDebug() << (uint32_t)inputData;
        if(findPreamble(preamleB, inputData, sizeInputData, outDataAfterPreamble, &sizeDataAfterPreambke, &preambleB_val, NULL, NULL))
        {
//            inputOldPreamble.clear();
            inputOldPreamble.remove(0, FFT_SIZE_OFDM256 - FILTER_PARAM_PREAMBLE*4);
//            qDebug() << "find preamble B" << sizeDataAfterPreambke << preambleB_val.image << preambleB_val.real;
//            qDebug() << "find preamble B";
            beginPhase = qAtan2(preambleB_val.image, preambleB_val.real);
            addPhase = (beginPhase - qAtan2(preambleA_val.image, preambleA_val.real))/FFT_SIZE_OFDM256;

//            qDebug() << "beginPhase" << beginPhase;
//            qDebug() << "addPhase" << addPhase;

            state = WaytFCH;
            if(sizeDataAfterPreambke == 0)
                return 0;
        }
        else
            return 0;
        inputData = /*outDataAfterPreamble*/&inputData[sizeInputData - sizeDataAfterPreambke];//смещаю указатель
        sizeInputData = sizeDataAfterPreambke;// и размер
    }

    if(sizeInputData < (FFT_SIZE_OFDM256 + settings.CP_lenght))
        return 0;

    //freq sync
    for(int i = 0; i < (sizeInputData); i++)//BUG note worck
    {
        beginPhase += addPhase;
        inputData[i] = addPhaseComplex(inputData[i], beginPhase);
    }


    static uint32_t nRecivePacket = 0;
    static QVector<MyComplex> pulotSumbolS(FFT_SIZE_OFDM256);

    if(state == WaytFCH)
    {
        static MyComplex FCH_spector[FFT_SIZE_OFDM256];

        doFFT(&inputData[settings.CP_lenght/*removeCP*/], FCH_spector, FFT_SIZE_OFDM256, false/*forvard FFT*/, settings.comp);
        getEqalizingData(FCH_spector, pulotSumbolS.data());
        equalizing(FCH_spector, FCH_spector, &pulotSumbolS);

//        if(outDataModulate != NULL)
//            memcpy(&outDataModulate[0], FCH_spector, sizeof(FCH_spector[0])*FFT_SIZE_OFDM256);

        //TODO reframe FCH data

        state == WaytData;

        pakcetRecives = 0;

        sizeInputData -= FFT_SIZE_OFDM256 + settings.CP_lenght;

        if(sizeInputData == 0)
            return 0;

        inputData = &inputData[FFT_SIZE_OFDM256 + settings.CP_lenght];
//        qDebug() << "resive FCH";
    }

//    for(int i = 0; i <= nRecivePacket/*FCH + DATA packet*/; i++)
//    {

//    }

//    if(state == WaytData) {}


    int bytsRes = 0;
    while(pakcetRecives < settings.sizeSymbolsDataOFDM)
    {
        if(sizeInputData < (FFT_SIZE_OFDM256 + settings.CP_lenght))
            return bytsRes;

        static MyComplex dataSpector[FFT_SIZE_OFDM256];

        doFFT(&inputData[settings.CP_lenght/*removeCP*/], dataSpector, FFT_SIZE_OFDM256, false/*forvard FFT*/, settings.comp);//~300us
//        qDebug() << "DO FFT";
        getEqalizingData(dataSpector, pulotSumbolS.data());//<1us
        equalizing(dataSpector, dataSpector, &pulotSumbolS);//80us

        if(outDataModulate != NULL)
            memcpy(&outDataModulate[FFT_SIZE_OFDM256*pakcetRecives], dataSpector, sizeof(dataSpector[0])*FFT_SIZE_OFDM256);

        bytsRes += recyvePayloadData(dataSpector, settings, &outputData[bytsRes]);//13us

        pakcetRecives++;
        sizeInputData -= (FFT_SIZE_OFDM256 + settings.CP_lenght);

        if(sizeInputData >= (FFT_SIZE_OFDM256 + settings.CP_lenght))
            inputData = &inputData[FFT_SIZE_OFDM256 + settings.CP_lenght];
    }


    return bytsRes;
//    return 0;
}

void resetRx()
{
//    MyComplex zeros;
//    zeros.real = zeros.image = 0.0;
    filterScol = MIN_POROG_PREAMBLE;
    inputOldPreamble.clear();
//    for(int i = 0; i < FFT_SIZE_OFDM256; i++)
//        inputOldPreamble.append(zeros);
    state = WaytFerstPreamble;
}

