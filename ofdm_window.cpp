#include "ofdm_window.h"
#include "ui_ofdm_window.h"
#include <QDebug>
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>

#include "my_map_modulation.h"
#include "my_demap_modulation.h"
#include "my_ofdm_payload_gen.h"
#include "my_ofdm_payload_recyve.h"
#include "my_ofdm_frame_gen.h"
#include "my_ofdm_frame_recive.h"

#include "my_preambles.h"

OFDM_Window::OFDM_Window(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::OFDM_Window)
{
    ui->setupUi(this);

//    generatePrambles();

    initPreambles();

    initGraphic(ui->widget_otputGraph);
    initGraphic(ui->widget_plotPreamble);
    initGraphic(ui->widget_plotEqalizing);
    initGraphic(ui->widget_plotDemapSpector);
//    readTestData();

//    testVodulations();

//    testPayloadGen();

    testPayloadRecive();

    ui->comboBox_modulation->addItem("BPSK", BPSK);
    ui->comboBox_modulation->addItem("QPSK", QPSK);
    ui->comboBox_modulation->addItem("QAM16", QAM16);
    ui->comboBox_modulation->addItem("QAM64", QAM64);
    ui->comboBox_modulation->addItem("QAM256", QAM256);
}

OFDM_Window::~OFDM_Window()
{
    delete ui;
}

void OFDM_Window::readTestData()
{

}

void OFDM_Window::initGraphic(QWidget *widget)
{
    QCustomPlot *customPlot = (QCustomPlot *)widget;
    // set some pens, brushes and backgrounds:
    customPlot->xAxis->setBasePen(QPen(Qt::white, 1));
    customPlot->yAxis->setBasePen(QPen(Qt::white, 1));
    customPlot->xAxis->setTickPen(QPen(Qt::white, 1));
    customPlot->yAxis->setTickPen(QPen(Qt::white, 1));
    customPlot->xAxis->setSubTickPen(QPen(Qt::white, 1));
    customPlot->yAxis->setSubTickPen(QPen(Qt::white, 1));
    customPlot->xAxis->setTickLabelColor(Qt::white);
    customPlot->yAxis->setTickLabelColor(Qt::white);
    customPlot->xAxis->grid()->setPen(QPen(QColor(140, 140, 140), 1, Qt::DotLine));
    customPlot->yAxis->grid()->setPen(QPen(QColor(140, 140, 140), 1, Qt::DotLine));
    customPlot->xAxis->grid()->setSubGridPen(QPen(QColor(80, 80, 80), 1, Qt::DotLine));
    customPlot->yAxis->grid()->setSubGridPen(QPen(QColor(80, 80, 80), 1, Qt::DotLine));
    customPlot->xAxis->grid()->setSubGridVisible(true);
    customPlot->yAxis->grid()->setSubGridVisible(true);
    customPlot->xAxis->grid()->setZeroLinePen(Qt::NoPen);
    customPlot->yAxis->grid()->setZeroLinePen(Qt::NoPen);
//    customPlot->xAxis->setUpperEnding(QCPLineEnding::esSpikeArrow);
//    customPlot->yAxis->setUpperEnding(QCPLineEnding::esSpikeArrow);
    QLinearGradient plotGradient;
    plotGradient.setStart(0, 0);
    plotGradient.setFinalStop(0, 350);
    plotGradient.setColorAt(0, QColor(80, 80, 80));
    plotGradient.setColorAt(1, QColor(50, 50, 50));
    customPlot->setBackground(plotGradient);
//    QLinearGradient axisRectGradient;
//    axisRectGradient.setStart(0, 0);
//    axisRectGradient.setFinalStop(0, 350);
//    axisRectGradient.setColorAt(0, QColor(80, 80, 80));
//    axisRectGradient.setColorAt(1, QColor(30, 30, 30));
//    customPlot->axisRect()->setBackground(axisRectGradient);
    // Note: we could have also just called customPlot->rescaleAxes(); instead
    // Allow user to drag axis ranges with mouse, zoom with mouse wheel and select graphs by clicking:
    customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
}

void OFDM_Window::testVodulations()
{
    quint8 mas[8], masD[8];
    mas[0] = 0xB1;
    mas[1] = 0x5A;
    mas[2] = 0x75;
    mas[3] = 0x12;
    mas[4] = 0x78;
    mas[5] = 0x1D;
    mas[6] = 0x00;
    mas[7] = 0xAA;

    MyComplex_f outDdata[8];

    modulation_f(mas, outDdata, BPSK);
    demodulation_f(outDdata, masD, BPSK);

//    for(int i = 0; i < 8; i ++)
//    {
//        qDebug("[%d] = %f + %f*i", i, outDdata[i].real, outDdata[i].image);
//    }

    for(int i = 0; i < 8; i ++)
    {
//        qDebug("[%d] = %f + %f*i", i, outDdata[i].real, outDdata[i].image);
        qDebug("[%d] 0x%X; 0x%X", i, mas[i], masD[i]);
    }

//    _6x8byset_to_8x3bytes testl;
//    testl.mas[0].data[0] = 0xFF;
//    testl.mas[0].data[1] = 0x5A;
//    testl.mas[0].data[2] = 0x75;

//    testl.mas[1].data[0] = 0x12;
//    testl.mas[1].data[1] = 0x78;
//    testl.mas[1].data[2] = 0x1D;

////    for(int i = 0; i < 8; i ++)
////    {
////        qDebug("[%d] = %d + %d*i", i, testl.data.structData[i].real_0, testl.data.structData[i].image_0);
////    }

//    struct StructData dd;

//    qDebug() << sizeof(dd);
//    qDebug() << sizeof(testl);

//    qDebug("[%d] = %d + %d*i", 0, testl.structData[0].real_0, testl.structData[0].image_0);
//    qDebug("[%d] = %d + %d*i", 1, testl.structData[0].real_1, testl.structData[0].image_1);
//    qDebug("[%d] = %d + %d*i", 2, testl.structData[0].real_2, testl.structData[0].image_2);
//    qDebug("[%d] = %d + %d*i", 3, testl.structData[0].real_3, testl.structData[0].image_3);

//    qDebug("[%d] = %d + %d*i", 4, testl.structData[1].real_0, testl.structData[1].image_0);
//    qDebug("[%d] = %d + %d*i", 5, testl.structData[1].real_1, testl.structData[1].image_1);
//    qDebug("[%d] = %d + %d*i", 6, testl.structData[1].real_2, testl.structData[1].image_2);
//    qDebug("[%d] = %d + %d*i", 7, testl.structData[1].real_3, testl.structData[1].image_3);
}

