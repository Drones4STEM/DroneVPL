#ifndef PROPERTYWIDGET_H
#define PROPERTYWIDGET_H

#include <QWidget>
#include <QComboBox>
#include <QLineEdit>
#include <QLabel>
#include <QTextEdit>

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
    QString m_direction;

    QLabel *directionLabel;

public slots:
    void setTextColor(const QColor &color);
    void setOutlineColor(const QColor &color);
    void setBackgroundColor(const QColor &color);
    void setIdentifier(const QString str);
    void setDirection(const QString str);
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

class MutableWidget :public QWidget
{
    Q_OBJECT
public:
    explicit MutableWidget(QWidget *parent=0);

    double altitude() const{return m_altitude;}
    double time() const{return m_time;}
    double speed() const{return m_speed;}
    double groundSpeed() const{return m_groundSpeed;}
    double m_altitude;
    double m_time;
    double m_speed;
    double m_groundSpeed;

    QLineEdit *altitudeLineEdit;
    QLineEdit *timeLineEdit;
    QLineEdit *speedLineEdit;
    QLineEdit *groundSpeedLineEdit;
public slots:
    void setAltitude(const double a);
    void setTime(const double t);
    void setSpeed(const double s);
    void setGroundSpeed(const double s);
signals:
    void altitudeChanged(const double a);
    void timeChanged(const double t);
    void speedChanged(const double s);
    void groundSpeedChanged(const double s);
private slots:
    void updateAltitude(QString str);
    void updateTime(QString str);
    void updateSpeed(QString str);
    void updateGroundSpeed(QString str);

private:
    void createWidgets();
    void createLayouts();
    void createConnections();
};


class ProcessOutputWidget:public QWidget
{
    Q_OBJECT
public:
    ProcessOutputWidget(QWidget *parent=0);

    QTextEdit *textEdit;

public slots:
    void setTextEdit(QString str);
};


#endif // PROPERTYWIDGET_H
