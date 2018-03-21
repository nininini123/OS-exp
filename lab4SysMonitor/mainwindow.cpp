#include<sys/types.h>
#include<signal.h>
#include <QPalette>
#include <QDebug>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QVBoxLayout>
#include <QGridLayout>
#include <QTabWidget>
#include <QFile>
#include <QStringList>
#include <QMessageBox>

/************************************************/

#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>
#include <QtCharts/QAbstractBarSeries>
#include <QtCharts/QPercentBarSeries>
#include <QtCharts/QStackedBarSeries>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QLineSeries>
#include <QtCharts/QSplineSeries>
#include <QtCharts/QScatterSeries>
#include <QtCharts/QAreaSeries>
#include <QtCharts/QLegend>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLabel>
#include <QtCore/QRandomGenerator>
#include <QtCharts/QBarCategoryAxis>
#include <QtWidgets/QApplication>
#include <QtCharts/QValueAxis>

/****************************************/

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->dial->setMinimum(0);
    ui->dial->setMaximum(100);

    pageNum = ui->tabWidget->currentIndex();

    timer = new QTimer(this);
    timer->start(300);
    flag = 0;
    tCount = 6;
    pidSelected = 0;
    connect(timer,SIGNAL(timeout()),this,SLOT(updateInfo()));

    line1 = new QLineSeries();
    for(double x=0;x<10;x+=0.1)
    {
        line1->append(x,sin(x));
    }
    QChart* c = new QChart();
    c->addSeries(line1);
    ui->widget->setChart(c);

    //CPU info///////////////////////////
    QFile f("/proc/cpuinfo");//
        if(!f.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            qDebug( "Open failed." );
            return ;
        }
        QTextStream txtInput(&f);
        QString lineStr;
        QStringList strList;
        while (1) {
            lineStr = txtInput.readLine();
            if (lineStr.startsWith("model name")) {
                lineStr = lineStr.split(":").at(1);
                ui->textBrowser->append("<b>CPU : </b>"+ lineStr);
                ui->textBrowser->append(NULL);
            }
            else if(lineStr.startsWith("siblings")) {
                lineStr = lineStr.split(":").at(1);
                ui->textBrowser->append("<b>CPU threads : </b>"+ lineStr);
                ui->textBrowser->append(NULL);
            }
            else if(lineStr.startsWith("cpu cores")){
                lineStr = lineStr.split(":").at(1);
                ui->textBrowser->append("<b>CPU cores : </b>\n"+ lineStr);
                ui->textBrowser->append(NULL);
                break;
            }
        }
        f.close();
        /////////////////////
    //system infomation
        f.setFileName("/proc/version");
            if ( !f.open(QIODevice::ReadOnly) )
            {
                QMessageBox::warning(this, tr("warning"), tr("The version file can not open!"), QMessageBox::Yes);
                return ;
            }
            lineStr = f.readLine();
            ui->textBrowser->append("<b>Linux Version : </b><br>"+lineStr.split("(gcc").at(0).mid(14));
            ui->textBrowser->append(NULL);
            f.close();

        f.setFileName("/proc/version_signature");
            if ( !f.open(QIODevice::ReadOnly) )
            {
                QMessageBox::warning(this, tr("warning"), tr("The version file can not open!"), QMessageBox::Yes);
                return ;
            }
            lineStr = f.readLine();
            ui->textBrowser->append("<b>Version signature : </b><br>"+lineStr);
            f.close();

}

MainWindow::~MainWindow()
{
    delete line1;
    delete timer;
    delete ui;
}

//void MainWindow::on_dial_valueChanged(int value)
//{
//    ui->label->setText(QString::number(value));
//}