void OFDM_Window::testPayloadGen()
{
    if(testData.size() < MAX_DATA_IN_SYMBOL_SIZE)
        return;


    OFDM_Settings settings;
    settings.CP_lenght = 0;
    settings.modulation = QAM256;
    settings.sizeSymbolsDataOFDM = 1;
    settings.comp = AddBit;

    MyComplex payloadGen[FFT_SIZE_OFDM256];
    genPayload(testData.data(), testData.size(), settings, payloadGen);

    for(int i = 0; i < FFT_SIZE_OFDM256; i++)
    {
        ui->textEdit->append(QString::asprintf("fft[%4d]=%6d  %6dj", i, payloadGen[i].real*2/256, payloadGen[i].image*2/256));
    }
}

void OFDM_Window::testPayloadRecive()
{
    if(testData.size() < MAX_DATA_IN_SYMBOL_SIZE)
        return;


    OFDM_Settings settings;
    settings.CP_lenght = 0;
    settings.modulation = QAM256;
    settings.sizeSymbolsDataOFDM = 1;
    settings.comp = AddBit;

    MyComplex payloadGen[FFT_SIZE_OFDM256];
    genPayloadData(testData.data(), testData.size(), settings, payloadGen);

    uint8_t dataRes[MAX_DATA_IN_SYMBOL_SIZE];

    uint8_t reciveSize = recyvePayloadData(payloadGen, settings, dataRes);

    for(int i = 0; i < reciveSize; i++)
    {
        ui->textEdit->append(QString::asprintf("fft[%4d]=%3d  %3d", i, testData[i], dataRes[i]));
    }
}

#include "my_math_operations.h"
//extern QVector<MyComplex> inputOldPreamble;
extern bool findPreamble(QVector<MyComplex> preamble, MyComplex *iputData, uint32_t sizeInputData, MyComplex *outData, uint32_t *sizeOutputData, MyComplex *outPreamble, QVector<MyComplex> *summPreamblesall, QVector<uint64_t> *dynamucPorog);
extern void getEqalizingData(MyComplex *inputData, MyComplex *pulotSumbolS);
extern void equalizing(MyComplex *inputData, MyComplex *outputData, QVector<MyComplex> *pulotSumbolS);
extern MyComplex summMultComplexConjArryaEasy(QVector<MyComplex> preamble, QVector<MyComplex> data, uint32_t sizePreabble);

void OFDM_Window::testPreamble(MyComplex *data, uint32_t size)
{
    resetRx();
    QVector<MyComplex> dataOFMDFrame;

    QVector<MyComplex> preamble = preamleA;


//    for(int i = 0; i < size; i++)
//    {
////        data[i].real = data[i].real/256/10;
////        data[i].image = data[i].image/256/10;
//        dataOFMDFrame.append(data[i]);
//    }

    for(int i = 0; i < 256; i++)
    {
        dataOFMDFrame.append(preamble[i]);
    }

//    dataOFMDFrame = divComplexonIntArray(dataOFMDFrame, 256);

    for(int i = 0; i < 200; i++)
    {
        dataOFMDFrame.prepend(dataOFMDFrame[0]);
        dataOFMDFrame.append(dataOFMDFrame[0]);
    }

//    getEqalizingData(dataOFMDFrame.data(), dataOFMDFrame.data());

    QVector<MyComplex> dataOFMDFrameAftePreamble;
    QVector<MyComplex> preambleSumm;
    QVector<uint64_t> preamblesPogod;
    MyComplex maxVal;
    dataOFMDFrameAftePreamble.resize(dataOFMDFrame.size());
    uint32_t sizeRxData;
    bool preambleFind;
//    uint8_t dataOut[4096];

//    OFDM_Settings settings;
//    settings.CP_lenght = 0;
//    settings.modulation = QAM256;
//    settings.sizeSymbolsDataOFDM = 1;
//    settings.comp = AddBit;

//    OFDM_recive(dataOFMDFrame.data(), dataOFMDFrame.size(), dataOut, settings);
    preambleFind = findPreamble(preamble, dataOFMDFrame.data(), dataOFMDFrame.size(), dataOFMDFrameAftePreamble.data(), &sizeRxData, &maxVal, &preambleSumm, &preamblesPogod);

    qDebug() << preambleFind << sizeRxData << dataOFMDFrame.size() << maxVal.real << maxVal.image;

    QVector<uint64_t> preambleABS = absComplexArray(preambleSumm);


    QVector<double> X, Y, Y2;
    for(int i = 0; i < preambleABS.size(); i++)
    {
        X.append(i);
        Y.append(preambleABS[i]);
        Y2.append(preamblesPogod[i]);
    }


    ui->widget_plotPreamble->clearGraphs();

    ui->widget_plotPreamble->addGraph();
    ui->widget_plotPreamble->addGraph();
    ui->widget_plotPreamble->graph(0)->setData(X, Y);
    ui->widget_plotPreamble->graph(1)->setData(X, Y2);

    ui->widget_plotPreamble->graph(0)->rescaleAxes();
    ui->widget_plotPreamble->graph(1)->rescaleAxes(true);
    ui->widget_plotPreamble->graph(1)->setPen(QPen(QColor(255, 110, 40)));

    ui->widget_plotPreamble->replot();
}

