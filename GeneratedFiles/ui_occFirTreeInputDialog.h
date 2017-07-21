/********************************************************************************
** Form generated from reading UI file 'occFirTreeInputDialog.ui'
**
** Created by: Qt User Interface Compiler version 5.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_OCCFIRTREEINPUTDIALOG_H
#define UI_OCCFIRTREEINPUTDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Dialog
{
public:
    QTabWidget *tabWidget;
    QWidget *tab;
    QRadioButton *radioButton;
    QRadioButton *radioButton_2;
    QListWidget *listWidget;
    QListWidget *listWidget_2;
    QWidget *gridLayoutWidget_3;
    QGridLayout *gridLayout_3;
    QWidget *gridLayoutWidget_4;
    QGridLayout *gridLayout_4;
    QPushButton *pushButton_5;
    QLabel *label_8;
    QDoubleSpinBox *doubleSpinBox_3;
    QLabel *label_9;
    QDoubleSpinBox *doubleSpinBox_4;
    QWidget *tab_2;
    QWidget *gridLayoutWidget_2;
    QGridLayout *gridLayout_2;
    QLabel *label_3;
    QLabel *label_4;
    QDoubleSpinBox *a1_doubleSpinBox;
    QDoubleSpinBox *a2_doubleSpinBox;
    QCheckBox *setHubAngles_checkBox;
    QLabel *a1Indegrees_label;
    QLabel *label_14;
    QLabel *a2Indegrees_label;
    QLabel *label_16;
    QWidget *tab_8;
    QWidget *gridLayoutWidget;
    QGridLayout *gridLayout;
    QLabel *label;
    QLabel *label_2;
    QFrame *line;
    QFrame *line_2;
    QTableWidget *depthFromFrontFace_tableWidget;
    QTableWidget *depthFromNextToFrontFace_tableWidget;
    QPushButton *addDepthFromFront_pushButton;
    QPushButton *removeDepthFromFront_pushButton;
    QPushButton *addDepthFromNextToFront_pushButton;
    QPushButton *removeDepthFromNextToFront_pushButton;
    QCheckBox *setDepthCutting_checkBox;
    QWidget *tab_7;
    QCheckBox *setFirTree_checkBox;
    QTabWidget *tabWidget_2;
    QWidget *tab_5;
    QLabel *label_18;
    QLabel *label_5;
    QDoubleSpinBox *a3_doubleSpinBox;
    QLabel *label_6;
    QLabel *label_20;
    QDoubleSpinBox *a4_doubleSpinBox;
    QLabel *a3Indegrees_label;
    QLabel *a4Indegrees_label;
    QWidget *gridLayoutWidget_5;
    QGridLayout *gridLayout_5;
    QLabel *label_13;
    QDoubleSpinBox *firTreeHubThickness_doubleSpinBox;
    QFrame *line_5;
    QWidget *tab_6;
    QLabel *label_7;
    QLabel *label_10;
    QPushButton *removeLobe_pushButton;
    QDoubleSpinBox *fiTreeBottomFilletRadius_doubleSpinBox;
    QLabel *label_12;
    QPushButton *addLobe_pushButton;
    QDoubleSpinBox *fiTreeUpperFilletRadius_doubleSpinBox;
    QLabel *label_11;
    QDoubleSpinBox *neckBottomWidth_doubleSpinBox;
    QTableWidget *lobeProperties_tableWidget;
    QDoubleSpinBox *neckHeight_doubleSpinBox;
    QFrame *line_3;
    QWidget *gridLayoutWidget_6;
    QGridLayout *gridLayout_6;
    QFrame *line_4;
    QWidget *tab_3;
    QCheckBox *checkBox_4;
    QWidget *tab_4;
    QCheckBox *checkBox_5;
    QWidget *horizontalLayoutWidget;
    QHBoxLayout *horizontalLayout;
    QPushButton *finishButton;
    QPushButton *cancelButton;
    QPushButton *pushButton;

    void setupUi(QDialog *Dialog)
    {
        if (Dialog->objectName().isEmpty())
            Dialog->setObjectName(QStringLiteral("Dialog"));
        Dialog->resize(621, 426);
        tabWidget = new QTabWidget(Dialog);
        tabWidget->setObjectName(QStringLiteral("tabWidget"));
        tabWidget->setGeometry(QRect(6, 9, 611, 361));
        tab = new QWidget();
        tab->setObjectName(QStringLiteral("tab"));
        radioButton = new QRadioButton(tab);
        radioButton->setObjectName(QStringLiteral("radioButton"));
        radioButton->setGeometry(QRect(30, 60, 111, 17));
        radioButton_2 = new QRadioButton(tab);
        radioButton_2->setObjectName(QStringLiteral("radioButton_2"));
        radioButton_2->setGeometry(QRect(30, 190, 111, 17));
        listWidget = new QListWidget(tab);
        listWidget->setObjectName(QStringLiteral("listWidget"));
        listWidget->setGeometry(QRect(50, 80, 256, 101));
        listWidget_2 = new QListWidget(tab);
        listWidget_2->setObjectName(QStringLiteral("listWidget_2"));
        listWidget_2->setGeometry(QRect(50, 210, 256, 111));
        gridLayoutWidget_3 = new QWidget(tab);
        gridLayoutWidget_3->setObjectName(QStringLiteral("gridLayoutWidget_3"));
        gridLayoutWidget_3->setGeometry(QRect(350, 80, 161, 101));
        gridLayout_3 = new QGridLayout(gridLayoutWidget_3);
        gridLayout_3->setObjectName(QStringLiteral("gridLayout_3"));
        gridLayout_3->setContentsMargins(0, 0, 0, 0);
        gridLayoutWidget_4 = new QWidget(tab);
        gridLayoutWidget_4->setObjectName(QStringLiteral("gridLayoutWidget_4"));
        gridLayoutWidget_4->setGeometry(QRect(350, 210, 161, 111));
        gridLayout_4 = new QGridLayout(gridLayoutWidget_4);
        gridLayout_4->setObjectName(QStringLiteral("gridLayout_4"));
        gridLayout_4->setContentsMargins(0, 0, 0, 0);
        pushButton_5 = new QPushButton(tab);
        pushButton_5->setObjectName(QStringLiteral("pushButton_5"));
        pushButton_5->setGeometry(QRect(520, 300, 75, 23));
        label_8 = new QLabel(tab);
        label_8->setObjectName(QStringLiteral("label_8"));
        label_8->setEnabled(false);
        label_8->setGeometry(QRect(120, 20, 71, 20));
        doubleSpinBox_3 = new QDoubleSpinBox(tab);
        doubleSpinBox_3->setObjectName(QStringLiteral("doubleSpinBox_3"));
        doubleSpinBox_3->setEnabled(false);
        doubleSpinBox_3->setGeometry(QRect(204, 20, 62, 22));
        label_9 = new QLabel(tab);
        label_9->setObjectName(QStringLiteral("label_9"));
        label_9->setEnabled(false);
        label_9->setGeometry(QRect(374, 20, 91, 16));
        doubleSpinBox_4 = new QDoubleSpinBox(tab);
        doubleSpinBox_4->setObjectName(QStringLiteral("doubleSpinBox_4"));
        doubleSpinBox_4->setEnabled(false);
        doubleSpinBox_4->setGeometry(QRect(474, 20, 62, 22));
        tabWidget->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QStringLiteral("tab_2"));
        tab_2->setAutoFillBackground(false);
        gridLayoutWidget_2 = new QWidget(tab_2);
        gridLayoutWidget_2->setObjectName(QStringLiteral("gridLayoutWidget_2"));
        gridLayoutWidget_2->setGeometry(QRect(350, 80, 231, 181));
        gridLayout_2 = new QGridLayout(gridLayoutWidget_2);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        gridLayout_2->setContentsMargins(0, 0, 0, 0);
        label_3 = new QLabel(tab_2);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(70, 120, 71, 20));
        label_4 = new QLabel(tab_2);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setGeometry(QRect(66, 160, 71, 20));
        a1_doubleSpinBox = new QDoubleSpinBox(tab_2);
        a1_doubleSpinBox->setObjectName(QStringLiteral("a1_doubleSpinBox"));
        a1_doubleSpinBox->setEnabled(false);
        a1_doubleSpinBox->setGeometry(QRect(140, 120, 62, 22));
        a1_doubleSpinBox->setDecimals(2);
        a1_doubleSpinBox->setMinimum(-100);
        a1_doubleSpinBox->setMaximum(100);
        a1_doubleSpinBox->setSingleStep(0.01);
        a2_doubleSpinBox = new QDoubleSpinBox(tab_2);
        a2_doubleSpinBox->setObjectName(QStringLiteral("a2_doubleSpinBox"));
        a2_doubleSpinBox->setEnabled(false);
        a2_doubleSpinBox->setGeometry(QRect(140, 160, 62, 22));
        a2_doubleSpinBox->setDecimals(2);
        a2_doubleSpinBox->setMinimum(-100);
        a2_doubleSpinBox->setMaximum(100);
        a2_doubleSpinBox->setSingleStep(0.01);
        setHubAngles_checkBox = new QCheckBox(tab_2);
        setHubAngles_checkBox->setObjectName(QStringLiteral("setHubAngles_checkBox"));
        setHubAngles_checkBox->setGeometry(QRect(480, 10, 91, 17));
        a1Indegrees_label = new QLabel(tab_2);
        a1Indegrees_label->setObjectName(QStringLiteral("a1Indegrees_label"));
        a1Indegrees_label->setGeometry(QRect(210, 120, 31, 16));
        label_14 = new QLabel(tab_2);
        label_14->setObjectName(QStringLiteral("label_14"));
        label_14->setGeometry(QRect(240, 120, 41, 16));
        a2Indegrees_label = new QLabel(tab_2);
        a2Indegrees_label->setObjectName(QStringLiteral("a2Indegrees_label"));
        a2Indegrees_label->setGeometry(QRect(210, 160, 31, 16));
        label_16 = new QLabel(tab_2);
        label_16->setObjectName(QStringLiteral("label_16"));
        label_16->setGeometry(QRect(240, 160, 41, 16));
        tabWidget->addTab(tab_2, QString());
        tab_8 = new QWidget();
        tab_8->setObjectName(QStringLiteral("tab_8"));
        gridLayoutWidget = new QWidget(tab_8);
        gridLayoutWidget->setObjectName(QStringLiteral("gridLayoutWidget"));
        gridLayoutWidget->setGeometry(QRect(359, 80, 181, 171));
        gridLayout = new QGridLayout(gridLayoutWidget);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        label = new QLabel(tab_8);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(40, 30, 81, 16));
        label_2 = new QLabel(tab_8);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(40, 170, 141, 16));
        line = new QFrame(tab_8);
        line->setObjectName(QStringLiteral("line"));
        line->setGeometry(QRect(40, 40, 231, 16));
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);
        line_2 = new QFrame(tab_8);
        line_2->setObjectName(QStringLiteral("line_2"));
        line_2->setGeometry(QRect(40, 180, 231, 16));
        line_2->setFrameShape(QFrame::HLine);
        line_2->setFrameShadow(QFrame::Sunken);
        depthFromFrontFace_tableWidget = new QTableWidget(tab_8);
        if (depthFromFrontFace_tableWidget->columnCount() < 2)
            depthFromFrontFace_tableWidget->setColumnCount(2);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        depthFromFrontFace_tableWidget->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        depthFromFrontFace_tableWidget->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        depthFromFrontFace_tableWidget->setObjectName(QStringLiteral("depthFromFrontFace_tableWidget"));
        depthFromFrontFace_tableWidget->setEnabled(false);
        depthFromFrontFace_tableWidget->setGeometry(QRect(40, 50, 211, 111));
        depthFromNextToFrontFace_tableWidget = new QTableWidget(tab_8);
        if (depthFromNextToFrontFace_tableWidget->columnCount() < 2)
            depthFromNextToFrontFace_tableWidget->setColumnCount(2);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        depthFromNextToFrontFace_tableWidget->setHorizontalHeaderItem(0, __qtablewidgetitem2);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        depthFromNextToFrontFace_tableWidget->setHorizontalHeaderItem(1, __qtablewidgetitem3);
        depthFromNextToFrontFace_tableWidget->setObjectName(QStringLiteral("depthFromNextToFrontFace_tableWidget"));
        depthFromNextToFrontFace_tableWidget->setEnabled(false);
        depthFromNextToFrontFace_tableWidget->setGeometry(QRect(40, 190, 211, 131));
        addDepthFromFront_pushButton = new QPushButton(tab_8);
        addDepthFromFront_pushButton->setObjectName(QStringLiteral("addDepthFromFront_pushButton"));
        addDepthFromFront_pushButton->setEnabled(false);
        addDepthFromFront_pushButton->setGeometry(QRect(260, 70, 31, 23));
        removeDepthFromFront_pushButton = new QPushButton(tab_8);
        removeDepthFromFront_pushButton->setObjectName(QStringLiteral("removeDepthFromFront_pushButton"));
        removeDepthFromFront_pushButton->setEnabled(false);
        removeDepthFromFront_pushButton->setGeometry(QRect(260, 100, 31, 23));
        addDepthFromNextToFront_pushButton = new QPushButton(tab_8);
        addDepthFromNextToFront_pushButton->setObjectName(QStringLiteral("addDepthFromNextToFront_pushButton"));
        addDepthFromNextToFront_pushButton->setEnabled(false);
        addDepthFromNextToFront_pushButton->setGeometry(QRect(260, 220, 31, 23));
        removeDepthFromNextToFront_pushButton = new QPushButton(tab_8);
        removeDepthFromNextToFront_pushButton->setObjectName(QStringLiteral("removeDepthFromNextToFront_pushButton"));
        removeDepthFromNextToFront_pushButton->setEnabled(false);
        removeDepthFromNextToFront_pushButton->setGeometry(QRect(260, 250, 31, 23));
        setDepthCutting_checkBox = new QCheckBox(tab_8);
        setDepthCutting_checkBox->setObjectName(QStringLiteral("setDepthCutting_checkBox"));
        setDepthCutting_checkBox->setEnabled(true);
        setDepthCutting_checkBox->setGeometry(QRect(470, 10, 111, 17));
        tabWidget->addTab(tab_8, QString());
        tab_7 = new QWidget();
        tab_7->setObjectName(QStringLiteral("tab_7"));
        setFirTree_checkBox = new QCheckBox(tab_7);
        setFirTree_checkBox->setObjectName(QStringLiteral("setFirTree_checkBox"));
        setFirTree_checkBox->setGeometry(QRect(490, 10, 81, 17));
        tabWidget_2 = new QTabWidget(tab_7);
        tabWidget_2->setObjectName(QStringLiteral("tabWidget_2"));
        tabWidget_2->setGeometry(QRect(0, 30, 611, 311));
        tab_5 = new QWidget();
        tab_5->setObjectName(QStringLiteral("tab_5"));
        label_18 = new QLabel(tab_5);
        label_18->setObjectName(QStringLiteral("label_18"));
        label_18->setGeometry(QRect(210, 50, 41, 16));
        label_5 = new QLabel(tab_5);
        label_5->setObjectName(QStringLiteral("label_5"));
        label_5->setGeometry(QRect(30, 50, 71, 20));
        a3_doubleSpinBox = new QDoubleSpinBox(tab_5);
        a3_doubleSpinBox->setObjectName(QStringLiteral("a3_doubleSpinBox"));
        a3_doubleSpinBox->setEnabled(false);
        a3_doubleSpinBox->setGeometry(QRect(110, 50, 62, 22));
        a3_doubleSpinBox->setDecimals(2);
        a3_doubleSpinBox->setMinimum(-100);
        a3_doubleSpinBox->setMaximum(100);
        a3_doubleSpinBox->setSingleStep(0.01);
        label_6 = new QLabel(tab_5);
        label_6->setObjectName(QStringLiteral("label_6"));
        label_6->setGeometry(QRect(30, 70, 71, 20));
        label_20 = new QLabel(tab_5);
        label_20->setObjectName(QStringLiteral("label_20"));
        label_20->setGeometry(QRect(210, 70, 41, 16));
        a4_doubleSpinBox = new QDoubleSpinBox(tab_5);
        a4_doubleSpinBox->setObjectName(QStringLiteral("a4_doubleSpinBox"));
        a4_doubleSpinBox->setEnabled(false);
        a4_doubleSpinBox->setGeometry(QRect(110, 70, 62, 22));
        a4_doubleSpinBox->setDecimals(2);
        a4_doubleSpinBox->setMinimum(-100);
        a4_doubleSpinBox->setMaximum(100);
        a4_doubleSpinBox->setSingleStep(0.01);
        a3Indegrees_label = new QLabel(tab_5);
        a3Indegrees_label->setObjectName(QStringLiteral("a3Indegrees_label"));
        a3Indegrees_label->setGeometry(QRect(180, 50, 31, 16));
        a4Indegrees_label = new QLabel(tab_5);
        a4Indegrees_label->setObjectName(QStringLiteral("a4Indegrees_label"));
        a4Indegrees_label->setGeometry(QRect(180, 70, 31, 16));
        gridLayoutWidget_5 = new QWidget(tab_5);
        gridLayoutWidget_5->setObjectName(QStringLiteral("gridLayoutWidget_5"));
        gridLayoutWidget_5->setGeometry(QRect(380, 40, 161, 171));
        gridLayout_5 = new QGridLayout(gridLayoutWidget_5);
        gridLayout_5->setObjectName(QStringLiteral("gridLayout_5"));
        gridLayout_5->setContentsMargins(0, 0, 0, 0);
        label_13 = new QLabel(tab_5);
        label_13->setObjectName(QStringLiteral("label_13"));
        label_13->setGeometry(QRect(20, 100, 71, 16));
        firTreeHubThickness_doubleSpinBox = new QDoubleSpinBox(tab_5);
        firTreeHubThickness_doubleSpinBox->setObjectName(QStringLiteral("firTreeHubThickness_doubleSpinBox"));
        firTreeHubThickness_doubleSpinBox->setEnabled(false);
        firTreeHubThickness_doubleSpinBox->setGeometry(QRect(110, 100, 62, 22));
        firTreeHubThickness_doubleSpinBox->setDecimals(4);
        firTreeHubThickness_doubleSpinBox->setMinimum(-99.9999);
        firTreeHubThickness_doubleSpinBox->setMaximum(99.9999);
        firTreeHubThickness_doubleSpinBox->setSingleStep(0.0001);
        line_5 = new QFrame(tab_5);
        line_5->setObjectName(QStringLiteral("line_5"));
        line_5->setGeometry(QRect(90, 50, 20, 71));
        line_5->setFrameShape(QFrame::VLine);
        line_5->setFrameShadow(QFrame::Sunken);
        tabWidget_2->addTab(tab_5, QString());
        tab_6 = new QWidget();
        tab_6->setObjectName(QStringLiteral("tab_6"));
        label_7 = new QLabel(tab_6);
        label_7->setObjectName(QStringLiteral("label_7"));
        label_7->setGeometry(QRect(10, 10, 151, 16));
        label_10 = new QLabel(tab_6);
        label_10->setObjectName(QStringLiteral("label_10"));
        label_10->setGeometry(QRect(10, 30, 151, 20));
        removeLobe_pushButton = new QPushButton(tab_6);
        removeLobe_pushButton->setObjectName(QStringLiteral("removeLobe_pushButton"));
        removeLobe_pushButton->setEnabled(false);
        removeLobe_pushButton->setGeometry(QRect(110, 260, 75, 23));
        fiTreeBottomFilletRadius_doubleSpinBox = new QDoubleSpinBox(tab_6);
        fiTreeBottomFilletRadius_doubleSpinBox->setObjectName(QStringLiteral("fiTreeBottomFilletRadius_doubleSpinBox"));
        fiTreeBottomFilletRadius_doubleSpinBox->setEnabled(false);
        fiTreeBottomFilletRadius_doubleSpinBox->setGeometry(QRect(180, 10, 62, 22));
        fiTreeBottomFilletRadius_doubleSpinBox->setDecimals(4);
        fiTreeBottomFilletRadius_doubleSpinBox->setMinimum(-99.9999);
        fiTreeBottomFilletRadius_doubleSpinBox->setMaximum(99.9999);
        fiTreeBottomFilletRadius_doubleSpinBox->setSingleStep(0.0001);
        label_12 = new QLabel(tab_6);
        label_12->setObjectName(QStringLiteral("label_12"));
        label_12->setGeometry(QRect(40, 80, 121, 16));
        addLobe_pushButton = new QPushButton(tab_6);
        addLobe_pushButton->setObjectName(QStringLiteral("addLobe_pushButton"));
        addLobe_pushButton->setEnabled(false);
        addLobe_pushButton->setGeometry(QRect(10, 260, 75, 23));
        fiTreeUpperFilletRadius_doubleSpinBox = new QDoubleSpinBox(tab_6);
        fiTreeUpperFilletRadius_doubleSpinBox->setObjectName(QStringLiteral("fiTreeUpperFilletRadius_doubleSpinBox"));
        fiTreeUpperFilletRadius_doubleSpinBox->setEnabled(false);
        fiTreeUpperFilletRadius_doubleSpinBox->setGeometry(QRect(180, 30, 62, 22));
        fiTreeUpperFilletRadius_doubleSpinBox->setDecimals(4);
        fiTreeUpperFilletRadius_doubleSpinBox->setMinimum(-99.9999);
        fiTreeUpperFilletRadius_doubleSpinBox->setMaximum(99.9999);
        fiTreeUpperFilletRadius_doubleSpinBox->setSingleStep(0.0001);
        label_11 = new QLabel(tab_6);
        label_11->setObjectName(QStringLiteral("label_11"));
        label_11->setGeometry(QRect(80, 60, 81, 16));
        neckBottomWidth_doubleSpinBox = new QDoubleSpinBox(tab_6);
        neckBottomWidth_doubleSpinBox->setObjectName(QStringLiteral("neckBottomWidth_doubleSpinBox"));
        neckBottomWidth_doubleSpinBox->setEnabled(false);
        neckBottomWidth_doubleSpinBox->setGeometry(QRect(180, 80, 62, 22));
        lobeProperties_tableWidget = new QTableWidget(tab_6);
        if (lobeProperties_tableWidget->columnCount() < 6)
            lobeProperties_tableWidget->setColumnCount(6);
        QTableWidgetItem *__qtablewidgetitem4 = new QTableWidgetItem();
        lobeProperties_tableWidget->setHorizontalHeaderItem(0, __qtablewidgetitem4);
        QTableWidgetItem *__qtablewidgetitem5 = new QTableWidgetItem();
        lobeProperties_tableWidget->setHorizontalHeaderItem(1, __qtablewidgetitem5);
        QTableWidgetItem *__qtablewidgetitem6 = new QTableWidgetItem();
        lobeProperties_tableWidget->setHorizontalHeaderItem(2, __qtablewidgetitem6);
        QTableWidgetItem *__qtablewidgetitem7 = new QTableWidgetItem();
        lobeProperties_tableWidget->setHorizontalHeaderItem(3, __qtablewidgetitem7);
        QTableWidgetItem *__qtablewidgetitem8 = new QTableWidgetItem();
        lobeProperties_tableWidget->setHorizontalHeaderItem(4, __qtablewidgetitem8);
        QTableWidgetItem *__qtablewidgetitem9 = new QTableWidgetItem();
        lobeProperties_tableWidget->setHorizontalHeaderItem(5, __qtablewidgetitem9);
        lobeProperties_tableWidget->setObjectName(QStringLiteral("lobeProperties_tableWidget"));
        lobeProperties_tableWidget->setEnabled(false);
        lobeProperties_tableWidget->setGeometry(QRect(0, 150, 611, 101));
        lobeProperties_tableWidget->setMaximumSize(QSize(16777209, 16777215));
        lobeProperties_tableWidget->setLineWidth(0);
        lobeProperties_tableWidget->setAutoScrollMargin(5);
        neckHeight_doubleSpinBox = new QDoubleSpinBox(tab_6);
        neckHeight_doubleSpinBox->setObjectName(QStringLiteral("neckHeight_doubleSpinBox"));
        neckHeight_doubleSpinBox->setEnabled(false);
        neckHeight_doubleSpinBox->setGeometry(QRect(180, 60, 62, 22));
        line_3 = new QFrame(tab_6);
        line_3->setObjectName(QStringLiteral("line_3"));
        line_3->setGeometry(QRect(160, 10, 20, 91));
        line_3->setFrameShape(QFrame::VLine);
        line_3->setFrameShadow(QFrame::Sunken);
        gridLayoutWidget_6 = new QWidget(tab_6);
        gridLayoutWidget_6->setObjectName(QStringLiteral("gridLayoutWidget_6"));
        gridLayoutWidget_6->setGeometry(QRect(360, 0, 160, 141));
        gridLayout_6 = new QGridLayout(gridLayoutWidget_6);
        gridLayout_6->setObjectName(QStringLiteral("gridLayout_6"));
        gridLayout_6->setContentsMargins(0, 0, 0, 0);
        tabWidget_2->addTab(tab_6, QString());
        line_4 = new QFrame(tab_7);
        line_4->setObjectName(QStringLiteral("line_4"));
        line_4->setGeometry(QRect(-3, 20, 611, 16));
        line_4->setFrameShape(QFrame::HLine);
        line_4->setFrameShadow(QFrame::Sunken);
        tabWidget->addTab(tab_7, QString());
        tab_3 = new QWidget();
        tab_3->setObjectName(QStringLiteral("tab_3"));
        checkBox_4 = new QCheckBox(tab_3);
        checkBox_4->setObjectName(QStringLiteral("checkBox_4"));
        checkBox_4->setGeometry(QRect(430, 10, 121, 20));
        tabWidget->addTab(tab_3, QString());
        tab_4 = new QWidget();
        tab_4->setObjectName(QStringLiteral("tab_4"));
        checkBox_5 = new QCheckBox(tab_4);
        checkBox_5->setObjectName(QStringLiteral("checkBox_5"));
        checkBox_5->setGeometry(QRect(470, 10, 121, 20));
        tabWidget->addTab(tab_4, QString());
        horizontalLayoutWidget = new QWidget(Dialog);
        horizontalLayoutWidget->setObjectName(QStringLiteral("horizontalLayoutWidget"));
        horizontalLayoutWidget->setGeometry(QRect(349, 380, 239, 31));
        horizontalLayout = new QHBoxLayout(horizontalLayoutWidget);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        finishButton = new QPushButton(horizontalLayoutWidget);
        finishButton->setObjectName(QStringLiteral("finishButton"));

        horizontalLayout->addWidget(finishButton);

        cancelButton = new QPushButton(horizontalLayoutWidget);
        cancelButton->setObjectName(QStringLiteral("cancelButton"));
        cancelButton->setEnabled(true);

        horizontalLayout->addWidget(cancelButton);

        pushButton = new QPushButton(horizontalLayoutWidget);
        pushButton->setObjectName(QStringLiteral("pushButton"));

        horizontalLayout->addWidget(pushButton);


        retranslateUi(Dialog);
        QObject::connect(cancelButton, SIGNAL(clicked()), Dialog, SLOT(reject()));

        tabWidget->setCurrentIndex(3);
        tabWidget_2->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(Dialog);
    } // setupUi

    void retranslateUi(QDialog *Dialog)
    {
        Dialog->setWindowTitle(QApplication::translate("Dialog", "Dialog", Q_NULLPTR));
        radioButton->setText(QApplication::translate("Dialog", "Cutting Tool Faces", Q_NULLPTR));
        radioButton_2->setText(QApplication::translate("Dialog", "Cutting Tool Solids", Q_NULLPTR));
        pushButton_5->setText(QApplication::translate("Dialog", "Show", Q_NULLPTR));
        label_8->setText(QApplication::translate("Dialog", "PlatForm Hight", Q_NULLPTR));
        label_9->setText(QApplication::translate("Dialog", "PlatForm Thickness", Q_NULLPTR));
        tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("Dialog", "Navigator", Q_NULLPTR));
        label_3->setText(QApplication::translate("Dialog", "A1 (degrees)", Q_NULLPTR));
        label_4->setText(QApplication::translate("Dialog", "A2 (degrees)", Q_NULLPTR));
        setHubAngles_checkBox->setText(QApplication::translate("Dialog", "Set HubAngles", Q_NULLPTR));
        a1Indegrees_label->setText(QApplication::translate("Dialog", "...", Q_NULLPTR));
        label_14->setText(QApplication::translate("Dialog", "rad", Q_NULLPTR));
        a2Indegrees_label->setText(QApplication::translate("Dialog", "...", Q_NULLPTR));
        label_16->setText(QApplication::translate("Dialog", "rad", Q_NULLPTR));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QApplication::translate("Dialog", "Set Hub Angles", Q_NULLPTR));
        label->setText(QApplication::translate("Dialog", "From Front Face", Q_NULLPTR));
        label_2->setText(QApplication::translate("Dialog", "From Next To Front Face", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem = depthFromFrontFace_tableWidget->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QApplication::translate("Dialog", "Depth", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem1 = depthFromFrontFace_tableWidget->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QApplication::translate("Dialog", "Hub Thickness", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem2 = depthFromNextToFrontFace_tableWidget->horizontalHeaderItem(0);
        ___qtablewidgetitem2->setText(QApplication::translate("Dialog", "Depth", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem3 = depthFromNextToFrontFace_tableWidget->horizontalHeaderItem(1);
        ___qtablewidgetitem3->setText(QApplication::translate("Dialog", "Hub Thickness", Q_NULLPTR));
        addDepthFromFront_pushButton->setText(QApplication::translate("Dialog", "+", Q_NULLPTR));
        removeDepthFromFront_pushButton->setText(QApplication::translate("Dialog", "-", Q_NULLPTR));
        addDepthFromNextToFront_pushButton->setText(QApplication::translate("Dialog", "+", Q_NULLPTR));
        removeDepthFromNextToFront_pushButton->setText(QApplication::translate("Dialog", "-", Q_NULLPTR));
        setDepthCutting_checkBox->setText(QApplication::translate("Dialog", "Set Depth Cutting", Q_NULLPTR));
        tabWidget->setTabText(tabWidget->indexOf(tab_8), QApplication::translate("Dialog", "Set Depth Cutting", Q_NULLPTR));
        setFirTree_checkBox->setText(QApplication::translate("Dialog", "Set Fir Tree", Q_NULLPTR));
        label_18->setText(QApplication::translate("Dialog", "rad", Q_NULLPTR));
        label_5->setText(QApplication::translate("Dialog", "A3 (degrees)", Q_NULLPTR));
        label_6->setText(QApplication::translate("Dialog", "A4 (degrees)", Q_NULLPTR));
        label_20->setText(QApplication::translate("Dialog", "rad", Q_NULLPTR));
        a3Indegrees_label->setText(QApplication::translate("Dialog", "...", Q_NULLPTR));
        a4Indegrees_label->setText(QApplication::translate("Dialog", "...", Q_NULLPTR));
        label_13->setText(QApplication::translate("Dialog", "Hub Thickness", Q_NULLPTR));
        tabWidget_2->setTabText(tabWidget_2->indexOf(tab_5), QApplication::translate("Dialog", "Set Position", Q_NULLPTR));
        label_7->setText(QApplication::translate("Dialog", "Fir Tree Bottom Fillet Radius (R)", Q_NULLPTR));
        label_10->setText(QApplication::translate("Dialog", "Fir Tree Upper Fillet Radius (r)", Q_NULLPTR));
        removeLobe_pushButton->setText(QApplication::translate("Dialog", "-", Q_NULLPTR));
        label_12->setText(QApplication::translate("Dialog", "Neck Bottom Width (L0)", Q_NULLPTR));
        addLobe_pushButton->setText(QApplication::translate("Dialog", "+", Q_NULLPTR));
        label_11->setText(QApplication::translate("Dialog", "Neck Height (nh)", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem4 = lobeProperties_tableWidget->horizontalHeaderItem(0);
        ___qtablewidgetitem4->setText(QApplication::translate("Dialog", "Load Length", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem5 = lobeProperties_tableWidget->horizontalHeaderItem(1);
        ___qtablewidgetitem5->setText(QApplication::translate("Dialog", "Load Angle", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem6 = lobeProperties_tableWidget->horizontalHeaderItem(2);
        ___qtablewidgetitem6->setText(QApplication::translate("Dialog", "Lobe Thickness", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem7 = lobeProperties_tableWidget->horizontalHeaderItem(3);
        ___qtablewidgetitem7->setText(QApplication::translate("Dialog", "Widths (L)", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem8 = lobeProperties_tableWidget->horizontalHeaderItem(4);
        ___qtablewidgetitem8->setText(QApplication::translate("Dialog", "Convex Radius", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem9 = lobeProperties_tableWidget->horizontalHeaderItem(5);
        ___qtablewidgetitem9->setText(QApplication::translate("Dialog", "Concave Radius", Q_NULLPTR));
        tabWidget_2->setTabText(tabWidget_2->indexOf(tab_6), QApplication::translate("Dialog", "Set Shape", Q_NULLPTR));
        tabWidget->setTabText(tabWidget->indexOf(tab_7), QApplication::translate("Dialog", "Set Fir Tree", Q_NULLPTR));
        checkBox_4->setText(QApplication::translate("Dialog", "Set Hub Bottom line", Q_NULLPTR));
        tabWidget->setTabText(tabWidget->indexOf(tab_3), QApplication::translate("Dialog", "Set Hub Bottom line", Q_NULLPTR));
        checkBox_5->setText(QApplication::translate("Dialog", "Set Cutting Angle", Q_NULLPTR));
        tabWidget->setTabText(tabWidget->indexOf(tab_4), QApplication::translate("Dialog", "Set Cutting Angle", Q_NULLPTR));
        finishButton->setText(QApplication::translate("Dialog", "Finish", Q_NULLPTR));
        cancelButton->setText(QApplication::translate("Dialog", "Cancel", Q_NULLPTR));
        pushButton->setText(QApplication::translate("Dialog", "Apply", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class Dialog: public Ui_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_OCCFIRTREEINPUTDIALOG_H
