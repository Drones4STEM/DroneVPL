#ifndef PROPERTYWIDGET_H
#define PROPERTYWIDGET_H

#include <QWidget>
#include <QComboBox>
#include <QLineEdit>
#include <QLabel>


class ColorWidget:public QWidget
{
    Q_OBJECT

public:
    explicit ColorWidget(QWidget *parent=0);

    QColor textColor() const {return m_textColor;}
    QColor m_textColor;
    QColor m_outlineColor;
    QColor m_backgroundColor;
    QString m_identifier;

public slots:
    void setTextColor(const QColor &color);
    void setOutlineColor(const QColor &color);
    void setBackgroundColor(const QColor &color);
    void setIdentifier(const QString str);
signals:
    void textColorChanged(const QColor &color);
    void outlineColorChanged(const QColor &color);
    void backgroundColorChanged(const QColor &color);
private slots:
    void updateTextColor(int index);
    void updateOutlineColor(int index);
    void updateBackgroundColor(int index);

private:
    void createWidgets();
    void createLayout();
    void createConnections();

    QComboBox *textColorComboBox;
    QComboBox *outlineColorComboBox;
    QComboBox *backgroundColorComboBox;
    QLabel *identifierLabel;
};


class PositionWidget :public QWidget
{
    Q_OBJECT
public:
    explicit PositionWidget(QWidget *parent=0);

    QPoint position() const{return m_pos;}
    QPoint m_pos;

public slots:
    void setPosition(const QPoint pos);
signals:
    void positionChanged(const QPoint pos);
private slots:
    void updateXPosition(QString x);
    void updateYPosition(QString y);

private:
    void createWidgets();
    void createLayouts();
    void createConnections();

    QLineEdit *xLineEdit;
    QLineEdit *yLineEdit;
};


#endif // PROPERTYWIDGET_H
