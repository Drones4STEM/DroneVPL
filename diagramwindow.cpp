/*******************************************************************
 * File:diagramwindow.cpp
 * Author: Ryan Feng
 * Description: This file includes the realization of class 
 *        DiagramWindow. DiagramWindow is the main window of 
 *        DroneVPL.
******************************************************************/

#include <QtGui>
#include <QDebug>
#include <QFile>
#include <QFileDialog>

#include "aqp/aqp.hpp"
#include "aqp/alt_key.hpp"
#include "diagramwindow.h"
#include "link.h"
#include "node.h"
#include "newnode.h"
#include "takeoffnode.h"
#include "varnode.h"
#include "vardefnode.h"
#include "computenode.h"
#include "ionode.h"
#include "yuan.h"
#include "rec.h"
#include "propertiesdialog.h"
#include "itemtypes.h"

const int StatusTimeout = AQP::MSecPerSecond * 30;
const QString MostRecentFile("MostRecentFile");
const qint32 MagicNumber = 0x5A93DE5;
const qint16 VersionNumber = 1;
const QString MimeType = "application/vnd.qtrac.pagedesigner";
const int OffsetIncrement = 5;

/*******************************************************************
 * Function name: DiagramWindow()
 * Description: This is a constructor of DiagramWindow class
 * Callee: creatActions(), createMenus(), createToolBars()
 * Inputs:
 * Outputs:
******************************************************************/
DiagramWindow::DiagramWindow()
{
    printer = new QPrinter(QPrinter::HighResolution);
   scene = new QGraphicsScene(0, 0, 1000, 1000);
   // scene = new QGraphicsScene;

     //   view = new QGraphicsView;
  view = new newview;
    view->setScene(scene);
     setMouseTracking(true);


    view->setDragMode(QGraphicsView::RubberBandDrag);
    view->setRenderHints(QPainter::Antialiasing
                         | QPainter::TextAntialiasing);
    view->setContextMenuPolicy(Qt::ActionsContextMenu);
    setCentralWidget(view);

    minZ = 0;
    maxZ = 0;
    seqNumber = 0;
    varNodeNum = 0;

    createActions();
    createMenus();
    createToolBars();

    connect(scene, SIGNAL(selectionChanged()),
            this, SLOT(updateActions()));

        setWindowTitle(tr("Diagram"));
        updateActions();
}

/*******************************************************************
 * Function name: sizeHint()
 * Description:
 * Callee:
 * Inputs:
 * Outputs:
******************************************************************/
QSize DiagramWindow::sizeHint() const
{
    //QSize size = printer->paperSize(QPrinter::Point).toSize() * 1.2;
    //size.rwidth() += brushWidget->sizeHint().width();
    //return size.boundedTo(
    //        QApplication::desktop()->availableGeometry().size());
}

/*******************************************************************
 * Function name: setDirty()
 * Description: This function notify that the window has unsaved
 *     changes and then updateActions.
 * Callee: setWindowModified()
 * Inputs: bool on
 * Outputs:
******************************************************************/
void DiagramWindow::setDirty(bool on)
{
    setWindowModified(on);
    updateActions();
}

/*******************************************************************
 * Function name: fileNew()
 * Description: This function creates a new file
 * Callee: okToClearData(), selectAllItems(), del(), setDirty()
 *         setWindowFilePath()
 * Inputs:
 * Outputs:
******************************************************************/
void DiagramWindow::fileNew()
{
    if (!okToClearData())
        return;
    selectAllItems();
    del();
    setWindowFilePath(tr("Unnamed"));
    setDirty(false);

}

/*******************************************************************
 * Function name: okToClearData()
 * Description: This function determins whether the data on the window
 *     can be cleared.
 * Callee: isWindowModified(), AQP::okToClearData()
 * Inputs:
 * Outputs: bool
******************************************************************/
bool DiagramWindow::okToClearData()
{
    if (isWindowModified())
        return AQP::okToClearData(&DiagramWindow::fileSave, this,
                tr("Unsaved changes"), tr("Save unsaved changes?"));
    return true;
}

/*******************************************************************
 * Function name: selectAllItems()
 * Description: This function select all Items on the scene.
 * Callee: QGraphicsScene::clearSelection()
 *         QGraphicsItem::setSelected()
 * Inputs:
 * Outputs:
******************************************************************/
void DiagramWindow::selectAllItems()
{
    scene->clearSelection();
    foreach (QGraphicsItem *item, scene->items())
        item->setSelected(true);
}

/*******************************************************************
 * Function name: fileOpen()
 * Description: This function opens an exsiting file
 * Callee: okToClearData(), QFileDialog::getOpenFileName()
 *         QString::isEmpty(), loadFile()
 * Inputs:
 * Outputs:
******************************************************************/
void DiagramWindow::fileOpen()
{
    if (!okToClearData())
        return;
    const QString &filename = QFileDialog::getOpenFileName(this,
            tr("%1 - Open").arg(QApplication::applicationName()),
            ".", tr("Page Designer (*.pd)"));
    if (filename.isEmpty())
        return;
    setWindowFilePath(filename);
    loadFile();
}

/*******************************************************************
 * Function name: loadFile()
 * Description: This function loads the data of an existing file
 * Callee:
 * Inputs:
 * Outputs:
******************************************************************/
void DiagramWindow::loadFile()
{
    QFile file(windowFilePath());
    QDataStream in;
    if (!openPageDesignerFile(&file, in))
        return;
    in.setVersion(QDataStream::Qt_4_5);
    selectAllItems();
    del();
    readItems(in);
    statusBar()->showMessage(tr("Loaded %1").arg(windowFilePath()),
                             StatusTimeout);
    setDirty(false);
}

/*******************************************************************
 * Function name: openPageDesignerFile()
 * Description: This function loads the data of an existing file
 * Callee:
 * Inputs:QFile *file, QDataStream &in
 * Outputs:bool - whether the file is loaded successfully or not
******************************************************************/
bool DiagramWindow::openPageDesignerFile(QFile *file, QDataStream &in)
{
    if (!file->open(QIODevice::ReadOnly)) {
        AQP::warning(this, tr("Error"), tr("Failed to open file: %1")
                                        .arg(file->errorString()));
        return false;
    }
    in.setDevice(file);
    qint32 magicNumber;
    in >> magicNumber;
    if (magicNumber != MagicNumber) {
        AQP::warning(this, tr("Error"),
                tr("%1 is not a %2 file").arg(file->fileName())
                .arg(QApplication::applicationName()));
        return false;
    }
    qint16 versionNumber;
    in >> versionNumber;
    if (versionNumber > VersionNumber) {
        AQP::warning(this, tr("Error"),
                tr("%1 needs a more recent version of %2")
                .arg(file->fileName())
                .arg(QApplication::applicationName()));
        return false;
    }
    return true;
}

