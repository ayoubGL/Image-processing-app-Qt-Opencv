#include "video.h"
#include "ui_video.h"
#include "opencv2/opencv.hpp"
#include <iostream>
#include <stdlib.h>
#include<string>
#include<math.h>
video::video(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::video)
{
    ui->setupUi(this);
}

video::~video()
{
    delete ui;
}

void video::on_surv_clicked()
{
    /************************/

    /********************************************************************************
    *
    *   	  sauvgarde de la vidéo envoyée par le caméras dont le nom
    *							est donné pa l'utilisateur
    *
    *
    *********************************************************************************/



     cv:: VideoCapture cap(0);





      int frame_w = cap.get(CV_CAP_PROP_FRAME_WIDTH);
      int frame_h = cap.get(CV_CAP_PROP_FRAME_HEIGHT);



      cv::VideoWriter video("./video_eng.avi",CV_FOURCC('M','J','P','G'),20, cv::Size(frame_w,frame_h));

      while(1)
      {
        cv::Mat frame;


        cap >> frame;


         if (frame.empty())
          break;


        video.write(frame);




/**************/

        cvtColor(frame,frame,CV_BGR2RGB);
      QImage img= QImage((const unsigned char*)(frame.data),
         frame.cols,frame.rows,QImage::Format_RGB888);
      // display on label
      ui->label_6->setStyleSheet("border: 1px solid black");
      ui->label_6->setPixmap(QPixmap::fromImage(img));
      // resize the label to fit the image
      ui->label_6->resize(ui->label_6->pixmap()->size());
       ui->label_7->setText("Enregistrement de vidéo");
       /**********************/





       ui->det_peau->setEnabled(false);
       ui->surv->setEnabled(false);



        char ext = (char)cv::waitKey(1);
        if( ext == 27 ){
              break;
            ui->label->clear();
            ui->label_2->clear();
            ui->label_3->clear();
            ui->label_4->clear();
            ui->label_5->clear();
            ui->label_6->clear();
            ui->label_7->clear();

        }

      }


      cap.release();
      video.release();


      cv::destroyAllWindows();






}



void video::on_det_peau_clicked()
{
   cv:: VideoCapture vcap(0);

       int frame_width=   vcap.get(CV_CAP_PROP_FRAME_WIDTH);
       int frame_height=   vcap.get(CV_CAP_PROP_FRAME_HEIGHT);
       cv::VideoWriter video("./out.avi",CV_FOURCC('M','J','P','G'),10, cv::Size(frame_width,frame_height),true);

       for(;;){
          cv:: Mat frame;
          cv:: Mat frame2;
           vcap >> frame;
           vcap >> frame2;
           video.write(frame);
           video.write(frame2);
           for (int i = 0; i < frame.rows-10; i++) {
                for (int j = 0; j < frame.cols-10; j++) {
                    if (frame.at<cv::Vec3b>(i,j)[2] > 95 &&
                            frame.at<cv::Vec3b>(i,j)[1] > 40 &&
                            frame.at<cv::Vec3b>(i,j)[0] > 20 &&
                            (cv::max(cv::max(frame.at<cv::Vec3b>(i,j)[2],
                                frame.at<cv::Vec3b>(i,j)[1]),
                                frame.at<cv::Vec3b>(i,j)[0]) - cv::min(cv::min(frame.at<cv::Vec3b>(i,j)[2],
                                frame.at<cv::Vec3b>(i,j)[1]),
                                frame.at<cv::Vec3b>(i,j)[0])) > 15 &&
                            abs(frame.at<cv::Vec3b>(i,j)[2] - frame.at<cv::Vec3b>(i,j)[1]) > 15 &&
                            frame.at<cv::Vec3b>(i,j)[2] > frame.at<cv::Vec3b>(i,j)[1] &&
                            frame.at<cv::Vec3b>(i,j)[2] > frame.at<cv::Vec3b>(i,j)[0])
                    {
                        frame.at<cv::Vec3b>(i,j)[0] = 255;
                        frame.at<cv::Vec3b>(i,j)[1] = 255;
                        frame.at<cv::Vec3b>(i,j)[2] = 255;
                    }
                    else {
                        frame.at<cv::Vec3b>(i,j)[0] = 0;
                        frame.at<cv::Vec3b>(i,j)[1] = 0;
                        frame.at<cv::Vec3b>(i,j)[2] = 0;
                    }
                }
           }
           // cv::imshow( "original", frame2 );
          // cv::imshow( "peau detection", frame );

           //
             cvtColor(frame2,frame2,CV_BGR2RGB);
           QImage img= QImage((const unsigned char*)(frame2.data),
              frame2.cols,frame2.rows,QImage::Format_RGB888);
           // display on label
           ui->label->setStyleSheet("border: 1px solid black");
           ui->label->setPixmap(QPixmap::fromImage(img));
           // resize the label to fit the image
           ui->label->resize(ui->label->pixmap()->size()/1.2);
            ui->label_4->setText("Video original");


           QImage img_2= QImage((const unsigned char*)(frame.data),
              (frame.cols),(frame.rows),QImage::Format_RGB888);
           // display on label
           ui->label_3->setStyleSheet("border: 1px solid black");
           ui->label_3->setPixmap(QPixmap::fromImage(img_2));
           // resize the label to fit the image
           ui->label_3->resize(ui->label_3->pixmap()->size()/1.2);

           ui->label_5->setText("Detection de peau");



           char ext = (char)cv::waitKey(1);
           if( ext == 27 ){
                 break;
               ui->label->clear();
               ui->label_2->clear();
               ui->label_3->clear();
               ui->label_4->clear();
               ui->label_5->clear();
               ui->label_6->clear();
               ui->label_7->clear();

           }

           ui->det_peau->setEnabled(false);
           ui->surv->setEnabled(false);

        }



}

void video::on_quit_clicked()
{

    ui->label->clear();
    ui->label_2->clear();
    ui->label_3->clear();
    ui->label_4->clear();
    ui->label_5->clear();
    ui->label_6->clear();
    ui->label_7->clear();

}
