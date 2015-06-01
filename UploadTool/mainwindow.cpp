#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QDialog>
#include <QFileDialog>
#include <QMessageBox>
#include <QProgressBar>
#include <QThread>

#include "uploadworker.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    mUploadWorker(NULL)
{
    mUploadWorker = new UploadWorker();
    ui->setupUi(this);
    ui->progressBar->hide();
    connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(slotOkClicked()));
    connect(ui->buttonBox, SIGNAL(rejected()), this, SLOT(slotCancelClicked()));
    connect(ui->openBtn, SIGNAL(clicked()), this, SLOT(slotOpenFile()));
    connect(mUploadWorker, SIGNAL(progress(int)), this, SLOT(handleProgressChanged(int)));
    connect(mUploadWorker, SIGNAL(uploadFailed(int)), this, SLOT(handleProgressChanged(int)));
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
    if (QMessageBox::Yes == QMessageBox::warning(this,
                                                  tr("您确定要上传该版本吗？"),
                                                  tr("请问您是否确定要上传该版本文件至服务器？"),
                                                  QMessageBox::Yes | QMessageBox::No,
                                                  QMessageBox::Yes)) {
        mUploadWorker->setVersionNo(ui->leVersionNo->text());
        mUploadWorker->setVersionDesc(ui->leVersionDesc->toHtml());
        mUploadWorker->setUploadUser(ui->leUploader->text());
        mUploadWorker->setFileName(ui->leUploadFile->text());
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
    else if(value > 0)
    {
        ui->progressBar->setValue(value);
    }
    if(value == 100)
    {
       ui->progressBar->hide();
    }
}