/*******************************************************************
 * Function name: readItems()
 * Description: This function reads the data of an existing file
 * Callee:
 * Inputs: QDataStream &in, int offset, bool select
 * Outputs:
******************************************************************/
void DiagramWindow::readItems(QDataStream &in, int offset, bool select)
{
    QSet<QGraphicsItem*>items;
    qint32 itemType;
    QGraphicsItem *item=0;
    while(!in.atEnd())
    {
        in>>itemType;
        switch (itemType) {
        case TakeoffNodeType:
        {
            TakeoffNode* node=new TakeoffNode;
            in>>*node;

            node->setText(tr("take off\n %1 s").arg(node->time));
            //node->yuan->setPos(QPointF((node->pos().x()),
            //                   (node->pos().y() + node->outlineRect().height()/2)+node->yuan->boundingRect().height()/2));
            node->setPos(node->pos());
            scene->addItem(node);
            //scene->addItem(node->yuan);
            //update();
            item=node;
            break;
        }
        }
        if(item)
        {
            item->moveBy(offset,offset);
            if(select)
                items<<item;
            item=0;
        }
    }
    if(select)
    {
        selectItems(items);
    }

}

/*******************************************************************
 * Function name: selectItems()
 * Description: This function set some items selected.
 * Callee:
 * Inputs: QSet<QGraphicsItem *> &items - the items you want to select
 * Outputs:
******************************************************************/
void DiagramWindow::selectItems(const QSet<QGraphicsItem *> &items)
{
    scene->clearSelection();
    foreach (QGraphicsItem*item, items) {
       item->setSelected(true);
    }
}

/*******************************************************************
 * Function name: fileSave()
 * Description: This function saves an existing file.
 * Callee:
 * Inputs:
 * Outputs: bool - whether the file is saved successfully or not
******************************************************************/
bool DiagramWindow::fileSave()
{
    const QString filename = windowFilePath();
    if (filename.isEmpty() || filename == tr("Unnamed"))
        return fileSaveAs();
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly))
        return false;
    QDataStream out(&file);
    out << MagicNumber << VersionNumber;
    out.setVersion(QDataStream::Qt_4_5);
    writeItems(out, scene->items());
    file.close();
    setDirty(false);
    return true;
}

/*******************************************************************
 * Function name: writeItems()
 * Description: This function writes datas to a QDataStream.
 * Callee:
 * Inputs: QDataStream &out, const QList<QGraphicsItem *> &items
 * Outputs:
******************************************************************/
void DiagramWindow::writeItems(QDataStream &out, const QList<QGraphicsItem *> &items)
{
     foreach(QGraphicsItem*item,items)
     {
         qint32 type=static_cast<qint32>(item->type());
         out<<type;
         switch(type)
         {
         case TakeoffNodeType:
         {
             out<<*static_cast<TakeoffNode*>(item);
             break;
         }
         //default:
         //    Q_ASSERT(false);
         }
     }
}

/*******************************************************************
 * Function name: fileSaveAs()
 * Description: This function saves an unexisting file.
 * Callee:
 * Inputs:
 * Outputs: bool - whether the file is saved successfully or not
******************************************************************/
bool DiagramWindow::fileSaveAs()
{
    QString filename = QFileDialog::getSaveFileName(this,
            tr("%1 - Save As").arg(QApplication::applicationName()),
            ".", tr("Page Designer (*.pd)"));
    if (filename.isEmpty())
        return false;
    if (!filename.toLower().endsWith(".pd"))
        filename += ".pd";
    setWindowFilePath(filename);
    return fileSave();
}

/*******************************************************************
 * Function name: fileExport()
 * Description: This function exports a graph of the window.
 * Callee:
 * Inputs:
 * Outputs:
******************************************************************/
void DiagramWindow::fileExport()
{

}

/*******************************************************************
 * Function name: filePrint()
 * Description: This function prints the window.
 * Callee:
 * Inputs:
 * Outputs:
******************************************************************/
void DiagramWindow::filePrint()
{

}

/*******************************************************************
 * Function name: addTakeoffNode()
 * Description: This function add a TakeoffNode on the scene.
 * Callee:
 * Inputs:
 * Outputs:
******************************************************************/
void DiagramWindow::addTakeoffNode()
{
    TakeoffNode *node=new TakeoffNode;
    node->setText(tr("take off\n %1 s").arg(node->time));
    setupNode(node);
    node->yuan->setPos(QPointF((node->pos().x()),
                       (node->pos().y() + node->outlineRect().height()/2)+node->yuan->boundingRect().height()/2));
    scene->addItem(node->yuan);
    takeoffNodeNum++;
    node->controlsId=takeoffNodeNum;
    setDirty(true);
}

/*******************************************************************
 * Function name: addLandonNode()
 * Description: This function add a LandonNode on the scene.
 * Callee:
 * Inputs:
 * Outputs:
******************************************************************/
void DiagramWindow::addLandonNode()
{
    LandonNode *node=new LandonNode;
    node->setText(tr("Land on\n %1 s").arg(node->time));
    setupNewNode(node);
    node->yuan2->setPos(QPointF((node->pos().x()),
                       (node->pos().y() - node->outlineRect().height()/2)-node->yuan2->boundingRect().height()/2));
    scene->addItem(node->yuan2);

    landonNodeNum++;
    node->controlsId=landonNodeNum;

    setDirty(true);
}

/*******************************************************************
 * Function name: addTranslationNode()
 * Description: This function add a TranslationNode on the scene.
 * Callee:
 * Inputs:
 * Outputs:
******************************************************************/
void DiagramWindow::addTranslationNode()
{
    TranslationNode *node=new TranslationNode;
    addTranslation(node);

    setDirty(true);
}