void OFDM_Window::testEuqlizing(MyComplex *data, uint32_t size, int cpLenght)
{
    if(size < (cpLenght + SIZE_ALL_PREAMBLE + FFT_SIZE_OFDM256))
        return;

    QVector<MyComplex> dataResive(size + 3);
    QVector<MyComplex> ray0(size);
    MyComplex *_ray0 = ray0.data();
    for(int i = 0; i < size; i++)
    {
        _ray0[i] = data[i];
    }

    QVector<MyComplex> ray1 = ray0;
    QVector<MyComplex> ray2 = ray0;
    QVector<MyComplex> ray3 = ray0;
    MyComplex zero = {0,0};
    ray0.append(zero);
    ray0.append(zero);
    ray0.append(zero);

    ray1.prepend(zero);
    ray1.append(zero);
    ray1.append(zero);

    ray2.prepend(zero);
    ray2.prepend(zero);
    ray2.append(zero);

    ray3.prepend(zero);
    ray3.prepend(zero);
    ray3.prepend(zero);

    MyComplex *_dataResive = dataResive.data();
    for(int i = 0; i < dataResive.size(); i++)
    {
        _dataResive[i].real = ray0.data()[i].real + ray1.data()[i].real*7/10 \
                + ray2.data()[i].real/2 + ray3.data()[i].real/5;

        _dataResive[i].image = ray0.data()[i].image + ray1.data()[i].image*7/10 \
                + ray2.data()[i].image/2 + ray3.data()[i].image/5;
    }


    QVector<MyComplex> pilotSumbols(FFT_SIZE_OFDM256);
    QVector<MyComplex> ferstSymbol(FFT_SIZE_OFDM256);
    QVector<MyComplex> eqalizData(FFT_SIZE_OFDM256);
    doFFT(&(dataResive.data()[SIZE_ALL_PREAMBLE+cpLenght]), ferstSymbol.data(), FFT_SIZE_OFDM256, false/*forvard FFT*/, None);
    getEqalizingData(ferstSymbol.data(), pilotSumbols.data());
    equalizing(ferstSymbol.data(), eqalizData.data(), &pilotSumbols);

    QVector<uint64_t> ferstFrameABS = absComplexArray(ferstSymbol);
    QVector<uint64_t> pilotABS = absComplexArray(pilotSumbols);

    QVector<double> X, Y1, Y2, Y3;
    for(int i = 0; i < /*FFT_SIZE_OFDM256*/128; i++)
    {
        X.append(i);
        Y1.append(ferstFrameABS[i]);
        Y2.append(pilotABS[i]);
        Y3.append(absComplex(eqalizData[i]));

        X.prepend(-i-1);
        Y1.prepend(ferstFrameABS[255-i]);
        Y2.prepend(pilotABS[255-i]);
        Y3.prepend(absComplex(eqalizData[255-i]));
    }

    ui->widget_plotEqalizing->clearGraphs();

    ui->widget_plotEqalizing->addGraph();
    ui->widget_plotEqalizing->addGraph(ui->widget_plotEqalizing->xAxis, ui->widget_plotEqalizing->yAxis2);
    ui->widget_plotEqalizing->addGraph();
    ui->widget_plotEqalizing->yAxis2->setVisible(true);

    ui->widget_plotEqalizing->graph(0)->setData(X, Y1);
    ui->widget_plotEqalizing->graph(1)->setData(X, Y2);
    ui->widget_plotEqalizing->graph(2)->setData(X, Y3);

    ui->widget_plotEqalizing->graph(0)->rescaleAxes();
    ui->widget_plotEqalizing->graph(1)->rescaleAxes();
    ui->widget_plotEqalizing->graph(1)->setPen(QPen(QColor(255, 110, 40)));
    ui->widget_plotEqalizing->graph(2)->setPen(QPen(QColor(120, 240, 30)));

    ui->widget_plotEqalizing->replot();

    //    recyvePayloadData()
}

