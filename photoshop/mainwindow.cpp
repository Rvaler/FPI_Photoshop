#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QImage>
#include <string>
#include <QString>
#include <QFile>
#include <QtCore>
#include <QtGui>
#include <QGraphicsScene>
#include <QFileDialog>
#include <QMessageBox>
#include <iostream>

using namespace std;
QString fileLocal;
int oldBrigthValue = 0;
int oldContrastValue = 0;



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

// actions

void MainWindow::initImages()
{
    QWidget window;
    window.resize(320, 240);
    window.show();
    QImage original (fileLocal);
    QImage modified (original.width(), original.height(), QImage::Format_RGB32);

    showImageO(original);

    //copy the content of an original image to a another one modified
    for (int i = 0; i < original.width(); i++){
        for (int j = 0; j < original.height();j++)
        {
            QRgb orValue = original.pixel(i,j);
            modified.setPixel(i,j,orValue);
        }
    }

    showImageM(modified);

    setModified(modified);
}

void MainWindow::showImageO(QImage m)
{
    QImage resized;
    if (ui->fitCheck->isChecked()==1)
        resized = m.scaled (ui->modifiedGV->width()-3,ui->modifiedGV->height()-3,Qt::KeepAspectRatio);
    else
        resized = m;

    QGraphicsScene *toBeShown = new QGraphicsScene();
    toBeShown->addPixmap(QPixmap::fromImage(resized));
    ui->originalGV->setScene(toBeShown);
}

void MainWindow::showImageM(QImage m)
{
    QImage resized;

    if (ui->fitCheck->isChecked()==1)
        resized = m.scaled (ui->modifiedGV->width()-3,ui->modifiedGV->height()-3,Qt::KeepAspectRatio);
    else

        resized = m;

    QGraphicsScene *toBeShown = new QGraphicsScene();
    toBeShown->addPixmap(QPixmap::fromImage(resized));
    ui->modifiedGV->setScene(toBeShown);
}

// buttons

void MainWindow::on_openButton_clicked()
{
    fileLocal = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                     "/home",
                                                     tr("Images (*.png *.xpm *.jpg)"));
    initImages();
}

void MainWindow::on_saveButton_clicked()
{
    QImage modifiedImage = getModified();
   // QString fileName = "";
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"),
                                (fileLocal.remove(fileLocal.length()-4,fileLocal.length()-1).append("_modified.jpg")),
                                tr("Images ( *.jpg)"));
    modifiedImage.save(fileName, "jpg");
//QMessageBox::information(this,"title",fileName);
}

void MainWindow::on_vertFlipButton_clicked()
{

    QImage modifiedImage = getModified();
    QRgb auxPixel;
    QImage auxImg = getModified();
    int x, y;

    for(x = 0; x < modifiedImage.width() - 1; x++){
        for(y = 0; y < modifiedImage.height() - 1; y++){

            auxPixel = modifiedImage.pixel(x,y);
            auxImg.setPixel(x,(auxImg.height() - y - 1), auxPixel);
        }
    }

    showImageM(auxImg);
    setModified(auxImg);
}

void MainWindow::on_horizFlipButton_clicked()
{
    QImage modifiedImage = getModified();
    QImage auxiliarImage = getModified();
    QRgb auxPixel;

    int x, y;

    for(x = 0; x < modifiedImage.width() - 1; x++ ){
        for(y = 0; y < modifiedImage.height() -1; y++){
               auxPixel = modifiedImage.pixel(x,y);
               auxiliarImage.setPixel(modifiedImage.width() -x -1, y , auxPixel);
        }
    }
    showImageM(auxiliarImage);
    setModified(auxiliarImage);
}

