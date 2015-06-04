#include "appconfig.h"
#include "dbutils.h"
#include "loadingdialog.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "waitingdialog.h"
#include "quazip.h"
#include "quazipfile.h"
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
#include <QProgressDialog>
#include <QApplication>
#include <QDir>
#include <QProcess>

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
    QString latestVersion = versionNo;
    dbUtils.connectDB();
    QSqlQuery query;
    query.prepare("SELECT version_no, file_name, upload_file FROM t_ats_update_ghj WHERE update_time > (SELECT update_time FROM t_ats_update_ghj WHERE version_no = ?)  ORDER BY update_time DESC");
    query.addBindValue(versionNo);
    query.exec();
    if(query.next())  //有新版本更新
    {
        latestVersion = query.value("version_no").toString();
        qDebug() << "Got an new version[" << latestVersion << "] to update.";
        ui->textBrowser->append("获取到最新版本为："+latestVersion);
        if( QMessageBox::Yes == QMessageBox::question(this, "软件升级", "发现新版本["+latestVersion+"]，是否升级？") )
        {
            qDebug() << "Starting to update the latest version[" << latestVersion << "]...";
            //读取BLOB文件
            QString fileName = query.value("file_name").toString();
            QString tempFile = fileName.section('/', -1);
            QFile file(tempFile);
            QDataStream fos(&file);
            QByteArray blob = query.value("upload_file").toByteArray();
            QBuffer buffer(&blob);
            QDataStream fis(&buffer);
            file.open(QIODevice::WriteOnly);
            buffer.open(QIODevice::ReadOnly);
            //显示下载进度条
            QProgressDialog process(this);
            process.setAttribute(Qt::WA_DeleteOnClose);
            process.setLabelText(tr("正在下载新版本..."));
            process.setRange(0,blob.size()*2);
            process.setValue(0);
            process.open();
            int len = 0;
            int sum = 0;
            bool canceled = false;
            char b[1024] = {0};
            while (!fis.atEnd())
            {
                if(process.wasCanceled())
                {
                    canceled = true;
                    break;
                }
                len = fis.readRawData(b, 1024);
                fos.writeRawData(b,len);
                sum += len;
                process.setValue(sum);
                qApp->processEvents();
            }
            file.close();
            buffer.close();
            if( canceled )
            {
                process.close();
                return;
            }
//            if( !canceled )
//            {
//                if( QMessageBox::Yes == QMessageBox::question(this, "软件升级", "新版本["+latestVersion+"]已下载完成，是否立即升级新版本？") )
//                {
                    process.setLabelText(tr("下载完成，正在安装新版本..."));
                    QuaZip archive(tempFile);
                    if (!archive.open(QuaZip::mdUnzip))
                    {
                        qDebug() << "Can not open .zip file successfully.";
                        QMessageBox::critical(this, "软件升级", "最新版本在下载过程中文件有误，无法正常打开", QMessageBox::Ok);
                         return;
                    }
                    qDebug() << "Starting to extracting .zip file[" << tempFile << "]...";
//                    QProgressDialog process(this);
//                    process.setAttribute(Qt::WA_DeleteOnClose);
//                    process.setLabelText(tr("正在安装新版本程序..."));
//                    process.setRange(0, sum);
//                    process.setValue(0);
//                    process.open();
//                    sum = 0;
                    QDir dir(".");
                    for( bool f = archive.goToFirstFile(); f; f = archive.goToNextFile() )
                    {
                        QString filePath = archive.getCurrentFileName();
                        QuaZipFile zFile(archive.getZipName(), filePath);
                        zFile.open(QIODevice::ReadOnly );
                        //QByteArray ba = zFile.readAll();
                        qDebug() << "extracting "<<zFile.size()<<" Bytes\t" << "--------\t"<<filePath;
                        if (filePath.endsWith("/"))
                        {
                            dir.mkpath(filePath);
                        }
                        else
                        {
                            QDataStream fis(&zFile);
                            QFile dstFile(filePath);
                            QDataStream fos(&dstFile);
                            dstFile.open(QIODevice::WriteOnly);
                            while (!fis.atEnd())
                            {
                                if(process.wasCanceled())
                                {
                                    canceled = true;
                                    break;
                                }
                                len = fis.readRawData(b, 1024);
                                fos.writeRawData(b,len);
                                sum += len;
                                process.setValue(sum);
                                qApp->processEvents();
                            }
                            dstFile.flush();
                            dstFile.close();
                        }
                        zFile.close();
                    }
                    archive.close();
                    qDebug() << "Deleting the .zip files...";
                    QFile::remove(tempFile);
                    if( !canceled )
                    {
                        ui->textBrowser->append("Update version["+latestVersion+"] by changing the config.ini...");
                        AppConfig::getInstance().setVersionNo(latestVersion);
                        QString mainProgram = "./"+AppConfig::getInstance().getMainProgram();
                        qDebug() << "Starting to run runnable file from extracted files["<<mainProgram<<"]...";
                        QProcess::startDetached(mainProgram, QStringList());
                    }
//                }
//            }
            process.close();
        }
    }
    else   //当前版本已最新
    {        
        ui->textBrowser->append("当前版本["+versionNo+"]已是最新版本");
        QMessageBox::information(this, "软件升级", "当前版本已是最新版本，没有可用升级", QMessageBox::Ok);
//        WaitingDialog *dialog = new WaitingDialog;
//        dialog->setWindowTitle("Please wait...");
//        QEventLoop *loop = new QEventLoop;
//        dialog->Start(50, 150),
//        dialog->show();
//        //开启一个事件循环，10秒后退出
//        QTimer::singleShot(10000, loop, SLOT(quit()));
//        loop->exec();
    }
}

