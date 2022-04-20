#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QFileDialog>
#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>

using namespace cv;

std::string pic1, pic2, outpic;
bool help;
bool in1 = 0, in2 = 0, sav = 0;
bool lang = 0;
bool completed = 0;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
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
        //YNTKTS
    }

    else
    {
     int x, y;
     Mat img(imread(pic1));
     x = img.size().width;
     y = img.size().height;

     if((x == 2481) || (y == 3506))
     {
         std::string fname = filename.toStdString();
         std::string name = fname.substr(fname.find_last_of("/\\") + 1);
         filename = filename.fromStdString(name);
         ui->img1_name->setText(filename);
         ui->img1_name->setStyleSheet("QLabel { color: rgb(0, 0, 0);}");
         in1 = 1;

         if((in1 == true) && (in2 == true) && (sav == true))
         {
             ui->start->setEnabled(1);
         }

         else
         {
             ui->start->setEnabled(0);
         }
     }

     else{
         ui->start->setEnabled(0);
         ui->img1_name->setText("Please input image with 3506x2481 pixel");
         ui->img1_name->setStyleSheet("QLabel { color: rgb(255, 0, 0);}");
     }
    }
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
        //YNTKTS
    }

    else{
        int x, y;
        Mat img(imread(pic2));
        x = img.size().width;
        y = img.size().height;

        if((x == 2481) || (y == 3506))

        {
            std::string fname = filename.toStdString();
            std::string name = fname.substr(fname.find_last_of("/\\") + 1);
            filename = filename.fromStdString(name);
            ui->img2_name->setText(filename);
            ui->img2_name->setStyleSheet("QLabel { color: rgb(0, 0, 0);}");
            in2 = 1;

            if((in1 == true) && (in2 == true) && (sav == true))
            {
                ui->start->setEnabled(1);
            }

            else
            {
                ui->start->setEnabled(0);
            }
        }

        else{
            in2 = 0;
            ui->start->setEnabled(0);
            ui->img2_name->setText("Please input image with 3506x2481 pixel");
            ui->img2_name->setStyleSheet("QLabel { color: rgb(255, 0, 0);}");
        }
    }
}


void MainWindow::on_output_button_clicked()
{
    QString filename = QFileDialog::getSaveFileName(
                nullptr,
                QObject::tr("Open Image"),
                "./Output/",
                QObject::tr("Images File (*.jpg *.png *.webp)"));
    outpic = filename.toStdString();
    std::string fname = outpic.substr(outpic.find_last_of("/\\") + 1);
    filename = filename.fromStdString(fname);
    ui->output_name->setText(filename);
    sav =1;

    if((in1 == true) && (in2 == true) && (sav == true))
    {
        ui->start->setEnabled(1);
    }

    else
    {
        ui->start->setEnabled(0);
    }
}


void MainWindow::on_start_clicked()
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
    ui->status->setText("Completed");
    //set color
    ui->status->setStyleSheet("QLabel { color: rgb(0, 255, 0);}");
    completed = 1;
}


void MainWindow::on_help_button_clicked()
{
    if (help == 0)
    {
        help = 1;
    }

    else
    {
        help = 0;
    }
}


void MainWindow::on_lang_button_clicked()
{
    if(lang == false)
    {
        lang = 1;

        ui->img1_label->setText("Gambar 1 :");
        ui->img2_label->setText("Gambar 2 :");
        ui->output_label->setText("File Hasil");
        ui->img1_button->setText("Pilih File");
        ui->img2_button->setText("Pilih File");
        ui->output_button->setText("Pilih File");
        ui->start->setText("Mulai");
        ui->lang_button->setText("ID");

        if(completed == false)
        {
            ui->status->setText("SIAP");
            ui->status->setStyleSheet("QLabel { color: rgb(0, 255, 0);}");
        }

        else
        {
            ui->status->setText("SELESAI");
            ui->status->setStyleSheet("QLabel { color: rgb(0, 255, 0);}");
        }
    }

    else
    {
        lang = 0;

        ui->img1_label->setText("Image 1 :");
        ui->img2_label->setText("Image 2 :");
        ui->output_label->setText("Output");
        ui->img1_button->setText("Browse File");
        ui->img2_button->setText("Browse File");
        ui->output_button->setText("Browse File");
        ui->start->setText("Start");
        ui->lang_button->setText("EN");

        if(completed == false)
        {
            ui->status->setText("READY");
            ui->status->setStyleSheet("QLabel { color: rgb(0, 255, 0);}");
        }

        else
        {
            ui->status->setText("COMPLETED");
            ui->status->setStyleSheet("QLabel { color: rgb(0, 255, 0);}");
        }
    }
}

