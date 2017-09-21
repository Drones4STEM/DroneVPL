#include "propertywidget.h"
#include "aqp/alt_key.hpp"
#include "QFormLayout"
#include "swatch.h"
#include "QDebug"

ColorWidget::ColorWidget(QWidget *parent)
    :QWidget(parent)
{
    createWidgets();
    setTextColor(QColor());
    setOutlineColor(QColor());
    setBackgroundColor(QColor());
    setIdentifier(NULL);
    setDirection(NULL);
    createLayout();
    createConnections();
    setFixedSize(minimumSizeHint());
}

void ColorWidget::createWidgets()
{
    textColorComboBox = new QComboBox;
    foreach (const QString &name, QColor::colorNames()) {
        QColor color(name);
        textColorComboBox->addItem(colorSwatch(color),name,color);
    }
    outlineColorComboBox = new QComboBox;
    foreach (const QString &name, QColor::colorNames()) {
        QColor color(name);
        outlineColorComboBox->addItem(colorSwatch(color),name,color);
    }
    backgroundColorComboBox = new QComboBox;
    foreach (const QString &name, QColor::colorNames()) {
        QColor color(name);
        backgroundColorComboBox->addItem(colorSwatch(color),name,color);
    }
    identifierLabel = new QLabel;
    directionLabel = new QLabel;
}


void ColorWidget::createLayout()
{
    QFormLayout *layout = new QFormLayout;
    layout->addRow(tr("type"),identifierLabel);
    layout->addRow(tr("Direction"),directionLabel);
    layout->addRow(tr("textColor"),textColorComboBox);
    layout->addRow(tr("outlineColor"),outlineColorComboBox);
    layout->addRow(tr("backgroundColor"),backgroundColorComboBox);
    setLayout(layout);
}

void ColorWidget::createConnections()
{
    connect(textColorComboBox,SIGNAL(currentIndexChanged(int)),
            this,SLOT(updateTextColor(int)));
    connect(outlineColorComboBox,SIGNAL(currentIndexChanged(int)),
            this,SLOT(updateOutlineColor(int)));
    connect(backgroundColorComboBox,SIGNAL(currentIndexChanged(int)),
            this,SLOT(updateBackgroundColor(int)));
}

void ColorWidget::setTextColor(const QColor &color)
{
    if(m_textColor!=color)
    {
        m_textColor=color;
        textColorComboBox->setCurrentIndex(
                    textColorComboBox->findData(m_textColor));
    }
}

void ColorWidget::setOutlineColor(const QColor &color)
{
    if(m_outlineColor!=color)
    {
        m_outlineColor=color;
        outlineColorComboBox->setCurrentIndex(
                    outlineColorComboBox->findData(m_outlineColor));
    }
}

void ColorWidget::setBackgroundColor(const QColor &color)
{
    if(m_backgroundColor!=color)
        m_backgroundColor=color;
    backgroundColorComboBox->setCurrentIndex(
                backgroundColorComboBox->findData(m_backgroundColor));
}

void ColorWidget::setIdentifier(const QString str)
{
    if(m_identifier!=str)
        m_identifier=str;
    identifierLabel->setText(m_identifier);
}

void ColorWidget::setDirection(const QString str)
{
    if(m_direction!=str)
        m_direction=str;
    directionLabel->setText(m_direction);
}

void ColorWidget::updateTextColor(int index)
{
    m_textColor=textColorComboBox->itemData(index).value<QColor>();
    emit textColorChanged(m_textColor);
}

void ColorWidget::updateOutlineColor(int index)
{
    m_outlineColor=outlineColorComboBox->itemData(index).value<QColor>();
    emit outlineColorChanged(m_outlineColor);
}

void ColorWidget::updateBackgroundColor(int index)
{
    m_backgroundColor=backgroundColorComboBox->itemData(index).value<QColor>();
    emit backgroundColorChanged(m_backgroundColor);
}

/*////////////////               PositionWidget             ///////////////////*/
PositionWidget::PositionWidget(QWidget *parent)
    :QWidget(parent)
{
    createWidgets();
    QPoint pos(0,0);
    setPosition(pos);
    createLayouts();
    createConnections();
    setFixedSize(minimumSizeHint());
}

