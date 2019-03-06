#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QHostAddress>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->lineEdit_ip, &QLineEdit::textEdited, [this] {
        QString str = ui->lineEdit_ip->text();
        QHostAddress addr(str);
        if (addr.isNull()) {
            ui->lineEdit_ip->setStyleSheet("color: red");
        } else {
            ui->lineEdit_ip->setStyleSheet("color: black");
            ui->doubleSpinBox_int->setValue(addr.toIPv4Address());
        }
    });

    connect(ui->doubleSpinBox_int, &QDoubleSpinBox::editingFinished, [this] {
        quint32 val = (quint32)ui->doubleSpinBox_int->value();
        QHostAddress addr(val);
        if (addr.isNull()) {
            ui->doubleSpinBox_int->setStyleSheet("color: red");
        } else {
            ui->doubleSpinBox_int->setStyleSheet("color: black");
            ui->lineEdit_ip->setText(addr.toString());
        }
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}