/*******************************************************************
 * Function name: addTranslation()
 * Description: This function add a TranslationNode on the scene.
 * Callee:
 * Inputs: TranslationNode *node
 * Outputs:
******************************************************************/
void DiagramWindow::addTranslation(TranslationNode *node)
{
    node->setText(tr(" %1 m/s \n %2 s").arg(node->speed).arg(node->time));
    QGraphicsItem* item=scene->addWidget(node->box);
    node->item=item;
    setupNewNode(node);
    node->yuan->setPos(QPointF(node->pos().x(),
                      (node->pos().y() + node->outlineRect().height()/2 + node->yuan->boundingRect().height()/2)));
    node->yuan2->setPos(QPointF(node->pos().x() - node->outlineRect().width()/2 - node->yuan2->outlineRect().width()/2,
                      (node->pos().y() )));
    scene->addItem(node->yuan);
    scene->addItem(node->yuan2);

    item->setPos(QPointF(node->pos().x()-40,
                 (node->pos().y() - node->outlineRect().height()/2 - node->item->boundingRect().height())));
    item->setZValue(node->zValue()+1);
    node->box->addItem(tr("rise"));
    node->box->addItem(tr("fall"));
    node->box->addItem(tr("advance"));
    node->box->addItem(tr("back"));
    node->box->addItem(tr("right"));
    node->box->addItem(tr("left"));
}

/*******************************************************************
 * Function name: addRiseNode()
 * Description: This function add a RiseNode on the scene.
 * Callee:
 * Inputs:
 * Outputs:
******************************************************************/
void DiagramWindow::addRiseNode()
{
    TranslationNode *node=new TranslationNode;
    addTranslation(node);
    node->box->setCurrentIndex(0);

    setDirty(true);
}

/*******************************************************************
 * Function name: addFallNode()
 * Description: This function add a FallNode on the scene.
 * Callee:
 * Inputs:
 * Outputs:
******************************************************************/
void DiagramWindow::addFallNode()
{
    TranslationNode *node=new TranslationNode;
    addTranslation(node);
    node->box->setCurrentIndex(1);

    setDirty();
}

/*******************************************************************
 * Function name: addAdvanceNode()
 * Description: This function add an AdvanceNode on the scene.
 * Callee:
 * Inputs:
 * Outputs:
******************************************************************/
void DiagramWindow::addAdvanceNode()
{
    TranslationNode *node=new TranslationNode;
    addTranslation(node);
    node->box->setCurrentIndex(2);

    setDirty();
}

/*******************************************************************
 * Function name: addBackNode()
 * Description: This function add a BackNode on the scene.
 * Callee:
 * Inputs:
 * Outputs:
******************************************************************/
void DiagramWindow::addBackNode()
{
    TranslationNode *node=new TranslationNode;
    addTranslation(node);
    node->box->setCurrentIndex(3);

    setDirty();
}

/*******************************************************************
 * Function name: addRightNode()
 * Description: This function add a RightNode on the scene.
 * Callee:
 * Inputs:
 * Outputs:
******************************************************************/
void DiagramWindow::addRightNode()
{
    TranslationNode *node=new TranslationNode;
    addTranslation(node);
    node->box->setCurrentIndex(4);

    setDirty();
}

/*******************************************************************
 * Function name: addLeftNode()
 * Description: This function add a LeftNode on the scene.
 * Callee:
 * Inputs:
 * Outputs:
******************************************************************/
void DiagramWindow::addLeftNode()
{
    TranslationNode *node=new TranslationNode;
    addTranslation(node);
    node->box->setCurrentIndex(5);

    setDirty();
}

/*******************************************************************
 * Function name: addSomeNode()
 * Description: This function add a SomeNode on the scene.
 * Callee:
 * Inputs:
 * Outputs:
******************************************************************/
void DiagramWindow::addSomeNode()
{
    SomeNode *node=new SomeNode;
    addSome(node);
    node->box->setCurrentIndex(0);

    setDirty();
}

/*******************************************************************
 * Function name: addSome()
 * Description: This function add a SomeNode on the scene.
 * Callee:
 * Inputs: SomeNode *node
 * Outputs:
******************************************************************/
void DiagramWindow::addSome(SomeNode *node)
{
    node->setText(tr(" %1  \n %2 s").arg(node->angel).arg(node->time));
    QGraphicsItem* item=scene->addWidget(node->box);
    node->item=item;
    setupNewNode(node);

    node->yuan->setPos(QPointF(node->pos().x(),
                      (node->pos().y() + node->outlineRect().height()/2 + node->yuan->boundingRect().height()/2)));
    node->yuan2->setPos(QPointF(node->pos().x() - node->outlineRect().width()/2 - node->yuan2->outlineRect().width()/2,
                      (node->pos().y() )));
    scene->addItem(node->yuan);
    scene->addItem(node->yuan2);

    item->setPos(QPointF(node->pos().x()-40,
                 (node->pos().y() - node->outlineRect().height()/2 - node->item->boundingRect().height())));
    item->setZValue(node->zValue()+1);
    node->box->addItem(tr("turn left"));
    node->box->addItem(tr("turn right"));
    node->box->addItem(tr("hanging"));
    node->box->addItem(tr("delay"));
}

/*******************************************************************
 * Function name: addTurnLeftNode()
 * Description: This function add a TurnLeftNode on the scene.
 * Callee:
 * Inputs:
 * Outputs:
******************************************************************/
void DiagramWindow::addTurnLeftNode()
{
    SomeNode *node=new SomeNode;
    addSome(node);
    node->box->setCurrentIndex(0);

    setDirty();
}

/*******************************************************************
 * Function name: addTurnRightNode()
 * Description: This function add a TurnRightNode on the scene.
 * Callee:
 * Inputs:
 * Outputs:
******************************************************************/
void DiagramWindow::addTurnRightNode()
{
    SomeNode *node=new SomeNode;
    addSome(node);
    node->box->setCurrentIndex(1);

    setDirty();
}

/*******************************************************************
 * Function name: addHangingNode()
 * Description: This function add a HangingNode on the scene.
 * Callee:
 * Inputs:
 * Outputs:
******************************************************************/
void DiagramWindow::addHangingNode()
{
    SomeNode *node=new SomeNode;
    addSome(node);
    node->box->setCurrentIndex(2);

    setDirty();
}

/*******************************************************************
 * Function name: addDelayNode()
 * Description: This function add a DelayNode on the scene.
 * Callee:
 * Inputs:
 * Outputs:
******************************************************************/
void DiagramWindow::addDelayNode()
{
    SomeNode *node=new SomeNode;
    addSome(node);
    node->box->setCurrentIndex(3);

    setDirty();
}

/*******************************************************************
 * Function name: addVarNode()
 * Description: This function add a VarNode on the scene.
 * Callee:
 * Inputs:
 * Outputs:
******************************************************************/
void DiagramWindow::addVarNode()
{
    VarNode* node=new VarNode;
    node->setText(tr("int"));
    setupNode(node);

    varNodeNum++;
    node->controlsId=varNodeNum;

    setDirty();
}