void MainWindow::on_grayScaleButton_clicked()
{
    ui->quantizeButton->setEnabled(true);
    ui->quantizeValue->setEnabled(true);
    QImage modifiedImage = getModified();
    QRgb pixel, grayPixel;
    int gray;
    for(int linha = 0; linha < modifiedImage.height() - 1; linha++){
        for(int coluna = 0; coluna < modifiedImage.width() - 1; coluna++){
            pixel = modifiedImage.pixel(coluna,linha);
            gray = qRed(pixel)*0.299 + qGreen(pixel)*0.114 + qBlue(pixel)*0.587;
            grayPixel = qRgb(gray, gray, gray);
            modifiedImage.setPixel(coluna,linha,grayPixel);
        }
    }
    setModified(modifiedImage);
    showImageM(modifiedImage);
}


void MainWindow::on_quantizeButton_clicked()
{
    int nShades = ui->quantizeValue->value();
    int red, green, blue;
    QRgb pixel, newPixel;


    QImage modifiedImage = getModified();
    if(nShades > 0 && nShades <= 256){
        for (int y = 0; y < modifiedImage.height()-1; y++){
                   for (int x = 0; x < modifiedImage.width()-1; x++)
                   {
                       pixel = modifiedImage.pixel(x,y);

                       red = qRed (pixel);
                       green = qGreen (pixel);
                       blue = qBlue (pixel);

                       red = (red/(255/nShades))*(255/nShades/2);
                       green = (green/(255/nShades))*(255/nShades/2);
                       blue = (blue/(255/nShades))*(255/nShades/2);

                       newPixel = qRgb (red,green,blue);
                       modifiedImage.setPixel(x,y,newPixel);
                   }
        }
        setModified(modifiedImage);
        showImageM(modifiedImage);

    }else{
        QMessageBox::information(this,"title","Type values between 1 and 256");
    }
}

void MainWindow::on_histogramButton_clicked()
{
    on_grayScaleButton_clicked();
    QRgb pixel;
    QImage modifiedImage = getModified();
    int grayScale, max = 0;
    std::vector<int> vetor_cores(256, 0);
    for(int y = 0; y < modifiedImage.height() - 1; y++ ){
        for(int x = 0; x < modifiedImage.width() - 1; x ++){
            pixel = modifiedImage.pixel(x, y);
            grayScale = qRed(pixel);
            vetor_cores[grayScale]++;
            if(vetor_cores[grayScale] > max){
                max = vetor_cores[grayScale];
            }
        }
    }
    if(max == 0){
        max =1;
    }
    QRgb newPixel = qRgb(0,0,0);
    QImage newImage (256,256,QImage::Format_RGB32);
    newImage = paintImage(newImage, 255,255,255);
    for(int x = 0; x<256; x++){
        for(int y = ((255*vetor_cores[x])/max); y>0; y--){
            newImage.setPixel(255 - x,255 - y,newPixel);
        }
    }
    ui->originalGV->scene()->clear();
    showImageO(newImage);
}

QImage MainWindow::paintImage(QImage imagem, int red, int green, int blue){
    QRgb pixel = qRgb(red,green,blue);
    for(int x = 0; x < imagem.width(); x++){
        for(int y = 0; y < imagem.height(); y++){
            imagem.setPixel(x,y,pixel);
        }
    }
    return imagem;
}

// acessing properties

QImage MainWindow::getModified()
{
    return modified;
}

void MainWindow::setModified(QImage m)
{
    modified = m;
}




void MainWindow::on_brightButton_clicked()
{
    int sliderValue = ui->brightSlider->value();
    int realValue = sliderValue - oldBrigthValue;
        //cout << sliderValue << " " << oldBrigthValue << " " << realValue << endl;
    QImage modifiedImage = getModified();
    QRgb pixel, newPixel;
    int red, green, blue;
    for(int x = 0 ; x < modifiedImage.width() ; x++){
        for(int y = 0; y < modifiedImage.height() ; y++){
            pixel = modifiedImage.pixel(x,y);
            red = qRed(pixel);
            green = qGreen(pixel);
            blue = qBlue(pixel);
            newPixel = applyBright(red,green, blue,realValue);
            modifiedImage.setPixel(x,y,newPixel);
        }
    }
    oldBrigthValue = sliderValue;
    setModified(modifiedImage);
    showImageM(modified);
}

