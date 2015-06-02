#include "appconfig.h"
#include "dbutils.h"
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlError>

DBUtils::DBUtils()
{
}

DBUtils::~DBUtils()
{
    QSqlDatabase::database().close();
}

bool DBUtils::connectDB()
{
    //获取数据库的配置信息
    AppConfig& config = AppConfig::getInstance();
    //连接数据库
    QSqlDatabase db = QSqlDatabase::addDatabase(config.getDbDriverName());  //"QOCI"
    db.setDatabaseName(config.getDbName());         //"orcl"
    db.setPort(config.getDbServerPort());                    //1521
    db.setHostName(config.getDbServerIp());             //"127.0.0.1"
    db.setUserName(config.getUserName());              //"scott"
    db.setPassword(config.getPasswd());                    //"tiger"
    if (db.isOpen() || db.open())
    {
        qDebug() << "Database[" << config.getDbName() << "," << config.getDbServerIp() << ":" << config.getDbServerPort() << "] connected.";
        return true;
    }else{
        qDebug() << "Failed to connect database[" << config.getDbName() << "," << config.getDbServerIp() << ":" << config.getDbServerPort() << "], for reason: " << db.lastError().text();
        return false;
    }
}

void DBUtils::closeDB()
{
    QSqlDatabase::database().close();
}
