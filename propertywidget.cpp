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
}


void ColorWidget::createLayout()
{
    QFormLayout *layout = new QFormLayout;
    layout->addRow(tr("type"),identifierLabel);
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


