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
    Q_OBJECT
    Q_PROPERTY(QColor textColor READ textColor WRITE setTextColor)
    Q_PROPERTY(QColor backgroundColor READ backgroundColor WRITE setBackgroundColor)
    Q_PROPERTY(QColor outlineColor READ outlineColor WRITE setOutlineColor)

public:
    NewNode();
    ~NewNode();

    void setText(const QString &text);
    QString text() const;    
    QColor textColor() const;    
    QColor outlineColor() const;    
    QColor backgroundColor() const;
    int roundness(double size) const;


    QRectF outlineRect() const;
    QRectF boundingRect() const;
    QPainterPath shape() const;
    void paint(QPainter *painter,
               const QStyleOptionGraphicsItem *option, QWidget *widget);
/*
    bool set_controlsId(int id);
    bool set_identifier(QString idtf);
    bool set_name();
    //bool set_master(WidgetWrap* w);
*/
    triYuan* yuan;
    Yuan* yuan2;
    Yuan* myYuan()const;

    //控件名称与在同类控件中的编号
    QString identifier;
    int controlsId;
    QString name;   //identifier+controlsId
    //WidgetWrap* wrap;
    int lx,ly;

signals:
    void dirty();
public slots:
    void setTextColor(const QColor &color);
    void setOutlineColor(const QColor &color);
    void setBackgroundColor(const QColor &color);

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
