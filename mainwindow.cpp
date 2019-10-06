#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "qtosgwidget.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    QtOSGWidget* qOsg = new QtOSGWidget(1,1,this);
    ui->setupUi(this);
    ui->verticalLayout_2->addWidget(qOsg);


    connect(qOsg,SIGNAL(orientationChanged(std::vector<double>)),this,SLOT(changedOrientation(std::vector<double>)));
    connect(this,SIGNAL(colorChanged(QColor)),qOsg,SLOT(changedColor(QColor)));

    connect(ui->red_slider,SIGNAL(sliderMoved(int)),this,SLOT(changedR(int)));
    connect(ui->green_slider,SIGNAL(sliderMoved(int)),this,SLOT(changedG(int)));
    connect(ui->blue_slider,SIGNAL(sliderMoved(int)),this,SLOT(changedB(int)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::changedOrientation(std::vector<double> values)
{
    ui->eye_x->setText(QString::number(values[0]));
    ui->eye_y->setText(QString::number(values[1]));
    ui->eye_z->setText(QString::number(values[2]));

    ui->center_x->setText(QString::number(values[3]));
    ui->center_y->setText(QString::number(values[4]));
    ui->center_z->setText(QString::number(values[5]));

    ui->up_x->setText(QString::number(values[6]));
    ui->up_y->setText(QString::number(values[7]));
    ui->up_z->setText(QString::number(values[8]));
}

void MainWindow::changedR(int value)
{
    _color.setRed(value);

    emit colorChanged(_color);
}


void MainWindow::changedG(int value)
{
    _color.setGreen(value);

    emit colorChanged(_color);
}

void MainWindow::changedB(int value)
{
    _color.setBlue(value);

    emit colorChanged(_color);
}
