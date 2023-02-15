/********************************************************************************
** Form generated from reading UI file 'view.ui'
**
** Created by: Qt User Interface Compiler version 5.12.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_VIEW_H
#define UI_VIEW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_projectClass
{
public:
    QWidget *centralWidget;
    QGridLayout *gridLayout_2;
    QGroupBox *groupBox_3;
    QGridLayout *gridLayout;
    QLabel *label;
    QPushButton *pushButton_2;
    QSpacerItem *horizontalSpacer_2;
    QGroupBox *groupBox_2;
    QGridLayout *gridLayout_4;
    QTextEdit *textEdit;
    QGroupBox *groupBox;
    QGridLayout *gridLayout_3;
    QLabel *label_10;
    QLabel *label_7;
    QLabel *label_4;
    QLabel *label_11;
    QLabel *label_8;
    QLabel *label_5;
    QLabel *label_2;
    QLabel *label_12;
    QLabel *label_9;
    QLabel *label_3;
    QPushButton *pushButton;
    QSpacerItem *horizontalSpacer;
    QLabel *label_1;
    QLabel *label_6;
    QPushButton *pushButton_3;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *projectClass)
    {
        if (projectClass->objectName().isEmpty())
            projectClass->setObjectName(QString::fromUtf8("projectClass"));
        projectClass->resize(953, 747);
        centralWidget = new QWidget(projectClass);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        gridLayout_2 = new QGridLayout(centralWidget);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        groupBox_3 = new QGroupBox(centralWidget);
        groupBox_3->setObjectName(QString::fromUtf8("groupBox_3"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(1);
        sizePolicy.setVerticalStretch(1);
        sizePolicy.setHeightForWidth(groupBox_3->sizePolicy().hasHeightForWidth());
        groupBox_3->setSizePolicy(sizePolicy);
        QFont font;
        font.setFamily(QString::fromUtf8("Adobe \351\273\221\344\275\223 Std R"));
        font.setPointSize(10);
        font.setBold(true);
        font.setWeight(75);
        groupBox_3->setFont(font);
        gridLayout = new QGridLayout(groupBox_3);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        label = new QLabel(groupBox_3);
        label->setObjectName(QString::fromUtf8("label"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(1);
        sizePolicy1.setVerticalStretch(10);
        sizePolicy1.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy1);
        label->setStyleSheet(QString::fromUtf8("background-color: rgba(122, 122, 122);"));

        gridLayout->addWidget(label, 0, 0, 1, 2);

        pushButton_2 = new QPushButton(groupBox_3);
        pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));
        QSizePolicy sizePolicy2(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(1);
        sizePolicy2.setHeightForWidth(pushButton_2->sizePolicy().hasHeightForWidth());
        pushButton_2->setSizePolicy(sizePolicy2);

        gridLayout->addWidget(pushButton_2, 1, 1, 1, 1);

        horizontalSpacer_2 = new QSpacerItem(58, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer_2, 1, 0, 1, 1);


        gridLayout_2->addWidget(groupBox_3, 0, 1, 1, 1);

        groupBox_2 = new QGroupBox(centralWidget);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        QSizePolicy sizePolicy3(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy3.setHorizontalStretch(1);
        sizePolicy3.setVerticalStretch(1);
        sizePolicy3.setHeightForWidth(groupBox_2->sizePolicy().hasHeightForWidth());
        groupBox_2->setSizePolicy(sizePolicy3);
        gridLayout_4 = new QGridLayout(groupBox_2);
        gridLayout_4->setSpacing(6);
        gridLayout_4->setContentsMargins(11, 11, 11, 11);
        gridLayout_4->setObjectName(QString::fromUtf8("gridLayout_4"));
        textEdit = new QTextEdit(groupBox_2);
        textEdit->setObjectName(QString::fromUtf8("textEdit"));

        gridLayout_4->addWidget(textEdit, 0, 0, 1, 1);


        gridLayout_2->addWidget(groupBox_2, 1, 1, 1, 1);

        groupBox = new QGroupBox(centralWidget);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        QSizePolicy sizePolicy4(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy4.setHorizontalStretch(3);
        sizePolicy4.setVerticalStretch(3);
        sizePolicy4.setHeightForWidth(groupBox->sizePolicy().hasHeightForWidth());
        groupBox->setSizePolicy(sizePolicy4);
        groupBox->setFont(font);
        gridLayout_3 = new QGridLayout(groupBox);
        gridLayout_3->setSpacing(6);
        gridLayout_3->setContentsMargins(11, 11, 11, 11);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        label_10 = new QLabel(groupBox);
        label_10->setObjectName(QString::fromUtf8("label_10"));
        QSizePolicy sizePolicy5(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy5.setHorizontalStretch(1);
        sizePolicy5.setVerticalStretch(5);
        sizePolicy5.setHeightForWidth(label_10->sizePolicy().hasHeightForWidth());
        label_10->setSizePolicy(sizePolicy5);
        label_10->setStyleSheet(QString::fromUtf8("background-color: rgba(122, 122, 122);"));

        gridLayout_3->addWidget(label_10, 0, 0, 1, 1);

        label_7 = new QLabel(groupBox);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        sizePolicy5.setHeightForWidth(label_7->sizePolicy().hasHeightForWidth());
        label_7->setSizePolicy(sizePolicy5);
        label_7->setStyleSheet(QString::fromUtf8("background-color: rgba(122, 122, 122);"));

        gridLayout_3->addWidget(label_7, 0, 1, 1, 1);

        label_4 = new QLabel(groupBox);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        sizePolicy5.setHeightForWidth(label_4->sizePolicy().hasHeightForWidth());
        label_4->setSizePolicy(sizePolicy5);
        label_4->setStyleSheet(QString::fromUtf8("background-color: rgba(122, 122, 122);"));

        gridLayout_3->addWidget(label_4, 0, 3, 1, 1);

        label_11 = new QLabel(groupBox);
        label_11->setObjectName(QString::fromUtf8("label_11"));
        sizePolicy5.setHeightForWidth(label_11->sizePolicy().hasHeightForWidth());
        label_11->setSizePolicy(sizePolicy5);
        label_11->setStyleSheet(QString::fromUtf8("background-color: rgba(122, 122, 122);"));

        gridLayout_3->addWidget(label_11, 1, 0, 1, 1);

        label_8 = new QLabel(groupBox);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        sizePolicy5.setHeightForWidth(label_8->sizePolicy().hasHeightForWidth());
        label_8->setSizePolicy(sizePolicy5);
        label_8->setStyleSheet(QString::fromUtf8("background-color: rgba(122, 122, 122);"));

        gridLayout_3->addWidget(label_8, 1, 1, 1, 1);

        label_5 = new QLabel(groupBox);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        sizePolicy5.setHeightForWidth(label_5->sizePolicy().hasHeightForWidth());
        label_5->setSizePolicy(sizePolicy5);
        label_5->setStyleSheet(QString::fromUtf8("background-color: rgba(122, 122, 122);"));

        gridLayout_3->addWidget(label_5, 1, 3, 1, 1);

        label_2 = new QLabel(groupBox);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        sizePolicy5.setHeightForWidth(label_2->sizePolicy().hasHeightForWidth());
        label_2->setSizePolicy(sizePolicy5);
        label_2->setStyleSheet(QString::fromUtf8("background-color: rgba(122, 122, 122);"));

        gridLayout_3->addWidget(label_2, 1, 4, 1, 1);

        label_12 = new QLabel(groupBox);
        label_12->setObjectName(QString::fromUtf8("label_12"));
        sizePolicy5.setHeightForWidth(label_12->sizePolicy().hasHeightForWidth());
        label_12->setSizePolicy(sizePolicy5);
        label_12->setStyleSheet(QString::fromUtf8("background-color: rgba(122, 122, 122);"));

        gridLayout_3->addWidget(label_12, 2, 0, 1, 1);

        label_9 = new QLabel(groupBox);
        label_9->setObjectName(QString::fromUtf8("label_9"));
        sizePolicy5.setHeightForWidth(label_9->sizePolicy().hasHeightForWidth());
        label_9->setSizePolicy(sizePolicy5);
        label_9->setStyleSheet(QString::fromUtf8("background-color: rgba(122, 122, 122);"));

        gridLayout_3->addWidget(label_9, 2, 1, 1, 1);

        label_3 = new QLabel(groupBox);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        sizePolicy5.setHeightForWidth(label_3->sizePolicy().hasHeightForWidth());
        label_3->setSizePolicy(sizePolicy5);
        label_3->setStyleSheet(QString::fromUtf8("background-color: rgba(122, 122, 122);"));

        gridLayout_3->addWidget(label_3, 2, 4, 1, 1);

        pushButton = new QPushButton(groupBox);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        sizePolicy2.setHeightForWidth(pushButton->sizePolicy().hasHeightForWidth());
        pushButton->setSizePolicy(sizePolicy2);
        pushButton->setCursor(QCursor(Qt::PointingHandCursor));

        gridLayout_3->addWidget(pushButton, 3, 0, 1, 1);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_3->addItem(horizontalSpacer, 3, 1, 1, 1);

        label_1 = new QLabel(groupBox);
        label_1->setObjectName(QString::fromUtf8("label_1"));
        label_1->setEnabled(true);
        sizePolicy5.setHeightForWidth(label_1->sizePolicy().hasHeightForWidth());
        label_1->setSizePolicy(sizePolicy5);
        label_1->setAutoFillBackground(false);
        label_1->setStyleSheet(QString::fromUtf8("background-color: rgb(122, 122, 122);"));
        label_1->setScaledContents(false);

        gridLayout_3->addWidget(label_1, 0, 4, 1, 1);

        label_6 = new QLabel(groupBox);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        sizePolicy5.setHeightForWidth(label_6->sizePolicy().hasHeightForWidth());
        label_6->setSizePolicy(sizePolicy5);
        label_6->setStyleSheet(QString::fromUtf8("background-color: rgba(122, 122, 122);"));

        gridLayout_3->addWidget(label_6, 2, 3, 1, 1);

        pushButton_3 = new QPushButton(groupBox);
        pushButton_3->setObjectName(QString::fromUtf8("pushButton_3"));
        QFont font1;
        font1.setBold(false);
        font1.setWeight(50);
        pushButton_3->setFont(font1);

        gridLayout_3->addWidget(pushButton_3, 3, 3, 1, 2);


        gridLayout_2->addWidget(groupBox, 0, 0, 2, 1);

        projectClass->setCentralWidget(centralWidget);
        mainToolBar = new QToolBar(projectClass);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        projectClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(projectClass);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        projectClass->setStatusBar(statusBar);

        retranslateUi(projectClass);

        QMetaObject::connectSlotsByName(projectClass);
    } // setupUi

    void retranslateUi(QMainWindow *projectClass)
    {
        projectClass->setWindowTitle(QApplication::translate("projectClass", "project", nullptr));
        groupBox_3->setTitle(QApplication::translate("projectClass", "\345\233\272\345\256\232\347\233\270\346\234\272", nullptr));
        label->setText(QString());
        pushButton_2->setText(QApplication::translate("projectClass", "\346\213\215\347\205\247", nullptr));
        groupBox_2->setTitle(QString());
        groupBox->setTitle(QApplication::translate("projectClass", "\346\234\272\346\242\260\350\207\202\347\233\270\346\234\272", nullptr));
        label_10->setText(QString());
        label_7->setText(QString());
        label_4->setText(QString());
        label_11->setText(QString());
        label_8->setText(QString());
        label_5->setText(QString());
        label_2->setText(QString());
        label_12->setText(QString());
        label_9->setText(QString());
        label_3->setText(QString());
        pushButton->setText(QApplication::translate("projectClass", "\346\213\215\347\205\247", nullptr));
        label_1->setText(QApplication::translate("projectClass", "<html><head/><body><p><br/></p></body></html>", nullptr));
        label_6->setText(QString());
        pushButton_3->setText(QApplication::translate("projectClass", "Change:test Result is [1-4]", nullptr));
    } // retranslateUi

};

namespace Ui {
    class projectClass: public Ui_projectClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_VIEW_H
