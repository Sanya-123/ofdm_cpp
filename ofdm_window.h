#ifndef OFDM_WINDOW_H
#define OFDM_WINDOW_H

#include <QMainWindow>
#include <QVector>
#include "common_ofdm.h"

namespace Ui {
class OFDM_Window;
}

class OFDM_Window : public QMainWindow
{
    Q_OBJECT

public:
    explicit OFDM_Window(QWidget *parent = 0);
    ~OFDM_Window();

public slots:
    void plotGrapsOFDM_frame(MyComplex *data, uint16_t size, OFDM_Settings settings);

private:
    void readTestData();
    void initGraphic(QWidget *widget);
    void generatePrambles();

private slots:
    void testVodulations();
    void testPayloadGen();
    void testPayloadRecive();
    void testPreamble(MyComplex *data, uint32_t size);
    void testEuqlizing(MyComplex *data, uint32_t size, int cpLenght);
    void testReciveFrame(MyComplex *data, uint32_t size, uint32_t sizeBytes, OFDM_Settings settings);

    void on_pushButton_openData_clicked();

    void on_pushButton_openSaveFile_i_clicked();

    void on_pushButton_openSaveFile_q_clicked();

    void on_pushButton_readData_clicked();

    void on_comboBox_modulation_currentIndexChanged(int index);

    void on_pushButton_clicked();

    void on_checkBox_enData_I_clicked(bool checked);

    void on_checkBox_enData_Q_clicked(bool checked);

    void on_pushButton_saveModData_clicked();

    void on_pushButton_openResiveFile_i_clicked();

    void on_pushButton_openResiveFile_q_clicked();

    void on_pushButton_testReciveData_clicked();

private:
    Ui::OFDM_Window *ui;
    QVector <uint8_t> testData;
    QVector <MyComplex> frameOFDM;
};

#endif // OFDM_WINDOW_H
