#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include<vector>

#include "qtosgwidget.h"
#include "osghelper.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QColor _color;
    QtOSGWidget* qOsg;

public slots:

    void changedOrientation(std::vector<double>);
    void changedR(int);
    void changedG(int);
    void changedB(int);
    void createFileDialog();
    void updateBottle();

signals:
    void colorChanged(QColor);
    void openFile(std::string);
};

#endif // MAINWINDOW_H
