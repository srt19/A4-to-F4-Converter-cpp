#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QFileDialog>
#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>

using namespace cv;

std::string pic1, pic2, outpic;
int help = 0;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->git->setOpenExternalLinks(1);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_img1_button_clicked()
{
    QString filename = QFileDialog::getOpenFileName(
                nullptr,
                QObject::tr("Open Image"),
                "./Input/",
                QObject::tr("Images File (*.jpg *.png *.webp)"));
    pic1 = filename.toStdString();

    if (filename.isEmpty())
    {
        //none
    }

    else(ui->img1_label->setText(filename));
}


void MainWindow::on_img2_button_clicked()
{
    QString filename = QFileDialog::getOpenFileName(
                nullptr,
                QObject::tr("Open Image"),
                "./Input/",
                QObject::tr("Images File (*.jpg *.png *.webp)"));
    pic2 = filename.toStdString();

    if (filename.isEmpty())
    {
        //none
    }

    else(ui->img2_label->setText(filename));
}


void MainWindow::on_output_button_clicked()
{
    QString filename = QFileDialog::getSaveFileName(
                nullptr,
                QObject::tr("Open Image"),
                "./Output/",
                QObject::tr("Images File (*.jpg *.png *.webp)"));
    outpic = filename.toStdString();

    if (filename.isEmpty())
    {
     //none
    }

    else(ui->output_label->setText(filename));
}


void MainWindow::on_start_clicked()
{
    if ((pic1.empty()) || (pic2.empty()) || (outpic.empty()))
    {
        ui->status->setText("Please input the file correctly");
        ui->status->setStyleSheet("QLabel { color : rgb(255, 0, 0);}");
    }

    else
    {
    Mat img1(imread(pic1));
    Mat img2(imread(pic2));
    ui->start->setEnabled(0);

    //Correcting img1 position
    Mat a1;
    a1 = img1(Rect(0, 0, 2481, 3376));

    Mat a2(3376, 29, CV_8UC3, Scalar(255, 255, 255));
    hconcat(a2, a1, a1);
    hconcat(a1, a2, a1);

    Mat a3(522, 2539, CV_8UC3, Scalar(255, 255, 255));
    vconcat(a1, a3, a1);

    //Correcting img2 position
    Mat b1;
    b1 = img2(Rect(0, 0, 2481, 3376));

    //Rotate image
    int angle = 180;
    Point2f center((b1.cols - 1) / 2.0, (b1.rows - 1) / 2.0);
    Mat rot = getRotationMatrix2D(center, angle, 1.0);
    warpAffine(b1, b1, rot, b1.size());

    Mat b2(495, 2481, CV_8UC3, Scalar(255, 255, 255));
    Mat b3(27, 2481, CV_8UC3, Scalar(255, 255, 255));
    vconcat(b2, b1, b1);
    vconcat(b1, b3, b1);

    b1 = b1(Rect(0, 0, 2467, 3898));
    Mat b4(3898, 116, CV_8UC3, Scalar(255, 255, 255));
    hconcat(b4, b1, b1);

    //Final Merge
    a1 = a1(Rect(0, 0, 2539, 1949));

    Point2f centers((b1.cols - 1) / 2.0, (b1.rows - 1) / 2.0);
    Mat rots = getRotationMatrix2D(centers, angle, 1.0);
    warpAffine(b1, b1, rots, b1.size());

    b1 = b1(Rect(0, 0, 2539, 1949));
    Point2f centre((b1.cols - 1) / 2.0, (b1.rows - 1) / 2.0);
    Mat rota = getRotationMatrix2D(centre, angle, 1.0);
    warpAffine(b1, b1, rota, b1.size());
    vconcat(a1, b1, a1);

    imwrite(outpic, a1);
    ui->start->setEnabled(1);
    ui->status->setText("READY");
    //set color
    ui->status->setStyleSheet("QLabel { color: rgb(0, 255, 0);}");
    }
}


void MainWindow::on_help_button_clicked()
{
    if (help == '0')
    {
        help = 1;
    }

    else
    {
        help = 0;
    }
}