/*******************************************************************
 * Function name: addVardefNod()
 * Description: This function add a VardefNod on the scene.
 * Callee:
 * Inputs:
 * Outputs:
******************************************************************/
void DiagramWindow::addVardefNode()
{
    QList<QGraphicsItem *> items = scene->selectedItems();
    if(items.count()==0)
    {
        VardefNode* node=new VardefNode;
        node->node=0;
        node->setPos(QPoint(80 + (100 * (seqNumber % 5)),
                            80 + (50 * ((seqNumber / 5) % 7))));
        scene->addItem(node);
        ++seqNumber;
        node->yuan2->setPos(node->pos().x(),
                           node->pos().y() - 16 - node->yuan2->boundingRect().height()/2);
        node->yuan->setPos(node->pos().x(),
                           node->pos().y() + 16 + node->yuan->boundingRect().height()/2);
        scene->addItem(node->yuan);
        scene->addItem(node->yuan2);

        vardefNodeNum++;
        node->controlsId=vardefNodeNum;
    }
    else if(items.count()==1)
    {
        VarNode* node=dynamic_cast<VarNode*>(scene->selectedItems().first());
        if(!node)return;

        int flag=0;
        while(node->flags[node->num])//这个位置已经有了vardefnode
        {
            if(flag==6)return;
            node->num=node->num%6+1;
            flag++;
        }

        //计算添加的位置
        int i=node->num%3;
        int j;
        if(node->num==0||node->num==2)j=-17;
        else if(node->num==3||node->num==5)j=17;
        else if(node->num==1)j=-35;
        else j=35;

        node->array[node->num]->node=node;//使vardefnode知道它属于varnode

        node->array[node->num]->setPos(node->pos().x() + (1-i)*30,
                             node->pos().y() + j);
        node->flags[node->num]=true;
        scene->addItem(node->array[node->num]);
        node->num=node->num%6+1;

        vardefNodeNum++;
        node->controlsId=vardefNodeNum;
        }
    setDirty();
}

/*******************************************************************
 * Function name: addComputeNode()
 * Description: This function add a ComputeNode on the scene.
 * Callee:
 * Inputs:
 * Outputs:
******************************************************************/
void DiagramWindow::addComputeNode()
{
    ComputeNode *node=new ComputeNode;
    node->setText(tr("Compute"));
    QGraphicsItem* item=scene->addWidget(node->box);
    node->item=item;
    setupNode(node);
    node->yuan->setPos(QPointF(node->pos().x(),
                               node->pos().y() + node->outlineRect().height()/2 +node->yuan->boundingRect().height()/2));
    node->yuan2->setPos(QPointF(node->pos().x() - node->outlineRect().width()/2 - node->yuan2->outlineRect().width()/2,
                                node->pos().y()));
    node->yuan3->setPos(QPointF(node->pos().x() + node->outlineRect().width()/2 + node->yuan3->outlineRect().width()/2,
                                node->pos().y()));
    scene->addItem(node->yuan);
    scene->addItem(node->yuan2);
    scene->addItem(node->yuan3);

    item->setPos(QPointF(node->pos().x()- item->boundingRect().width()/2,
                 node->pos().y() - node->outlineRect().height()/2 - item->boundingRect().height()));
    item->setZValue(node->zValue()+1);
    node->box->addItem(tr("+"));
    node->box->addItem(tr("-"));
    node->box->addItem(tr("*"));
    node->box->addItem(tr("/"));
    node->box->addItem(tr("cos"));
    node->box->addItem(tr("sin"));
    node->box->addItem(tr("tan"));
    node->box->addItem(tr("log"));
    node->box->addItem(tr("e"));
    node->box->addItem(tr("="));
    node->box->addItem(tr(">"));
    node->box->addItem(tr("<"));

    computeNodeNum++;
    node->controlsId=computeNodeNum;

    setDirty();
}

/*******************************************************************
 * Function name: addIoNode()
 * Description: This function add an IoNode on the scene.
 * Callee:
 * Inputs:
 * Outputs:
******************************************************************/
void DiagramWindow::addIoNode()
{
    IoNode* node=new IoNode;
    node->setText(tr("sensor"));
    QGraphicsItem* item=scene->addWidget(node->box);
    node->item=item;
    setupNewNode(node);

    node->yuan->setPos(QPointF(node->pos().x(),
                      (node->pos().y() + node->outlineRect().height()/2 + node->yuan->boundingRect().height()/2)));
    node->yuan2->setPos(QPointF(node->pos().x()- node->outlineRect().width()/2 - node->yuan2->outlineRect().width()/2,
                       (node->pos().y())));
    scene->addItem(node->yuan);
    scene->addItem(node->yuan2);

    node->node2->setPos(node->pos().x() + node->outlineRect().width()/2 + node->node2->outlineRect().width()/2,
                        node->pos().y());
    node->node1->setPos(node->node2->pos().x(),
                        node->node2->pos().y() - node->node2->outlineRect().height());
    node->node3->setPos(node->node2->pos().x(),
                        node->node2->pos().y() + node->node2->outlineRect().height());
    scene->addItem(node->node2);
    scene->addItem(node->node1);
    scene->addItem(node->node3);
    scene->addItem(node->node2->yuan);
    scene->addItem(node->node1->yuan);
    scene->addItem(node->node3->yuan);


    item->setPos(QPointF(node->pos().x()-node->outlineRect().width()/2,
                 (node->pos().y() - node->outlineRect().height()/2 - item->boundingRect().height())));
    item->setZValue(node->zValue()+1);
    node->box->addItem(tr("detection sensor"));
    node->box->addItem(tr("A sensor"));
    node->box->addItem(tr("B sensor"));
    node->box->addItem(tr("delay"));
    node->box->setCurrentIndex(0);

    ioNodeNum++;
    node->controlsId=ioNodeNum;

    setDirty();
}

/*******************************************************************
 * Function name: addLink()
 * Description: This function add a Link on the scene.
 * Callee:
 * Inputs:
 * Outputs:
******************************************************************/
void DiagramWindow::addLink()
{
    YuanPair yuans = selectedYuanPair();
    if (yuans == YuanPair())
        return;
    if(yuans.first->inout()==yuans.second->inout())
    {
        QMessageBox::warning(this,"","You should link one input yuan and "
                                     "one output yuan",
                             QMessageBox::Yes);
        scene->clearSelection();
        return;
    }
    Link *link = new Link(yuans.first, yuans.second);
    link->setZValue(100);
    scene->addItem(link);

    setDirty();
}

