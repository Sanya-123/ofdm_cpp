#include "my_fft.h"
#include <QDebug>
#include <QtMath>

//MyComplex multComplex(MyComplex a, MyComplex b)
//{
//    MyComplex res;
//    res.real = a.real * b.real - a.image * b.image;
//    res.image = a.image * b.real + a.real * b.image;
//    return res;
//}

inline MyComplex summComplex(MyComplex a, MyComplex b)
{
    MyComplex res;
    res.real = a.real + b.real;
    res.image = a.image + b.image;
    return res;
}

//MyComplex multEComplex(MyComplex a, float fi_deg)
//{
//    MyComplex arg;
//    arg.real = cos(fi_deg/180.0*M_PI);
//    arg.image = sin(fi_deg/180.0*M_PI);
//    return multComplex(a, arg);
//}

inline MyComplex_f summComplex_f(MyComplex_f a, MyComplex_f b)
{
    MyComplex_f res;
    res.real = a.real + b.real;
    res.image = a.image + b.image;
    return res;
}

inline MyComplex_f summComplex_R4_f(MyComplex_f a, MyComplex_f b, MyComplex_f c, MyComplex_f d)
{
    MyComplex_f res;
    res.real = a.real + b.real + c.real + d.real;
    res.image = a.image + b.image + c.image + d.image;
    return res;
}

inline void multComplexEFFT_f(MyComplex_f a, int sizeFFT, int fi_deg, MyComplex_f *minus, MyComplex_f *plus, bool invers)
{
    qreal phi = -2.0*M_PI/sizeFFT*fi_deg;
    if(invers)
    {
        phi = 2.0*M_PI/sizeFFT*fi_deg;
    }
//    qreal real = qCos(phi);
//    qreal image = qSin(phi);

    qreal real = qFastCos(phi);
    qreal image = qFastSin(phi);

    minus->real = a.real * real - a.image * image;
    minus->image = a.image * real + a.real * image;

    plus->real = -minus->real;
    plus->image = -minus->image;
}

void multComplexEFFT(MyComplex a, int sizeFFT, int fi_deg, MyComplex *minus, MyComplex *plus, bool invers, CompensFP comp)
{
    qreal phi = -2.0*M_PI/sizeFFT*fi_deg;
    if(invers)
    {
        phi = 2.0*M_PI/sizeFFT*fi_deg;
    }
//    int real = qCos(phi)*32768;
//    int image = qSin(phi)*32768;

    int real = qFastCos(phi)*32768;
    int image = qFastSin(phi)*32768;



    minus->real = a.real * real - a.image * image;
    minus->image = a.image * real + a.real * image;

    if(comp == Compens)
    {
        //рабочий алгоритм
        if(minus->real > 0)
            minus->real += 16384;
        else
            minus->real -= 16384;

        if(minus->image > 0)
            minus->image += 16384;
        else
            minus->image -= 16384;
    }

    if(comp == AddBit)
    {
        //второй рабочий алгоритм
        minus->real = (minus->real)/16384;
        minus->image = (minus->image)/16384;
    }
    else
    {
        minus->real = (minus->real)/32768;
        minus->image = (minus->image)/32768;
    }



    plus->real = -minus->real;
    plus->image = -minus->image;
}

void doFFT(MyComplex *inputData, MyComplex *outputData, int size, bool invers, CompensFP comp)
{
    if(size%2 == 1)
    {
        return;
    }

    if(size > 2)
    {
        MyComplex *dataCHetn, *dataNCHetn;
        dataCHetn = new MyComplex [size/2];
        dataNCHetn = new MyComplex [size/2];

        for(int i = 0; i < size/2; i++)
        {
            dataCHetn[i].real = inputData[i*2].real;
            dataCHetn[i].image = inputData[i*2].image;

            dataNCHetn[i].real = inputData[i*2 + 1].real;
            dataNCHetn[i].image = inputData[i*2 + 1].image;

        }
        MyComplex *outputDataCHetn, *outputDataNCHetn;
        outputDataCHetn = new MyComplex [size/2];
        outputDataNCHetn = new MyComplex [size/2];

        MyComplex *plusFi, *minusFi;
        plusFi = new MyComplex [size/2];
        minusFi = new MyComplex [size/2];

        doFFT(dataCHetn, outputDataCHetn, size/2, invers, comp);
        doFFT(dataNCHetn, outputDataNCHetn, size/2, invers, comp);



        for(int i = 0; i < size/2; i++)//поворачиваем части
        {
            //второй рабочий алгоритм
            if(comp == AddBit)
            {
                outputDataCHetn[i].real = outputDataCHetn[i].real * 2;
                outputDataCHetn[i].image = outputDataCHetn[i].image * 2;
            }

            multComplexEFFT(outputDataNCHetn[i], size, i, &minusFi[i], &plusFi[i], invers, comp);
        }

        for(int i = 0; i < size/2; i++)
        {
            outputData[i] = summComplex(outputDataCHetn[i], minusFi[i]);//первая половина
            outputData[i+size/2] = summComplex(outputDataCHetn[i], plusFi[i]);//вторая половина
        }

#if USE_SCALE

        if(invers)//масштабирование
        {
            for(int i = 0; i < size; i++)
            {
                outputData[i].image = outputData[i].image/2;
                outputData[i].real = outputData[i].real/2;
            }
        }
#endif

        delete dataCHetn;
        delete dataNCHetn;
        delete outputDataCHetn;
        delete outputDataNCHetn;
        delete plusFi;
        delete minusFi;
    }
    else if (size == 2)
    {

        outputData[0] = summComplex(inputData[0], inputData[1]);
//        outputData[0] = summComplex(outputData[0], inputData[2]);
//        outputData[0] = summComplex(outputData[0], inputData[3]);
        MyComplex x2;
        x2.real = -inputData[1].real;
        x2.image = -inputData[1].image;
        outputData[1] = summComplex(inputData[0], x2);
#if USE_SCALE
        if(invers)
        {
            outputData[0].image = outputData[0].image/2;
            outputData[1].image = outputData[1].image/2;
            outputData[0].real = outputData[0].real/2;
            outputData[1].real = outputData[1].real/2;
        }
#endif
    }
}

