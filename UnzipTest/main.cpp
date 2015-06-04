#include <QCoreApplication>
#include <QDir>
#include <QDebug>
#include "quazip.h"
#include "quazipfile.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QuaZip archive("Desktop.zip");
    if (!archive.open(QuaZip::mdUnzip))
        return false;

    QString out_dir = "./zip";
    QString path = out_dir;
    if (!path.endsWith("/") && !out_dir.endsWith("\\"))
        path += "/";

    QDir dir(out_dir);
    if (!dir.exists())
        dir.mkpath(".");

    for( bool f = archive.goToFirstFile(); f; f = archive.goToNextFile() )
    {
        QString filePath = archive.getCurrentFileName();
        QuaZipFile zFile(archive.getZipName(), filePath);
        zFile.open(QIODevice::ReadOnly );
        QByteArray ba = zFile.readAll();
        qDebug() << ba.size()/1024 <<"Byte\t" << "--------\t" << filePath;
        zFile.close();

        if (filePath.endsWith("/"))
        {
            dir.mkpath(filePath);
        }
        else
        {
            QFile dstFile(path + filePath);
            if (!dstFile.open(QIODevice::WriteOnly))
                return false;
            dstFile.write(ba);
            dstFile.close();
        }
    }
    if(archive.getZipError()==UNZ_OK) {
           // ok, there was no error
        qDebug() << "ok";
    }
    return a.exec();
}
