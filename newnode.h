#ifndef NewNode_H
#define NewNode_H

#include <QApplication>
#include <QColor>
#include <QGraphicsItem>
#include <QSet>

class Link;
class Yuan;
class triYuan;

/*******************************************************************
 * Class name: NewNode
 * Base class: Node
 * Desciption:This is a class for developers, and determines some
 *     to follow. This class is the parent class of IoNode, IoSmallNode,
 *     Rec, LandonNode, TranslationNode, SomeNode, VardefNode.
******************************************************************/
class NewNode : public QGraphicsObject
{
    //Q_DECLARE_TR_FUNCTIONS(Yuan)

public:
    NewNode();
    ~NewNode();

    void setText(const QString &text);
    QString text() const;
    void setTextColor(const QColor &color);
    QColor textColor() const;
    void setOutlineColor(const QColor &color);
    QColor outlineColor() const;
    void setBackgroundColor(const QColor &color);
    QColor backgroundColor() const;
    int roundness(double size) const;


    QRectF outlineRect() const;
    QRectF boundingRect() const;
    QPainterPath shape() const;
    void paint(QPainter *painter,
               const QStyleOptionGraphicsItem *option, QWidget *widget);

    triYuan* yuan;
    Yuan* yuan2;
    Yuan* myYuan()const;

    //控件名称与在同类控件中的编号
    QString identifier;
    int controlsId;
    QString name;   //identifier+controlsId

protected:
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
    QVariant itemChange(GraphicsItemChange change,
                        const QVariant &value);


private:
    QString myText;
    QColor myTextColor;
    QColor myBackgroundColor;
    QColor myOutlineColor;
};

#endif
