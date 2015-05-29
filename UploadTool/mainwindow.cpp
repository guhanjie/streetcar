#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include <QDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->progressBar->hide();
    connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(slotOkClicked()));
    connect(ui->buttonBox, SIGNAL(rejected()), this, SLOT(slotCancelClicked()));
    connect(ui->openBtn, SIGNAL(clicked()), this, SLOT(slotOpenFile()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::slotOkClicked()
{
    this->connectDB();
    ui->progressBar->show();
}

void MainWindow::slotCancelClicked()
{
    ui->leVersionNo->clear();
    ui->leVersionDesc->clear();
    ui->leUploader->clear();
    ui->leUploadFile->clear();
    ui->progressBar->hide();
}

void MainWindow::slotOpenFile()
{
    qDebug() << "xxx";
    QDialog* dialog = new QDialog(this);
    dialog->setAttribute(Qt::WA_DeleteOnClose);
    dialog->setWindowTitle("请选择文件");
    dialog->exec();
}

void MainWindow::connectDB()
{
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
    db.close();
}

