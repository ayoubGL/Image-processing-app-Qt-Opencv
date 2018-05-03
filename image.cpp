#include "image.h"
#include "ui_image.h"
#include <iostream>
using namespace std;
Image::Image(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Image)
{

    ui->setupUi(this);
    if(!my_image.data) ui->ToGray->setEnabled(false);
    if(!my_image.data) ui->Histogramm->setEnabled(false);
   if(!my_image.data) ui->add_noise->setEnabled(false);
    if(!my_image.data) ui->con_dete->setEnabled(false);
    if(!my_image.data) ui->comboBox->setEnabled(false);

    ui->save->setVisible(false);


}

Image::~Image()
{
    delete ui;
}
/************ fonction pour la normalisation **********/

//*********************/

void Image::on_Open_clicked()
{
    ui->label_2->clear();
    ui->label->clear();
    fileName = QFileDialog::getOpenFileName(this,
         tr("Open Image"), ".",
       tr("Image Files (*.png *.jpg *.jpeg *.bmp)"));
    my_image= cv::imread(fileName.toAscii().data());




    cvtColor(my_image,my_image,CV_BGR2RGB);
    QImage img= QImage((const unsigned char*)(my_image.data),
       my_image.cols,my_image.rows,QImage::Format_RGB888);
    // display on label
    ui->label->setPixmap(QPixmap::fromImage(img));
    // resize the label to fit the image
    ui->label->resize(ui->label->pixmap()->size());
    ui->label->setStyleSheet("border: 1px solid black");
    ui->label_4->setText("image original");


    if(!my_image.data) {
        ui->ToGray->setEnabled(false);
        ui->Histogramm->setEnabled(false);
    }
    else{
        ui->ToGray->setEnabled(true);
        ui->Histogramm->setEnabled(true);
        ui->add_noise->setEnabled(true);
        ui->con_dete->setEnabled(true);
         ui->comboBox->setEnabled(true);
    }
}

Mat Image::on_ToGray_clicked()
{
        // utilisation de la fourmule gray=(R+G+B)/2

        Mat gray;
        gray.create(my_image.size(),CV_8U);	 //allocation d'une matrac gray avec la même taille de image de src
        /********************************  image color to gary ***********************************/

        // cvtColor(my_image,gray,CV_RGB2GRAY);

        for(int i=0;i<my_image.rows;i++)
                for(int j=0;j<my_image.cols;j++)
                    gray.at<uchar>(i,j)=(uchar)((my_image.at<Vec3b>(i,j)[0]+my_image.at<Vec3b>(i,j)[1]+my_image.at<Vec3b>(i,j)[2])/3);

        QImage img_2= QImage((const unsigned char*)(gray.data),
           gray.cols,gray.rows,QImage::Format_Grayscale8);
        // display on label

        ui->label_3->setText("Image in gray");
        ui->label_2->setStyleSheet("border: 1px solid black");

        ui->label_2->setPixmap(QPixmap::fromImage(img_2));
        // resize the label to fit the image
        ui->label_2->resize(ui->label_2->pixmap()->size());
        ui->ToGray->setEnabled(false);
        ui->save->setVisible(true);

        return gray;


}

