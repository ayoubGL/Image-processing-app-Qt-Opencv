#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_image_clicked()
{
      image_ui = new Image(this);
      image_ui->show();
}

void MainWindow::on_video_clicked()
{
    video_tr = new video(this);
    video_tr->show();





}
