#ifndef NTPUTIL_H
#define NTPUTIL_H
#include <QUdpSocket>

class NTPUtil
{
Q_OBJECT
public:
    NTPUtil();
    ~NTPUtil();

public:
    int ntpUpdate(QString ntpServerIp = "time.windows.com", int port = 123);

private slots:
    void readingDataGrams();
    void connectsucess();

private:
    QUdpSocket *udpsocket;
};

#endif // NTPUTIL_H
