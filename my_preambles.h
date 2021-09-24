#ifndef PREAMBLES_H
#define PREAMBLES_H

#include "common_ofdm.h"
#include <QVector>

extern int PA_I[];
extern int PA_Q[];

extern int PB_I[];
extern int PB_Q[];

extern MyComplex PA[];
extern MyComplex PB[];

extern QVector<MyComplex> preamleA;
extern QVector<MyComplex> preamleB;

void initPreambles();
bool findPreamble(QVector<MyComplex> preamble, MyComplex *iputData, uint32_t sizeInputData, MyComplex *outData/*почемуто не работает*/, uint32_t *sizeOutputData, MyComplex *outPreamble, QVector<MyComplex> *summPreamblesall, QVector<uint64_t> *dynamucPorog);
void resetPreamble();

#endif // PREAMBLES_H
