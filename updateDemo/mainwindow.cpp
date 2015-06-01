#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QSettings>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->pushButton, SIGNAL(clicked()), this, SIGNAL(buttonClicked()));
    connect(this, SIGNAL(buttonClicked()), this, SLOT(onButtonClicked()));
    QSettings settings("MySoft", "Star Runner");
    settings.setValue("user", "guhanjie");
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
