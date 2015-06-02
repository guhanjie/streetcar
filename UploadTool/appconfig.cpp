#include "appconfig.h"

#include <QSettings>

AppConfig* AppConfig::instance = NULL;

AppConfig::AppConfig()
{
}

AppConfig& AppConfig::getInstance()
{
    if(instance == NULL)
    {
        instance = new AppConfig();
        QSettings settings("./config.ini", QSettings::IniFormat, instance);
        instance->dbDriverName = settings.value("Database/dbDriverName", "QOCI").toString();
        instance->dbName = settings.value("Database/dbName", "orcl").toString();
        instance->dbServerPort = settings.value("Database/dbServerPort", "1521").toInt();
        instance->dbServerIp = settings.value("Database/dbServerIp", "127.0.0.1").toString();
        instance->userName = settings.value("Database/userName", "scott").toString();
        instance->passwd = settings.value("Database/passwd", "tiger").toString();
    }
    return *instance;
}

QString AppConfig::getDbDriverName() const
{
    return dbDriverName;
}

QString AppConfig::getDbName() const
{
    return dbName;
}

int AppConfig::getDbServerPort() const
{
    return dbServerPort;
}

QString AppConfig::getDbServerIp() const
{
    return dbServerIp;
}

QString AppConfig::getUserName() const
{
    return userName;
}

QString AppConfig::getPasswd() const
{
    return passwd;
}
