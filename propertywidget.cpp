#include "propertywidget.h"
#include "aqp/alt_key.hpp"
#include "QFormLayout"
#include "swatch.h"

ColorWidget::ColorWidget(QWidget *parent)
    :QWidget(parent)
{
    createWidgets();
    setTextColor(QColor());
    setOutlineColor(QColor());
    setBackgroundColor(QColor());
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
}


void ColorWidget::createLayout()
{
    QFormLayout *layout = new QFormLayout;
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