void PositionWidget::createWidgets()
{
    xLineEdit = new QLineEdit;
    yLineEdit = new QLineEdit;
}

void PositionWidget::createLayouts()
{
    QFormLayout *layout = new QFormLayout;
    layout->addRow(tr("x:"),xLineEdit);
    layout->addRow(tr("y:"),yLineEdit);
    setLayout(layout);
}

void PositionWidget::createConnections()
{
    connect(xLineEdit,SIGNAL(textChanged(QString)),
            this,SLOT(updateXPosition(QString)));
    connect(yLineEdit,SIGNAL(textChanged(QString)),
            this,SLOT(updateYPosition(QString)));
}

void PositionWidget::setPosition(const QPoint pos)
{
    if(m_pos!=pos)
    {
        m_pos.setX(pos.x());
        m_pos.setY(pos.y());
        xLineEdit->setText(QString::number(pos.x()));
        yLineEdit->setText(QString::number(pos.y()));
    }
}

void PositionWidget::updateXPosition(QString x)
{

    m_pos.setX(x.toInt());
    emit positionChanged(m_pos);
}

void PositionWidget::updateYPosition(QString y)
{
    m_pos.setY(y.toInt());
    emit positionChanged(m_pos);
}


/*/////////////////////    MutableWidget       /////////////////*/
MutableWidget::MutableWidget(QWidget *parent)
    :QWidget(parent)
{
    createWidgets();
    setAltitude(0.0);
    setTime(0.0);
    setSpeed(0.0);
    setGroundSpeed(0.0);
    createLayouts();
    createConnections();
    setFixedSize(minimumSizeHint());
}

void MutableWidget::createWidgets()
{
    altitudeLineEdit = new QLineEdit;
    timeLineEdit = new QLineEdit;
    speedLineEdit = new QLineEdit;
    groundSpeedLineEdit = new QLineEdit;
}

void MutableWidget::createLayouts()
{
    QFormLayout *layout = new QFormLayout;
    layout->addRow(tr("altitude:"),altitudeLineEdit);
    layout->addRow(tr("time:"),timeLineEdit);
    layout->addRow(tr("speed"),speedLineEdit);
    layout->addRow(tr("groundSpeed"),groundSpeedLineEdit);
    setLayout(layout);
}

void MutableWidget::createConnections()
{
    connect(altitudeLineEdit,SIGNAL(textChanged(QString)),
            this,SLOT(updateAltitude(QString)));
    connect(timeLineEdit,SIGNAL(textChanged(QString)),
            this,SLOT(updateTime(QString)));
    connect(speedLineEdit,SIGNAL(textChanged(QString)),
            this,SLOT(updateSpeed(QString)));
    connect(groundSpeedLineEdit,SIGNAL(textChanged(QString)),
            this,SLOT(updateGroundSpeed(QString)));
}

void MutableWidget::setAltitude(const double a)
{
    if(m_altitude!=a)
    {
        m_altitude=a;
        altitudeLineEdit->setText(QString::number(a));
    }
}

void MutableWidget::setTime(const double t)
{
    if(m_time!=t)
    {
        m_time=t;
        timeLineEdit->setText(QString::number(t));
    }
}

void MutableWidget::setSpeed(const double s)
{
    if(m_speed!=s)
    {
        m_speed=s;
        speedLineEdit->setText(QString::number(s));
    }
}

void MutableWidget::setGroundSpeed(const double s)
{
    if(m_groundSpeed!=s)
    {
        m_groundSpeed=s;
        groundSpeedLineEdit->setText(QString::number(s));
    }
}

void MutableWidget::updateAltitude(QString str)
{
    m_altitude=str.toDouble();
    emit altitudeChanged(m_altitude);
}

void MutableWidget::updateTime(QString str)
{
    m_time=str.toDouble();
    emit timeChanged(m_time);
}

void MutableWidget::updateSpeed(QString str)
{
    m_speed=str.toDouble();
    emit speedChanged(m_speed);
}

void MutableWidget::updateGroundSpeed(QString str)
{
    m_groundSpeed=str.toDouble();
    emit groundSpeedChanged(m_groundSpeed);
}
