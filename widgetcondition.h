#ifndef WIDGETCONDITION_H
#define WIDGETCONDITION_H

#include <QWidget>

class QLabel;
class QLineEdit;
class QComboBox;

class WidgetCondition : public QWidget
{
    Q_OBJECT

//private slots:
    //set

public:
    WidgetCondition(QWidget *parent = 0);
    void updateConditon(){};

private:
    QLabel *parameters;
    QLabel *x_axis;
    QLabel *y_axis;
    QLineEdit *x_coordinate;
    QLineEdit *y_coordinate;
    QLabel *widgetType;
    QLabel *WidgetType;
};

#endif // WIDGET_H
