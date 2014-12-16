#include "mainwindow.h"
#include "filters.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    Filters f;

    //conects the button to the filter menu
    QObject::connect(&w, SIGNAL(chooseFilter()), &f, SLOT(show()));


    QObject::connect(&f, SIGNAL(applyFilter(float*)), &w, SLOT(on_applyFilter(float*)));

    w.show();



    return a.exec();
}
