#include "ntputil.h"
#include <QDebug>
#include <QHostInfo>
#include <QDateTime>
//#include <QtMultimedia/QAudioFormat>

#define NTPFRAC(x) ( 4294*(x) + ( (1981*(x))>>11 ) )

struct ntptime {
    unsigned int coarse;
    unsigned int fine;
};

static double ntpdiff( struct ntptime *start, struct ntptime *stop)
{
    int a;
    unsigned int b;
    a = stop->coarse - start->coarse;
    if (stop->fine >= start->fine) {
        b = stop->fine - start->fine;
    } else {
        b = start->fine - stop->fine;
        b = ~b;
        a -= 1;
    }
    return a*1.e6 + b * (1.e6/4294967296.0);
}

static quint32 bytesToInt(QByteArray TransmitTimeStamp, int count)
{
    if(count == 0)
    {
        return 0;
    }
    quint32 seconds=TransmitTimeStamp[0];
    quint8 temp=0;
    for(int i=1;i<count;i++)
    {
        seconds=seconds<<8;
        temp=TransmitTimeStamp[i];
        seconds=seconds+temp;
    }
    return seconds;
}

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
    qint8 LI = (newTime[0]&0xC0)>>6;  //0;
    qint8 VN = (newTime[0]&0x38)>>3; //3;
    qint8 MODE= (newTime[0]&0x07); //3;
    qint8 STRATUM= newTime[1];  //0;
    qint8 POLL= newTime[2];  //4;
    qint8 PREC= newTime[3];  //-6;
    qDebug()<<"LI="<<LI
                    <<", VN="<<VN
                    <<", MODE="<<MODE
                    <<", STRATUM="<<STRATUM
                    <<", POLL="<<POLL
                    <<", PREC="<<PREC;
    quint32 delay = bytesToInt(newTime.mid(4, 4), 2);
    quint32 dispersion = bytesToInt(newTime.mid(8, 4), 2);
    quint32 identifier = bytesToInt(newTime.mid(12, 4), 2);
    qDebug()<<"delay="<<delay<<", dispersion="<<dispersion<<", identifier="<<identifier;
    struct ntptime reftime, orgtime, rectime, xmttime, arrivaltime;
    reftime.coarse = bytesToInt(newTime.mid(16, 4), 4);
    reftime.fine = bytesToInt(newTime.mid(20, 4), 4);
    orgtime.coarse = bytesToInt(newTime.mid(24, 4), 4);
    orgtime.fine = bytesToInt(newTime.mid(28, 4), 4);
    rectime.coarse = bytesToInt(newTime.mid(32, 4), 4);
    rectime.fine = bytesToInt(newTime.mid(36, 4), 4);
    xmttime.coarse = bytesToInt(newTime.mid(40, 4), 4);
    xmttime.fine = bytesToInt(newTime.right(4), 4);
    arrivaltime.coarse=quint32(Epoch.secsTo(QDateTime::currentDateTime()));
    qint32 arrivalSinceEpoch = quint32(unixStart.secsTo(QDateTime::currentDateTime()));
    arrivaltime.fine = NTPFRAC(arrivalSinceEpoch);
    qDebug()<<"reftime="<<reftime.coarse<<reftime.fine<<endl
                    <<"orgtime="<<orgtime.coarse<<orgtime.fine<<endl
                    <<"rectime="<<rectime.coarse<<rectime.fine<<endl
                   <<"xmttime="<<xmttime.coarse<<xmttime.fine<<endl
                  <<"arrivaltime="<<arrivaltime.coarse<<arrivaltime.fine;

    //计算时间误差
    double el_time,st_time,skew1,skew2, skew;
    el_time=ntpdiff(&orgtime,&arrivaltime);   /*整个NTP同步请求耗时 elapsed */
    st_time=ntpdiff(&rectime,&xmttime);  /*NTP服务端处理请求耗时 stall */
    skew1=ntpdiff(&orgtime,&rectime);  /*网络延迟1，到达服务端*/
    skew2=ntpdiff(&xmttime,&arrivaltime);   /*网络延迟2，返回客户端*/
    skew = (skew1-skew2)/2;
    qDebug()<<"以下统计信息时间单位均为：微秒"<<endl
                    <<"本次NTP同步请求总耗时(Total elapsed): \t\t"<<el_time<<endl
                    <<"NTP服务端处理请求耗时(Server stall):  \t\t"<<st_time<<endl
                    <<"去除服务器处理时间后的网络耗时(Slop):  \t\t"<<(el_time-st_time)<<endl
                    <<"网络延迟时间误差(Skew): \t\t"<<skew;

    QByteArray TransmitTimeStamp ;
    TransmitTimeStamp=newTime.right(8);
    quint32 seconds = bytesToInt(TransmitTimeStamp, 4);
    QDateTime time;
    time.setTime_t(seconds-Epoch.secsTo(unixStart));
    this->updateTime = time.toString(Qt::ISODate);
    qDebug() << this->updateTime;
    time.setTime_t(arrivaltime.coarse-Epoch.secsTo(unixStart)+skew*1.0e-6);
    this->updateTime = time.toString(Qt::ISODate);
    qDebug() << this->updateTime;
    this->udpsocket->disconnectFromHost();
    this->udpsocket->close();
}

int NTPUtil::f_System_UpdateTime(QString ntpServerIp, int port)
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

