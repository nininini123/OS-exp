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
    QDateTime curTime ;
    curTime = QDateTime::currentDateTime();
    ui->label->setText(curTime.toString("yyyy-MM-dd hh:mm:ss"));
    timer = new QTimer(this);
    timer->start(1000);
    connect(timer,SIGNAL(timeout()),this,SLOT(updateTime()));
//        update();
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_actionexit_triggered()
{
    close();
}

void MainWindow::updateTime()
{

    QDateTime curTime ;
    curTime = QDateTime::currentDateTime();
    ui->label->setText(curTime.toString("yyyy-MM-dd hh:mm:ss"));
}
