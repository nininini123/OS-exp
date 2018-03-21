#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QtCharts>
using namespace QtCharts ;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
//    void on_dial_valueChanged(int value);
    void updateInfo();

    void on_tabWidget_currentChanged(int index);

    void on_listWidget_itemClicked(QListWidgetItem *item);

    void on_pushButtonKill_clicked();

private:
    Ui::MainWindow *ui;
//    QChart * chart;
    QTimer *timer;
    int flag;
    int pageNum;
    QLineSeries* line1;
    int cpuData[2][9];
protected:
    int tCount;
    int pidSelected;
};

#endif // MAINWINDOW_H