/*******************************************************************
 * Function name: addRec()
 * Description: This function add a Rec on the scene.
 * Callee:
 * Inputs:
 * Outputs:
******************************************************************/
void DiagramWindow::addRec()
{
    Rec *rec=new Rec;
    QGraphicsItem* item= scene->addWidget(rec->box);
    rec->item=item;
    rec->setPos(100,100);
    scene->addItem(rec);
    scene->clearSelection();
    rec->setSelected(true);

    rec->yuan2->setPos(QPointF(rec->pos().x() - rec->outlineRect().height()/2 + item->boundingRect().width()/2,
                               rec->pos().y() - rec->outlineRect().height()/2 +item->boundingRect().height()*1.5));
    scene->addItem(rec->yuan2);

    item->setPos(QPointF(rec->pos().x()-rec->outlineRect().width()/2,
                         (rec->pos().y() - rec->outlineRect().height()/2)));
    item->setZValue(rec->zValue()+1);
    rec->box->addItem(tr("if"));
    rec->box->addItem(tr("else"));
    rec->box->addItem(tr("while"));

    recNodeNum++;
    rec->controlsId=recNodeNum;

    setDirty();
}

/*******************************************************************
 * Function name: del()
 * Description: This function delete the selected items.
 * Callee:
 * Inputs:
 * Outputs:
******************************************************************/
void DiagramWindow::del()
{
    QList<QGraphicsItem *> items = scene->selectedItems();
    int itemsCount = items.count();
    int i=0;
    QList<Link*>itemLinks;
    for(i=0;i<itemsCount;i++)
    {
        if(dynamic_cast<Link*>(items[i]))
            itemLinks<<dynamic_cast<Link*>(items[i]);
    }
    QList<TakeoffNode*>itemTakeoffs;
    for(i=0;i<itemsCount;i++)
    {
        if(dynamic_cast<TakeoffNode*>(items[i]))
            itemTakeoffs<<dynamic_cast<TakeoffNode*>(items[i]);
    }
    QList<LandonNode*>itemLandons;
    for(i=0;i<itemsCount;i++)
    {
        if(dynamic_cast<LandonNode*>(items[i]))
            itemLandons<<dynamic_cast<LandonNode*>(items[i]);
    }
    QList<ComputeNode*>itemComputes;
    for(i=0;i<itemsCount;i++)
    {
        if(dynamic_cast<ComputeNode*>(items[i]))
            itemComputes<<dynamic_cast<ComputeNode*>(items[i]);
    }
    QList<IoNode*>itemIos;
    for(i=0;i<itemsCount;i++)
    {
        if(dynamic_cast<IoNode*>(items[i]))
            itemIos<<dynamic_cast<IoNode*>(items[i]);
    }
    QList<Rec*>itemRecs;
    for(i=0;i<itemsCount;i++)
    {
        if(dynamic_cast<Rec*>(items[i]))
            itemRecs<<dynamic_cast<Rec*>(items[i]);
    }
    QList<TranslationNode*>itemTranslations;
    for(i=0;i<itemsCount;i++)
    {
        if(dynamic_cast<TranslationNode*>(items[i]))
            itemTranslations<<dynamic_cast<TranslationNode*>(items[i]);
    }
    QList<SomeNode*>itemSomes;
    for(i=0;i<itemsCount;i++)
    {
        if(dynamic_cast<SomeNode*>(items[i]))
            itemSomes<<dynamic_cast<SomeNode*>(items[i]);
    }
    QList<VardefNode*>itemVardefs;
    for(i=0;i<itemsCount;i++)
    {
        if(dynamic_cast<VardefNode*>(items[i]))
            itemVardefs<<dynamic_cast<VardefNode*>(items[i]);
    }
    QList<VarNode*>itemVars;
    for(i=0;i<itemsCount;i++)
    {
        if(dynamic_cast<VarNode*>(items[i]))
            itemVars<<dynamic_cast<VarNode*>(items[i]);
    }
    foreach (Link* item, itemLinks) {
        delete item;
    }
    foreach (TakeoffNode* item, itemTakeoffs) {
        delete item;
    }
    foreach (LandonNode* item, itemLandons) {
        delete item;
    }
    foreach (ComputeNode* item, itemComputes) {
        delete item;
    }
    foreach (IoNode* item, itemIos) {
        delete item;
    }
    foreach (Rec* item, itemRecs) {
        delete item;
    }
    foreach (TranslationNode* item, itemTranslations) {
        delete item;
    }
    foreach (SomeNode* item, itemSomes) {
        delete item;
    }
    foreach (VardefNode* item, itemVardefs) {
        delete item;
    }
    foreach (VarNode* item, itemVars) {
        delete item;
    }
}


/*******************************************************************
 * Function name: copy()
 * Description: This function copy the selected items.
 * Callee:
 * Inputs:
 * Outputs:
******************************************************************/
void DiagramWindow::copy()
{
    QList<QGraphicsItem*>items = scene->selectedItems();
    if(items.isEmpty())
        return;
    pasteOffset=OffsetIncrement;
    copyItems(items);
    updateActions();
}

/*******************************************************************
 * Function name: copyItems()
 * Description: This function delete the selected items.
 * Callee:
 * Inputs:  QList<QGraphicsItem *> &items
 * Outputs:
******************************************************************/
void DiagramWindow::copyItems(const QList<QGraphicsItem *> &items)
{
    QByteArray copiedItems;
    QDataStream out(&copiedItems,QIODevice::WriteOnly);
    writeItems(out,items);
    QMimeData *mimeData = new QMimeData;
    mimeData->setData(MimeType,copiedItems);
    QClipboard*clipboard = QApplication::clipboard();
    clipboard->setMimeData(mimeData);
}

/*******************************************************************
 * Function name: paste()
 * Description:
 * Callee:
 * Inputs:
 * Outputs:
******************************************************************/
void DiagramWindow::paste()
{
    QClipboard*clipboard = QApplication::clipboard();
    const QMimeData*mimeData = clipboard->mimeData();
    if(!mimeData)
        return;
    if(mimeData->hasFormat(MimeType))
    {
        QByteArray copiedItems = mimeData->data(MimeType);
        QDataStream in(&copiedItems,QIODevice::ReadOnly);
        readItems(in,pasteOffset,true);
        pasteOffset+=OffsetIncrement;
    }
    else return;
    setDirty(true);
}

