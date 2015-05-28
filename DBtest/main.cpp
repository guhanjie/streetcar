#include <QCoreApplication>
#include <QDateTime>
#include <QBitArray>
#include <QDataStream>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include <QStringList>
#include <QDebug>
#include <iostream>
#include <QFile>
#include <QBuffer>

using namespace std;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    //测试数据库是否连接正常
    QSqlDatabase db = QSqlDatabase::addDatabase("QOCI");
    db.setDatabaseName("orcl");
    db.setPort(1521);
    db.setHostName("127.0.0.1");
    db.setUserName("scott");
    db.setPassword("tiger");
    if (db.open())
    {
        qDebug() << "Database connected.";
    }else{
        qDebug() << db.lastError().text();
    }

    QSqlQuery query;

    QString version = "1.0.0.12";

    //开启事务，写入BLOB字段
    QSqlDatabase::database().transaction();
    //插入
    query.prepare("INSERT INTO t_ats_update_ghj (version, upload_user, update_time, file_name, file_desc, upload_file) VALUES (:version, :upload_user, :update_time, :file_name, :file_desc, EMPTY_BLOB())");
    query.bindValue(":version", version);
    query.bindValue(":upload_user", "guhanjie");
    QDateTime now = QDateTime::currentDateTime();
    query.bindValue(":update_time", now);
    query.bindValue(":file_name", "test.zip");
    query.bindValue(":file_desc", "This is a test.");
    if (!query.exec())
        qDebug() << query.lastError();

    query.setForwardOnly(true);
    query.prepare("SELECT upload_file FROM t_ats_update_ghj WHERE version = ?");
    query.addBindValue(version);
    if (!query.exec())
        qDebug() << query.lastError();
    if (query.next())
    {
        QByteArray blob = query.value(0).toByteArray();
        QFile file("/home/guhanjie/test.zip");
        file.open(QIODevice::ReadOnly);
        QDataStream fis(&file);
        QBuffer buffer(&blob);
        buffer.open(QIODevice::Append);
        QDataStream fos(&buffer);
        int len = 0;
        char b[1024] = {0};
        while (!fis.atEnd())
        {
            len = fis.readRawData(b, 1024);
            fos.writeRawData(b,len);
        }
        query.prepare("UPDATE t_ats_update_ghj SET upload_file = ? WHERE version = ?");
        query.addBindValue(blob);
        query.addBindValue(version);
        if(!query.exec())
            qDebug() << query.lastError();
        buffer.close();
        file.close();
    }
    QSqlDatabase::database().commit();

//    //读取BLOB文件
//    query.prepare("SELECT file_name, upload_file FROM t_ats_update_ghj WHERE version = ?");
//    query.addBindValue(version);
//    if(!query.exec())
//        qDebug() << query.lastError();
//    while (query.next())
//    {
//            QString fileName = query.value(0).toString(); //取第i条记录第1个字段(从0开始计数)的结果
//            QByteArray blob = query.value(1).toByteArray();
//            QString filePath = "/home/guhanjie/";
//            filePath += fileName;
//            QFile file(filePath);
//            QDataStream fos(&file);
//            QBuffer buffer(&blob);
//            QDataStream fis(&buffer);
//            file.open(QIODevice::WriteOnly);
//            buffer.open(QIODevice::ReadOnly);
//            int len = 0;
//            char b[1024] = {0};
//            while (!fis.atEnd())
//            {
//                len = fis.readRawData(b, 1024);
//                fos.writeRawData(b,len);
//            }
//            file.close();
//            buffer.close();
//    }


    return 0;
}
