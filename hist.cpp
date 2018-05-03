#include "hist.h"
#include "ui_hist.h"

Hist::Hist(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Hist)
{
    ui->setupUi(this);
}

Hist::~Hist()
{
    delete ui;
}
