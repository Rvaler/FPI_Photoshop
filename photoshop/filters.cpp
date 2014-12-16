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


#include "filters.h"
#include "ui_filters.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"

using namespace std;



Filters::Filters(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Filters)
{
    ui->setupUi(this);
}

Filters::~Filters()
{
    delete ui;
}

float acresc;

void Filters::on_applyButton_clicked()
{
    //vai pegar os valores dos spinboxes e passar como parametro


    float a[10];
    a[0] = ui->doubleSpinBox_1->value();
    a[1] = ui->doubleSpinBox_2->value();
    a[2] = ui->doubleSpinBox_3->value();
    a[3] = ui->doubleSpinBox_4->value();
    a[4] = ui->doubleSpinBox_5->value();
    a[5] = ui->doubleSpinBox_6->value();
    a[6] = ui->doubleSpinBox_7->value();
    a[7] = ui->doubleSpinBox_8->value();
    a[8] = ui->doubleSpinBox_9->value();
    a[9] = acresc;
    this->applyFilter(a);
}



void Filters::on_gaussianButton_clicked()
{
    //seta os valores dos spinboxes
    ui->doubleSpinBox_1->setValue(0.0625);
    ui->doubleSpinBox_2->setValue(0.125);
    ui->doubleSpinBox_3->setValue(0.0625);
    ui->doubleSpinBox_4->setValue(0.125);
    ui->doubleSpinBox_5->setValue(0.25);
    ui->doubleSpinBox_6->setValue(0.125);
    ui->doubleSpinBox_7->setValue(0.0625);
    ui->doubleSpinBox_8->setValue(0.125);
    ui->doubleSpinBox_9->setValue(0.0625);
    acresc = 0;
}

void Filters::on_laplacianoButton_clicked()
{
    ui->doubleSpinBox_1->setValue(0);
    ui->doubleSpinBox_2->setValue(-1);
    ui->doubleSpinBox_3->setValue(0);
    ui->doubleSpinBox_4->setValue(-1);
    ui->doubleSpinBox_5->setValue(4);
    ui->doubleSpinBox_6->setValue(-1);
    ui->doubleSpinBox_7->setValue(0);
    ui->doubleSpinBox_8->setValue(-1);
    ui->doubleSpinBox_9->setValue(0);
    acresc = 0;
}

void Filters::on_passaAltasButton_clicked()
{
    ui->doubleSpinBox_1->setValue(-1);
    ui->doubleSpinBox_2->setValue(-1);
    ui->doubleSpinBox_3->setValue(-1);
    ui->doubleSpinBox_4->setValue(-1);
    ui->doubleSpinBox_5->setValue(8);
    ui->doubleSpinBox_6->setValue(-1);
    ui->doubleSpinBox_7->setValue(-1);
    ui->doubleSpinBox_8->setValue(-1);
    ui->doubleSpinBox_9->setValue(-1);
    acresc = 0;
}

void Filters::on_prewittHxButton_clicked()
{
    ui->doubleSpinBox_1->setValue(-1);
    ui->doubleSpinBox_2->setValue(0);
    ui->doubleSpinBox_3->setValue(1);
    ui->doubleSpinBox_4->setValue(-1);
    ui->doubleSpinBox_5->setValue(0);
    ui->doubleSpinBox_6->setValue(1);
    ui->doubleSpinBox_7->setValue(-1);
    ui->doubleSpinBox_8->setValue(0);
    ui->doubleSpinBox_9->setValue(1);
    acresc = 127;
}

void Filters::on_prewittHyHxButton_clicked()
{
    ui->doubleSpinBox_1->setValue(-1);
    ui->doubleSpinBox_2->setValue(-1);
    ui->doubleSpinBox_3->setValue(-1);
    ui->doubleSpinBox_4->setValue(0);
    ui->doubleSpinBox_5->setValue(0);
    ui->doubleSpinBox_6->setValue(0);
    ui->doubleSpinBox_7->setValue(1);
    ui->doubleSpinBox_8->setValue(1);
    ui->doubleSpinBox_9->setValue(1);
    acresc = 127;
}

void Filters::on_sobelHxButton_clicked()
{
    ui->doubleSpinBox_1->setValue(-1);
    ui->doubleSpinBox_2->setValue(0);
    ui->doubleSpinBox_3->setValue(1);
    ui->doubleSpinBox_4->setValue(-2);
    ui->doubleSpinBox_5->setValue(0);
    ui->doubleSpinBox_6->setValue(2);
    ui->doubleSpinBox_7->setValue(-1);
    ui->doubleSpinBox_8->setValue(0);
    ui->doubleSpinBox_9->setValue(1);
    acresc = 127;
}

void Filters::on_sobelHyButton_clicked()
{
    ui->doubleSpinBox_1->setValue(-1);
    ui->doubleSpinBox_2->setValue(-2);
    ui->doubleSpinBox_3->setValue(-1);
    ui->doubleSpinBox_4->setValue(0);
    ui->doubleSpinBox_5->setValue(0);
    ui->doubleSpinBox_6->setValue(0);
    ui->doubleSpinBox_7->setValue(1);
    ui->doubleSpinBox_8->setValue(0);
    ui->doubleSpinBox_9->setValue(1);
    acresc = 127;
}
