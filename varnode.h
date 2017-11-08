#ifndef VARNODE_H
#define VARNODE_H

#include "node.h"
#include "newnode.h"
#include "varinstancenode.h"
#include "yuan.h"
#include <QtGui>
#include <vector>



/*******************************************************************
 * Class name: VarNode
 * Base class: Node
 * Description: This is the declaration of class VarNode. VarNode
 *       represents variables read and written by other actions.
******************************************************************/
//class VarNode:public Node
//{
//public:
//    enum {Type = VardefNodeType};
//    VarNode();
//    ~VarNode();

//    int type() const { return Type; }

//    QPolygonF outlineRect() const;
//    QRectF boundingRect() const;
//    QPainterPath shape() const;
//    void paint(QPainter *painter,
//               const QStyleOptionGraphicsItem *option, QWidget *widget);
//    void sethw(){wide = 40; high = 32;}
//    //为了方便，直接在头文件里定死长宽，若修改outlineRect()中的六边形长宽，
//    //则这里的数据也要改

//    QComboBox* box;
//    QGraphicsItem* item;

//    int num;
//    bool flags[6];
//    VardefNode* array[6];


//protected:
//    QVariant itemChange(GraphicsItemChange change,
//                        const QVariant &value);
//    /*void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);*/
//};


class VarNode:public QGraphicsObject
{
    Q_OBJECT

public:
    VarNode(QGraphicsItem *parent = 0);
    ~VarNode();

    int roundness(double szie)const;

    QRectF outlineRect()const;
    QRectF boundingRect()const;
    QPainterPath shape() const;
    void paint(QPainter *painter,
               const QStyleOptionGraphicsItem *option, QWidget *widget);


    QPushButton *button0;  //顶部增加变量按钮
    QGraphicsItem *button0Item;
    /*QComboBox *typeBox1;
    QLineEdit *nameEdit1;
    QLineEdit *valueEdit1;
    QPushButton *button1;
    QGraphicsItem *typeItem1, *nameItem1, *valueItem1;
    QGraphicsItem *button0Item, *button1Item;*/
    QComboBox *typeBox[4];
    QLineEdit *nameEdit[4];
    QLineEdit *valueEdit[4];
    QPushButton *button[4];
    QGraphicsItem *typeItem[4];
    QGraphicsItem *nameItem[4];
    QGraphicsItem *valueItem[4];
    QGraphicsItem *buttonItem[4];

    //vector用不熟练，先放弃
    /*vector<QComboBox*>typeVec;
    vector<QLineEdit*>nameVec;
    vector<QLineEdit*>valueVec;
    vector<QPushButton*>buttonVec;
    vector<QGraphicsItem*>typeItemVec;
    vector<QGraphicsItem*>nameItemVec;
    vector<QGraphicsItem*>valueItemVec;
    vector<QGraphicsItem*>buttonItemVec;*/

    QString type[4];
    QString vname[4];
    QString value[4];
    int lx,ly;
    int getvarnum(){return varnum;}
    void setvarnum(int a){varnum = a;}

    int controlsId;
    QString identifier;
    QString name;
signals:
    void dirty();
    void addVarSignal(VarNode *node,QString varType,
                      QString varName,QString varValue);
public slots:
    void addVarType();
    void emitSignal();

private:
    int varnum;//记录产生的变量的数目
};



#endif // VARNODE_H
