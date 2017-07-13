#include <QtGui>
#include "widgetcondition.h"

WidgetCondition::WidgetCondition(QWidget *parent) :
    QWidget(parent)

{
    x_axis = new QLabel(tr("&x:"));
    x_coordinate = new QLineEdit;
    x_axis->setBuddy(x_coordinate);
    y_axis = new QLabel(tr("&y:"));
    y_coordinate = new QLineEdit;
    y_axis->setBuddy(y_coordinate);

    widgetType = new QLabel(tr("Type:"));
    chooseWidgetType = new QComboBox;

    QHBoxLayout *topLayout = new QHBoxLayout;
    topLayout->addWidget(x_axis);
    topLayout->addWidget(x_coordinate);
    topLayout->addWidget(y_axis);
    topLayout->addWidget(y_coordinate);

    QHBoxLayout *middleLayout = new QHBoxLayout;
    middleLayout->addWidget(widgetType);
    middleLayout->addWidget(chooseWidgetType);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(topLayout);
    mainLayout->addLayout(middleLayout);
    setLayout(mainLayout);
}
