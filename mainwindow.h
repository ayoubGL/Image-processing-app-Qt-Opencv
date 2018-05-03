#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "image.h"
#include "video.h"
#include <QFileDialog>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_image_clicked();

    void on_video_clicked();

private:
    Ui::MainWindow *ui;
    Image *image_ui;
    video *video_tr;


};

#endif // MAINWINDOW_H
