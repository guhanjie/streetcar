#ifndef UPLOADWORKER_H
#define UPLOADWORKER_H

#include <QThread>

class QSqlDatabase;

class UploadWorker : public QThread
{
        Q_OBJECT
    public:
        explicit UploadWorker(QObject *parent = 0);
        void         run() Q_DECL_OVERRIDE;
        void         stop();

        QString getVersionNo() const;
        void setVersionNo(const QString &value);

        QString getVersionDesc() const;
        void setVersionDesc(const QString &value);

        QString getUploadUser() const;
        void setUploadUser(const QString &value);

        QString getFileName() const;
        void setFileName(const QString &value);

    signals:
        void         progress(int value);
        void         uploadFailed(int value);

    public slots:

    private:
        bool            cancel;

        //数据库相关配置项
        QString       dbDriverName;
        QString       dbName;
        int               dbServerPort;
        QString       dbServerIp;
        QString       userName;
        QString       passwd;

        QString       versionNo;
        QString       versionDesc;
        QString       uploadUser;
        QString       fileName;
};

#endif // UPLOADWORKER_H
