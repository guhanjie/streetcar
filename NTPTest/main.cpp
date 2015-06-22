#include <QCoreApplication>
#include <ntputil.h>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    NTPUtil ntpUtil;
    ntpUtil.ntpUpdate();
    return a.exec();
}