void OFDM_Window::testReciveFrame(MyComplex *data, uint32_t size, uint32_t sizeBytes, OFDM_Settings settings)
{
    qDebug() << "********************************rx********************************";
    QVector<MyComplex> rxData;
    MyComplex demapData[10*FFT_SIZE_OFDM256];

    rxData.resize(size + 3);

    QVector<MyComplex> ray0(size);
    MyComplex *_ray0 = ray0.data();
    for(int i = 0; i < size; i++)
    {
        _ray0[i] = data[i];
    }

    QVector<MyComplex> ray1 = ray0;
    QVector<MyComplex> ray2 = ray0;
    QVector<MyComplex> ray3 = ray0;
    MyComplex zero = {0,0};
    ray0.append(zero);
    ray0.append(zero);
    ray0.append(zero);

    ray1.prepend(zero);
    ray1.append(zero);
    ray1.append(zero);

    ray2.prepend(zero);
    ray2.prepend(zero);
    ray2.append(zero);

    ray3.prepend(zero);
    ray3.prepend(zero);
    ray3.prepend(zero);

//    memcpy(rxData.data(), data, sizeof(MyComplex)*size);

    MyComplex *_dataResive = rxData.data();
    for(int i = 0; i < rxData.size(); i++)
    {
        _dataResive[i].real = ray0.data()[i].real + ray1.data()[i].real*7/10*0 \
                + ray2.data()[i].real/2*0 + ray3.data()[i].real/5*0;

        _dataResive[i].image = ray0.data()[i].image + ray1.data()[i].image*7/10*0 \
                + ray2.data()[i].image/2*0 + ray3.data()[i].image/5*0;
    }


    for(int i = 0; i < 200; i++)
        rxData.prepend(data[0]);

//    state = WaytFerstPreamble;
    uint8_t dd[4096];
//    inputOldPreamble.clear();
    resetRx();
//    qDebug() << "res frame:" << OFDM_recive(data, size, dd, settings);
    int resSize = OFDM_recive(rxData.data(), rxData.size(), dd, settings, demapData);
//    qDebug() << "res frame:" << resSize;
    ui->textEdit->append(QString::asprintf("Res frame=%d", resSize));
//    ui->textEdit->append(QString::asprintf("Tx    ::Rx    "));
    int errors = 0;
    uint8_t err;
    for(int i = 0; i < resSize; i++)
    {
//        ui->textEdit->append(QString::asprintf("0x%02X::0x%02X", testData.at(i), dd[i]));
        err = testData.at(i) ^ dd[i];
        for(int j = 0; j < 8; j++)
            errors += (err >> j) & 0x01;
    }

    ui->textEdit->append(QString::asprintf("Error bits %d/%d", errors, resSize*8));


    QVector<double> X, Y;

    for(int i = 0; i < (settings.sizeSymbolsDataOFDM*FFT_SIZE_OFDM256); i++)
    {
        X.append(demapData[i].real*1.0);
        Y.append(demapData[i].image*1.0);
    }



    ui->widget_plotDemapSpector->clearGraphs();

    ui->widget_plotDemapSpector->addGraph();
    ui->widget_plotDemapSpector->graph()->setLineStyle(QCPGraph::lsNone);
    ui->widget_plotDemapSpector->graph()->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 10));
    ui->widget_plotDemapSpector->graph()->setData(X, Y);

//    ui->widget_plotDemapSpector->graph()->rescaleAxes();
    ui->widget_plotDemapSpector->xAxis->setRange(-35,35);
    ui->widget_plotDemapSpector->yAxis->setRange(-35,35);

    ui->widget_plotDemapSpector->replot();

//    //time test
//    QTime timer(QTime::currentTime());
//    for(int i = 0; i < 100; i++)
//    {
//        resetRx();
//        OFDM_recive(rxData.data(), rxData.size(), dd, settings);
////        MyComplex summpreamble = summMultComplexConjArryaEasy(preamleA, rxData, 64);
////        QVector<MyComplex> multPreamble = multComplexConjArrya(preamleA, rxData, SIZE_PREAMBLE_A);
////        MyComplex summpreamble = summComplexArray(multPreamble);
////        summpreamble.real = summpreamble.real/1024;
////        summpreamble.image = summpreamble.image/1024;
//    }
//    int ms = timer.elapsed();
//    qDebug() << "Time use for 100 resivers " << ms << "ms";
}

