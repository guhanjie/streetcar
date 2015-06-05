#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QHostInfo>
#include <QDateTime>
//#include <QtMultimedia/QAudioFormat>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::connectsucess()
{
   // ui->pushButton->setEnabled(false);
    qint8 LI=0;
    qint8 VN=3;
    qint8 MODE=3;
    qint8 STRATUM=0;
    qint8 POLL=4;
    qint8 PREC=-6;
    QDateTime Epoch(QDate(1900, 1, 1));
    qint32 second=quint32(Epoch.secsTo(QDateTime::currentDateTime()));
    ui->label->setText("Connected");
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
void MainWindow::readingDataGrams()
{
    ui->label->setText("Reading...");
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
    ui->textEdit->append(time.toString());
    this->udpsocket->disconnectFromHost();
    this->udpsocket->close();
    ui->pushButton->setEnabled(true);
}

void MainWindow::on_pushButton_clicked()
{
   QHostInfo info=QHostInfo::fromName("time.windows.com");
   QString ipAddress=info.addresses().first().toString();
   qDebug()<<"ipAddress="<<ipAddress;
    udpsocket=new QUdpSocket(this);
    connect(udpsocket,SIGNAL(connected()),this,SLOT(connectsucess()));
    connect(udpsocket,SIGNAL(readyRead()),this,SLOT(readingDataGrams()));
    udpsocket->connectToHost("time.windows.com",123);
}
