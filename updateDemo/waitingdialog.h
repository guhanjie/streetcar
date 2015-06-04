#ifndef WAITINGDIALOG_H
#define WAITINGDIALOG_H

#include <QDialog>
#include <QProgressBar>
#include <QTimer>

class WaitingDialog : public QDialog
{
        Q_OBJECT
    private:
        int m_CurrentValue;         //当前值
        int m_UpdateInterval;      //更新间隔
        int m_MaxValue;              //最大值
        QTimer m_Timer;
        QProgressBar *m_ProgressBar;

    public:
        explicit WaitingDialog(QWidget *parent = 0);
        void Start(int interval=100, int maxValue=100);
        void Stop();

    signals:

    public slots:
        void UpdateSlot();

};

#endif // WAITINGDIALOG_H