void OFDM_Window::plotGrapsOFDM_frame(MyComplex *data, uint16_t size, OFDM_Settings settings)
{
    QCustomPlot *customPlot = ui->widget_otputGraph;
    customPlot->clearGraphs();
    //rectangle
    QVector<double> xp0(1), yp0(1);//ferst preamble
    QVector<double> xp1(1), yp1(1);//second preamble
    QVector<double> xpc(settings.sizeSymbolsDataOFDM+1), ypc(settings.sizeSymbolsDataOFDM+1);
    QVector<double> xdata(settings.sizeSymbolsDataOFDM+1), ydata(settings.sizeSymbolsDataOFDM+1);

    QVector<double> dataX, dataY_i, dataY_q;


    xp0[0] = 128;
    yp0[0] = 1;
    QCPBars *bars1 = new QCPBars(customPlot->xAxis, customPlot->yAxis2);
    bars1->setWidth(256);
    bars1->setData(xp0, yp0);
    bars1->setPen(Qt::NoPen);
    bars1->setBrush(QColor(184, 176, 0, 30));

    xp1[0] = 128+256;
    yp1[0] = 1;
    QCPBars *bars2 = new QCPBars(customPlot->xAxis, customPlot->yAxis2);
    bars2->setWidth(256);
    bars2->setData(xp1, yp1);
    bars2->setPen(Qt::NoPen);
    bars2->setBrush(QColor(251, 176, 0, 30));

    for(uint32_t i = 0; i < (settings.sizeSymbolsDataOFDM+1); i++)
    {
        xpc[i] = 512 + i*256 + 4 + 8*i;
        ypc[i] = 1;

        xdata[i] = 512 + 8*i + 128 + i*256 + 8;
        ydata[i] = 1;
    }

    QCPBars *bars3 = new QCPBars(customPlot->xAxis, customPlot->yAxis2);
    bars3->setWidth(8);
    bars3->setData(xpc, ypc);
    bars3->setPen(Qt::NoPen);
    bars3->setBrush(QColor(0, 119, 251, 30));

    QCPBars *bars4 = new QCPBars(customPlot->xAxis, customPlot->yAxis2);
    bars4->setWidth(256);
    bars4->setData(xdata, ydata);
    bars4->setPen(Qt::NoPen);
    bars4->setBrush(QColor(10, 140, 70, 30));


    for(int i = 0; i < size; i ++)
    {
        dataX.append(i);
        dataY_i.append(data[i].real);
        dataY_q.append(data[i].image);
    }
    // create and configure plottables:
    QCPGraph *graph1 = customPlot->addGraph();
    graph1->setData(dataX, dataY_i);
    graph1->setPen(QPen(QColor(40, 110, 255)));
    QCPGraph *graph2 = customPlot->addGraph();
    graph2->setData(dataX, dataY_q);
    graph2->setPen(QPen(QColor(255, 110, 40)));


    customPlot->graph(0)->rescaleAxes();
    customPlot->graph(1)->rescaleAxes(true);
    customPlot->yAxis2->setRange(0, 1);
    customPlot->replot();

    ui->checkBox_enData_I->setEnabled(true);
    ui->checkBox_enData_Q->setEnabled(true);
}


void OFDM_Window::on_pushButton_openData_clicked()
{
    QString file = QFileDialog::getOpenFileName(this, "Data file", ui->lineEdit_fileData->text(), tr("Text data (*.txt);;All file (*)"));
    if(!file.isEmpty())
    {
        ui->lineEdit_fileData->setText(file);
    }
}

void OFDM_Window::on_pushButton_openSaveFile_i_clicked()
{
    QString file = QFileDialog::getSaveFileName(this, "File data I", ui->lineEdit_fileSaveData_i->text(), tr("Text data (*.txt);;All file (*)"));
    if(!file.isEmpty())
    {
        ui->lineEdit_fileSaveData_i->setText(file);
    }
}

void OFDM_Window::on_pushButton_openSaveFile_q_clicked()
{
    QString file = QFileDialog::getSaveFileName(this, "File data Q", ui->lineEdit_fileSaveData_q->text(), tr("Text data (*.txt);;All file (*)"));
    if(!file.isEmpty())
    {
        ui->lineEdit_fileSaveData_q->setText(file);
    }
}

void OFDM_Window::on_pushButton_openResiveFile_i_clicked()
{
    QString file = QFileDialog::getOpenFileName(this, "File data I", ui->lineEdit_fileResiveData_i->text(), tr("Text data (*.txt);;All file (*)"));
    if(!file.isEmpty())
    {
        ui->lineEdit_fileResiveData_i->setText(file);
    }
}

void OFDM_Window::on_pushButton_openResiveFile_q_clicked()
{
    QString file = QFileDialog::getOpenFileName(this, "File data Q", ui->lineEdit_fileResiveData_q->text(), tr("Text data (*.txt);;All file (*)"));
    if(!file.isEmpty())
    {
        ui->lineEdit_fileResiveData_q->setText(file);
    }
}

void OFDM_Window::on_pushButton_readData_clicked()
{
    QFile fileRealData(ui->lineEdit_fileData->text());

    if(fileRealData.open(QIODevice::ReadOnly))
    {
        testData.clear();
        while(!fileRealData.atEnd())
        {
            QByteArray readData = fileRealData.readLine();

            readData = readData.mid(0, readData.length() - 1);//remove \n
            testData.append(readData.toUInt());
        }

        fileRealData.close();
        qDebug() << "read done" << testData.size();

        Modulation modulation = (Modulation)ui->comboBox_modulation->itemData(ui->comboBox_modulation->currentIndex()).toInt();
        uint32_t sizeSymbols = testData.size()/(getBytesInmodulation[modulation]*DATA_CARRIER_IN_OFDM256 >> 3);
        ui->spinBox_symbolsData->setMaximum(sizeSymbols);
    }
    else
    {
        qDebug() << "couldn't open file" << fileRealData.fileName();
        QMessageBox::critical(this, "Open file", "Can't open file " + fileRealData.fileName());
    }
}


