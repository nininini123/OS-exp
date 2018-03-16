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
    void updateSum();

private:
    Ui::MainWindow *ui;
    QTimer *timer;
    int sum;
    int counter;
};

#endif // MAINWINDOW_H