void Image::on_Histogramm_clicked()
{

    ui->label_2->clear();
    ui->label_3->clear();
    /// Separate the image in 3 places ( B, G and R )
     vector<Mat> bgr_planes;
     split( my_image, bgr_planes );

     /// Establish the number of bins
     int histSize = 256;

     /// Set the ranges ( for B,G,R) )
     float range[] = { 0, 256 } ;
     const float* histRange = { range };

     bool uniform = true; bool accumulate = false;

     Mat b_hist, g_hist, r_hist;

     /// Compute the histograms:
     calcHist( &bgr_planes[0], 1, 0, Mat(), b_hist, 1, &histSize, &histRange, uniform, accumulate );
     calcHist( &bgr_planes[1], 1, 0, Mat(), g_hist, 1, &histSize, &histRange, uniform, accumulate );
     calcHist( &bgr_planes[2], 1, 0, Mat(), r_hist, 1, &histSize, &histRange, uniform, accumulate );

     // Draw the histograms for B, G and R
     int hist_w = my_image.cols; int hist_h = my_image.rows;
     int bin_w = cvRound( (double) hist_w/histSize );

     Mat histImage( hist_h, hist_w, CV_8UC3, Scalar( 0,0,0) );

     /// Normalize the result to [ 0, histImage.rows ]
     normalize(b_hist, b_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );
     normalize(g_hist, g_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );
     normalize(r_hist, r_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );

     /// Draw for each channel
     for( int i = 1; i < histSize; i++ )
     {
         line( histImage, Point( bin_w*(i-1), hist_h - cvRound(b_hist.at<float>(i-1)) ) ,
                          Point( bin_w*(i), hist_h - cvRound(b_hist.at<float>(i)) ),
                          Scalar( 255, 0, 0), 2, 8, 0  );
         line( histImage, Point( bin_w*(i-1), hist_h - cvRound(g_hist.at<float>(i-1)) ) ,
                          Point( bin_w*(i), hist_h - cvRound(g_hist.at<float>(i)) ),
                          Scalar( 0, 255, 0), 2, 8, 0  );
         line( histImage, Point( bin_w*(i-1), hist_h - cvRound(r_hist.at<float>(i-1)) ) ,
                          Point( bin_w*(i), hist_h - cvRound(r_hist.at<float>(i)) ),
                          Scalar( 0, 0, 255), 2, 8, 0  );
     }

     QImage img_2= QImage((const unsigned char*)(histImage.data),
        histImage.cols,histImage.rows,QImage::Format_RGB888);
     // display on label

     ui->label_3->setText("Histogramme ");

     ui->label_2->setPixmap(QPixmap::fromImage(img_2));
     // resize the label to fit the image
     ui->label_2->resize(ui->label_2->pixmap()->size());
     ui->label_2->setStyleSheet("border: 1px solid black");
     ui->Histogramm->setEnabled(false);
     ui->save->setVisible(false);



}

void Image::on_save_clicked()
{
    Mat gray;
    cvtColor(my_image,gray,CV_BGR2GRAY);
    imwrite("./gary_image.jpg",gray);
    ui->save->setText("image_gray_seved");
    ui->save->setEnabled(false);

}




Mat Image::on_add_noise_clicked()
{
     ui->label_3->clear();

      Mat result;
      cvtColor(my_image,result,CV_BGR2GRAY);


      for (int k=0; k<3000; k++) {
         // rand() is the MFC random number generator
         // try qrand() with Qt
         int i= rand()%result.cols;
         int j= rand()%result.rows;
         if (result.channels() == 1) { // gray-level image
            result.at<uchar>(j,i)= 255;
         } else if (result.channels() == 3) { // color image
            result.at<cv::Vec3b>(j,i)[0]= 255;
            result.at<cv::Vec3b>(j,i)[1]= 0;
            result.at<cv::Vec3b>(j,i)[2]= 255;
         }
      }


      QImage img_2= QImage((const unsigned char*)(result.data),
         result.cols,result.rows,QImage::Format_Grayscale8);
      // display on label
      ui->label_3->setText("Image bruité ");
      ui->label->setStyleSheet("border: 1px solid black");

      ui->label_2->setPixmap(QPixmap::fromImage(img_2));
      // resize the label to fit the image
      ui->label_2->resize(ui->label_2->pixmap()->size());


      //ui->add_noise->setEnabled(false);
      return result;


}

