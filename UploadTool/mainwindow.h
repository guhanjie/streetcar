#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
    class MainWindow;
}

class UploadWorker;

class MainWindow : public QMainWindow
{
        Q_OBJECT

    public:
        explicit MainWindow(QWidget *parent = 0);
        ~MainWindow();

    private slots:
        void            slotOkClicked();
        void            slotCancelClicked();
        void            slotOpenFile();
        void            handleProgressChanged(int value);

    private:
        Ui::MainWindow *ui;
        UploadWorker *mUploadWorker;
};

#endif // MAINWINDOW_H