QRgb MainWindow::applyBright(int red, int green, int blue, int value){
    int MAX_VALUE = 255, MIN_VALUE = 0;
    red = red + value;
    if(red > MAX_VALUE){
        red = MAX_VALUE;
    }else if(red < MIN_VALUE){
        red = MIN_VALUE;
    }

    green = green + value;
    if(green > MAX_VALUE){
        green = MAX_VALUE;
    }else if(green < MIN_VALUE){
        green = MIN_VALUE;
    }

    blue = blue + value;
    if(blue > MAX_VALUE){
        blue = MAX_VALUE;
    }else if(blue < MIN_VALUE){
        blue = MIN_VALUE;
    }
    QRgb pixel = qRgb(red, green, blue);
    return pixel;
}



void MainWindow::on_contrastButton_clicked()
{
    int sliderValue = ui->contrastSlider->value();
    int realValue = sliderValue - oldContrastValue;
    float value;
    if(realValue == 0){
        value = 0.0;
    }else if(realValue > 0){
        value = realValue + 1;
    }else if(realValue < 0){
        value = 1 / 1 + realValue;
    }

    /*
    switch (sliderValue)
    {
    case -4 : value = 1.0/5;  break;
    case -3 : value = 1.0/4;  break;
    case -2 : value = 1.0/3;  break;
    case -1 : value = 1.0/2;  break;
    case 0 : value = 1.0;  break;
    case 1 : value = 2.0;  break;
    case 2 : value = 3.0;  break;
    case 3 : value = 4.0;  break;
    case 4 : value = 5.0;  break;
    }
    */
    QImage modifiedImage = getModified();
    QRgb pixel, newPixel;
    int red, blue, green;
    for(int x = 0; x < modifiedImage.width(); x++){
        for(int y = 0; y < modifiedImage.height(); y++){
            pixel = modifiedImage.pixel(x,y);
            red = qRed(pixel);
            green = qGreen(pixel);
            blue = qBlue(pixel);
            newPixel = applyContrast(red, green, blue, value);
            modifiedImage.setPixel(x,y,newPixel);
        }
    }
    //cout << value << " " << realValue;
    oldContrastValue = sliderValue;
    setModified(modifiedImage);
    showImageM(modified);

}


QRgb MainWindow::applyContrast(int red, int green, int blue, float value){
    if(value == 0){
        value = 1;
    }
    red = red*value;
    int MAX_VALUE = 255;
    if(red > MAX_VALUE){
        red = MAX_VALUE;
    }
    green = green * value;
    if(green > MAX_VALUE){
        green = MAX_VALUE;
    }
    blue = blue * value;
    if(blue > MAX_VALUE){
        blue = MAX_VALUE;
    }
    QRgb pixel = qRgb(red, green, blue);
    return pixel;
}

void MainWindow::on_negativeButton_clicked()
{
    QImage modifiedImage = getModified();
    int red,green,blue;
    QRgb pixel,newPixel;
    for (int y = 0; y < modifiedImage.height()-1; y++)
        for (int x = 0; x < modifiedImage.width()-1; x++)
        {
            pixel = modifiedImage.pixel(x,y);
            red = 255 - (qRed (pixel));
            green = 255 - (qGreen (pixel));
            blue = 255 - (qBlue (pixel));
            newPixel = qRgb (red,green,blue);
            modifiedImage.setPixel(x,y,newPixel);
        }
    setModified(modifiedImage);
    showImageM(modified);

}

