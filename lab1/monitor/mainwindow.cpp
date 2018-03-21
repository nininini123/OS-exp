#include <unistd.h>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFile>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    timer = new QTimer(this);
    timer->start(1000);
    flag = 0;
    connect(timer,SIGNAL(timeout()),this,SLOT(updateCPU()));
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

//http://blog.csdn.net/jk110333/article/details/8683478
void MainWindow::updateCPU()
{

    QFile f("/proc/stat");// = new QFile(QString(fileName));
    if(!f.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug( "Open failed." );
        return ;
    }
    QTextStream txtInput(&f);
    QString lineStr;
    QStringList strList;

    lineStr = txtInput.readLine();
    strList = lineStr.split(" ");
    for(int i=0;i<9;i++) {
        cpuData[flag][i] = ( (QString)strList.at(i+1) ).toInt();
    }
    if (flag) {
        int totalCPUTime = 0;
        for(int i=0;i<9;i++) {
            totalCPUTime = totalCPUTime + cpuData[1][i] - cpuData[0][i];
        }
        //float usage = cpuData[1][3]-cpuData[0][3];
        float usage =cpuData[1][4] -cpuData[0][4]+ cpuData[1][3]-cpuData[0][3];
        usage = 100 - usage/totalCPUTime * 100;

        ui->listWidget->addItem( QString::number(usage));
//        ui->label->setText( QString::number(usage) );
        flag = 0;
    }
    else {
        flag = 1;
    }

    f.close();
}

//int avl::load( char *fileName, AVLTree &T)
//{
//
//
//        key = ( (QString)strList.at(0) ).toInt();
//        name = strList.at(1);
//        bool taller;
//        AVLData* _data = new AVLData(key,strList.at(1));
//        if (! insertAVL(T,  _data ,  taller) ) delete _data;
//    }

//    f.close();
//}
