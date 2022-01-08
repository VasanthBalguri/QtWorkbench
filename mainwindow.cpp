#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <iostream>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    qOsg = new QtOSGWidget(1,1,this);
    ui->setupUi(this);
    ui->verticalLayout_2->addWidget(qOsg);
    this->setWindowTitle(QString("Qt osg step Viewer"));
    resize(1024,768);
    connect(qOsg,SIGNAL(orientationChanged(std::vector<double>)),this,SLOT(changedOrientation(std::vector<double>)));
    connect(this,SIGNAL(colorChanged(QColor)),qOsg,SLOT(changedColor(QColor)));
    connect(this,SIGNAL(openFile(std::string)),qOsg,SLOT(openScene(std::string)));
    connect(this,SIGNAL(saveFile(std::string)),qOsg,SLOT(saveScene(std::string)));

    connect(ui->height_value,SIGNAL(valueChanged(double)),this,SLOT(updateBottle()));
    connect(ui->width_value,SIGNAL(valueChanged(double)),this,SLOT(updateBottle()));
    connect(ui->thickness_value,SIGNAL(valueChanged(double)),this,SLOT(updateBottle()));
    connect(ui->red_slider,SIGNAL(sliderMoved(int)),this,SLOT(changedR(int)));
    connect(ui->green_slider,SIGNAL(sliderMoved(int)),this,SLOT(changedG(int)));
    connect(ui->blue_slider,SIGNAL(sliderMoved(int)),this,SLOT(changedB(int)));
    connect(ui->actionOpen,SIGNAL(triggered(bool)),this,SLOT(createFileDialog()));
    connect(ui->actionsave,SIGNAL(triggered(bool)),this,SLOT(saveFileDialog()));

    //qOsg->addScene(createScene());
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
    _color.setAlpha(255);
    emit colorChanged(_color);
}


void MainWindow::changedG(int value)
{
    _color.setGreen(value);
    _color.setAlpha(255);
    emit colorChanged(_color);
}

void MainWindow::changedB(int value)
{
    _color.setBlue(value);
    _color.setAlpha(255);
    emit colorChanged(_color);
}

void MainWindow::createFileDialog()
{
    QFileDialog fileDialog(this);

    fileDialog.setFileMode(QFileDialog::AnyFile);
    fileDialog.setViewMode(QFileDialog::Detail);

    QStringList fileNames;
    if (fileDialog.exec())
        if(fileDialog.selectedFiles().size() != 0)
        {
            fileNames = fileDialog.selectedFiles();
            QString filePath = fileNames.first();
            emit openFile(std::string(filePath.toUtf8().constData()));
        }


}

void MainWindow::saveFileDialog()
{
    QFileDialog fileDialog(this);
    fileDialog.setAcceptMode(QFileDialog::AcceptSave);
    fileDialog.setFileMode(QFileDialog::AnyFile);
    fileDialog.setViewMode(QFileDialog::Detail);

    QStringList fileNames;
    if (fileDialog.exec())
        fileNames = fileDialog.selectedFiles();

     QString filePath = fileNames.first();

     emit saveFile(std::string(filePath.toUtf8().constData()));
}

void MainWindow::updateBottle()
{
    osg::ref_ptr<osg::Group> root;
    osg::ref_ptr<osg::Geode> bottle;

    double mythickness = ui->thickness_value->value();
    double mywidth = ui->width_value->value();
    double myheight = ui->height_value->value();
    osg::Vec3 myColor = osg::Vec3( 1.0f, 1.0f, 1.0f );


    qOsg->getScene()->removeChild(0,1);
    qOsg->getScene()->addChild(createBottle(mywidth,myheight,mythickness,myColor/*,viewMatrix*/));


}
