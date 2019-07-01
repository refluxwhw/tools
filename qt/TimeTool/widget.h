#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

protected:
    void timerEvent(QTimerEvent* e);

private slots:
    void onComboxIndexChanged();

private:
    Ui::Widget *ui;
    int m_curIndex = 0;
    int m_timerID = 0;
};

#endif // WIDGET_H
