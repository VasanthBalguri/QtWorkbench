#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include<vector>

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

public slots:

    void changedOrientation(std::vector<double>);
    void changedR(int);
    void changedG(int);
    void changedB(int);
    void createFileDialog();

signals:
    void colorChanged(QColor);
    void openFile(std::string);
};

#endif // MAINWINDOW_H
