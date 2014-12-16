#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "filters.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void initImages();
    void showImageO(QImage m);
    void showImageM(QImage m);
    QImage paintImage(QImage imagem, int red, int green, int blue);
    QImage getModified();
    QString fileName;
    QRgb applyBright(int red, int green, int blue, int value);
    QRgb applyContrast(int red, int green, int blue, float value);
    void teste();
    void setModified(QImage m);


private slots:
    void on_openButton_clicked();  
    void on_saveButton_clicked();
    void on_vertFlipButton_clicked();

    void on_horizFlipButton_clicked();

    void on_grayScaleButton_clicked();

    void on_quantizeButton_clicked();



    void on_histogramButton_clicked();

    void on_brightButton_clicked();


    void on_contrastButton_clicked();

    void on_negativeButton_clicked();

    void on_equalizationButton_clicked();

    void on_filtersButton_clicked();


    void on_horaryRotButton_clicked();

    void on_antiHoraryRotButton_clicked();

    void on_zoomOutButton_clicked();

    void on_zoomInButton_clicked();

    void on_applyFilter(float * a);

private:
    Ui::MainWindow *ui;

    void setOriginal(QImage o);
    QImage modified;

    //Filters *filters;

signals:

    bool chooseFilter();

};

#endif // MAINWINDOW_H
