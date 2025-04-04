#pragma once
#ifndef QUESTIONWIDGET_H
#define QUESTIONWIDGET_H
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

namespace Ui {
    class QuestionWidget {
    public:
        QVBoxLayout* mainLayout;
        QLabel* questionTitle;
        QGridLayout* optionLayout;
        QWidget* optionASpacer;
        QVBoxLayout* optionAAligner;
        QPushButton* optionA;
        QWidget* optionBSpacer;
        QVBoxLayout* optionBAligner;
        QPushButton* optionB;
        QWidget* optionCSpacer;
        QVBoxLayout* optionCAligner;
        QPushButton* optionC;
        QWidget* optionDSpacer;
        QVBoxLayout* optionDAligner;
        QPushButton* optionD;
        void setupUi(){
            mainLayout = new QVBoxLayout;
            questionTitle = new QLabel;
            questionTitle->setObjectName("questionTitle");
            QFont font;
            font.setFamilies({QString::fromUtf8("Microsoft JhengHei")});
            font.setPointSize(26);
            questionTitle->setFont(font);
            questionTitle->setAlignment(Qt::AlignmentFlag::AlignCenter);
            questionTitle->setWordWrap(true);

            mainLayout->addWidget(questionTitle);

            optionLayout = new QGridLayout();
            optionLayout->setSpacing(16);
            optionLayout->setContentsMargins(80, 8, 80, 8);
            optionASpacer = new QWidget;
            optionASpacer->setMinimumSize(QSize(180, 70));
            optionAAligner = new QVBoxLayout(optionASpacer);
            optionAAligner->setObjectName("optionAAligner");
            optionA = new QPushButton(optionASpacer);
            optionA->setObjectName("optionA");
            optionA->setMinimumSize(QSize(1, 100));
            QFont font1;
            font1.setFamilies({QString::fromUtf8("Microsoft JhengHei")});
            font1.setPointSize(15);
            optionA->setFont(font1);
            optionA->hide();

            optionAAligner->addWidget(optionA);

            optionLayout->addWidget(optionASpacer, 1, 1, 1, 1);

            optionBSpacer = new QWidget;
            optionBAligner = new QVBoxLayout(optionBSpacer);
            optionB = new QPushButton(optionBSpacer);
            optionB->setObjectName("optionB");
            optionB->setMinimumSize(QSize(0, 100));
            optionB->setFont(font1);
            optionB->hide();

            optionBAligner->addWidget(optionB);

            optionLayout->addWidget(optionBSpacer, 1, 2, 1, 1);

            optionCSpacer = new QWidget;
            optionCAligner = new QVBoxLayout(optionCSpacer);
            optionC = new QPushButton(optionCSpacer);
            optionC->setObjectName("optionC");
            optionC->setMinimumSize(QSize(0, 100));
            optionC->setFont(font1);
            optionC->hide();

            optionCAligner->addWidget(optionC);

            optionLayout->addWidget(optionCSpacer, 2, 1, 1, 1);

            optionDSpacer = new QWidget;
            optionDAligner = new QVBoxLayout(optionDSpacer);
            optionD = new QPushButton(optionDSpacer);
            optionD->setMinimumSize(QSize(0, 100));
            optionD->setFont(font1);
            optionD->hide();

            optionDAligner->addWidget(optionD);

            optionLayout->addWidget(optionDSpacer, 2, 2, 1, 1);

            mainLayout->addLayout(optionLayout);
        } // setupUi
    };
}

#endif