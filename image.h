#ifndef IMAGE_H
#define IMAGE_H

#include <QMainWindow>
#include <QFileDialog>
#include <QMainWindow>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/imgproc/imgproc.hpp>


using namespace cv;

namespace Ui {
class Image;
}

class Image : public QMainWindow
{
    Q_OBJECT

public:
    explicit Image(QWidget *parent = 0);
    ~Image();

private slots:
    void on_Open_clicked();

    Mat on_ToGray_clicked();

    void on_Histogramm_clicked();

    void on_save_clicked();

    Mat on_add_noise_clicked();



    void on_con_dete_clicked();

    void on_comboBox_activated(int index);

     double val_median(double data[], int length);


     void on_pushButton_clicked();

     void on_seg_clicked();

private:
    Ui::Image *ui;
    cv::Mat my_image;
     QString fileName;

};

#endif // IMAGE_H
