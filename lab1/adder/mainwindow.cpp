#include <unistd.h>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFile>
#include <QDateTime>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    timer = new QTimer(this);
    sum = 0;
    counter = 0;

    ui->label->setText(QString::number(sum));
    timer->start(3000);
    connect(timer,SIGNAL(timeout()),this,SLOT(updateSum()));

}

MainWindow::~MainWindow()
{
    delete timer;
    delete ui;
}


void MainWindow::on_actionexit_triggered()
{
    close();
}

void MainWindow::updateSum()
{
    if (counter == 100 ) {
        counter = 0;
        sum = 0;
    }
    else {
        counter ++;
        sum += counter;
    }
    ui->label->setText(QString::number(sum));
}
