#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>

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
    void on_actionexit_triggered();
    void updateCPU();

private:
    Ui::MainWindow *ui;
    int flag ;
    int cpuData[2][9];
    QTimer *timer;
};

#endif // MAINWINDOW_H
