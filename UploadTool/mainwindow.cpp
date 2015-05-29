#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "uploaddialog.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->progressBar->hide();
    connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(slotOkClicked()));
    connect(ui->buttonBox, SIGNAL(rejected()), this, SLOT(slotCancelClicked()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::slotOkClicked()
{

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
}