/*******************************************************************
 * Function name: cut()
 * Description:
 * Callee:
 * Inputs:
 * Outputs:
******************************************************************/
void DiagramWindow::cut()
{
    QList<QGraphicsItem*>items = scene->selectedItems();
    if(items.isEmpty())
        return;
    copyItems(items);
    QListIterator<QGraphicsItem*>i(items);
    while(i.hasNext())
    {
        QScopedPointer<QGraphicsItem>item(i.next());
        scene->removeItem(item.data());
    }
    setDirty(true);
}

/*******************************************************************
 * Function name: bringToFront()
 * Description: This function changes the Z-coordinate of item.
 * Callee:
 * Inputs:
 * Outputs:
******************************************************************/
void DiagramWindow::bringToFront()
{
    ++maxZ;
    setZValue(maxZ);
}

/*******************************************************************
 * Function name: bringToBack()
 * Description: This function changes the Z-coordinate of item.
 * Callee:
 * Inputs:
 * Outputs:
******************************************************************/
void DiagramWindow::sendToBack()
{
    --minZ;
    setZValue(minZ);
}

/*******************************************************************
 * Function name: properties()
 * Description: This function show the property window.
 * Callee:
 * Inputs:
 * Outputs:
******************************************************************/
void DiagramWindow::properties()
{
    Node *node = selectedNode();
    NewNode *newnode = selectedNewNode();
    Yuan *yuan = selectedYuan();
    Link *link = selectedLink();

    if (node) {
        PropertiesDialog dialog(node, this);
        dialog.exec();
    }else if (newnode) {
        PropertiesDialog dialog(newnode, this);
        dialog.exec();
    }else if (link) {
        QColor color = QColorDialog::getColor(link->color(), this);
        if (color.isValid())
            link->setColor(color);
    } else if (yuan) {
        QColor color = QColorDialog::getColor(yuan->outlineColor(), this);
        if (color.isValid())
            yuan->setOutlineColor(color);
    }
}

/*******************************************************************
 * Function name: updateActions()
 * Description: This function changes the state of actions according
 *     to the selected items.
 * Callee:
 * Inputs:
 * Outputs:
******************************************************************/
void DiagramWindow::updateActions()
{
    bool isNode = (selectedNode() != 0||selectedNewNode()!=0);
    bool isYuanPair = (selectedYuanPair() != YuanPair());
    bool isRec = (selectedRec() != 0);
    bool hasSelection = !scene->selectedItems().isEmpty();

    cutAction->setEnabled(isNode);
    copyAction->setEnabled(isNode);
    addLinkAction->setEnabled(isYuanPair);
    deleteAction->setEnabled(hasSelection);
    bringToFrontAction->setEnabled(isNode||isRec);
    sendToBackAction->setEnabled(isNode||isRec);
    propertiesAction->setEnabled(isNode);

    foreach (QAction *action, view->actions())
        view->removeAction(action);

    foreach (QAction *action, editMenu->actions()) {
        if (action->isEnabled())
            view->addAction(action);
    }
}

