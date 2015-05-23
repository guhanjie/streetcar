#include <QCoreApplication>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QStringList>
#include <QDebug>
#include <iostream>

using namespace std;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QSqlDatabase db;

    QStringList drivers = QSqlDatabase::drivers();
    foreach (QString driver, drivers)
        qDebug() <<"/t"<< driver;

    db = QSqlDatabase::addDatabase("QOCI");
    db.setDatabaseName("orcl");
    db.setPort(1521);
    db.setHostName("127.0.0.1");
    db.setUserName("scott");
    db.setPassword("tiger");
    if (db.open())
    {
        qDebug() << "connect ok";
    }else{
        qDebug() << db.lastError().text();
    }
    return 0;
}
