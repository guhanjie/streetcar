#include "ntputil.h"
#include <QDebug>
#include <QHostInfo>
#include <QDateTime>
//#include <QtMultimedia/QAudioFormat>

void NTPUtil::connectsucess()
{
    qint8 LI=0;
    qint8 VN=3;
    qint8 MODE=3;
    qint8 STRATUM=0;
    qint8 POLL=4;
    qint8 PREC=-6;
    QDateTime Epoch(QDate(1900, 1, 1));
    qint32 second=quint32(Epoch.secsTo(QDateTime::currentDateTime()));
    qDebug("Connected");
    qint32 temp=0;
    QByteArray timeRequest(48, 0);
    timeRequest[0]=(LI <<6) | (VN <<3) | (MODE);
    timeRequest[1]=STRATUM;
    timeRequest[2]=POLL;
    timeRequest[3]=PREC & 0xff;
    timeRequest[5]=1;
    timeRequest[9]=1;
    timeRequest[40]=(temp=(second&0xff000000)>>24);
    temp=0;
    timeRequest[41]=(temp=(second&0x00ff0000)>>16);
    temp=0;
    timeRequest[42]=(temp=(second&0x0000ff00)>>8);
    temp=0;
    timeRequest[43]=((second&0x000000ff));
    udpsocket->flush();
    udpsocket->write(timeRequest);
    udpsocket->flush();
}

void NTPUtil::readingDataGrams()
{
    qDebug("Reading ntp data...");
    QByteArray newTime;
    QDateTime Epoch(QDate(1900, 1, 1));
    QDateTime unixStart(QDate(1970, 1, 1));
    do
    {
        newTime.resize(udpsocket->pendingDatagramSize());
        udpsocket->read(newTime.data(), newTime.size());
    }while(udpsocket->hasPendingDatagrams());
    QByteArray TransmitTimeStamp ;
    TransmitTimeStamp=newTime.right(8);
    quint32 seconds=TransmitTimeStamp[0];
    quint8 temp=0;
    for(int j=1;j<=3;j++)
    {
        seconds=seconds<<8;
        temp=TransmitTimeStamp[j];
        seconds=seconds+temp;
    }
    QDateTime time;
    time.setTime_t(seconds-Epoch.secsTo(unixStart));
    this->updateTime = time.toString(Qt::ISODate);
    qDebug() << this->updateTime;
    this->udpsocket->disconnectFromHost();
    this->udpsocket->close();
}

int NTPUtil::ntpUpdate(QString ntpServerIp, int port)
{
   QHostInfo info=QHostInfo::fromName(ntpServerIp);
   QString ipAddress=info.addresses().first().toString();
   qDebug()<<"ipAddress="<<ipAddress;
    udpsocket=new QUdpSocket(this);
    connect(udpsocket,SIGNAL(connected()),this,SLOT(connectsucess()));
    connect(udpsocket,SIGNAL(readyRead()),this,SLOT(readingDataGrams()));
    udpsocket->connectToHost(ntpServerIp, port);
    QString ntpCmd = "ntpdate "+ipAddress;
    //QByteArray ba = ntpCmd.toLatin1();
    //return system(ba.data());
    std::string str = ntpCmd.toStdString();
    return system(str.c_str());
}

QString NTPUtil::f_System_Now(QString ntpServerIp, int port)
{
    QHostInfo info=QHostInfo::fromName(ntpServerIp);
    QString ipAddress=info.addresses().first().toString();
    qDebug()<<"ipAddress="<<ipAddress;
    udpsocket=new QUdpSocket(this);
    connect(udpsocket,SIGNAL(connected()),this,SLOT(connectsucess()));
    connect(udpsocket,SIGNAL(readyRead()),this,SLOT(readingDataGrams()));
    udpsocket->connectToHost(ntpServerIp, port);
    return this->updateTime;
}

NTPUtil::NTPUtil()
{

}

NTPUtil::~NTPUtil()
{
    delete udpsocket;
}