void OFDM_Window::on_pushButton_saveModData_clicked()
{
    QFile fileData_i(ui->lineEdit_fileSaveData_i->text());

    if(fileData_i.open(QIODevice::WriteOnly))
    {
        for(int i = 0; i < frameOFDM.size(); i++)
        {
//            fileData_i.write(QByteArray::number(frameOFDM[i].real*2/256));
//            if(i < (frameOFDM.size()-1))
//                fileData_i.write("\n", 1);
            int32_t data = frameOFDM[i].real;
            uint8_t dara[4] = {data & 0xFF, (data >> 8) & 0xFF, (data >> 16) & 0xFF, (data >> 24) & 0xFF};
            fileData_i.write((char*)dara, 4);
        }
    }
    else
    {
        QMessageBox::critical(this, "File write I data", "Can't open file " + fileData_i.fileName());
    }

    QFile fileData_q(ui->lineEdit_fileSaveData_q->text());

    if(fileData_q.open(QIODevice::WriteOnly))
    {
        for(int i = 0; i < frameOFDM.size(); i++)
        {
//            fileData_q.write(QByteArray::number(frameOFDM[i].image*2/256));
//            if(i < (frameOFDM.size()-1))
//                fileData_q.write("\n", 1);

            int32_t data = frameOFDM[i].image;
            uint8_t dara[4] = {data & 0xFF, (data >> 8) & 0xFF, (data >> 16) & 0xFF, (data >> 24) & 0xFF};
            fileData_q.write((char*)dara, 4);
        }
    }
    else
    {
        QMessageBox::critical(this, "File write Q data", "Can't open file " + fileData_q.fileName());
    }
}

void OFDM_Window::on_comboBox_modulation_currentIndexChanged(int index)
{
    Q_UNUSED(index);
    Modulation modulation = (Modulation)ui->comboBox_modulation->itemData(ui->comboBox_modulation->currentIndex()).toInt();
    uint32_t sizeSymbols = testData.size()/(getBytesInmodulation[modulation]*DATA_CARRIER_IN_OFDM256 >> 3);
    ui->spinBox_symbolsData->setMaximum(sizeSymbols);
}

void OFDM_Window::on_pushButton_clicked()
{
    OFDM_Settings settings;
    settings.comp = AddBit;
    settings.CP_lenght = 8;
    settings.modulation = (Modulation)ui->comboBox_modulation->itemData(ui->comboBox_modulation->currentIndex()).toInt();
    settings.sizeSymbolsDataOFDM = ui->spinBox_symbolsData->value();
    uint32_t sizeOutputData = sizeOutputDataForFrame(settings);

    frameOFDM.resize(sizeOutputData);
    ui->textEdit->clear();
    ui->textEdit->append(QString::asprintf("Size sample in %d data symbols is %d", settings.sizeSymbolsDataOFDM, sizeOutputData));

    uint32_t genData_size = genFramOFDM(testData.data(), testData.size(), settings, frameOFDM.data());
    ui->textEdit->append(QString::asprintf("Res size=%d", genData_size));

    plotGrapsOFDM_frame(frameOFDM.data(), genData_size, settings);

    testPreamble(frameOFDM.data(), genData_size);

    testEuqlizing(frameOFDM.data(), genData_size, settings.CP_lenght);

    testReciveFrame(frameOFDM.data(), genData_size, sizeInputDataForFrame(settings), settings);
}

void OFDM_Window::on_checkBox_enData_I_clicked(bool checked)
{
    ui->widget_otputGraph->graph(0)->setVisible(checked);
    ui->widget_otputGraph->replot();
}

void OFDM_Window::on_checkBox_enData_Q_clicked(bool checked)
{
    ui->widget_otputGraph->graph(1)->setVisible(checked);
    ui->widget_otputGraph->replot();
}

void OFDM_Window::on_pushButton_testReciveData_clicked()
{
    OFDM_Settings settings;
    settings.comp = AddBit;
    settings.CP_lenght = 8;
    settings.modulation = (Modulation)ui->comboBox_modulation->itemData(ui->comboBox_modulation->currentIndex()).toInt();
    settings.sizeSymbolsDataOFDM = ui->spinBox_symbolsData->value();

    QVector <MyComplex> readFrameOFDM;
    MyComplex tmp;

    QFile fileData_i(ui->lineEdit_fileResiveData_i->text());
    QFile fileData_q(ui->lineEdit_fileResiveData_q->text());


    if(fileData_i.open(QIODevice::ReadOnly) && fileData_q.open(QIODevice::ReadOnly))
    {
        while((!fileData_i.atEnd()) && (!fileData_i.atEnd()))
        {
            uint8_t dara[4] = {/*data & 0xFF, (data >> 8) & 0xFF*/0};
//            fileData_i.read((char*)dara, 4);
            QByteArray daraB = fileData_i.read(4);
            int32_t data = (uint8_t)daraB[0] | (((uint8_t)daraB[1]) << 8) | (((uint8_t)daraB[2]) << 16) | (((uint8_t)daraB[3]) << 24);
            tmp.real = data >= 0 ? data : data | 0xFFFFFFFF00000000;

//            fileData_q.read((char*)dara, 4);
            daraB = fileData_q.read(4);
            data = (uint8_t)daraB[0] | (((uint8_t)daraB[1]) << 8) | (((uint8_t)daraB[2]) << 16) | (((uint8_t)daraB[3]) << 24);
            tmp.image = data >= 0 ? data : data | 0xFFFFFFFF00000000;


            readFrameOFDM.append(tmp);
        }
    }
    else
    {
        QMessageBox::critical(this, "File read I Q data", "Can't open file");
        return ;
    }


    testReciveFrame(readFrameOFDM.data(), readFrameOFDM.size(), sizeInputDataForFrame(settings), settings);
}


/********************************************************************************/

