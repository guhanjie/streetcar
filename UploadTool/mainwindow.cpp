#include <QDebug>
#include <QDialog>
#include <QFileDialog>
#include <QMessageBox>
#include <QProgressBar>
#include <QSqlQuery>
#include <QThread>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dbutils.h"
#include "uploadworker.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    mUploadWorker(NULL)
{
    mUploadWorker = new UploadWorker(this);
    ui->setupUi(this);
    ui->progressBar->hide();
    DBUtils dbUtils;
    if(dbUtils.connectDB())
    {
        QSqlQuery query;
        query.exec("SELECT version_no FROM t_ats_update_ghj ORDER BY update_time DESC");
        if(query.next())
        {
            QString latestVersion = query.value(0).toString();
            ui->leVersionNo->setPlaceholderText("请填写合适的版本号(当前最新版本为:"+latestVersion+")");
        }
    }
    else
    {
         slotCriticalDialog("数据库连接失败", "数据库连接失败，请确认数据库服务正常，并为本程序配置正确参数！");
    }
    dbUtils.closeDB();
    connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(slotOkClicked()));
    connect(ui->buttonBox, SIGNAL(rejected()), this, SLOT(slotCancelClicked()));
    connect(ui->openBtn, SIGNAL(clicked()), this, SLOT(slotOpenFile()));
    connect(mUploadWorker, SIGNAL(progress(int)), this, SLOT(handleProgressChanged(int)));
    connect(mUploadWorker, SIGNAL(uploadFailed(int)), this, SLOT(handleProgressChanged(int)));
    connect(mUploadWorker, SIGNAL(infoUpDialog(const QString&, const QString&)), this, SLOT(slotInfoDialog(const QString&, const QString&)));
    connect(mUploadWorker, SIGNAL(warningUpDialog(const QString&, const QString&)), this, SLOT(slotWarningDialog(const QString&, const QString&)));
    connect(mUploadWorker, SIGNAL(criticalUpDialog(const QString&, const QString&)), this, SLOT(slotCriticalDialog(const QString&, const QString&)));
    //connect(mUploadWorker, &QUploadWorker::finished, mUploadWorker, &QObject::deleteLater);
}

MainWindow::~MainWindow()
{
    delete ui;
    if(!mUploadWorker)
    {
        delete mUploadWorker;
        mUploadWorker = NULL;
    }
}

void MainWindow::slotOkClicked()
{
    QString versionNo = ui->leVersionNo->text();
    QString versionDesc = ui->leVersionDesc->toHtml();
    QString uploadUser = ui->leUploader->text();
    QString fileName = ui->leUploader->text();
    //使用正则表达式验证用户输入的版本号是否合法
    QRegExp rx("\\d{1,3}\\.\\d{1,3}\\.\\d{1,3}\\.\\d{1,3}");
    if( !rx.exactMatch(versionNo) )
    {
        QMessageBox::warning(this, "输入有误", "您输入的版本号不符合规则，请输入正确格式的版本号", QMessageBox::Ok);
        return;
    }
    if(uploadUser.isEmpty())
    {
        QMessageBox::warning(this, "输入有误", "您还没有填写上传者姓名，请填写姓名以便验证上传者身份", QMessageBox::Ok);
        return;
    }
    QFile file(fileName);
    if( !file.exists() )
    {
        QMessageBox::warning(this, "输入有误", "您选择的文件不存在，请选择正确的文件", QMessageBox::Ok);
        return;
    }
    if (QMessageBox::Yes == QMessageBox::question(this,
                                                  tr("您确定要上传该版本吗？"),
                                                  tr("请问您是否确定要上传该版本文件至服务器？"),
                                                  QMessageBox::Yes | QMessageBox::No,
                                                  QMessageBox::Yes)) {
        mUploadWorker->setVersionNo(versionNo);
        mUploadWorker->setVersionDesc(versionDesc);
        mUploadWorker->setUploadUser(uploadUser);
        mUploadWorker->setFileName(fileName);
        mUploadWorker->start();
        ui->progressBar->show();
    }
}

void MainWindow::slotCancelClicked()
{
    mUploadWorker->stop();
    ui->leVersionNo->clear();
    ui->leVersionDesc->clear();
    ui->leUploader->clear();
    ui->leUploadFile->clear();
    ui->progressBar->hide();
}

void MainWindow::slotOpenFile()
{
    QString path = QFileDialog::getOpenFileName(this, tr("请选择上传文件"), ".", tr("Zip File(*.zip)"));
    if(path.length() == 0) {
            //QMessageBox::information(NULL, tr("选择文件"), tr("您还未选择任何文件"));
    } else {
            //QMessageBox::information(NULL, tr("选择文件"), tr("您选择的文件是： ") + path);
        ui->leUploadFile->setText(path);
    }
}

void MainWindow::handleProgressChanged(int value)
{
    if(value == -1)
    {
        ui->progressBar->setValue(0);
        ui->progressBar->hide();
    }
    else
    {
        ui->progressBar->setValue(value);
    }
}

void MainWindow::slotInfoDialog(const QString &title="通知", const QString &text="")
{
    QMessageBox::information(this, title, text, QMessageBox::Ok);
}

void MainWindow::slotWarningDialog(const QString &title="警告", const QString &text="")
{
    QMessageBox::warning(this, title, text, QMessageBox::Ok);
}

void MainWindow::slotCriticalDialog(const QString &title="错误", const QString &text="")
{
    QMessageBox::critical(this, title, text, QMessageBox::Ok);
}


