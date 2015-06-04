#ifndef APPCONFIG_H
#define APPCONFIG_H

#include <QString>

class AppConfig
{
    private:
        AppConfig();

    public:
        static AppConfig& getInstance();

        QString getVersionNo() const;
        void setVersionNo(const QString);
        QString getDbDriverName() const;
        QString getDbName() const;
        int getDbServerPort() const;
        QString getDbServerIp() const;
        QString getUserName() const;
        QString getPasswd() const;
        QString getMainProgram() const;

    private:
        static AppConfig* instance;
        //软件版本号
        QString       versionNo;
        //主程序入口
        QString       mainProgram;
        //数据库相关配置项
        QString       dbDriverName;
        QString       dbName;
        int               dbServerPort;
        QString       dbServerIp;
        QString       userName;
        QString       passwd;
};

#endif // APPCONFIG_H
