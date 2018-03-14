#include <QtGui>
#include "widgetcondition.h"
#include "diagramwindow.h"

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
    WidgetType = new QLabel(tr("WidgetType"));

    parameters = new QLabel(tr("Something"));

    QHBoxLayout *topLayout = new QHBoxLayout;
    topLayout->addWidget(widgetType);
    topLayout->addWidget(WidgetType);

    QHBoxLayout *middleLayout = new QHBoxLayout;
    middleLayout->addWidget(x_axis);
    middleLayout->addWidget(x_coordinate);
    middleLayout->addWidget(y_axis);
    middleLayout->addWidget(y_coordinate);

    QHBoxLayout *buttomLayout = new QHBoxLayout;
    buttomLayout->addWidget(parameters);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(topLayout);
    mainLayout->addLayout(middleLayout);
    mainLayout->addLayout(buttomLayout);
    setLayout(mainLayout);

}

void WidgetCondition::setCondition(QGraphicsItem *item){
    qreal x=item->pos().x();
    QString x1 = QString::number(x,'g',6);
    this->x_coordinate->setText(x1);
    qreal y=item->pos().y();
    QString y1 = QString::number(y,'g',6);
    this->y_coordinate->setText(y1);

}
