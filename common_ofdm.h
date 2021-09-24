#ifndef COMMON_OFDM_H
#define COMMON_OFDM_H

#include <stdint.h>

typedef struct
{
    int64_t real;
    int64_t image;
}MyComplex;

typedef struct
{
    float real;
    float image;
}MyComplex_f;

typedef enum{
    BPSK = 0,
    QPSK = 1,
    QAM16 = 2,
    QAM64 = 3,
    QAM256 = 4,
}Modulation;

//TODO add compensation to define for faster FFT
typedef enum{
    None,
    Compens,
    AddBit,
}CompensFP;

typedef struct{
    Modulation modulation = QPSK;
    uint32_t sizeSymbolsDataOFDM = 10;
    //TODO сделать адаптивные
//    uint8_t FFT_N = 8;
    uint32_t CP_lenght = 8;//CP_lenght < (1 << FFT_N);
    CompensFP comp = AddBit;
}OFDM_Settings;

/* FFT_N | SIZE_FFT
 * 1     |  2
 * 2     |  4
 * 3     |  8
 * 4     |  16
 * 5     |  32
 * 6     |  64
 * 7     |  128
 * 8     |  256
 * 9     |  512
 * 10    |  1024
 * 11    |  2048
 * 12    |  4096
 * 13    |  8192
 * 14    |  16384
 * -------------
*/


#define FLOAT_COMPLEX_TO_INT_COMPLEX(f,i)       do{i.real = (int)f.real;i.image = (int)f.image;}while(0)
#define INT_COMPLEX_TO_FLOAT_COMPLEX(i,f)       do{f.real = (float)i.real;f.image = (float)i.image;}while(0)

//MAP modulation
/* BPSK  I   Q
 *  0    -1  0
 *  1    1   0
*/
/* QPSK  I(Q)
 *  0    -1
 *  1    1
*/
/* QAM16  I(Q)
 *  00    -3
 *  01    -1
 *  11    1
 *  10    3
*/
/* QAM64  I(Q)
 *  000   -7
 *  001   -5
 *  011   -3
 *  010   -1
 *  110   1
 *  111   3
 *  101   5
 *  100   7
*/
/* QAM256  I(Q)
 *  0001   -15
 *  0101   -13
 *  0111   -11
 *  0011   -9
 *  0010   -7
 *  0110   -5
 *  0100   -3
 *  0000   -1
 *  1000   1
 *  1100   3
 *  1110   5
 *  1010   7
 *  1011   9
 *  1111   11
 *  1101   13
 *  1001   15
*/

//scale modulation = val*2/sqrt(N)  N - point of modulation
#define BPSK__1                 -21.2132 /*-21.2132 = -1*15*2/sqrt(2)*/
#define BPSK_1                  21.2132 /*21.2132 = 1*15*2/sqrt(2)*/

#define QPSK__1                 -15 /*-15 = -1*15*2/sqrt(4)*/
#define QPSK_1                  15 /*15 = 1*15*2/sqrt(4)*/

#define QAM16__3                -22.5 /*-22.5 = -3*15*2/sqrt(16)*/
#define QAM16__1                -7.5 /*-7.5 = -1*15*2/sqrt(16)*/
#define QAM16_1                 7.5 /*7.5 = 1*15*2/sqrt(16)*/
#define QAM16_3                 22.5 /*722.5 = 3*15*2/sqrt(16)*/

#define QAM64__7                -26.25 /*-26.25 = -7*15*2/sqrt(64)*/
#define QAM64__5                -18.75 /*-18.75 = -5*15*2/sqrt(64)*/
#define QAM64__3                -11.25 /*-11.25 = -3*15*2/sqrt(64)*/
#define QAM64__1                -3.75 /*-3.75 = -1*15*2/sqrt(64)*/
#define QAM64_1                 3.75 /*3.75 = 1*15*2/sqrt(64)*/
#define QAM64_3                 11.25 /*11.25 = 3*15*2/sqrt(64)*/
#define QAM64_5                 18.75 /*18.75 = 5*15*2/sqrt(64)*/
#define QAM64_7                 26.25 /*26.25 = 7*15*2/sqrt(64)*/

#define QAM256__15              -28.125 /*-28.125 = -15*15*2/sqrt(256)*/
#define QAM256__13              -24.375 /*-24.375 = -13*15*2/sqrt(256)*/
#define QAM256__11              -20.625 /*-20.625 = -11*15*2/sqrt(256)*/
#define QAM256__9               -16.875 /*-16.875 = -9*15*2/sqrt(256)*/
#define QAM256__7               -13.125 /*-13.125 = -7*15*2/sqrt(256)*/
#define QAM256__5               -9.375 /*-9.375 = -5*15*2/sqrt(256)*/
#define QAM256__3               -5.625 /*-5.625 = -3*15*2/sqrt(256)*/
#define QAM256__1               -1.875 /*-1.875 = -1*15*2/sqrt(256)*/
#define QAM256_1                1.875 /*1.875 = 1*15*2/sqrt(256)*/
#define QAM256_3                5.625 /*5.625 = 3*15*2/sqrt(256)*/
#define QAM256_5                9.375 /*9.375 = 5*15*2/sqrt(256)*/
#define QAM256_7                13.125 /*13.125 = 7*15*2/sqrt(256)*/
#define QAM256_9                16.875 /*16.875 = 9*15*2/sqrt(256)*/
#define QAM256_11               20.625 /*20.625 = 11*15*2/sqrt(256)*/
#define QAM256_13               24.375 /*24.375 = 13*15*2/sqrt(256)*/
#define QAM256_15               28.125 /*28.125 = 15*15*2/sqrt(256)*/

