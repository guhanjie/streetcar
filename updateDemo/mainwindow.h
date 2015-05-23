#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
signals:
    void buttonClicked();

private slots:
    void on_action_C_triggered();
    void on_action_U_triggered();
    void onButtonClicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
