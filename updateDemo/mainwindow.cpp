#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QSettings>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QSettings settings("./config.ini", QSettings::IniFormat);
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
    ui->textBrowser->append("ready to update...\n");
    emit buttonClicked();
}

void MainWindow::onButtonClicked()
{
    ui->textBrowser->append("ready to update...\n");
}