void MainWindow::on_equalizationButton_clicked()
{
    QImage modifiedImage = getModified();
    QRgb pixel,newPixel;
    int newRed, newGreen, newBlue;
    int histogram [3][256];

    for (int j = 0; j < 3; j++)
        for (int i =0; i < 256 ; i++)
            histogram [j][i] = 0;


    for (int y = 0; y < modifiedImage.height()-1; y++)
        for (int x = 0; x < modifiedImage.width()-1; x++)
        {
            pixel = modifiedImage.pixel (x,y);
            histogram [0][qRed(pixel)]++;
            histogram [1][qGreen(pixel)]++;
            histogram [2][qBlue(pixel)]++;
        }

    for (int i = 1; i < 256; i++)
    {
        histogram [0][i] = histogram [0][i] + histogram [0][i-1];
        histogram [1][i] = histogram [1][i] + histogram [1][i-1];
        histogram [2][i] = histogram [2][i] + histogram [2][i-1];
    }

    for (int y = 0; y < modifiedImage.height()-1; y++)
        for (int x = 0; x < modifiedImage.width()-1; x++)
        {
            pixel = modifiedImage.pixel (x,y);

            newRed = (histogram [0][qRed(pixel)])*(255.0/(modifiedImage.height()*modifiedImage.width()));
            newGreen = (histogram [1][qGreen(pixel)])*(255.0/(modifiedImage.height()*modifiedImage.width()));
            newBlue = (histogram [2][qBlue(pixel)])*(255.0/(modifiedImage.height()*modifiedImage.width()));

            newPixel = qRgb (newRed,newGreen,newBlue);

            modifiedImage.setPixel(x,y,newPixel);

        }
    setModified(modifiedImage);
    showImageM(modified);
}





void MainWindow::on_filtersButton_clicked()
{
    this->chooseFilter();
}


/*
void MainWindow::on_horaryRotateButton_clicked()
{
    cout << " rafa";
    QImage modifiedImage = getModified();
    modifiedImage = modifiedImage.transformed(QMatrix().rotate(90));
    setModified(modifiedImage);
    showImageM(modifiedImage);

   cout << "MATHEUS VIADO";
   QImage m = getModified();
   QImage newImage (m.height(),m.width(), QImage::Format_RGB32);
   for (int i = 0; i < m.width(); i++)
   {
        for (int j = 0; j < m.height();j++)
        {
            QRgb orValue = m.pixel(i,j);
            newImage.setPixel(m.height()-j-1,i,orValue);
        }
   }
   setModified(newImage);
   showImageM(newImage);
}
*/

void MainWindow::on_horaryRotButton_clicked()
{
    /*
    QImage modifiedImage = getModified();
    modifiedImage = modifiedImage.transformed(QMatrix().rotate(90));
    setModified(modifiedImage);
    showImageM(modifiedImage);
    */
    QImage modifiedImage = getModified();
    QImage newImage (modifiedImage.height(),modifiedImage.width(), QImage::Format_RGB32);
    for (int x = 0; x < modifiedImage.width(); x++)
    {
         for (int y = 0; y < modifiedImage.height();y++)
         {
             QRgb newPixel = modifiedImage.pixel(x,y);
             newImage.setPixel(modifiedImage.height()-y-1,x,newPixel);
         }
    }
    setModified(newImage);
    showImageM(modified);
}

void MainWindow::on_antiHoraryRotButton_clicked()
{
    QImage modifiedImage = getModified();
    QImage newImage (modifiedImage.height(), modifiedImage.width(), QImage::Format_RGB32);

    QRgb newPixel;

    for(int x = 0; x < modifiedImage.width(); x++)
    {
        for(int y = 0; y < modifiedImage.height(); y++)
        {
            newPixel = modifiedImage.pixel(x,y);
            newImage.setPixel(y, modifiedImage.width()-x-1, newPixel);
        }
    }
    setModified(newImage);
    showImageM(modified);
}