/*******************************************************************
 * Function name: createActions()
 * Description: This function creates all the actions shown on the
 *     scene.
 * Callee:
 * Inputs:
 * Outputs:
******************************************************************/
void DiagramWindow::createActions()
{
    fileNewAction = new QAction(tr("New"),this);
    connect(fileNewAction, SIGNAL(triggered()), this, SLOT(fileNew()));
    fileNewAction->setIcon(QIcon(":/images/filenew.png"));

    fileOpenAction = new QAction(tr("Open"),this);
    connect(fileOpenAction, SIGNAL(triggered()), this, SLOT(fileOpen()));
    fileOpenAction->setIcon(QIcon(":/images/fileopen.png"));

    fileSaveAction = new QAction(tr("Save"),this);
    connect(fileSaveAction, SIGNAL(triggered()), this, SLOT(fileSave()));
    fileSaveAction->setIcon(QIcon(":/images/filesave.png"));

    fileSaveAsAction = new QAction(tr("Save As"),this);
    connect(fileSaveAsAction, SIGNAL(triggered()), this, SLOT(fileSaveAs()));

    fileExportAction = new QAction(tr("Export"),this);
    connect(fileExportAction, SIGNAL(triggered()), this, SLOT(fileExport()));
    fileExportAction->setIcon(QIcon(":/images/fileexport.png"));

    filePrintAction = new QAction(tr("Print"),this);
    connect(filePrintAction, SIGNAL(triggered()), this, SLOT(filePrint()));
    filePrintAction->setIcon(QIcon(":/images/fileprint.png"));

    exitAction = new QAction(tr("E&xit"), this);
    exitAction->setShortcut(tr("Ctrl+Q"));
    connect(exitAction, SIGNAL(triggered()), this, SLOT(close()));

    addActionNodeAction = new QAction(tr("action"),this);

    addTakeoffNodeAction = new QAction(tr("takeoff"), this);
    connect(addTakeoffNodeAction, SIGNAL(triggered()), this, SLOT(addTakeoffNode()));

    addLandonNodeAction = new QAction(tr("landon"),this);
    connect(addLandonNodeAction, SIGNAL(triggered()), this, SLOT(addLandonNode()));

    addTranslationNodeAction = new QAction(tr("Translation"),this);

    addRiseNodeAction = new QAction(tr("rise"),this);
    connect(addRiseNodeAction, SIGNAL(triggered()), this, SLOT(addRiseNode()));
    addFallNodeAction = new QAction(tr("fall"),this);
    connect(addFallNodeAction, SIGNAL(triggered()), this, SLOT(addFallNode()));
    addAdvanceNodeAction = new QAction(tr("advance"),this);
    connect(addAdvanceNodeAction, SIGNAL(triggered()), this, SLOT(addAdvanceNode()));
    addBackNodeAction = new QAction(tr("backward"),this);
    connect(addBackNodeAction, SIGNAL(triggered()), this, SLOT(addBackNode()));
    addRightNodeAction = new QAction(tr("right"),this);
    connect(addRightNodeAction, SIGNAL(triggered()), this, SLOT(addRightNode()));
    addLeftNodeAction = new QAction(tr("left"),this);
    connect(addLeftNodeAction, SIGNAL(triggered()), this, SLOT(addLeftNode()));

    addSomeNodeAction = new QAction(tr("Add Some..."),this);
    connect(addSomeNodeAction,SIGNAL(triggered()),this,SLOT(addSomeNode()));

    addTurnLeftNodeAction = new QAction(tr("turn left"),this);
    connect(addTurnLeftNodeAction, SIGNAL(triggered()), this, SLOT(addTurnLeftNode()));
    addTurnRightNodeAction = new QAction(tr("turn right"),this);
    connect(addTurnRightNodeAction, SIGNAL(triggered()), this, SLOT(addTurnRightNode()));
    addHangingNodeAction = new QAction(tr("hanging"),this);
    connect(addHangingNodeAction, SIGNAL(triggered()), this, SLOT(addHangingNode()));
    addDelayNodeAction = new QAction(tr("delay"),this);
    connect(addDelayNodeAction, SIGNAL(triggered()), this, SLOT(addDelayNode()));

    addVarNodeAction = new QAction(tr("Variable"),this);
    connect(addVarNodeAction,SIGNAL(triggered()),this,SLOT(addVarNode()));

    addVardefNodeAction = new QAction(tr("Vardefine"),this);
    connect(addVardefNodeAction,SIGNAL(triggered()),this,SLOT(addVardefNode()));

    addComputeNodeAction = new QAction(tr("Compute"),this);
    connect(addComputeNodeAction,SIGNAL(triggered()),this,SLOT(addComputeNode()));

    addIoNodeAction = new QAction(tr("IO"),this);
    connect(addIoNodeAction,SIGNAL(triggered()),this,SLOT(addIoNode()));

    addLinkAction = new QAction(tr("&Link"), this);
    addLinkAction->setIcon(QIcon(":/images/link.png"));
    addLinkAction->setShortcut(tr("Ctrl+L"));
    connect(addLinkAction, SIGNAL(triggered()), this, SLOT(addLink()));

    addRecAction = new QAction(tr("Logic Rec"), this);
    connect(addRecAction, SIGNAL(triggered()), this, SLOT(addRec()));

    deleteAction = new QAction(tr("&Delete"), this);
    deleteAction->setIcon(QIcon(":/images/delete.png"));
    deleteAction->setShortcut(tr("Del"));
    connect(deleteAction, SIGNAL(triggered()), this, SLOT(del()));

    cutAction = new QAction(tr("Cu&t"), this);
    cutAction->setIcon(QIcon(":/images/cut.png"));
    cutAction->setShortcut(tr("Ctrl+X"));
    connect(cutAction, SIGNAL(triggered()), this, SLOT(cut()));

    copyAction = new QAction(tr("&Copy"), this);
    copyAction->setIcon(QIcon(":/images/copy.png"));
    copyAction->setShortcut(tr("Ctrl+C"));
    connect(copyAction, SIGNAL(triggered()), this, SLOT(copy()));

    pasteAction = new QAction(tr("&Paste"), this);
    pasteAction->setIcon(QIcon(":/images/paste.png"));
    pasteAction->setShortcut(tr("Ctrl+V"));
    connect(pasteAction, SIGNAL(triggered()), this, SLOT(paste()));

    bringToFrontAction = new QAction(tr("Bring to &Front"), this);
    bringToFrontAction->setIcon(QIcon(":/images/bringtofront.png"));
    connect(bringToFrontAction, SIGNAL(triggered()),
            this, SLOT(bringToFront()));

    sendToBackAction = new QAction(tr("&Send to Back"), this);
    sendToBackAction->setIcon(QIcon(":/images/sendtoback.png"));
    connect(sendToBackAction, SIGNAL(triggered()),
            this, SLOT(sendToBack()));

    propertiesAction = new QAction(tr("P&roperties..."), this);
    connect(propertiesAction, SIGNAL(triggered()),
            this, SLOT(properties()));
}

/*******************************************************************
 * Function name: createMenus()
 * Description: This function creates menus.
 * Callee:
 * Inputs:
 * Outputs:
******************************************************************/
void DiagramWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(fileNewAction);
    fileMenu->addAction(fileOpenAction);
    fileMenu->addAction(fileSaveAction);
    fileMenu->addAction(fileSaveAsAction);
    fileMenu->addSeparator();
    fileMenu->addAction(fileExportAction);
    fileMenu->addAction(filePrintAction);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAction);
// ////////////////////////////////////////////////////////////////////////////////////////////////////
    editMenu = menuBar()->addMenu(tr("&Edit"));
    editMenu->addAction(addLinkAction);

    QMenu *translationMenu = new QMenu(tr("translation"),this);
    foreach(QAction *action,QList<QAction*>()
            <<addRiseNodeAction<<addFallNodeAction
            <<addAdvanceNodeAction<<addBackNodeAction
            <<addRightNodeAction<<addLeftNodeAction)
        translationMenu->addAction(action);
    addTranslationNodeAction->setMenu(translationMenu);

    QMenu *actionMenu = new QMenu(tr("action"),this);
    foreach (QAction *action,QList<QAction*>()
             <<addTakeoffNodeAction<<addLandonNodeAction
             <<addTranslationNodeAction
             <<addTurnLeftNodeAction<<addTurnRightNodeAction
             <<addHangingNodeAction<<addDelayNodeAction)
        actionMenu->addAction(action);
    addActionNodeAction->setMenu(actionMenu);

    editMenu->addAction(addActionNodeAction);
    editMenu->addAction(addVarNodeAction);
    editMenu->addAction(addVardefNodeAction);
    editMenu->addAction(addComputeNodeAction);
    editMenu->addAction(addIoNodeAction);
    editMenu->addAction(addRecAction);
    editMenu->addSeparator();

    editMenu->addAction(deleteAction);
    editMenu->addAction(cutAction);
    editMenu->addAction(copyAction);
    editMenu->addAction(pasteAction);
    editMenu->addSeparator();

    editMenu->addAction(bringToFrontAction);
    editMenu->addAction(sendToBackAction);
    editMenu->addSeparator();
    editMenu->addAction(propertiesAction);
}

