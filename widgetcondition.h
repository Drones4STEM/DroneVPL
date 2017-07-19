#ifndef WIDGETCONDITION_H
#define WIDGETCONDITION_H

#include <QWidget>

class QLabel;
class QLineEdit;
class QComboBox;

class WidgetCondition : public QWidget
{
    Q_OBJECT

public:
    WidgetCondition(QWidget *parent = 0);

private:
    QLabel *x_axis;
    QLabel *y_axis;
    QLineEdit *x_coordinate;
    QLineEdit *y_coordinate;
    QLabel *widgetType;
    QComboBox *chooseWidgetType;
};

#endif // WIDGET_H
