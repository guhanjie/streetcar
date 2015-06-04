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
#include "appconfig.h"
#include "dbutils.h"
#include "uploadworker.h"

UploadWorker::UploadWorker(QObject *parent) :
    QThread(parent)
{    
    cancel = false;
}

void UploadWorker::run()
{
    cancel = false;
    //连接数据库
    DBUtils dbUtils;
    if( !dbUtils.connectDB() )
    {
        emit criticalUpDialog("数据库连接失败", "数据库连接失败，请确认数据库服务正常，并为本程序配置正确参数！");
        emit uploadFailed(-1);
        return;
    }
    QSqlQuery query;
    query.prepare("SELECT version_no FROM t_ats_update_ghj WHERE update_time >= (SELECT update_time FROM t_ats_update_ghj WHERE version_no = ?)");
    query.addBindValue(versionNo);
    query.exec();
    //提交的版本低于数据库中最新版本
    if(query.next())
    {
        QString latestVersion = query.value(0).toString();
        qDebug() << "Failed to insert the record " << latestVersion << ", as this version_no has exits.";
        emit warningUpDialog("该版本已存在", "对不起，您提交的版本["+versionNo+"]在服务器中已存在，请提交比之更新的版本！");
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
            emit criticalUpDialog("数据库插入记录失败", "数据库插入记录失败，错误原因：\n"+query.lastError().text());
            emit uploadFailed(-1);
            return;
        }
        //更新BLOB字段
        query.setForwardOnly(true);
        query.prepare("SELECT upload_file FROM t_ats_update_ghj WHERE version_no = ?");
        query.addBindValue(versionNo);
        if (!query.exec())
        {
            qDebug() << "Failed to search the just inserted record, for reason: " << query.lastError();
            QSqlDatabase::database().rollback();
            emit criticalUpDialog("数据库插入记录丢失", "数据库插入记录丢失，错误原因：\n"+query.lastError().text());
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
                emit progress(gotSize*100 / fileSize);
                if(this->cancel)
                {
                    qDebug() << "Canceling to insert the new record by user, database will be rollback...";
                    QSqlDatabase::database().rollback();
                    file.close();
                    buffer.close();
                    return;
                }
                len = fis.readRawData(b, 1024);
                fos.writeRawData(b,len);
                gotSize += len;
            }
            file.close();
            buffer.close();
            query.prepare("UPDATE t_ats_update_ghj SET upload_file = ? WHERE version_no = ?");
            query.addBindValue(blob);
            query.addBindValue(versionNo);
            if(!query.exec())
            {
                qDebug() << "Failed to insert the BLOB field for new record, for reason: " << query.lastError();
                QSqlDatabase::database().rollback();
                emit criticalUpDialog("数据库写入文件失败", "数据库写入文件失败，错误原因：\n"+query.lastError().text());
                emit uploadFailed(-1);
                return;
            }
        }
        QSqlDatabase::database().commit();
        qDebug() << "Successed to insert the new record into database.";
        emit progress(100);
        emit infoUpDialog("新版本更新成功", "恭喜，新版本["+versionNo+"]已成功更新至服务器！");
    }
    dbUtils.closeDB();
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