//For demap modulation
#define DMP_QAM16__3            -15 /*-15 = QAM16__3 + step/2 = -22.5+7.5*/
#define DMP_QAM16__1            0 /*0 = QAM16__1 + step/2 = -7.5+7.5*/
#define DMP_QAM16_1             15 /*15 = QAM16_1 + step/2 = 7.5+7.5*/

#define DMP_QAM64__7            -22.5 /*-22.5 = QAM64__7 + step/2 = -26.25+3.75*/
#define DMP_QAM64__5            -15 /*-15 = QAM64__5 + step/2 = -22.5+3.75*/
#define DMP_QAM64__3            -7.5 /*-22.5 = QAM64__3 + step/2 = -11.25+3.75*/
#define DMP_QAM64__1            0 /*-22.5 = QAM64__1 + step/2 = -3.75+3.75*/
#define DMP_QAM64_1             7.5 /*-22.5 = QAM64_1 + step/2 = 3.75+3.75*/
#define DMP_QAM64_3             15 /*-22.5 = QAM64_3 + step/2 = 11.25+3.75*/
#define DMP_QAM64_5             22.5 /*-22.5 = QAM64_5 + step/2 = 18.75+3.75*/

#define DMP_QAM256__15          -26.25 /*-26.25 = QAM256__15 + step/2 = -28.125+1.875*/
#define DMP_QAM256__13          -22.5 /*-22.5 = QAM256__13 + step/2 = -24.375+1.875*/
#define DMP_QAM256__11          -18.75 /*-18.75 = QAM256__11 + step/2 = -20.625+1.875*/
#define DMP_QAM256__9           -15 /*-15 = QAM256__9 + step/2 = -16.875+1.875*/
#define DMP_QAM256__7           -11.25 /*-11.25 = QAM256__7 + step/2 = -13.125+1.875*/
#define DMP_QAM256__5           -7.5 /*-7.5 = QAM256__5 + step/2 = -9.375+1.875*/
#define DMP_QAM256__3           -3.75 /*-3.75 = QAM256__3 + step/2 = -5.625+1.875*/
#define DMP_QAM256__1           0 /*0 = QAM256__1 + step/2 = -1.875+1.875*/
#define DMP_QAM256_1            3.75 /*3.75 = QAM256_1 + step/2 = 1.875+1.875*/
#define DMP_QAM256_3            7.5 /*7.5 = QAM256_3 + step/2 = 5.625+1.875*/
#define DMP_QAM256_5            11.25 /*11.25 = QAM256_5 + step/2 = 9.375+1.875*/
#define DMP_QAM256_7            15 /*15 = QAM256_7 + step/2 = 13.125+1.875*/
#define DMP_QAM256_9            18.75 /*18.75 = QAM256_9 + step/2 = 16.875+1.875*/
#define DMP_QAM256_11           22.5 /*22.5 = QAM256_11 + step/2 = 20.625+1.875*/
#define DMP_QAM256_13           26.25 /*26.25 = QAM256_13 + step/2 = 24.375+1.875*/


//OFDM 256
#define PILOT_P13_I             QPSK_1
#define PILOT_P38_I             QPSK_1
#define PILOT_P63_I             QPSK_1
#define PILOT_P88_I             QPSK_1
#define PILOT_P_P88_I           QPSK_1
#define PILOT_P_P63_I           QPSK_1
#define PILOT_P_P38_I           QPSK_1
#define PILOT_P_P13_I           QPSK_1

#define PILOT_P13_Q             QPSK_1
#define PILOT_P38_Q             QPSK_1
#define PILOT_P63_Q             QPSK_1
#define PILOT_P88_Q             QPSK_1
#define PILOT_P_P88_Q           QPSK_1
#define PILOT_P_P63_Q           QPSK_1
#define PILOT_P_P38_Q           QPSK_1
#define PILOT_P_P13_Q           QPSK_1

#define PILOT_SYMBOL_SIZE       8

#define FFT_SIZE_OFDM256            256 //количество точек FFT
#define DATA_CARRIER_IN_OFDM256     192 //количество гармоник с информацией
#define MAX_DATA_IN_SYMBOL_SIZE     192 //максимальное колмчество байтов при самой многопозиционнгой модуляции

#define SIZE_ALL_PREAMBLE           512 //кличетво отсчетов в 2 преамбулах
#define SIZE_PREAMBLE_A             256
#define SIZE_PREAMBLE_B             256

#define MODULATION_FCH              QPSK /*модуляция символа информации*/

#define MAX_DATA_IN_FRAME_SIZE          10 /*максимальное количесто символов информации в OFDM кадру*/
#define MAX_CP_SIZE                 FFT_SIZE_OFDM256/4


#define FILTER_PARAM_PREAMBLE       32
#define USE_EQSY_PREAMBKE           1
#if USE_EQSY_PREAMBKE
#define MIN_POROG_PREAMBLE          1000000
#else
#define MIN_POROG_PREAMBLE          200000000
#endif


//масив писотных символов
extern int pilots_i_OFDM256[];
extern int pilots_q_OFDM256[];


#endif // COMMON_OFDM_H
