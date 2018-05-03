#ifndef HISTOGRAMME_H
#define HISTOGRAMME_H
#include "image.h"
#include <QMainWindow>

namespace Ui {
class histogramme;
}

class histogramme : public QMainWindow
{
    Q_OBJECT

public:
    explicit histogramme(QWidget *parent = 0);
    ~histogramme();

private:
    Ui::histogramme *ui;
};

#endif // HISTOGRAMME_H
