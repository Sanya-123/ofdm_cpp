#include "ofdm_window.h"
#include <QApplication>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    OFDM_Window w;
    w.show();


    return a.exec();
}
