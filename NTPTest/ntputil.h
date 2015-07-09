#ifndef NTPUTIL_H
#define NTPUTIL_H
#include <QUdpSocket>

class NTPUtil : public QObject
{
    Q_OBJECT
    public:
        NTPUtil();
        ~NTPUtil();

    public:
        int f_System_UpdateTime(QString ntpServerIp = "time.windows.com", int port = 123);
        QString f_System_Now(QString ntpServerIp = "time.windows.com", int port = 123);

    private slots:
        void readingDataGrams();
        void connectsucess();

    private:
        QString updateTime;
        QUdpSocket *udpsocket;
};

#endif // NTPUTIL_H
