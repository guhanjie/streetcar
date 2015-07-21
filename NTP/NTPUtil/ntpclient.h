#ifndef NTPCLIENT
#define NTPCLIENT



class NTPCLIENT
{
    //Q_OBJECT

    public:
        void ntpUpdate(QString ntpServerIp = "time.windows.com", int port = 123);

//    private slots:
//        void readingDataGrams();
//        void connectsucess();

    private:
        QUdpSocket *udpsocket;
};

#endif // NTPCLIENT

