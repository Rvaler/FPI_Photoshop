#ifndef FILTERS_H
#define FILTERS_H


#include <QDialog>

namespace Ui {
class Filters;
}

class Filters : public QDialog
{
    Q_OBJECT

public:
    explicit Filters(QWidget *parent = 0);
    ~Filters();

private slots:
    void on_applyButton_clicked();

    void on_gaussianButton_clicked();

    void on_laplacianoButton_clicked();

    void on_passaAltasButton_clicked();

    void on_prewittHxButton_clicked();

    void on_prewittHyHxButton_clicked();

    void on_sobelHxButton_clicked();

    void on_sobelHyButton_clicked();

private:
    Ui::Filters *ui;

signals:
    bool applyFilter(float *a);

};





#endif // FILTERS_H
