#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QHostAddress>
#include <QtEndian>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowFlags(windowFlags() & ~Qt::WindowMinMaxButtonsHint);

    connect(ui->lineEdit_ip, &QLineEdit::textEdited, [this] {
        QString str = ui->lineEdit_ip->text();
        QHostAddress addr(str);
        if (addr.isNull()) {
            ui->lineEdit_ip->setStyleSheet("color: red");
        } else {
            ui->lineEdit_ip->setStyleSheet("color: black");
            quint32 val = addr.toIPv4Address(); // 主机字节序
            ui->doubleSpinBox_host->setValue(val);
            ui->doubleSpinBox_network->setValue(qToBigEndian(val));
        }
    });

    connect(ui->doubleSpinBox_network, &QDoubleSpinBox::editingFinished, [this] {
        quint32 val = (quint32)ui->doubleSpinBox_network->value();
        val = qToBigEndian(val);
        ui->doubleSpinBox_host->setValue(val);

        QHostAddress addr(val); // 要求传入主机字节序
        ui->lineEdit_ip->setText(addr.toString());
    });

    connect(ui->doubleSpinBox_host, &QDoubleSpinBox::editingFinished, [this] {
        quint32 val = (quint32)ui->doubleSpinBox_host->value();
        QHostAddress addr(val);
        ui->lineEdit_ip->setText(addr.toString());

        val = qToBigEndian(val); // 转为网络字节序
        ui->doubleSpinBox_network->setValue(val);
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}
