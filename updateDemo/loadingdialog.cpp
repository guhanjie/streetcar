#include "loadingdialog.h"

#include <QVBoxLayout>
#include <QLabel>
#include <QMovie>

LoadingDialog::LoadingDialog(QWidget *parent) :
    QDialog(parent)
{
    this->setFixedSize(150, 100);
    //设置透明度
    this->setWindowOpacity(0.8);
    //设置背景色为淡蓝色
    this->setStyleSheet("background-color: white;");
    //取消对话框标题
    this->setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);
    QLabel* label = new QLabel(this);
    label->setStyleSheet("background-color: transparent;");
    label->setGeometry(50, 15, 20, 20);
    QMovie* movie = new QMovie("loading.gif");
    label->setScaledContents(true);
    label->setMovie(movie);
    movie->start();
    QLabel* tip_label = new QLabel(this);
    tip_label->setStyleSheet("color: blue; background-color: transparent;");
    tip_label->setGeometry(20, 65, 80, 20);
    tip_label->setText("正在获取最新版本...");
    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(label);
    layout->addWidget(tip_label);
    setLayout(layout);
}
