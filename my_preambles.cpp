#include "my_preambles.h"
#include "my_math_operations.h"

MyComplex PA[256];
MyComplex PB[256];

QVector<MyComplex> preamleA;
QVector<MyComplex> preamleB;

void initPreambles()
{
    for(int i = 0; i < 256; i++)
    {
        PA[i].real = PA_I[i]*8/*TODO scale*/;
        PA[i].image = PA_Q[i]*8;
        preamleA.append(PA[i]);

        PB[i].real = PB_I[i]*8;
        PB[i].image = PB_Q[i]*8;
        preamleB.append(PB[i]);
    }
}

QVector<MyComplex> inputOldPreamble;

uint64_t porogPreamble = /*15000000*/MIN_POROG_PREAMBLE;//TODO dynamic preamble

MyComplex summMultComplexConjArryaEasy(QVector<MyComplex> preamble, QVector<MyComplex> data, uint32_t sizePreabble)
{
    MyComplex res = {0,0}, tmp;
    QVector<MyComplex> tmpVector;
//    tmp.resize(sizePreabble);
    int64_t real = 0, image = 0;
    for(int i = 0; i < sizePreabble; i++)
    {
//        if(preamble[i].real > 0)
//        {
//            res.real += data[i].real;
//            res.image += data[i].image;
//        }
//        else
//        {
//            res.real -= data[i].real;
//            res.image -= data[i].image;
//        }

//        if(preamble[i].image > 0)
//        {
//            res.real += data[i].image;
//            res.image -= data[i].real;
//        }
//        else
//        {
//            res.real -= data[i].image;
//            res.image += data[i].real;
//        }

        //TODO почемуто это дольше
        //swep conj
//        res.real += preamble[i].real > 0 ? data[i].real : -data[i].real;
//        res.image -= preamble[i].real > 0 ? data[i].image : -data[i].image;
//        if(preamble[i].real > 0)
//        {
//            res.real += data[i].real;
//            res.image -= data[i].image;
//        }
//        else
//        {
//            res.real -= data[i].real;
//            res.image += data[i].image;
//        }

//        if(preamble[i].image > 0)
//        {
//            res.real += data[i].image;
//            res.image += data[i].real;
//        }
//        else
//        {
//            res.real -= data[i].image;
//            res.image -= data[i].real;
//        }
        if(preamble[i].real > 0)
        {
            real += data[i].real;
            image -= data[i].image;
        }
        else
        {
            real -= data[i].real;
            image += data[i].image;
        }

        if(preamble[i].image > 0)
        {
            real += data[i].image;
            image += data[i].real;
        }
        else
        {
            real -= data[i].image;
            image -= data[i].real;
        }

//        MyComplex one;
////        one.real = (preamble[i].real > 0)*2-1;
////        one.image = (preamble[i].image > 0)*2-1;
//        one.real = (preamble[i].real > 0 ? 1 : -1);
//        one.image = (preamble[i].image > 0 ? 1 : -1);

//        one = multComplexConj(data[i], one);

//        res.real += one.real;
//        res.image += one.image;

    }

    res.real = real;
    res.image = image;

    return res;
}

uint64_t filterScol = MIN_POROG_PREAMBLE;

void dunamicPreamble(uint64_t preambleSumm)
{
//    static uint64_t filterScol = MIN_POROG_PREAMBLE;

    filterScol = (preambleSumm  + (FILTER_PARAM_PREAMBLE - 1)*filterScol)/FILTER_PARAM_PREAMBLE;

    porogPreamble = (filterScol << 2) > MIN_POROG_PREAMBLE ? (filterScol << 2) : MIN_POROG_PREAMBLE;
}

bool findPreamble(QVector<MyComplex> preamble, MyComplex *iputData, uint32_t sizeInputData, MyComplex *outData/*почемуто не работает*/, uint32_t *sizeOutputData, MyComplex *outPreamble, QVector<MyComplex> *summPreamblesall, QVector<uint64_t> *dynamucPorog)
{
    bool d_ret = false;
    uint32_t sizeAnalizeData = 0;
    uint64_t preambleMaxval = 0;
    do
    {
        inputOldPreamble.append(iputData[sizeAnalizeData]);
        sizeAnalizeData++;

        if(inputOldPreamble.size() > SIZE_PREAMBLE_A)
        {
            inputOldPreamble.removeFirst();
//            if(porogPreamble == 0)
//            {
//                QVector<MyComplex> multPreamble = multComplexConjArrya(inputOldPreambleA, inputOldPreambleA, SIZE_PREAMBLE_A);
//                MyComplex summpreamble = summComplexArray(multPreamble);
//                porogPreamble = absComplex(summpreamble)*10/8;
//            }

#if USE_EQSY_PREAMBKE
            MyComplex summpreamble = summMultComplexConjArryaEasy(preamble, inputOldPreamble, SIZE_PREAMBLE_A);
#else
            QVector<MyComplex> multPreamble = multComplexConjArrya(preamble, inputOldPreamble, SIZE_PREAMBLE_A);
            MyComplex summpreamble = summComplexArray(multPreamble);
            summpreamble.real = summpreamble.real/1024;
            summpreamble.image = summpreamble.image/1024;
#endif



            uint64_t modulPreamble = absComplex(summpreamble);

            //dynamic filter
            dunamicPreamble(modulPreamble);

            if(dynamucPorog != NULL)
                dynamucPorog->append(porogPreamble);

            if(summPreamblesall != NULL)//если требуеться вывести всю отладку преамбулы для всего сигнала
                summPreamblesall->append(summpreamble);

            if(d_ret)
            {
//                if(summPreamblesall != NULL)//если требуеться вывести всю отладку преамбулы для всего сигнала
                if(preambleMaxval > modulPreamble)//если преамбула была найдена в предыдушей итерации то просто выхожу
                {
                    if(summPreamblesall == NULL)//емли требуеться выводить преамбулу
                        return true;
                }
                else//если пик еще больше
                {
                    *outPreamble = summpreamble;
                    if(sizeAnalizeData < sizeInputData)
                    {
                        outData = &iputData[sizeAnalizeData];
                        *sizeOutputData = sizeInputData - sizeAnalizeData;
                    }
                    else
                    {
                        *sizeOutputData = 0;
                    }

                    preambleMaxval = modulPreamble;
                }
            }
            else
            {
                if(modulPreamble > porogPreamble)
                {
                    *outPreamble = summpreamble;
                    if(sizeAnalizeData < sizeInputData)
                    {
                        outData = &iputData[sizeAnalizeData];
                        *sizeOutputData = sizeInputData - sizeAnalizeData;
                    }
                    else
                    {
                        *sizeOutputData = 0;
                    }

                    preambleMaxval = modulPreamble;

//                    return true;
                    d_ret = true;//если уже достиг порого то провожу еще 1 итерацию вдруг там больший пик
                }
            }

        }
    }while(sizeAnalizeData != sizeInputData);

//    return false;
    return d_ret;
}

void resetPreamble()
{
    filterScol = MIN_POROG_PREAMBLE;
    inputOldPreamble.clear();
}
