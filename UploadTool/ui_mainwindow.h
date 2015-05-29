/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.4.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QGridLayout *gridLayout;
    QDialogButtonBox *buttonBox;
    QFrame *frame;
    QGridLayout *gridLayout_2;
    QLabel *lblVersionNo;
    QLineEdit *leVersionNo;
    QLabel *lblVersionDesc;
    QTextEdit *leVersionDesc;
    QLabel *lblUploader;
    QLineEdit *leUploader;
    QLabel *lblUploadFile;
    QHBoxLayout *horizontalLayout;
    QLineEdit *leUploadFile;
    QToolButton *openBtn;
    QProgressBar *progressBar;
    QMenuBar *menuBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(400, 309);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        gridLayout = new QGridLayout(centralWidget);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        buttonBox = new QDialogButtonBox(centralWidget);
        buttonBox->setObjectName(QStringLiteral("buttonBox"));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
        buttonBox->setCenterButtons(false);

        gridLayout->addWidget(buttonBox, 2, 0, 1, 1);

        frame = new QFrame(centralWidget);
        frame->setObjectName(QStringLiteral("frame"));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        gridLayout_2 = new QGridLayout(frame);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        lblVersionNo = new QLabel(frame);
        lblVersionNo->setObjectName(QStringLiteral("lblVersionNo"));

        gridLayout_2->addWidget(lblVersionNo, 0, 0, 1, 1);

        leVersionNo = new QLineEdit(frame);
        leVersionNo->setObjectName(QStringLiteral("leVersionNo"));
        leVersionNo->setMaxLength(15);
        leVersionNo->setClearButtonEnabled(true);

        gridLayout_2->addWidget(leVersionNo, 0, 1, 1, 1);

        lblVersionDesc = new QLabel(frame);
        lblVersionDesc->setObjectName(QStringLiteral("lblVersionDesc"));
        lblVersionDesc->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);

        gridLayout_2->addWidget(lblVersionDesc, 1, 0, 1, 1);

        leVersionDesc = new QTextEdit(frame);
        leVersionDesc->setObjectName(QStringLiteral("leVersionDesc"));

        gridLayout_2->addWidget(leVersionDesc, 1, 1, 1, 1);

        lblUploader = new QLabel(frame);
        lblUploader->setObjectName(QStringLiteral("lblUploader"));

        gridLayout_2->addWidget(lblUploader, 2, 0, 1, 1);

        leUploader = new QLineEdit(frame);
        leUploader->setObjectName(QStringLiteral("leUploader"));
        leUploader->setClearButtonEnabled(true);

        gridLayout_2->addWidget(leUploader, 2, 1, 1, 1);

        lblUploadFile = new QLabel(frame);
        lblUploadFile->setObjectName(QStringLiteral("lblUploadFile"));

        gridLayout_2->addWidget(lblUploadFile, 3, 0, 1, 1);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        leUploadFile = new QLineEdit(frame);
        leUploadFile->setObjectName(QStringLiteral("leUploadFile"));

        horizontalLayout->addWidget(leUploadFile);

        openBtn = new QToolButton(frame);
        openBtn->setObjectName(QStringLiteral("openBtn"));

        horizontalLayout->addWidget(openBtn);


        gridLayout_2->addLayout(horizontalLayout, 3, 1, 1, 1);

        lblUploadFile->raise();
        lblUploader->raise();
        lblVersionDesc->raise();
        lblVersionNo->raise();
        leVersionNo->raise();
        leVersionDesc->raise();
        leUploader->raise();

        gridLayout->addWidget(frame, 0, 0, 1, 1);

        progressBar = new QProgressBar(centralWidget);
        progressBar->setObjectName(QStringLiteral("progressBar"));
        progressBar->setValue(0);
        progressBar->setTextVisible(true);
        progressBar->setInvertedAppearance(false);
        progressBar->setTextDirection(QProgressBar::TopToBottom);

        gridLayout->addWidget(progressBar, 1, 0, 1, 1);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 400, 28));
        MainWindow->setMenuBar(menuBar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0));
        lblVersionNo->setText(QApplication::translate("MainWindow", "\347\211\210\346\234\254\345\217\267", 0));
#ifndef QT_NO_TOOLTIP
        leVersionNo->setToolTip(QApplication::translate("MainWindow", "<html><head/><body><p>\347\211\210\346\234\254\345\217\267\346\240\274\345\274\217\345\246\202\357\274\2321.0.0.0</p></body></html>", 0));
#endif // QT_NO_TOOLTIP
        leVersionNo->setPlaceholderText(QApplication::translate("MainWindow", "1.0.0.0", 0));
        lblVersionDesc->setText(QApplication::translate("MainWindow", "\347\211\210\346\234\254\346\217\217\350\277\260", 0));
        lblUploader->setText(QApplication::translate("MainWindow", "\344\270\212\344\274\240\350\200\205", 0));
        lblUploadFile->setText(QApplication::translate("MainWindow", "\344\270\212\344\274\240\346\226\207\344\273\266", 0));
        openBtn->setText(QApplication::translate("MainWindow", "...", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
