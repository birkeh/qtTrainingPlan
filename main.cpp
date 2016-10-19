#include <QApplication>
#include <QTranslator>
#include "mainwindow.h"
#include "common.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QString locale = QLocale::system().name();

    QTranslator translator;
    translator.load(QString("qtTrainingPlan_") + locale);
    //translator.load(QString("qtTrainingPlan_de"));
    a.installTranslator(&translator);

    MainWindow w;
    w.show();
    return a.exec();
}