/*******************************************************************
 * Function name: createToolBars()
 * Description: This function creates toolbars.
 * Callee:
 * Inputs:
 * Outputs:
******************************************************************/
void DiagramWindow::createToolBars()
{
    editToolBar = addToolBar(tr("Edit"));
    editToolBar->addAction(fileNewAction);
    editToolBar->addAction(fileOpenAction);
    editToolBar->addAction(fileSaveAction);
    editToolBar->addSeparator();
    editToolBar->addAction(deleteAction);
    editToolBar->addAction(cutAction);
    editToolBar->addAction(copyAction);
    editToolBar->addAction(pasteAction);
    editToolBar->addSeparator();
    editToolBar->addAction(bringToFrontAction);
    editToolBar->addAction(sendToBackAction);

    QToolBar* aToolBar = new QToolBar(tr("action"));
    aToolBar->addAction(addTakeoffNodeAction);
    aToolBar->addAction(addLandonNodeAction);
    aToolBar->addAction(addRiseNodeAction);
    aToolBar->addAction(addFallNodeAction);
    aToolBar->addAction(addAdvanceNodeAction);
    aToolBar->addAction(addBackNodeAction);
    aToolBar->addAction(addRightNodeAction);
    aToolBar->addAction(addLeftNodeAction);
    aToolBar->addAction(addTurnLeftNodeAction);
    aToolBar->addAction(addTurnRightNodeAction);
    aToolBar->addAction(addHangingNodeAction);
    aToolBar->addAction(addDelayNodeAction);
    aToolBar->addSeparator();
    aToolBar->addAction(addVarNodeAction);
    aToolBar->addAction(addVardefNodeAction);
    aToolBar->addSeparator();
    aToolBar->addAction(addComputeNodeAction);
    aToolBar->addAction(addIoNodeAction);
    aToolBar->addAction(addRecAction);
    aToolBar->addAction(addLinkAction);
    addToolBar(Qt::LeftToolBarArea,aToolBar);
}

/*******************************************************************
 * Function name: setZValue()
 * Description: This function changes the Z-coordinate of item.
 * Callee:
 * Inputs: int z - the new value of Z-coordinate.
 * Outputs:
******************************************************************/
void DiagramWindow::setZValue(int z)
{
    Node *node = selectedNode();
    if (node)
    {
        node->yuan->setZValue(z);
        foreach (Link*link, node->yuan->myLinks) {
            link->setZValue(z);
        }
        node->setZValue(z);
    }
    NewNode *newnode = selectedNewNode();
    if (newnode)
    {
        newnode->yuan->setZValue(z);
        newnode->yuan2->setZValue(z);
        foreach (Link*link, newnode->yuan->myLinks) {
            link->setZValue(z);
        }
        foreach (Link*link, newnode->yuan2->myLinks) {
            link->setZValue(z);
        }
        newnode->setZValue(z);}
}

/*******************************************************************
 * Function name: setupNode()
 * Description: This function add a Node on the scene.
 * Callee:
 * Inputs: Node *node
 * Outputs:
******************************************************************/
void DiagramWindow::setupNode(Node *node)
{
    //node->setPos(QPoint(80 + (100 * (seqNumber % 7)),
    //                    80 + (50 * ((seqNumber / 7) % 9))));
    node->setPos(100,100);
    scene->addItem(node);
    ++seqNumber;

    scene->clearSelection();
    node->setSelected(true);
    bringToFront();
}

/*******************************************************************
 * Function name: setupNewNode()
 * Description: This function add a NewNode on the scene.
 * Callee:
 * Inputs: NewNode *newnode
 * Outputs:
******************************************************************/
void DiagramWindow::setupNewNode(NewNode *newnode)
{
    //newnode->setPos(QPoint(80 + (100 * (seqNumber % 7)),
    //                    80 + (70 * ((seqNumber / 7) % 9))));
    newnode->setPos(100,100);
    scene->addItem(newnode);
    ++seqNumber;

    scene->clearSelection();
    newnode->setSelected(true);
    bringToFront();
}

/*******************************************************************
 * Function name: selectedNode()
 * Description: This function returns the pointer of a Node only when
 *     one Node is selected.
 * Callee:
 * Inputs:
 * Outputs: Node*
******************************************************************/
Node *DiagramWindow::selectedNode() const
{
    QList<QGraphicsItem *> items = scene->selectedItems();
    if (items.count() == 1) {
        {
            return dynamic_cast<Node *>(items.first());
        }
    } else {
        return 0;
    }
}

/*******************************************************************
 * Function name: selectedNewNode()
 * Description: This function returns the pointer of a NewNode only
 *     when one NewNode is selected.
 * Callee:
 * Inputs:
 * Outputs: NewNode*
******************************************************************/
NewNode *DiagramWindow::selectedNewNode() const
{
    QList<QGraphicsItem *> items = scene->selectedItems();
    if (items.count() == 1) {
        {
            return dynamic_cast<NewNode *>(items.first());
        }
    } else {
        return 0;
    }
}

/*******************************************************************
 * Function name: selectedRec()
 * Description: This function returns the pointer of a Rec only
 *     when one Rec is selected.
 * Callee:
 * Inputs:
 * Outputs: Rec*
******************************************************************/
Rec *DiagramWindow::selectedRec() const
{
    QList<QGraphicsItem *> items = scene->selectedItems();
    if (items.count() == 1) {
        {
            return dynamic_cast<Rec *>(items.first());
        }
    } else {
        return 0;
    }
}

/*******************************************************************
 * Function name: selectedLink()
 * Description: This function returns the pointer of a Link only
 *     when one Link is selected.
 * Callee:
 * Inputs:
 * Outputs: Link*
******************************************************************/
Link *DiagramWindow::selectedLink() const
{
    QList<QGraphicsItem *> items = scene->selectedItems();
    if (items.count() == 1) {
        return dynamic_cast<Link *>(items.first());
    } else {
        return 0;
    }
}

/*******************************************************************
 * Function name: selectedYuan()
 * Description: This function returns the pointer of a Yuan only
 *     when one Yuan is selected.
 * Callee:
 * Inputs:
 * Outputs: Yuan*
******************************************************************/
Yuan *DiagramWindow::selectedYuan() const
{
    QList<QGraphicsItem *> items = scene->selectedItems();
    if (items.count() == 1) {
        return dynamic_cast<Yuan *>(items.first());
    } else {
        return 0;
    }
}

/*******************************************************************
 * Function name: selectedYuanPair()
 * Description: This function returns YuanPair only when a pair of
 *     Yuan are selected.
 * Callee:
 * Inputs:
 * Outputs: YuanPair
******************************************************************/
DiagramWindow::YuanPair DiagramWindow::selectedYuanPair() const
{
    QList<QGraphicsItem *> items = scene->selectedItems();
    if (items.count() == 2) {
        Yuan *first = dynamic_cast<Yuan *>(items.first());
        Yuan *second = dynamic_cast<Yuan *>(items.last());
        if (first && second)
            return YuanPair(first, second);
    }
    return YuanPair();
}