void MainWindow::updateInfo()
{
//    for(double x=0;x<1;x+=0.1)
//    {
//        line1->append(x,sin(x));
//    }
    qDebug() << "updating ...";
    QString tempStr; //读取文件信息字符串
    QFile tempFile; //用于打开系统文件
    //    int pos; //读取文件的位置
    if ( pageNum == 1 ) {//process
        if(tCount==6) {
            tCount = 0;
            ui->listWidget->clear();
            QDir qd("/proc");
            QStringList qsList = qd.entryList();
            QString qs = qsList.join("\n");
            int find_start = 3;
            int a, b;
            int number_of_sleep = 0, number_of_run = 0, number_of_zombie = 0;
            int totalProNum = 0; //进程总数
            QString proName; //进程名
            QString proState; //进程状态
            QString proPri; //进程优先级
            QString proMem; //进程占用内存
            new QListWidgetItem( (QString)"PID\t" + "name\t\t" + "status\t" +
                                                         "priority" + "\t" +
                                                         "mem", ui->listWidget);
//            qDebug() << "updating page 1 (2.zya na i ...";
            for(int i=0;i<qsList.size();i++) {
                bool isDir;
                int PID = qsList.at(i).toInt(&isDir);
                if (isDir) {
                    totalProNum ++;
                    tempFile.setFileName("/proc/" + qsList.at(i) + "/stat");
                    if ( !tempFile.open(QIODevice::ReadOnly) )
                    {
                        QMessageBox::warning(this, tr("warning"), tr("The pid stat file can not open!"), QMessageBox::Yes);
                        return;
                    }
                    tempStr = tempFile.readLine();
                    if (tempStr.length() == 0)
                    {
                        break;
                    }
                    a = tempStr.indexOf("(");
                    b = tempStr.indexOf(")");
                    proName = tempStr.mid(a+1, b-a-1);
                    proName.trimmed(); //删除两端的空格
                    proState = tempStr.section(" ", 2, 2);
                    proPri = tempStr.section(" ", 17, 17);
                    proMem = tempStr.section(" ", 22, 22);
                    switch ( proState.at(0).toLatin1() )
                    {
                    case 'S':   number_of_sleep++; break; //Sleep
                    case 'R':   number_of_run++; break; //Running
                    case 'Z':   number_of_zombie++; break; //Zombie
                    default :   break;
                    }
                    if (proName.length() >= 12)
                    {
                        new QListWidgetItem(QString::number(PID) + "\t" +
                                    proName + "\t" +
                                    proState + "\t" +
                                    proPri + "\t" +
                                    proMem, ui->listWidget);
                    }
                    else
                    {
                        new QListWidgetItem(QString::number(PID)+ "\t" +
                                    proName + "\t\t" +
                                    proState + "\t" +
                                    proPri + "\t" +
                                    proMem, ui->listWidget);
                    }
                    tempFile.close();
                }
            }
            ui->label_proc->setText( QString::number(totalProNum, 10) + " Processes, " +
                                     QString::number(number_of_run, 10) + " running, " +
                                     QString::number(number_of_sleep, 10) + " sleeping , " +
                                     QString::number(number_of_zombie, 10) + " zombies" );
        }
        else {
                tCount ++;
        }
    }


    //page 1
    tempFile.setFileName("/proc/meminfo"); //打开内存信息文件
    if ( !tempFile.open(QIODevice::ReadOnly) )
    {
        QMessageBox::warning(this, tr("warning"), tr("The meminfo file can not open!"), QMessageBox::Yes);
        return ;
    }
    QStringList tempStrList;
    int nMemTotal, nMemFree,nSwapTotal, nSwapFree, tempNum;
    float nMemUsed,nSwapUsed ;//, nSwapTotal, nSwapFree, nSwapUsed , tempNum;

    tempStr = tempFile.readLine();
    if(tempStr.startsWith("MemTotal")) {
        tempStrList = tempStr.split(":");
        tempStr = tempStrList.at(1);
        tempStrList = tempStr.split(" ");
        tempNum = 0;
        tempStr = tempStrList.at(tempNum);
        while (tempStr.isEmpty()) {
            tempNum++;
            tempStr = tempStrList.at(tempNum);
        }
        ui->label_RAM->setText("RAM " + tempStr +" KB ");
        nMemTotal = tempStr.toInt();
    }
    tempStr = tempFile.readLine();
    if(tempStr.startsWith("MemFree")) {
        tempStrList = tempStr.split(":");
        tempStr = tempStrList.at(1);
        tempStrList = tempStr.split(" ");
        tempNum = 0;
        tempStr = tempStrList.at(tempNum);
        while (tempStr.isEmpty()) {
            tempNum++;
            tempStr = tempStrList.at(tempNum);
        }
        nMemFree = tempStr.toInt();
    }
    nMemUsed = nMemTotal - nMemFree;
    nMemUsed = 100* ( nMemUsed / nMemTotal);
    ui->progressBarRAM->setValue(nMemUsed);
    tempNum = nMemUsed / 20;
    switch (tempNum) {
    case 0:
        ui->progressBarRAM->setStyleSheet("background-color: #00ffff"); ;
        break;
    case 1:
        ui->progressBarRAM->setStyleSheet("background-color: #44bbbb"); ;
        break;
    case 2:
        ui->progressBarRAM->setStyleSheet("background-color: #888888"); ;
        break;
    case 3:
        ui->progressBarRAM->setStyleSheet("background-color: #cc4444"); ;
        break;
    case 4:
        ui->progressBarRAM->setStyleSheet("background-color: #ff4444"); ;
        break;
    case 5:
        ui->progressBarRAM->setStyleSheet("background-color: #ff0000"); ;
        break;
    default:
        break;
    }
    //swap
    for (int i=0; i<13;i++) {
        tempStr = tempFile.readLine();
    }
    if(tempStr.startsWith("SwapTotal")) {
        tempStrList = tempStr.split(":");
        tempStr = tempStrList.at(1);
        tempStrList = tempStr.split(" ");
        tempNum = 0;
        tempStr = tempStrList.at(tempNum);
        while (tempStr.isEmpty()) {
            tempNum++;
            tempStr = tempStrList.at(tempNum);
        }
        ui->label_SWAP->setText("SWAP " + tempStr +" KB ");
        nSwapTotal = tempStr.toInt();
    }
    tempStr = tempFile.readLine();
    if(tempStr.startsWith("SwapFree")) {
        tempStrList = tempStr.split(":");
        tempStr = tempStrList.at(1);
        tempStrList = tempStr.split(" ");
        tempNum = 0;
        tempStr = tempStrList.at(tempNum);
        while (tempStr.isEmpty()) {
            tempNum++;
            tempStr = tempStrList.at(tempNum);
        }
        nSwapFree = tempStr.toInt();
    }
    nSwapUsed = nSwapTotal - nSwapFree;
    nSwapUsed = 100* (nSwapUsed / nSwapTotal);
    ui->progressBar_SWAP->setValue(nSwapUsed);
    tempNum = nSwapUsed / 20;
    switch (tempNum) {
    case 0:
        ui->progressBar_SWAP->setStyleSheet("background-color: #00ffff"); ;
        break;
    case 1:
        ui->progressBar_SWAP->setStyleSheet("background-color: #44bbbb"); ;
        break;
    case 2:
        ui->progressBar_SWAP->setStyleSheet("background-color: #888888"); ;
        break;
    case 3:
        ui->progressBar_SWAP->setStyleSheet("background-color: #cc4444"); ;
        break;
    case 4:
        ui->progressBar_SWAP->setStyleSheet("background-color: #ff4444"); ;
        break;
    default:
        ui->progressBar_SWAP->setStyleSheet("background-color: #ffffff"); ;
        break;
    }

    tempFile.close(); //关闭内存信息文件

    //CPU
    QFile f("/proc/stat");// = new QFile(QString(fileName));
        if(!f.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            qDebug( "Open cpu info \"/proc/stat\" failed." );
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
            tempNum = usage / 20;
            switch (tempNum) {
            case 0:
                ui->dial->setStyleSheet("background-color: #00ffff"); ;
                break;
            case 1:
                ui->dial->setStyleSheet("background-color: #44bbbb"); ;
                break;
            case 2:
                ui->dial->setStyleSheet("background-color: #888888"); ;
                break;
            case 3:
                ui->dial->setStyleSheet("background-color: #cc4444"); ;
                break;
            case 4:
                ui->dial->setStyleSheet("background-color: #ff4444"); ;
                break;
            case 5:
                ui->dial->setStyleSheet("background-color: #ff0000"); ;
                break;
            default:
                break;
            }
            ui->dial->setValue(usage);

            ui->label->setText(QString::number(usage));
    //        ui->label->setText( QString::number(usage) );
            flag = 0;
        }
        else {
            int totalCPUTime = 0;
            for(int i=0;i<9;i++) {
                totalCPUTime = totalCPUTime + cpuData[1][i] - cpuData[0][i];
            }
            //float usage = cpuData[1][3]-cpuData[0][3];
            float usage =cpuData[1][4] -cpuData[0][4]+ cpuData[1][3]-cpuData[0][3];
            usage = 100 - usage/totalCPUTime * 100;

            ui->dial->setValue(usage);
            ui->label->setText(QString::number(usage));
            flag = 1;
        }

        f.close();


}


void MainWindow::on_tabWidget_currentChanged(int index)
{
    pageNum = index;
#ifdef QT_DEBUG
    qDebug() << index ;
#endif
    return ;
}


void MainWindow::on_listWidget_itemClicked(QListWidgetItem *item)
{
    QString str = item->text();
    qDebug() <<"selected pid " << str.split("\t").at(0);
    pidSelected = str.split("\t").at(0).toInt();
}

void MainWindow::on_pushButtonKill_clicked()
{
    if ( QMessageBox::question(this,"FBI WARNING!","are you sure you want to kill the process?") == QMessageBox::Yes ) {
        qDebug("23333333333333333333333333333333333 yes");
    }
    if (pidSelected > 0) {
        if (kill(pidSelected,SIGKILL) < 0 ) {
            QMessageBox::information(this,"error","cannot kill the process!");
        }
        else qDebug("sucess");
    }
    else QMessageBox::information(this,"wrong operation","no legal pid selected!");
}
