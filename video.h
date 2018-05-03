#ifndef VIDEO_H
#define VIDEO_H

#include <QMainWindow>
#include "opencv2/opencv.hpp"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
namespace Ui {
class video;
}

class video : public QMainWindow
{
    Q_OBJECT

public:
    explicit video(QWidget *parent = 0);
    ~video();

private slots:
    void on_surv_clicked();

    void on_det_peau_clicked();

    void on_det_peau_2_clicked();

    void on_quit_clicked();

private:
    Ui::video *ui;
    cv::VideoCapture *cap;
};

#endif // VIDEO_H