void doFFT_f(MyComplex_f *inputData, MyComplex_f *outputData, int size, bool invers)
{
    if(((size-1)&size) != 0)//проверка на степень 2
    {
        return;
    }
//    if(size%2 == 1)
//    {
//        return;
//    }

    if(size > 2)
    {
        MyComplex_f *dataCHetn, *dataNCHetn;
        dataCHetn = new MyComplex_f [size >> 1];
        dataNCHetn = new MyComplex_f [size >> 1];

        for(int i = 0; i < (size >> 1); i++)
        {
            dataCHetn[i].real = inputData[i*2].real;
            dataCHetn[i].image = inputData[i*2].image;

            dataNCHetn[i].real = inputData[i*2 + 1].real;
            dataNCHetn[i].image = inputData[i*2 + 1].image;

        }
        MyComplex_f *outputDataCHetn, *outputDataNCHetn;
        outputDataCHetn = new MyComplex_f [size >> 1];
        outputDataNCHetn = new MyComplex_f [size >> 1];

        MyComplex_f *plusFi, *minusFi;
        plusFi = new MyComplex_f [size >> 1];
        minusFi = new MyComplex_f [size >> 1];

        doFFT_f(dataCHetn, outputDataCHetn, size/2, invers);
        doFFT_f(dataNCHetn, outputDataNCHetn, size/2, invers);



        for(int i = 0; i < size/2; i++)//поворачиваем части
        {
            multComplexEFFT_f(outputDataNCHetn[i], size, i, &minusFi[i], &plusFi[i], invers);
        }

        for(int i = 0; i < size/2; i++)
        {
            outputData[i] = summComplex_f(outputDataCHetn[i], minusFi[i]);//первая половина
            outputData[i+size/2] = summComplex_f(outputDataCHetn[i], plusFi[i]);//вторая половина
        }

#if USE_SCALE

        if(invers)//масштабирование
        {
            for(int i = 0; i < size; i++)
            {
                outputData[i].image = outputData[i].image/2;
                outputData[i].real = outputData[i].real/2;
            }
        }
#endif

        delete dataCHetn;
        delete dataNCHetn;
        delete outputDataCHetn;
        delete outputDataNCHetn;
        delete plusFi;
        delete minusFi;
    }
    else if (size == 2)
    {

        outputData[0] = summComplex_f(inputData[0], inputData[1]);
        MyComplex_f x2;
        x2.real = -inputData[1].real;
        x2.image = -inputData[1].image;
        outputData[1] = summComplex_f(inputData[0], x2);
#if USE_SCALE
        if(invers)
        {
            outputData[0].image = outputData[0].image/2;
            outputData[1].image = outputData[1].image/2;
            outputData[0].real = outputData[0].real/2;
            outputData[1].real = outputData[1].real/2;
        }
#endif
    }
}

void doFFTR4_f(MyComplex_f *inputData, MyComplex_f *outputData, int size, bool invers)
{
    if(((size-1)&size) != 0)//проверка на степень 2
    {
        return;
    }

    if((size%4) != 0)
    {
        return;
    }

    if(size > 4)
    {
        MyComplex_f *dataC0, *dataC1, *dataC2, *dataC3;
        dataC0 = new MyComplex_f [size >> 2];
        dataC1 = new MyComplex_f [size >> 2];
        dataC2 = new MyComplex_f [size >> 2];
        dataC3 = new MyComplex_f [size >> 2];




        delete dataC0;
        delete dataC1;
        delete dataC2;
        delete dataC3;
    }
    else if(size == 4)
    {
        outputData[0] = summComplex_R4_f(inputData[0], inputData[1], inputData[2], inputData[3]);
        MyComplex_f x2_1, x2_3;
        x2_1.real = -inputData[1].real;
        x2_1.image = -inputData[1].image;
        x2_3.real = -inputData[3].real;
        x2_3.image = -inputData[3].image;
        outputData[2] = summComplex_R4_f(inputData[0], x2_1, inputData[2], x2_3);

        MyComplex_f x13_2;
        x13_2.real = -inputData[2].real;
        x13_2.image = -inputData[2].image;

        MyComplex_f x1_1, x1_3;
        x1_1.real = inputData[1].image;
        x1_1.image = -inputData[1].real;
        x1_3.real = -inputData[3].image;
        x1_3.image = inputData[3].real;
        outputData[invers ? 3 : 1] = summComplex_R4_f(inputData[0], x1_1, x13_2, x1_3);

        x1_1.real = -inputData[1].image;
        x1_1.image = inputData[1].real;
        x1_3.real = inputData[3].image;
        x1_3.image = -inputData[3].real;
        outputData[invers ? 1 : 3] = summComplex_R4_f(inputData[0], x1_1, x13_2, x1_3);
    }
}