void MainWindow::on_zoomOutButton_clicked()
{
    int value1 = ui->spinBox->value();
    int value2 = ui->spinBox_2->value();
    QImage modifiedImage = getModified();
    QImage resized;
    resized = modifiedImage.scaled(modifiedImage.width()/value1,modifiedImage.height()/value2,Qt::IgnoreAspectRatio);
    setModified(resized);
    showImageM(modified);

/*
    QImage modifiedImage = getModified();
    QImage resized (modifiedImage.width()/value1, modifiedImage.height()/value2, QImage::Format_RGB32);

    */

}

void MainWindow::teste(){
    cout << " AHSUHASUHASUHASUASHUH";
}

void MainWindow::on_zoomInButton_clicked()
{

    QImage modifiedImage = getModified();
    QImage resized (modifiedImage.width()*2 -1 , modifiedImage.height()*2 - 1, QImage::Format_RGB32);
    QRgb newPixel;

    //creating a matrix 2 times larger
    for(int x = 0; x < modifiedImage.width() ; x++)
    {
        for(int y = 0; y < modifiedImage.height() ; y++)
        {
            newPixel = modifiedImage.pixel(x,y);
            resized.setPixel(x*2, y*2, newPixel);
        }
    }

    //interpolating the columns
    QRgb onePixel, twoPixel;
    int newRed, newGreen, newBlue;
    for(int x = 1; x < resized.width() - 1; x=x+2)
    {
        for(int y = 0; y < resized.height(); y=y+2)
        {
            onePixel = resized.pixel(x - 1,y);
            twoPixel = resized.pixel(x + 1,y);
            newRed = (qRed(onePixel) + qRed(twoPixel)) / 2;
            newGreen = (qGreen(onePixel) + qGreen(twoPixel)) / 2;
            newBlue = (qBlue(onePixel) + qBlue(twoPixel)) / 2;

            newPixel = qRgb (newRed,newGreen,newBlue);
            resized.setPixel(x,y,newPixel);
        }
    }

    //interpolating the lines
    for(int x = 0; x < resized.width(); x++)
    {
        for(int y = 1; y < resized.height() - 1; y=y+2)
        {
            onePixel = resized.pixel(x,y-1);
            twoPixel = resized.pixel(x,y+1);
            newRed = (qRed(onePixel) + qRed(twoPixel)) / 2;
            newGreen = (qGreen(onePixel) + qGreen(twoPixel)) / 2;
            newBlue = (qBlue(onePixel) + qBlue(twoPixel)) / 2;

            newPixel = qRgb (newRed,newGreen,newBlue);
            resized.setPixel(x,y,newPixel);
        }
    }


    setModified(resized);
    showImageM(modified);

}


void MainWindow::on_applyFilter(float *a)
{
    //recebe os valores e aplica a rotaçao e o filtro na imagem
    on_grayScaleButton_clicked();
    QImage modifiedImage = getModified();
    QImage newImage = modifiedImage;


    for(int x = 1; x < modifiedImage.width() - 2; x++){
        for(int y =1; y < modifiedImage.height() - 2; y++){
            int value = 0;

            value += qRed(modifiedImage.pixel(x-1,y+1)) *a[8];
            value += qRed(modifiedImage.pixel(x,y+1)) *a[7];
            value += qRed(modifiedImage.pixel(x+1,y+1)) *a[6];
            value += qRed(modifiedImage.pixel(x-1,y)) *a[5];
            value += qRed(modifiedImage.pixel(x,y)) *a[4];
            value += qRed(modifiedImage.pixel(x+1,y)) *a[3];
            value += qRed(modifiedImage.pixel(x-1,y-1)) *a[2];
            value += qRed(modifiedImage.pixel(x,y-1)) *a[1];
            value += qRed(modifiedImage.pixel(x+1,y-1)) *a[0];
            value += a[9];
            if(value > 255)
            {
                value = 255;
            }
            if(value < 0)
            {
                value = 0;
            }

            QRgb newPixel = qRgb(value, value, value);
            newImage.setPixel(x,y, newPixel);

        }
    }

   setModified(newImage);
   showImageM(modified);



}
