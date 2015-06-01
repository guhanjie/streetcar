#include "uploadworker.h"
#include <QDebug>
#include <QString>
#include <QFile>
#include <QBuffer>
#include <QDateTime>
#include <QBitArray>
#include <QDataStream>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include <QSettings>
#include <QMessageBox>

UploadWorker::UploadWorker(QObject *parent) :
    QThread(parent)
{    
    this->cancel = false;
    QSettings settings("./config.ini", QSettings::IniFormat);
    dbDriverName = settings.value("Database/dbDriverName", "QOCI").toString();
    dbName = settings.value("Database/dbName", "orcl").toString();
    dbServerPort = settings.value("Database/dbServerPort", "1521").toInt();
    dbServerIp = settings.value("Database/dbServerIp", "127.0.0.1").toString();
    userName = settings.value("Database/userName", "scott").toString();
    passwd = settings.value("Database/passwd", "tiger").toString();
}

void UploadWorker::run()
{
    this->cancel = false;

    //测试数据库是否连接正常
    QSqlDatabase db = QSqlDatabase::addDatabase(dbDriverName);  //"QOCI"
    db.setDatabaseName(dbName);         //"orcl"
    db.setPort(dbServerPort);                    //1521
    db.setHostName(dbServerIp);             //"127.0.0.1"
    db.setUserName(userName);              //"scott"
    db.setPassword(passwd);                    //"tiger"
    if (db.isOpen() || db.open())
    {
        qDebug() << "Database[" << dbName << "," << dbServerIp << ":" << dbServerIp << " connected.";
    }else{
        qDebug() << "Failed to connect database[" << dbName << "," << dbServerIp << ":" << dbServerIp << "], for reason: " << db.lastError().text();
        QMessageBox::critical(NULL, "数据库连接失败", "数据库连接失败，请确认数据库服务正常，并为本程序配置正确参数！", QMessageBox::Yes, QMessageBox::Yes);
        emit uploadFailed(-1);
    }
    QSqlQuery query;
    query.prepare("SELECT version_no FROM t_ats_update_ghj WHERE update_time >= (SELECT update_time FROM t_ats_update_ghj WHERE version_no = ?)");
    query.addBindValue(versionNo);
    query.exec();
    //提交的版本低于数据库中最新版本
    if(query.next())
    {
        QString latestVersion = query.value(0).toString();
        qDebug() << "Failed to insert the record " << latestVersion << "as this version_no has exits.";
        QMessageBox::critical(NULL, tr("该版本已存在"),
                                                      tr("对不起，您提交的版本在服务器中已存在，请提交比之更新的版本！"),
                                                      QMessageBox::Ok);
        emit uploadFailed(-1);
    }
    //否则，插入新记录
    else
    {
        qDebug() << "Starting to insert an new  record..." << versionNo << versionDesc << uploadUser << fileName;
        //开启事务，写入BLOB字段
        QSqlDatabase::database().transaction();
        //插入
        query.prepare("INSERT INTO t_ats_update_ghj (version_no, version_desc, upload_user, update_time, file_name, upload_file) VALUES (:version_no, :version_desc, :upload_user, :update_time, :file_name, EMPTY_BLOB())");
        query.bindValue(":version_no", versionNo);
        query.bindValue(":version_desc", versionDesc);
        query.bindValue(":upload_user", uploadUser);
        query.bindValue(":update_time", QDateTime::currentDateTime());
        query.bindValue(":file_name", fileName);
        if (!query.exec())
        {
            qDebug() << "Failed to insert the new record into database, for reason: " << query.lastError();
            QString errMsg =  "数据库插入失败，错误原因：\n";
            errMsg += query.lastError().text();
            QMessageBox::critical(NULL, "数据库插入失败", errMsg, QMessageBox::Yes, QMessageBox::Yes);
            emit uploadFailed(-1);
        }
//        for(int i=0;i<100;i++)
//        {
//            QThread::msleep(100);
//            emit progress(i);
//            qDebug() << i;
//            if(this->cancel)
//            {
//                qDebug() << versionNo << versionDesc << uploadUser << fileName;
//                QSqlDatabase::database().rollback();
//                return;
//            }
//        }
        //更新BLOB字段
        query.setForwardOnly(true);
        query.prepare("SELECT upload_file FROM t_ats_update_ghj WHERE version_no = ?");
        query.addBindValue(versionNo);
        if (!query.exec())
        {
            qDebug() << "Failed to search the just inserted record, for reason: " << query.lastError();
            QMessageBox::critical(NULL, "数据库插入记录丢失", "数据库插入记录丢失，错误原因：\n"+query.lastError().text(), QMessageBox::Yes, QMessageBox::Yes);
            QSqlDatabase::database().rollback();
            emit uploadFailed(-1);
        }
        if (query.next())
        {
            QByteArray blob = query.value(0).toByteArray();
            QFile file(fileName);
            file.open(QIODevice::ReadOnly);
            qint64 fileSize = file.size();
            QDataStream fis(&file);
            QBuffer buffer(&blob);
            buffer.open(QIODevice::Append);
            QDataStream fos(&buffer);
            int len = 0;
            char b[1024] = {0};
            qint64 gotSize = 0;
            while (!fis.atEnd())
            {
                emit progress(gotSize*100 / fileSize /100);
                if(this->cancel)
                {
                    qDebug() << "Canceling to insert the new record by user, database will be rollback...";
                    QSqlDatabase::database().rollback();
                    buffer.close();
                    file.close();
                    db.close();
                    return;
                }
                len = fis.readRawData(b, 1024);
                fos.writeRawData(b,len);
                gotSize += len;
            }
            query.prepare("UPDATE t_ats_update_ghj SET upload_file = ? WHERE version_no = ?");
            query.addBindValue(blob);
            query.addBindValue(versionNo);
            if(!query.exec())
            {
                qDebug() << "Failed to insert the BLOB field for new record, for reason: " << query.lastError();
                QMessageBox::critical(NULL, "数据库写入文件失败", "数据库写入文件失败，错误原因：\n"+query.lastError().text(), QMessageBox::Yes, QMessageBox::Yes);
                QSqlDatabase::database().rollback();
                emit uploadFailed(-1);
            }
            buffer.close();
            file.close();
        }
        QSqlDatabase::database().commit();
        qDebug() << "Successed to insert the new record into database.";
        QMessageBox::information(NULL, tr("新版本更新成功"),
                                 tr("恭喜，新版本已成功更新至服务器！"),
                                 QMessageBox::Ok, QMessageBox::Ok);
        emit progress(100);
    }
    db.close();
}

void UploadWorker::stop()
{
    this->cancel = true;
}

QString UploadWorker::getVersionNo() const
{
    return versionNo;
}

void UploadWorker::setVersionNo(const QString &value)
{
    versionNo = value;
}

QString UploadWorker::getVersionDesc() const
{
    return versionDesc;
}

void UploadWorker::setVersionDesc(const QString &value)
{
    versionDesc = value;
}

QString UploadWorker::getUploadUser() const
{
    return uploadUser;
}

void UploadWorker::setUploadUser(const QString &value)
{
    uploadUser = value;
}

QString UploadWorker::getFileName() const
{
    return fileName;
}

void UploadWorker::setFileName(const QString &value)
{
    fileName = value;
}
