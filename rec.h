#ifndef REC_H
#define REC_H

#include <QApplication>
#include <QColor>
#include <QGraphicsItem>
#include <QSet>
#include <QtGui>

#include<QtCore>

#include "node.h"
#include "newnode.h"
#include "itemtypes.h"
#include "link.h"


class NewNode;
class DiagramWindow;
class Yuan;
class Link;
class QGraphicsSceneMouseEvent;
class QKeyEvent;

/*******************************************************************
 * Class name: Rec
 * Base class: NewNode
 * Description: This is the declaration of class Rec. Rec acts as
 *        logic rectangle.
******************************************************************/
class Rec : public NewNode
{
    Q_OBJECT
public:
    enum {Type = RecType};
    Rec();
    ~Rec();

    int type() const { return Type; }

    QRectF outlineRect() const;
    QRectF boundingRect() const;
    QPainterPath shape() const;
    void paint(QPainter *painter,
               const QStyleOptionGraphicsItem *option, QWidget *widget);
    void setText(const QString &text);
    QString text() const;
    void setLogichw();  //newnode中有sethw，但logic的绘图方法和正常的控件不一样，故另写一个

    QComboBox *box;
    QGraphicsItem *item;

    //控件名称与在同类控件中的编号
    QString identifier;
    int controlsId;

    QList<Link*> flink; //从logic指入的link
    QList<Link*> tlink; //从logic指出的link
private:
    QRectF mBoundingRect;

    bool m_resizing;

/*protected slots:
    void slot_changeRect();*/

protected:
    QVariant itemChange(GraphicsItemChange change,
                        const QVariant &value);
    //void keyPressEvent(QKeyEvent *event);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);


private slots:
    void showYuan();
};


#endif // REC_H