void Image::on_con_dete_clicked()
{
    ui->label_3->clear();
    ui->label_2->clear();


    /*****************-détection de contour par le gradient par mon méthode-*****************/

    /************ une approxiamation de gadient **********************/
    /*
     *    Ix=I(x,y) - I(x-i,y)
     *    Iy=I(x+i,y) - I(x-n,y)
     *
     *
    */


    Mat src;
 /*
    cvtColor(my_image, src, COLOR_BGR2RGB);
    GaussianBlur( src, src, Size(3,3), 0, 0, BORDER_DEFAULT );
    cvtColor(my_image, src, COLOR_RGB2GRAY);
    namedWindow("src");
    imshow("src",src);

    Mat dest;
    Mat dest1;
    Mat dest2;

    dest=src.clone();
    dest1=src.clone();
    dest2=src.clone();
            // le gradient % a x
        for(int i=0;i<dest.rows;i++)
            for(int j=0;j<dest.cols;j++)
                dest1.at<uchar>(i,j)=src.at<uchar>(i+1,j)-src.at<uchar>(i,j);
            // le gardient % a y
        for(int i=0;i<dest.rows;i++)
            for(int j=0;j<dest.cols;j++)
                dest2.at<uchar>(i,j)=src.at<uchar>(i,j)-src.at<uchar>(i,j-1);

    //imshow("gard_x",dest1);
    //imshow("gard_y",dest2);

    //le model

            for(int i=0;i<dest.rows;i++)
                for(int j=0;j<dest.cols;j++)
                dest.at<uchar>(i,j)=(uchar)sqrt(int(dest1.at<uchar>(i,j)*dest1.at<uchar>(i,j)) + int(dest2.at<uchar>(i,j)*dest2.at<uchar>(i,j)));


    //imshow("gard",dest);
         convertScaleAbs( dest1, dest1 );
         convertScaleAbs( dest2, dest2 );
        addWeighted(dest1, 0.5, dest2, 0.5, 0, dest );

            QImage img_2= QImage((const unsigned char*)(dest.data),
               dest.cols,dest.rows,QImage::Format_Grayscale8);
            // display on label
            ui->label_3->setText("Contour detection by gradient");
            ui->label_2->setPixmap(QPixmap::fromImage(img_2));
            // resize the label to fit the image
            ui->label_2->resize(ui->label_2->pixmap()->size());
*/

    /********************************* avec les fonctins pré**********************************/

    int scale = 1;
    int delta = 0;
    int ddepth = CV_16S;

    GaussianBlur(my_image, src, Size(3,3), 0, 0, BORDER_DEFAULT );

    /// Convert it to gray
    cvtColor( src, src, CV_BGR2GRAY );




    /// Generate grad_x and grad_y
    Mat grad_x, grad_y;
    Mat abs_grad_x, abs_grad_y;
    Mat grad;


    /// Gradient X
    //Scharr( src_gray, grad_x, ddepth, 1, 0, scale, delta, BORDER_DEFAULT );
    Sobel( src, grad_x, ddepth, 1, 0, 3, scale, delta, BORDER_DEFAULT );
    convertScaleAbs( grad_x, abs_grad_x );

    /// Gradient Y
    //Scharr( src_gray, grad_y, ddepth, 0, 1, scale, delta, BORDER_DEFAULT );
    Sobel( src, grad_y, ddepth, 0, 1, 3, scale, delta, BORDER_DEFAULT );
    convertScaleAbs( grad_y, abs_grad_y );

    /// Total Gradient (approximate)
    addWeighted( abs_grad_x, 0.5, abs_grad_y, 0.5, 0, grad );



    QImage img_2= QImage((const unsigned char*)(grad.data),
       grad.cols,grad.rows,QImage::Format_Grayscale8);
    // display on label
    ui->label_3->setText("detection de coutour gradient");
    ui->label_2->setPixmap(QPixmap::fromImage(img_2));
    // resize the label to fit the image
    ui->label_2->resize(ui->label_2->pixmap()->size());





}



/*********** fonction pour calculer la valeur median d'un tableau************/
double Image::val_median(double data[], int length){

    //triage de tableau pour extraire la valeur median

    double tmp, val_med;

  for(int i=0;i<length;i++){
    for(int j=i+1;j<length;j++){
      if(data[i]>data[j]){
          tmp=data[i];
          data[i]=data[j];
          data[j]=tmp;
      }
  }}
    int med = length / 2;
    val_med = data[med];

    return val_med;

}

