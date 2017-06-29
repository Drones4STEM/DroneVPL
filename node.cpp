#include <QtGui>

#include "link.h"
#include "node.h"
#include "yuan.h"

Node::Node()
{
    myTextColor = Qt::darkGreen;
    myOutlineColor = Qt::darkBlue;
    myBackgroundColor = Qt::white;

    QGraphicsItem* p=dynamic_cast<QGraphicsItem*>(this);
    yuan=new triYuan(p);

    setFlags(ItemIsMovable | ItemIsSelectable);
    setFlag(ItemSendsGeometryChanges);

    controlsId=0;
    identifier="Node";
}

Node::~Node()
{
    delete yuan;
}

void Node::setText(const QString &text)
{
    prepareGeometryChange();
    myText = text;
    update();
}

QString Node::text() const
{
    return myText;
}

void Node::setTextColor(const QColor &color)
{
    myTextColor = color;
    update();
}

QColor Node::textColor() const
{
    return myTextColor;
}

void Node::setOutlineColor(const QColor &color)
{
    myOutlineColor = color;
    update();
}

QColor Node::outlineColor() const
{
    return myOutlineColor;
}

void Node::setBackgroundColor(const QColor &color)
{
    myBackgroundColor = color;
    update();
}

QColor Node::backgroundColor() const
{
    return myBackgroundColor;
}

QRectF Node::boundingRect() const
{
    const int Margin = 6;
    return outlineRect().adjusted(-Margin, -Margin, +Margin, +Margin);
}

QPainterPath Node::shape() const
{
    QRectF rect = outlineRect();

    QPainterPath path;
    path.addRoundRect(rect, roundness(rect.width()),
                      roundness(rect.height()));
    return path;
}

void Node::paint(QPainter *painter,
                 const QStyleOptionGraphicsItem *option,
                 QWidget * /* widget */)
{
    QPen pen(outlineColor());
    if (option->state &QStyle::State_Selected) {
        pen.setStyle(Qt::DotLine);
        pen.setWidth(2);
    }
    painter->setPen(pen);
    painter->setBrush(backgroundColor());

    QRectF rect = outlineRect();
    painter->drawRoundRect(rect, roundness(rect.width()),
                           roundness(rect.height()));

    painter->setPen(textColor());
    painter->drawText(rect, Qt::AlignCenter, myText);
}

void Node::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    QString text = QInputDialog::getText(event->widget(),
                           tr("Edit Text"), tr("Enter new text:"),
                           QLineEdit::Normal, myText);
    if (!text.isEmpty())
        setText(text);
}

QVariant Node::itemChange(GraphicsItemChange change,
                    const QVariant &value)
{
    if (change & ItemPositionHasChanged) {
        yuan->setPos(pos().x(),
                     pos().y() + outlineRect().height()/2 +yuan->boundingRect().height()/2);
        foreach (Link *link, yuan->myLinks)
        {link->trackYuans();update();}
    }
    return QGraphicsItem::itemChange(change, value);
}


QRectF Node::outlineRect() const
{
    const int Padding = 8;
    QFontMetricsF metrics = qApp->font();
    QRectF rect = metrics.boundingRect(myText);
    rect.adjust(-Padding, -Padding, +Padding, +Padding);
    rect.translate(-rect.center());
    return rect;
}

int Node::roundness(double size) const
{
    const int Diameter = 12;
    return 100 * Diameter / int(size);
}

triYuan* Node::myYuan()const
{
    return yuan;
}
