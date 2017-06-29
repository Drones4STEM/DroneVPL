#include "ionode.h"
#include "yuan.h"
#include "link.h"

IoNode::IoNode()
{
    box=new QComboBox;
    node1=new IoSmallNode;
    node2=new IoSmallNode;
    node3=new IoSmallNode;

    identifier="IoNode";
}

IoNode::~IoNode()
{
    delete box;
    delete node1;
    delete node2;
    delete node3;
}

QRectF IoNode::outlineRect() const
{
    QRectF rect(0,0,60,60);
    rect.translate(-rect.center());
    return rect;
}

QRectF IoNode::boundingRect() const
{
    const int Margin = 6;
    return outlineRect().adjusted(-Margin, -Margin, +Margin, +Margin);
}

QPainterPath IoNode::shape() const
{
    QRectF rect = outlineRect();

    QPainterPath path;
    path.addRoundRect(rect, roundness(rect.width()),
                      roundness(rect.height()));
    return path;
}

void IoNode::paint(QPainter *painter,
                 const QStyleOptionGraphicsItem *option,
                 QWidget * /* widget */)
{
    QPen pen(outlineColor());
    if (option->state & QStyle::State_Selected) {
        pen.setStyle(Qt::DotLine);
        pen.setWidth(2);
    }
    painter->setPen(pen);
    painter->setBrush(backgroundColor());

    QRectF rect = outlineRect();
    painter->drawRoundRect(rect, roundness(rect.width()),
                           roundness(rect.height()));

    painter->setPen(textColor());
    QString myText = text();
    painter->drawText(rect, Qt::AlignCenter, myText);
}

QVariant IoNode::itemChange(GraphicsItemChange change, const QVariant &value)
{
    if (change & ItemPositionHasChanged) {
        yuan->setPos(pos().x(),
                     pos().y() + outlineRect().height()/2 + yuan->boundingRect().height()/2);
        foreach (Link *link, yuan->myLinks)
        {link->trackYuans();update();}

        yuan2->setPos(pos().x() - outlineRect().width()/2 - yuan2->outlineRect().width()/2,
                     pos().y());
        foreach (Link *link, yuan2->myLinks)
        {link->trackYuans();update();}

        item->setPos(pos().x() - outlineRect().width()/2,
                     pos().y() - outlineRect().height()/2 - item->boundingRect().height());

        node2->setPos(pos().x() + outlineRect().width()/2 + node2->outlineRect().width()/2,
                      pos().y());
        node1->setPos(pos().x() + outlineRect().width()/2 + node1->outlineRect().width()/2,
                      node2->pos().y() - node2->outlineRect().height());
        node3->setPos(pos().x() + outlineRect().width()/2 + node3->outlineRect().width()/2,
                      node2->pos().y() + node2->outlineRect().height());
    }
    return QGraphicsItem::itemChange(change, value);
}


// ////////////////////////////////////////////////////////////////////////

IoSmallNode::IoSmallNode()
{
    setFlag(ItemIsMovable,false);
    identifier="IoSmallNode";
}

void IoSmallNode::setIoType(QString &type)
{
    myIoType = type;
}

QString IoSmallNode::ioType()
{
    return myIoType;
}

void IoSmallNode::setVar(QString &var)
{
    myVar=var;
}

QString IoSmallNode::var()
{
    return myVar;
}

void IoSmallNode::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    QString type = QInputDialog::getText(event->widget(),
                           tr("Edit Type"), tr("Enter new Type:"),
                           QLineEdit::Normal, myIoType);
    if (!type.isEmpty())
        myIoType=type;

    QString v=var();
    v = QInputDialog::getText(event->widget(),
                           tr("Edit Text"), tr("Enter new text:"),
                           QLineEdit::Normal, v);
    if (!v.isEmpty())
        myVar=v;

    setText(tr("%1 %2").arg(myIoType).arg(v));
}

QVariant IoSmallNode::itemChange(GraphicsItemChange change, const QVariant &value)
{
    if (change & ItemPositionHasChanged) {
        yuan->setPos(pos().x() + outlineRect().width()/2 + yuan->boundingRect().width()/2,
                     pos().y());
        foreach (Link *link, yuan->myLinks)
        {link->trackYuans();update();}
    }
    return QGraphicsItem::itemChange(change, value);
}
