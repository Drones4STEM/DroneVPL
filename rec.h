#ifndef REC_H
#define REC_H

#include <QApplication>
#include <QColor>
#include <QGraphicsItem>
#include <QSet>
#include <QtGui>

#include "node.h"
#include "newnode.h"
#include "itemtypes.h"

class NewNode;
class DiagramWindow;
class Yuan;

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

    QComboBox *box;
    QGraphicsItem *item;

    //控件名称与在同类控件中的编号
    QString identifier;
    int controlsId;

protected:
    QVariant itemChange(GraphicsItemChange change,
                        const QVariant &value);
private slots:
    void showYuan();
};


#endif // REC_H