#include <QtGlobal>
#include <time.h>

#define GENERATE_PREAMBLE_RAND          1

int randBetven(int low, int hight)
{
    return (qrand() % ((hight + 1) - low)) + low;
}

void OFDM_Window::generatePrambles()
{
    //generate from https://ieee802.org/16/tg3_4/contrib/80216abc-01_39.pdf
    //но авторорреляционная функция получилась не очень и прям видно 4 переодичности
    float data_I_PA[256] = {
        0//0
        ,0,0,0,-1,0,0,0,-1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,//26

        //203 0
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,

        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,


        //-26
        0,0,1,0,0,0,-1,0,0,0,1,0,0,0,-1,0,0,0,-1,0,0,0,1,0,0,0//-1
    }/**sqrt(13/6)*/;

#if GENERATE_PREAMBLE_RAND
    //v2
    qsrand(time(NULL));
    float data_Q_PA[256] = {0};
    for(int i = 4; i < 76; i+=1/*4*/)
    {
        data_I_PA[i] = randBetven(0, 1) ? 1 : -1;
        data_Q_PA[i] = randBetven(0, 1) ? 1 : -1;
    }

    for(int i = 180; i < (256-3); i+=1/*4*/)
    {
        data_I_PA[i] = randBetven(0, 1) ? 1 : -1;
        data_Q_PA[i] = randBetven(0, 1) ? 1 : -1;
    }
#endif

    MyComplex_f pramble[256];
    for(int i = 0; i < 256; i++)
    {
        pramble[i].real = data_I_PA[i]*1.471960144;
        pramble[i].image = data_I_PA[i]*1.471960144;
#if GENERATE_PREAMBLE_RAND
        //v2
        pramble[i].image = data_Q_PA[i]*1.471960144;
#endif
    }
    MyComplex_f preambleSymbols[256];
    doFFT_f(pramble, preambleSymbols, 256, true);
    //write to file
    QFile file;

    //for CPP
    file.setFileName("E:\\Project\\Project_by_qt\\testOFDM\\PA.cpp");
    if(file.open(QIODevice::WriteOnly))
    {
        file.write("\n\n\n");
        file.write("int PA_I[256] = {\n");
        for(int i = 0; i < 256; i++)
        {
            file.write(QByteArray::number((int)(preambleSymbols[i].real*256)));
            if(i < 255)
                file.write(",\n");
        }
        file.write("\n};\n\n\n");

        file.write("int PA_Q[256] = {\n");
        for(int i = 0; i < 256; i++)
        {
            file.write(QByteArray::number((int)(preambleSymbols[i].image*256)));
            if(i < 255)
                file.write(",\n");
        }
        file.write("\n};\n\n\n");

        file.close();
    }
    //FOR FPGA
    file.setFileName("E:\\Project\\Project_by_qt\\testOFDM\\PA_I.mem");
    if(file.open(QIODevice::WriteOnly))
    {
        for(int i = 0; i < 256; i++)
        {
            file.write(QByteArray::number((int)(preambleSymbols[i].real*256), 16).right(4).rightJustified(4, '0'));
            if(i < 255)
                file.write("\n");
        }
        file.close();
    }
    file.setFileName("E:\\Project\\Project_by_qt\\testOFDM\\PA_Q.mem");
    if(file.open(QIODevice::WriteOnly))
    {
        for(int i = 0; i < 256; i++)
        {
            file.write(QByteArray::number((int)(preambleSymbols[i].image*256), 16).right(4).rightJustified(4, '0'));
            if(i < 255)
                file.write("\n");
        }
        file.close();
    }
    //FOR MATLAB
    file.setFileName("E:\\Project\\Project_by_qt\\testOFDM\\PA_I.txt");
    if(file.open(QIODevice::WriteOnly))
    {
        for(int i = 0; i < 256; i++)
        {
            file.write(QByteArray::number((int)(preambleSymbols[i].real*256)));
            if(i < 255)
                file.write(",\n");
        }
        file.close();
    }
    file.setFileName("E:\\Project\\Project_by_qt\\testOFDM\\PA_Q.txt");
    if(file.open(QIODevice::WriteOnly))
    {
        for(int i = 0; i < 256; i++)
        {
            file.write(QByteArray::number((int)(preambleSymbols[i].image*256)));
            if(i < 255)
                file.write(",\n");
        }
        file.close();
    }
    //FOR FPGA RX
    file.setFileName("E:\\Project\\Project_by_qt\\testOFDM\\PA_I.mmm");
    if(file.open(QIODevice::WriteOnly))
    {
        file.write("256'b");
        for(int i = 0; i < 256; i++)
        {
            file.write(QByteArray::number((int)((preambleSymbols[i].real*256) > 0)));
        }
        file.write("\n");
        file.close();
    }
    file.setFileName("E:\\Project\\Project_by_qt\\testOFDM\\PA_Q.mmm");
    if(file.open(QIODevice::WriteOnly))
    {
        file.write("256'b");
        for(int i = 0; i < 256; i++)
        {
            file.write(QByteArray::number((int)((preambleSymbols[i].image*256) > 0)));
        }
        file.write("\n");
        file.close();
    }



    //preamble B

    //generate from https://ieee802.org/16/tg3_4/contrib/80216abc-01_39.pdf
    //но авторорреляционная функция получилась тоже не очень
    float data_I_PB[256] = {
        0,//0
        0,1,0,-1,0,1,0,-1,0,1,0,-1,0,-1,0,1,0,-1,0,-1,0,1,0,1,0,1,0,1,0,1,0,-1,0,-1,0,-1,0,1,0,-
        1,0,1,0,-1,0,-1,0,-1,0,1,0,1,0,1,0,-1,0,-1,0,-1,0,-1,0,1,0,1,0,1,0,1,0,-1,0,1,0,-1,0,-1,0,-1,0,1,0,1,0,-
        1,0,1,0,1,0,1,0,-1,0,-1,0,-1,0,1,//100
        //x55 of 0
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        //-100
        1,0,1,0,1,0,1,0,-1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,-1,0,1,0,1,0,-1,0,-1,0,-1,0,1,0,-1,0,1,0,-
        1,0,1,0,1,0,1,0,-1,0,1,0,1,0,1,0,-1,0,-1,0,-1,0,-1,0,1,0,1,0,-1,0,1,0,1,0,-1,0,1,0,-1,0,-1,0,-1,0,1,0,-1,0,-
        1,0,1,0,1,0,1,0,-1,0//-1
    }/**sqrt(2)*/;

#if GENERATE_PREAMBLE_RAND
    float data_Q_PB[256] = {0};
    for(int i = 2; i < 100; i+=1/*2*/)
    {
        data_I_PB[i] = randBetven(0, 1) ? 1 : -1;
        data_Q_PB[i] = randBetven(0, 1) ? 1 : -1;
    }

    for(int i = 156; i < 255; i+=1/*2*/)
    {
        data_I_PB[i] = randBetven(0, 1) ? 1 : -1;
        data_Q_PB[i] = randBetven(0, 1) ? 1 : -1;
    }
#endif

    for(int i = 0; i < 256; i++)
    {
        pramble[i].real = data_I_PB[i]*1.414;
        pramble[i].image = 0;

#if GENERATE_PREAMBLE_RAND
        pramble[i].image = data_Q_PB[i]*1.414;
#endif
    }

    doFFT_f(pramble, preambleSymbols, 256, true);
    //write to file
    //FOR CPP
    file.setFileName("E:\\Project\\Project_by_qt\\testOFDM\\PB.cpp");
    if(file.open(QIODevice::WriteOnly))
    {
        file.write("\n\n\n");
        file.write("int PB_I[256] = {\n");
        for(int i = 0; i < 256; i++)
        {
            file.write(QByteArray::number((int)(preambleSymbols[i].real*256)));
            if(i < 255)
                file.write(",\n");
        }
        file.write("\n};\n\n\n");

        file.write("int PB_Q[256] = {\n");
        for(int i = 0; i < 256; i++)
        {
            file.write(QByteArray::number((int)(preambleSymbols[i].image*256)));
            if(i < 255)
                file.write(",\n");
        }
        file.write("\n};\n\n\n");

        file.close();
    }
    //FOR FPGA
    file.setFileName("E:\\Project\\Project_by_qt\\testOFDM\\PB_I.mem");
    if(file.open(QIODevice::WriteOnly))
    {
        for(int i = 0; i < 256; i++)
        {
            file.write(QByteArray::number((int)(preambleSymbols[i].real*256), 16).right(4).rightJustified(4, '0'));
            if(i < 255)
                file.write("\n");
        }
        file.close();
    }
    file.setFileName("E:\\Project\\Project_by_qt\\testOFDM\\PB_Q.mem");
    if(file.open(QIODevice::WriteOnly))
    {
        for(int i = 0; i < 256; i++)
        {
            file.write(QByteArray::number((int)(preambleSymbols[i].image*256), 16).right(4).rightJustified(4, '0'));
            if(i < 255)
                file.write("\n");
        }
        file.close();
    }
    //FOR MATLAB
    file.setFileName("E:\\Project\\Project_by_qt\\testOFDM\\PB_I.txt");
    if(file.open(QIODevice::WriteOnly))
    {
        for(int i = 0; i < 256; i++)
        {
            file.write(QByteArray::number((int)(preambleSymbols[i].real*256)));
            if(i < 255)
                file.write(",\n");
        }
        file.close();
    }
    file.setFileName("E:\\Project\\Project_by_qt\\testOFDM\\PB_Q.txt");
    if(file.open(QIODevice::WriteOnly))
    {
        for(int i = 0; i < 256; i++)
        {
            file.write(QByteArray::number((int)(preambleSymbols[i].image*256)));
            if(i < 255)
                file.write(",\n");
        }
        file.close();
    }
    //FOR FPGA RX
    file.setFileName("E:\\Project\\Project_by_qt\\testOFDM\\PB_I.mmm");
    if(file.open(QIODevice::WriteOnly))
    {
        file.write("256'b");
        for(int i = 0; i < 256; i++)
        {
            file.write(QByteArray::number((int)((preambleSymbols[i].real*256) > 0)));
        }
        file.write("\n");
        file.close();
    }
    file.setFileName("E:\\Project\\Project_by_qt\\testOFDM\\PB_Q.mmm");
    if(file.open(QIODevice::WriteOnly))
    {
        file.write("256'b");
        for(int i = 0; i < 256; i++)
        {
            file.write(QByteArray::number((int)((preambleSymbols[i].image*256) > 0)));
        }
        file.write("\n");
        file.close();
    }
}

