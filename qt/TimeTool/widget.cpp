#include "widget.h"
#include "ui_widget.h"
#include <QDateTime>
#include <QTimer>
#include <QMessageBox>
#include <QDebug>

#define TIME_FORMAT "yyyy-MM-dd HH:mm:ss"
#define TIME_FORMAT_MS "yyyy-MM-dd HH:mm:ss.zzz"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    connect(ui->comboBox, SIGNAL(currentIndexChanged(int)),
            this, SLOT(onComboxIndexChanged()));

    connect(ui->pushButton, &QPushButton::clicked,
            [this] {
        if (0 != m_timerID) { // 暂停
            killTimer(m_timerID);
            m_timerID = 0;
            ui->pushButton->setText("继续");
        } else { // 继续
            m_timerID = startTimer(100);
            ui->pushButton->setText("暂停");
        }
    });

    connect(ui->pushButton_int2str, &QPushButton::clicked,
            [this] {
        double val = ui->doubleSpinBox->value();
        qint64 ms = (double)val;
        QString str;
        QString format = TIME_FORMAT_MS;
        if (0 == m_curIndex) {
            ms = ms * 1000;
            format = TIME_FORMAT;
        }
        QDateTime t = QDateTime::fromMSecsSinceEpoch(ms);
        str = t.toString(format);
        ui->lineEdit->setText(str);
    });

    connect(ui->pushButton_str2int, &QPushButton::clicked,
            [this] {
        QString str = ui->lineEdit->text();
        QDateTime t = QDateTime::fromString(str, TIME_FORMAT);
        if (!t.isValid()) {
            t = QDateTime::fromString(str, TIME_FORMAT_MS);
            if (!t.isValid()) {
                QMessageBox::about(this, QString::fromLocal8Bit("格式错误"),
                                   QString::fromLocal8Bit("请按照以下格式输入时间：\n"
                                                          "yyyy-MM-dd HH:mm:ss\n"
                                                          "yyyy-MM-dd HH:mm:ss.zzz"));
                return;
            }
        }
        qint64 ms = t.toMSecsSinceEpoch();
        double val = ms;
        if (0 == m_curIndex) {
            val = val / 1000;
        }
        ui->doubleSpinBox->setValue(val);
    });

    m_timerID = startTimer(100);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::timerEvent(QTimerEvent *)
{
    QDateTime now = QDateTime::currentDateTime();
    QString str = now.toString(TIME_FORMAT_MS);
    qint64  val = now.toMSecsSinceEpoch();
    QString txt = QString("%1   %2 ms").arg(str).arg(val);
    ui->label_cur->setText(txt);
}

void Widget::onComboxIndexChanged()
{
    int index = ui->comboBox->currentIndex();
    if (index == m_curIndex) {
        return;
    }
    m_curIndex = index;

    // double val = ui->doubleSpinBox->value();
    // if (0 == index) {
    //     val = val / 1000;
    // } else {
    //     val = val * 1000;
    // }
    // ui->doubleSpinBox->setValue(val);
}
