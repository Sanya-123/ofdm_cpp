#ifndef MY_MATH_OPERATIONS_H
#define MY_MATH_OPERATIONS_H

#include <QObject>
#include "common_ofdm.h"
#include <QVector>
#include <QtMath>

#define DIFF_MODIFF                 0

inline MyComplex multComplex(MyComplex a, MyComplex b)
{
    MyComplex res;
    res.real = a.real * b.real + a.image * b.image;
    res.image = a.image * b.real + a.real * b.image;
    return res;
}

inline QVector<MyComplex> multComplexArrya(QVector<MyComplex> a, QVector<MyComplex> b, int sizeData)
{
    QVector<MyComplex> res(sizeData);
    MyComplex *data = res.data();
    for(int i = 0; i < sizeData; i++)
    {
        data[i] = multComplex(a[i], b[i]);
    }
    return res;
}


inline MyComplex multComplexConj(MyComplex a, MyComplex b)
{
    MyComplex res;
    res.real = a.real * b.real + a.image * b.image;
    res.image = a.image * b.real - a.real * b.image;
    return res;
}

inline QVector<MyComplex> multComplexConjArrya(QVector<MyComplex> a, QVector<MyComplex> b, int sizeData)
{
    QVector<MyComplex> res(sizeData);
    MyComplex *data = res.data();
    for(int i = 0; i < sizeData; i++)
    {
        data[i] = multComplexConj(a[i], b[i]);
    }

    return res;
}

inline MyComplex summComplexArray(QVector<MyComplex> a)
{
    MyComplex res;
    res = a[0];

    for(int i = 1; i < a.size(); i++)
    {
        res.real = res.real + a[i].real;
        res.image = res.image + a[i].image;
    }

    return res;
}

inline uint64_t absComplex(MyComplex a)
{
    return qSqrt(a.real*a.real + a.image*a.image);
}

inline QVector<uint64_t> absComplexArray(QVector<MyComplex> a)
{
    QVector<uint64_t> res(a.size());
    uint64_t *data = res.data();
    for(int i = 0; i < a.size(); i++)
    {
        data[i] = absComplex(a[i]);
    }
    return res;
}

inline MyComplex divComplex(MyComplex a, MyComplex b)
{
    MyComplex res = multComplexConj(a, b);
#if DIFF_MODIFF
    res.real = ((res.real*2)/(b.real*b.real + b.image*b.image) + 1)/2;
    res.image= ((res.image*2)/(b.real*b.real + b.image*b.image) + 1)/2;
#else
    res.real = res.real/(b.real*b.real + b.image*b.image);
    res.image= res.image/(b.real*b.real + b.image*b.image);
#endif
    return res;
}

inline QVector<MyComplex> divComplexAray(QVector<MyComplex> a, QVector<MyComplex> b, int sizeData)
{
    QVector<MyComplex> res(sizeData);
    MyComplex *data = res.data();
    for(int i = 0; i < sizeData; i++)
    {
        data[i] = divComplex(a[i], b[i]);
    }
    return res;
}

inline MyComplex divComplexonInt(MyComplex a, int64_t b)
{
    MyComplex res;
#if DIFF_MODIFF
    res.real = ((a.real*2)/b + 1)/2;
    res.image= ((a.image*2)/b + 1)/2;
#else
    res.real = a.real/b;
    res.image= a.image/b;
#endif
    return res;
}

inline QVector<MyComplex> divComplexonIntArray(QVector<MyComplex> a, int64_t b)
{
    QVector<MyComplex> res(a.size());
    MyComplex *data = res.data();
    for(int i = 0; i < a.size(); i++)
    {
        data[i] = divComplexonInt(a[i], b);
    }
    return res;
}

inline MyComplex addPhaseComplex(MyComplex a, float phi_rad)
{
    float _cos = qCos(phi_rad);
    float _sin = qSin(phi_rad);
    MyComplex res;
    res.real = a.real * _cos - a.image * _sin;
    res.image = a.image * _cos + a.real * _sin;
    return res;
}


#endif // MY_MATH_OPERATIONS_H