void Image::on_comboBox_activated(int index)
{
    if(index == 0) {
      //-------------------------------- passe bas --------------------------------//
    Mat result = on_add_noise_clicked();
     QImage img_2= QImage((const unsigned char*)(result.data),
               result.cols,result.rows,QImage::Format_Grayscale8);
            // display on label

            ui->label_3->setText("filtre passe bat");

            ui->label->setPixmap(QPixmap::fromImage(img_2));
            // resize the label to fit the image
            ui->label->resize(ui->label->pixmap()->size());
            ui->label_4->clear();
            ui->label_4->setText("image bruité");

            Mat imgFiltreMoyen;
              imgFiltreMoyen.create(result.size(),CV_8U);

             /* filtrage de l'image avec une filtre moyenne passe bas kernel
                     _     _
                    | 1 1 1 |
               1/9  | 1 1 1 |
                    |_1 1 1_|
            */


            for( int i=1;i<imgFiltreMoyen.rows-1;i++)
               for( int j=2;j < imgFiltreMoyen.cols-1;j++){
                   imgFiltreMoyen.at<uchar>(i,j)=(uchar)(
                           (
                               result.at<uchar>(i,j) 		+
                               result.at<uchar>(i-1,j-1) 	+
                               result.at<uchar>(i-1,j)		+
                               result.at<uchar>(i-1,j+1)	+
                               result.at<uchar>(i,j-1)		+
                               result.at<uchar>(i+1,j-1)	+
                               result.at<uchar>(i+1,j)		+
                               result.at<uchar>(i,j+1)		+
                               result.at<uchar>(i+1,j+1)

                           )/9
                       );
               }
              ui->label_2->clear();
              ui->save->setVisible(false);

            QImage img_f= QImage((const unsigned char*)(imgFiltreMoyen.data),
           imgFiltreMoyen.cols,imgFiltreMoyen.rows,QImage::Format_Grayscale8);
        // display on label

        ui->label_2->setPixmap(QPixmap::fromImage(img_f));
        // resize the label to fit the image
        ui->label_2->resize(ui->label_2->pixmap()->size());

    }
    if(index == 1) {
         //-------------------------------- passe haute --------------------------------//


        /* filtrage de l'image avec une filtre passe haute kernel
                _         _
               | -1 -1 -1  |
          1/9  | -1 -17 -1 |
               |_-1 -1 -1 _|
       */
        Mat result = on_ToGray_clicked();

        QImage img_2= QImage((const unsigned char*)(result.data),
                  result.cols,result.rows,QImage::Format_Grayscale8);
               // display on label

               ui->label_3->setText("filtre passe haute");

               ui->label->setPixmap(QPixmap::fromImage(img_2));
               // resize the label to fit the image
               ui->label->resize(ui->label->pixmap()->size());
               ui->label_4->clear();
               ui->label_4->setText("image grie");




        Mat imgFiltre_Phaut;
          imgFiltre_Phaut.create(result.size(),CV_8U);
        for( int i=1;i<imgFiltre_Phaut.rows-1;i++)
           for( int j=1;j < imgFiltre_Phaut.cols-1;j++){

               imgFiltre_Phaut.at<uchar>(i,j)=(uchar)(
                                   (
                               17*result.at<uchar>(i,j) 		-
                                   result.at<uchar>(i-1,j-1) 	-
                                  result.at<uchar>(i-1,j)		-
                                   result.at<uchar>(i-1,j+1)	-
                                     result.at<uchar>(i,j-1)		-
                                     result.at<uchar>(i+1,j-1)	-
                                      result.at<uchar>(i+1,j)		-
                                       result.at<uchar>(i,j+1)		-
                                          result.at<uchar>(i+1,j+1)


                                   )/9
                               );

           }
          ui->label_2->clear();
          ui->save->setVisible(false);

        QImage img_f= QImage((const unsigned char*)(imgFiltre_Phaut.data),
       imgFiltre_Phaut.cols,imgFiltre_Phaut.rows,QImage::Format_Grayscale8);
    // display on label

    ui->label_2->setPixmap(QPixmap::fromImage(img_f));
    // resize the label to fit the image
    ui->label_2->resize(ui->label_2->pixmap()->size());
    }
    if(index == 2){
//-------------------------------- Médian --------------------------------//

        Mat result = on_add_noise_clicked();
         QImage img_2= QImage((const unsigned char*)(result.data),
                   result.cols,result.rows,QImage::Format_Grayscale8);
                // display on label

                ui->label_3->setText("filtre passe bat");

                ui->label->setPixmap(QPixmap::fromImage(img_2));
                // resize the label to fit the image
                ui->label->resize(ui->label->pixmap()->size());
                ui->label_4->clear();
                ui->label_4->setText("image bruité");

                Mat imgFiltreMedian;
                  imgFiltreMedian.create(result.size(),CV_8U);


        double row[9];

         for(int i=1 ; i< imgFiltreMedian.rows-1;i++ )
            for(int j=1 ; j< imgFiltreMedian.cols-1; j++){

               row[0] = (double) result.at<uchar>(i,j)  ;
                row[1] =  (double)result.at<uchar>(i-1,j);
                row[2] =  (double)result.at<uchar>(i,j-1);
                row[3] =  (double)result.at<uchar>(i+1,j);
                row[4] = (double) result.at<uchar>(i,j+1);
                row[5] = (double) result.at<uchar>(i-1,j-1) ;
                row[6] =  (double)result.at<uchar>(i,j-1)    ;
                row[7] =  (double)result.at<uchar>(i+1,j-1)  ;
                row[8] =  (double)result.at<uchar>(i+1,j+1)  ;

                 imgFiltreMedian.at<uchar>(i,j)=(uchar)(val_median(row,9));
            }
         ui->label_2->clear();
         ui->save->setVisible(false);

       QImage img_f= QImage((const unsigned char*)(imgFiltreMedian.data),
      imgFiltreMedian.cols,imgFiltreMedian.rows,QImage::Format_Grayscale8);
   // display on label

   ui->label_2->setPixmap(QPixmap::fromImage(img_f));
   // resize the label to fit the image
   ui->label_2->resize(ui->label_2->pixmap()->size());






    }
}




void Image::on_pushButton_clicked()
{

    qApp->exit();
}
