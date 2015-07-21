#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QUdpSocket>

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
    void readingDataGrams();
    void connectsucess();
    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;
    QUdpSocket *udpsocket;
};

#endif // MAINWINDOW_H
