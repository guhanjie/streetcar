#include <QCoreApplication>
#include <ntputil.h>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    NTPUtil ntpUtil;
    ntpUtil.f_System_Now();
    return a.exec();
}
