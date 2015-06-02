#include "appconfig.h"
#include "dbutils.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QSettings>
#include <QFile>
#include <QBuffer>
#include <QDateTime>
#include <QBitArray>
#include <QDataStream>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include <QMessageBox>

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

void MainWindow::on_action_C_triggered()
{
    this->close();
}

void MainWindow::on_action_U_triggered()
{
    DBUtils dbUtils;
    QString versionNo = AppConfig::getInstance().getVersionNo();
    dbUtils.connectDB();
    QSqlQuery query;
    query.prepare("SELECT * FROM t_ats_update_ghj WHERE update_time >= (SELECT update_time FROM t_ats_update_ghj WHERE version_no = ?)  ORDER BY update_time DESC");
    query.addBindValue(versionNo);
    query.exec();
    if(query.next())  //有新版本更新
    {
        QString latestVersion = query.value("version_no").toString();
        qDebug() << "Got an new version[" << latestVersion << "] to update.";
    }
    else   //当前版本已最新
    {
        QMessageBox::information(this, "软件更新", "当前版本已是最新版本，没有可用升级", QMessageBox::Ok);
    }
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
    ui->textBrowser->append("ready to update...\n");
}

