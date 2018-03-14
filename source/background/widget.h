#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

/***************************************************
 * Class name: Widget
 * Base class: QWidget
 * Description: When the real widget code hadn't been
 *    finished, I have to replace that with something.
 *    This is a formally widget, only used to complet
 *    the grammar highlight, and has no real usage.
****************************************************/

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

private:
    Ui::Widget *ui;
};

#endif // WIDGET_H
