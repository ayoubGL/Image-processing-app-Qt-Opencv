#include "histogramme.h"
#include "ui_histogramme.h"

histogramme::histogramme(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::histogramme)
{
    ui->setupUi(this);
}

histogramme::~histogramme()
{
    delete ui;
}
