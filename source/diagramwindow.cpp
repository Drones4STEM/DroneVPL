/*******************************************************************
 * File:diagramwindow.cp
 * Author: Ryan Feng
 * Description: This file includes the realization of class
 *        DiagramWindow. DiagramWindow is the main window of
 *        DroneVPL
******************************************************************/

#include <QtGui>
#include <QDebug>
#include <QFile>
#include <QFileDialog>
#include <QSettings>
#include <QString>
#include <QMutableStringListIterator>

#include <QApplication>
#include <QWidget>
#include <QMainWindow>
#include <QDesktopServices>
#include <string>

#include "aqp/aqp.hpp"
#include "aqp/alt_key.hpp"
#include "diagramwindow.h"
#include "link.h"
#include "node.h"
#include "newnode.h"
#include "takeoffnode.h"
#include "varnode.h"
#include "varinstancenode.h"
#include "computenode.h"
#include "ionode.h"
#include "yuan.h"
#include "rec.h"
#include "propertiesdialog.h"
#include "canvasdialog.h"
#include "itemtypes.h"
#include "widgetcondition.h"
#include "propertywidget.h"
#include "oDocument.h"
#include "odescription.h"
#include "format.h"
#include "digraph.h"
#include <QProcess>
#include <QGraphicsTextItem>



namespace {

const int StatusTimeout = AQP::MSecPerSecond * 30;
const int OffsetIncrement = 5;
const qint32 MagicNumber = 0x5A93DE5;
const qint16 VersionNumber = 1;
const QString ShowGrid("ShowGrid");
const QString MostRecentFile("MostRecentFile");
const QString MimeType = "application/vnd.qtrac.pagedesigner";



#ifndef USE_STL
template<template<typename T> class S, typename T>
T min(const S<T> &sequence)
{
    Q_ASSERT(!sequence.isEmpty());
    T minimum = sequence.first();
    foreach (const T &item, sequence)
        if (item < minimum)
            minimum = item;
    return minimum;
}

template<template<typename T> class S, typename T>
T max(const S<T> &sequence)
{
    Q_ASSERT(!sequence.isEmpty());
    T maximum = sequence.first();
    foreach (const T &item, sequence)
        if (item > maximum)
            maximum = item;
    return maximum;
}
#endif

#ifdef NO_DYNAMIC_CAST
QObject *qObjectFrom(QGraphicsItem *item)
{
    if (!item)
        return 0;
    // Types not inheriting QGraphicsObject must be handled explicitly
    if (item->type() == BoxItemType)
        return qobject_cast<QObject*>(static_cast<BoxItem*>(item));
    // Types inheriting QGraphicsObject can be handled generically
    return item->toGraphicsObject();
}
#endif

} // anonymous namespace
/*const int StatusTimeout = AQP::MSecPerSecond * 30;
const QString MostRecentFile("MostRecentFile");
const qint32 MagicNumber = 0x5A93DE5;
const qint16 VersionNumber = 1;
const QString MimeType = "application/vnd.qtrac.pagedesigner";
const int OffsetIncrement = 5;*/


QStringList DiagramWindow::recentFiles = QStringList();
/*******************************************************************
* Function name: DiagramWindow()
 * Description: This is a constructor of DiagramWindow class
 * Callee: creatActions(), createMenus(), createToolBars(),setCurrentFile(const QString &fileName)
 *         updateRecentFileActions(),updateActions()
 * Inputs:
 * Outputs:
******************************************************************/
DiagramWindow::DiagramWindow()
{
    wm = new WidgetMap();
    wm->moveToThread(&t);    //把map移到子线程中操作
    t.start();
    LHM = new QMap<QString,LOGIC_Help*>();

    printer = new QPrinter(QPrinter::HighResolution);
    gridGroup = 0;
    scene = new newscene(wm,LHM);
    QSize pageSize = printer->paperSize(QPrinter::Point).toSize();
    scene->setSceneRect(0,0,2*pageSize.width(),2*pageSize.height());
    scene->setBackgroundBrush(QColor(52,73,94));
    widgetCondition = new WidgetCondition();
    view = new View;
    view->setScene(scene);
    setMouseTracking(true);

    view->setContextMenuPolicy(Qt::ActionsContextMenu);//显示文本菜单
    setCentralWidget(view);

    minZ = 0;
    maxZ = 0;
    seqNumber = 0;

    varNodeNum=0;  //计数varNode,命名每个varNode,下同
    takeoffNodeNum=0;
    landonNodeNum=0;
    VarInstanceNodeNum=0;
    computeNodeNum=0;
    ioNodeNum=0;
    recNodeNum=0;
    linkNodeNum=0;


    createActions();
    createMenus();
    createToolBars();
    //createWidgetConditionBar(widgetCondition);
    createDockWidgets();

    connect(scene, SIGNAL(selectionChanged()),
            this, SLOT(updateActions()));
    connect(scene, SIGNAL(selectionChanged()),
          this, SLOT(conditionChanged()));
    connect(this, SIGNAL(passWidget(QGraphicsItem *)),
            widgetCondition, SLOT(setCondition(QGraphicsItem *)));
    connect(scene,SIGNAL(itemInserted(int)),
            this,SLOT(setDirty()));
    connect(scene,SIGNAL(itemInserted(int)),
            this,SLOT(changeNodeNum(int)));
    connect(scene,SIGNAL(sig_bringtofront()),
            this,SLOT(bringToFront()));
    connect(scene,SIGNAL(sig_connectItem(QObject*)),
            this,SLOT(connectItem(QObject*)));
    connect(scene,SIGNAL(selectionChanged()),
            this,SLOT(selectionChanged()));

    setAttribute(Qt::WA_DeleteOnClose);

    QSettings settings("Software Inc.", "Page Designer");
    recentFiles = settings.value("recentFiles").toStringList();
    viewShowGridAction->setChecked(settings.value(ShowGrid,false).toBool());//在设置中找showgrid，默认false

    foreach (QWidget *win, QApplication::topLevelWidgets()) {
        if (DiagramWindow *mainWin = qobject_cast<DiagramWindow*>(win))
            mainWin->updateRecentFileActions();
    }
    //updateRecentFileActions();
    setCurrentFile("");
    //QString filename=settings.value(MostRecentFile).toString();
    //if(filename.isEmpty()||filename==tr("unnamed"))
    //    QTimer::singleShot(0,this,SLOT(fileNew()));
    /*else
    {
        setWindowFilePath(filename);
        QTimer::singleShot(0,this,SLOT(loadFile()));
    }*/
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
    QSize size = printer->paperSize(QPrinter::Point).toSize() * 1.2;
    size.rwidth()+=widgetCondition->sizeHint().width();
    return size.boundedTo(
           QApplication::desktop()->availableGeometry().size());
}

/*******************************************************************
 * Function name: setDirty()
 * Description: This function notify that the window has unsaved
 *     changes and then updateActions.
 * Callee: setWindowModified(),updateActions()
 * Inputs: bool on
 * Outputs:
******************************************************************/
void DiagramWindow::setDirty(bool on)
{
    setWindowModified(on);
    updateActions();
}

void DiagramWindow::setDirty()
{
    setWindowModified(true);
    updateActions();
}

void DiagramWindow::changeNodeNum(int num)
{
    seqNumber++;
    switch (num) {
    case 1:
        takeoffNodeNum++;
        break;
    case 2:
        landonNodeNum++;
        break;
    // 待补充完整
    default:
        break;
    }
}

void DiagramWindow::closeEvent(QCloseEvent *event)
{
    if (okToClearData()) {
        QSettings settings("Software Inc.", "Page Designer");
        settings.setValue(MostRecentFile, windowFilePath());
        settings.setValue("recentFiles",recentFiles);
        settings.setValue(ShowGrid,viewShowGridAction->isChecked());
        event->accept();
    }
    else
        event->ignore();
}

/*******************************************************************
 * Function name: fileNew()
 * Description: This function creates a new file
 * Callee: okToClearData(), selectAllItems(), del(), setDirty()
 *         setWindowFilePath(),setCurrentFile("Unnamed"), updateRecentFileActions();
 * Inputs:
 * Outputs:
******************************************************************/
DiagramWindow* DiagramWindow::fileNew()
{
    /*if (!okToClearData())
        return;
    selectAllItems();
    del();
    setCurrentFile("");
    setDirty(false);*/
    DiagramWindow *mainWin = new DiagramWindow;
    //mainWin->setWindowFilePath(tr("unnamed"));
    mainWin->show();
    //CanvasDialog canvas(mainWin->view,mainWin);
    //canvas.exec();//set the size of canvas when create a new file
    return mainWin;
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
    //if (!okToClearData())
      //  return;
    const QString &filename = QFileDialog::getOpenFileName(this,
            tr("%1 - Open").arg(QApplication::applicationName()),
            ".", tr("xml (*.xml)"));
    if (filename.isEmpty())
        return;
    DiagramWindow* mainWin = DiagramWindow::fileNew();
    mainWin->setWindowFilePath(filename);
    loadFile(mainWin);
}

/*******************************************************************
 * Function name: loadFile()
 * Description: This function loads the data of an existing file
 * Callee:selectAllItems(),del(),setDirty(),
    updateRecentFileActions()
 * Inputs:
 * Outputs:
******************************************************************/
void DiagramWindow::loadFile(DiagramWindow* mainWin)
{
    mainWin->setCurrentFile(mainWin->windowFilePath());
    gridGroup=0;
    viewShowGrid(viewShowGridAction->isChecked());

    qDebug()<<mainWin->windowFilePath();
    format formater;
    formater.read_frame_file(mainWin->windowFilePath());
    mainWin->wm->set_map(formater.get_map());
    qDebug()<<"diagramwindow::loadfile(): ";

    mainWin->scene->CreateWidgets();

    mainWin->statusBar()->showMessage(tr("Loaded %1").arg(mainWin->windowFilePath()),
                             StatusTimeout);
    mainWin->setDirty(false);
    updateRecentFileActions();
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
 * Function name: openRecentFile()
 * Description: This function provides options to open recently used files.
 * Callee:loadFile()
 * Inputs:
 * Outputs:
******************************************************************/
void DiagramWindow::openRecentFile()
{
    if (okToClearData()) {
        QAction *action = qobject_cast<QAction *>(sender());
        if (action)
        {
            setWindowFilePath(action->data().toString());
            loadFile(this);
        }
    }
}

/*******************************************************************
 * Function name: setCurrentFile()
 * Description:
 * Callee:updateRecentFileActions()
 * Inputs:
 * Outputs:
******************************************************************/
void DiagramWindow::setCurrentFile(const QString &fileName)
{
        curFile = fileName;
        setWindowModified(false);
        QString shownName = tr("Unnamed");
        if (!curFile.isEmpty()) {
            shownName=curFile;
            recentFiles.removeAll(curFile);
            recentFiles.prepend(curFile);
            foreach (QWidget *win, QApplication::topLevelWidgets()) {
                if (DiagramWindow *mainWin = qobject_cast<DiagramWindow*>(win))
                    mainWin->updateRecentFileActions();
            }
            updateRecentFileActions();
        }
        setWindowFilePath(tr("%1").arg(shownName));
}
/*******************************************************************
 * Function name: updateRecentFileActions()
 * Description: This function updates the recent files.
 * Callee:
 * Inputs:
 * Outputs:
****************************************************************/
void DiagramWindow::updateRecentFileActions()
{
    QMutableStringListIterator i(recentFiles);
    while (i.hasNext()) {
            if (!QFile::exists(i.next()))
                i.remove();
        }

        for (int j = 0; j < MaxRecentFiles; ++j) {
            if (j < recentFiles.count()) {
                QString text = tr("&%1 %2")
                               .arg(j + 1)
                               .arg(strippedName(recentFiles[j]));
                recentFileActions[j]->setText(text);
                recentFileActions[j]->setData(recentFiles[j]);
                recentFileActions[j]->setVisible(true);
            } else {
                recentFileActions[j]->setVisible(false);
            }
        }
        separatorAction->setVisible(!recentFiles.isEmpty());
}

/*******************************************************************
 * Function name: strippedName(QString &fullFileName)
 * Description:This function gets the filename.
 * Callee:
 * Inputs:
 * Outputs:
******************************************************************/
QString DiagramWindow::strippedName(QString &fullFileName)
{
    return QFileInfo(fullFileName).fileName();
}

/*******************************************************************
 * Function name: selectItems()
 * Description: This function set some items selected.
 * Callee:
 * Inputs: QSet<QGraphicsItem *> &items - the items you want to select
 * Outputs:
******************************************************************/
void DiagramWindow::selectItems( QSet<QGraphicsItem *> &items)//const?
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
    /*
    QDataStream out(&file);
    out << MagicNumber << VersionNumber;
    out.setVersion(QDataStream::Qt_4_5);
    writeItems(out, scene->items());
    file.close();
    setDirty(false);
    return true;
    */
    format formater;
    formater.set_map(&(wm->get_map()));
    qDebug()<<"whether wm is empty "<<wm->Store.isEmpty();
    if(formater.Map.isEmpty()) qDebug()<<"formater map is empty";
    else qDebug()<<"formater map is not empty";
    formater.save_frame_file(filename);
    setDirty(false);
   // file.close();
    return true;
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
            ".", tr("xml(*.xml)"));
    if (filename.isEmpty())
        return false;
    if (!filename.toLower().endsWith(".xml"))
        filename += ".xml";
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
    QString suffixes = AQP::filenameFilter(tr("Bitmap image"),
             QImageWriter::supportedImageFormats());
    suffixes += tr(";;Vector image (*.svg)");
    const QString filename = QFileDialog::getSaveFileName(this,
             tr("%1 - Export").arg(QApplication::applicationName()),
              ".",suffixes);
    if (filename.isEmpty())return;
    QImage image(printer->paperSize(QPrinter::Point).toSize(),
                 QImage::Format_ARGB32);
    {
        QPainter painter(&image);
        painter.setRenderHints(QPainter::Antialiasing|
                               QPainter::TextAntialiasing);
        bool showGrid = viewShowGridAction->isChecked();
        if(showGrid)
            viewShowGrid(false);
        QList<QGraphicsItem*>items = scene->selectedItems();
        scene->clearSelection();

        scene->render(&painter);
        if(showGrid)
            viewShowGrid(true);
        foreach (QGraphicsItem *item,items)
            item->setSelected(true);
    }
    if(image.save(filename))
        statusBar()->showMessage(tr("Exported %1").arg(filename),
                                 StatusTimeout);
    else
        AQP::warning(this,tr("Error"),tr("Failed to export: %1")
                                       .arg(filename));
    /*QPixmap pixmap;
    //pixmap=pixmap.grabWidget(this,0,0,scene->width(), scene->height());
    pixmap=pixmap.grabWidget(this,aToolBar->width(),editToolBar->height(),scene->width()+aToolBar->width(),scene->height()+editToolBar->height());
    QImage  image;
    image=pixmap.toImage();
    //pixmap.save("D:\qt\image.jpg","JPG");
    QString filename = QFileDialog::getSaveFileName(this,
            tr("%1 - Save As").arg(QApplication::applicationName()),
            ".", tr("JPG (*.jpg)"));
    pixmap.save(filename);*/

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
    /*QPrinter printer(QPrinter::HighResolution);
    QPrintDialog printdialog(&printer,this);
    if(printdialog.exec())
    {
        QPainter painter(&printer);
        QPixmap image;
        image=image.grabWidget(this,0,0,scene->width(), scene->height());
        QRect rect = painter.viewport();
        QSize size = image.size();
        size.scale(rect.size(), Qt::KeepAspectRatio);     //此处保证图片显示完整
        painter.setViewport(rect.x(), rect.y(),size.width(), size.height());
        painter.setWindow(image.rect());
        painter.drawPixmap(0,0,image);
     }*/
    QPrintDialog dialog(printer);
    if(dialog.exec())
    {
        QPainter painter(printer);
        bool showGrid = viewShowGridAction->isChecked();
        if(showGrid)
            viewShowGrid(false);
        QList<QGraphicsItem*>items = scene->selectedItems();
        scene->clearSelection();

        scene->render(&painter);
        if(showGrid)
            viewShowGrid(true);;
        foreach (QGraphicsItem *item,items)
            item->setSelected(true);
    }
    statusBar()->showMessage(tr("Printed %1")
             .arg(windowFilePath()),StatusTimeout);
}

void DiagramWindow::connectItem(QObject *item)
{
    connect(item,SIGNAL(dirty()),this,SLOT(setDirty()));
    connect(item,SIGNAL(positionChanged(QPoint)),positionWidget,SLOT(setPosition(QPoint)));

    const QMetaObject *metaObject = item->metaObject();
    if(metaObject->indexOfProperty("textColor")>-1)
        connect(colorWidget,SIGNAL(textColorChanged(QColor)),
                item,SLOT(setTextColor(const QColor&)));
    if(metaObject->indexOfProperty("outlineColor")>-1)
        connect(colorWidget,SIGNAL(outlineColorChanged(QColor)),
                item,SLOT(setOutlineColor(const QColor&)));
    if(metaObject->indexOfProperty("backgroundColor")>-1)
        connect(colorWidget,SIGNAL(backgroundColorChanged(QColor)),
                item,SLOT(setBackgroundColor(const QColor&)));
    if(metaObject->indexOfProperty("position")>-1)
        connect(positionWidget,SIGNAL(positionChanged(QPoint)),
                 item,SLOT(setPosition(QPoint)));
    //特有的属性
    if(metaObject->indexOfProperty("myAltitude")>-1)    //TakeOff
    {
        connect(mutableWidget,SIGNAL(altitudeChanged(double)),
                item,SLOT(setAltitude(double)));
        connect(item,SIGNAL(altitudeChanged(double)),
                mutableWidget,SLOT(setAltitude(double)));
    }
    if(metaObject->indexOfProperty("myTime")>-1)        //LandOn,Hover,Delay
    {
        connect(mutableWidget,SIGNAL(timeChanged(double)),
                item,SLOT(setTime(double)));
        connect(item,SIGNAL(timeChanged(double)),
                mutableWidget,SLOT(setTime(double)));
    }
    if(metaObject->indexOfProperty("myGroundSpeed")>-1) //Go
    {
        connect(mutableWidget,SIGNAL(groundSpeedChanged(double)),
                item,SLOT(setGroundSpeed(double)));
        connect(item,SIGNAL(groundSpeedChanged(double)),
                mutableWidget,SLOT(setGroundSpeed(double)));
    }
    if(metaObject->indexOfProperty("mySpeed")>-1)       //Turn
    {
        connect(mutableWidget,SIGNAL(speedChanged(double)),
                item,SLOT(setSpeed(double)));
        connect(item,SIGNAL(speedChanged(double)),
                mutableWidget,SLOT(setSpeed(double)));
    }

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
    scene->selected_Index=101;//1 means the 1st object
    //setCursor(Qt::CrossCursor);//设置鼠标为十字星
    scene->need_to_set=1;
   // TakeoffNode *node=new TakeoffNode;
   // node->setText(tr("take off\n %1 s").arg(node->time));
   // setupNode(node);
   // node->yuan->setPos(QPointF((node->pos().x()),
   //                    (node->pos().y() + node->outlineRect().height()/2)+node->yuan->boundingRect().height()/2));
   // scene->addItem(node->yuan);
   // takeoffNodeNum++;
   // node->controlsId=takeoffNodeNum;
   // setDirty(true);
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
    scene->need_to_set = 1;
    scene->selected_Index=102;
    //setCursor(Qt::CrossCursor);//设置鼠标为十字星
    //LandonNode *node=new LandonNode;
    //node->setText(tr("Land on\n %1 s").arg(node->time));
    //setupNewNode(node);
    //node->yuan2->setPos(QPointF((node->pos().x()),
    //                   (node->pos().y() - node->outlineRect().height()/2)-node->yuan2->boundingRect().height()/2));
    //scene->addItem(node->yuan2);

    //landonNodeNum++;
    //node->controlsId=landonNodeNum;

    //setDirty(true);
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
    scene->need_to_set = 1;
    scene->selected_Index=100;
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
    scene->need_to_set = 1;
    scene->selected_Index = 103;

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
    scene->need_to_set = 1;
    scene->selected_Index = 104;
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
    scene->need_to_set = 1;
    scene->selected_Index = 105;
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
    scene->need_to_set=1;
    scene->selected_Index = 106;
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
    scene->need_to_set=1;
    scene->selected_Index = 107;
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
    scene->need_to_set=1;
    scene->selected_Index = 108;
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
    scene->need_to_set=1;
    scene->selected_Index = 10;
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
    scene->need_to_set=1;
    scene->selected_Index = 110;
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
    scene->need_to_set=1;
    scene->selected_Index = 111;
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
    scene->need_to_set=1;
    scene->selected_Index = 112;
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
    scene->need_to_set=1;
    scene->selected_Index = 113;

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
    scene->need_to_set=1;
    scene->selected_Index = 201;

}

/*******************************************************************
 * Function name: addVarInstanceNod()
 * Description: This function add a VarInstanceNod on the scene.
 * Callee:
 * Inputs:
 * Outputs:
******************************************************************/
void DiagramWindow::addVarInstanceNode()
{
    scene->need_to_set=1;
    scene->selected_Index = 202;
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
    scene->need_to_set=1;
    scene->selected_Index = 301;
}

void DiagramWindow::addCompareNode()
{
    return;
}

void DiagramWindow::addAddNode()
{
    scene->need_to_set=1;
    scene->selected_Index = 301;
}

void DiagramWindow::addSubNode()
{
    scene->need_to_set=1;
    scene->selected_Index = 302;
}

void DiagramWindow::addMulNode()
{
    scene->need_to_set=1;
    scene->selected_Index = 303;
}

void DiagramWindow::addDivNode()
{
    scene->need_to_set=1;
    scene->selected_Index = 304;
}

void DiagramWindow::addEqualNode()
{
    scene->need_to_set=1;
    scene->selected_Index = 305;
}

void DiagramWindow::addMoreNode()
{
    scene->need_to_set=1;
    scene->selected_Index = 306;
}

void DiagramWindow::addLessNode()
{
    scene->need_to_set=1;
    scene->selected_Index = 307;
}

void DiagramWindow::addCosNode()
{
    scene->need_to_set=1;
    scene->selected_Index = 308;
}

void DiagramWindow::addSinNode()
{
    scene->need_to_set=1;
    scene->selected_Index = 309;
}

void DiagramWindow::addTanNode()
{
    scene->need_to_set=1;
    scene->selected_Index = 310;
}

void DiagramWindow::addLogNode()
{
    scene->need_to_set=1;
    scene->selected_Index = 311;
}

void DiagramWindow::addENode()
{
    scene->need_to_set=1;
    scene->selected_Index = 312;
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
    scene->need_to_set=1;
    scene->selected_Index = 400;
}

/*******************************************************************
 * Function name: addBatteryNode()
 * Description: This function add an BatteryNode on the scene.
 * Callee:
 * Inputs:
 * Outputs:
******************************************************************/
void DiagramWindow::addBatteryNode()
{
    scene->need_to_set=1;
    scene->selected_Index = 401;
}

/*******************************************************************
 * Function name: addGimbalNode()
 * Description: This function add an GimbalNode on the scene.
 * Callee:
 * Inputs:
 * Outputs:
******************************************************************/
void DiagramWindow::addGimbalNode()
{
    scene->need_to_set=1;
    scene->selected_Index = 402;
}

/*******************************************************************
 * Function name: addAttitudeNode()
 * Description: This function add an AttitudeNode on the scene.
 * Callee:
 * Inputs:
 * Outputs:
******************************************************************/
void DiagramWindow::addAttitudeNode()
{
    scene->need_to_set=1;
    scene->selected_Index = 403;
}

/*******************************************************************
 * Function name: addChannelNode()
 * Description: This function add an ChannelNode on the scene.
 * Callee:
 * Inputs:
 * Outputs:
******************************************************************/
void DiagramWindow::addChannelNode()
{
    scene->need_to_set=1;
    scene->selected_Index = 404;
}

/*******************************************************************
 * Function name: addRangeFinderNode()
 * Description: This function add an RangeFinderNode on the scene.
 * Callee:
 * Inputs:
 * Outputs:
******************************************************************/
void DiagramWindow::addRangeFinderNode()
{
    scene->need_to_set=1;
    scene->selected_Index = 405;
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
    scene->linkNodeNum++;

    link->controlsId = scene->linkNodeNum;
    link->identifier = "Link";
    QString cid = QString::number(link->controlsId,10);
    link->name = link->identifier + cid;
    WidgetWrap* tmp = new WidgetWrap(link);   //包装节点
    wm->add(tmp);
    qDebug()<<"diagramwindow::addlink(): ";
    qDebug()<<"type: "<<link->identifier;
    qDebug()<<"id: "<<link->controlsId;
    qDebug()<<"name: "<<link->name;

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
    scene->need_to_set=1;
    scene->selected_Index = 501;
}

void DiagramWindow::addIf()
{
    scene->need_to_set=1;
    scene->selected_Index = 502;
}

void DiagramWindow::addElse()
{
    scene->need_to_set=1;
    scene->selected_Index = 503;
}

void DiagramWindow::addWhile()
{
    scene->need_to_set=1;
    scene->selected_Index = 504;
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
    scene->del();

//    QList<QGraphicsItem *> items = scene->selectedItems();
//    int itemsCount = items.count();
//    int i=0;
//    QList<Link*>itemLinks;
//    for(i=0;i<itemsCount;i++)
//    {
//        if(dynamic_cast<Link*>(items[i]))
//            itemLinks<<dynamic_cast<Link*>(items[i]);
//    }
//    QList<TakeOffNode*>itemTakeoffs;
//    for(i=0;i<itemsCount;i++)
//    {
//        if(dynamic_cast<TakeOffNode*>(items[i]))
//            itemTakeoffs<<dynamic_cast<TakeOffNode*>(items[i]);
//    }
//    QList<LandNode*>itemLandons;
//    for(i=0;i<itemsCount;i++)
//    {
//        if(dynamic_cast<LandNode*>(items[i]))
//            itemLandons<<dynamic_cast<LandNode*>(items[i]);
//    }
//    QList<GoNode*>itemTranslations;
//    for(i=0;i<itemsCount;i++)
//    {
//        if(dynamic_cast<GoNode*>(items[i]))
//            itemTranslations<<dynamic_cast<GoNode*>(items[i]);
//    }
//    QList<TurnNode*>itemTurn;
//    for(i=0;i<itemsCount;i++)
//    {
//        if(dynamic_cast<TurnNode*>(items[i]))
//            itemTurn<<dynamic_cast<TurnNode*>(items[i]);
//    }
//    QList<HoverNode*>itemHover;
//    for(i=0;i<itemsCount;i++)
//    {
//        if(dynamic_cast<HoverNode*>(items[i]))
//            itemHover<<dynamic_cast<HoverNode*>(items[i]);
//    }
//    QList<DelayNode*>itemDelay;
//    for(i=0;i<itemsCount;i++)
//    {
//        if(dynamic_cast<DelayNode*>(items[i]))
//            itemDelay<<dynamic_cast<DelayNode*>(items[i]);
//    }
//    QList<ComputeNode*>itemComputes;
//    for(i=0;i<itemsCount;i++)
//    {
//        if(dynamic_cast<ComputeNode*>(items[i]))
//            itemComputes<<dynamic_cast<ComputeNode*>(items[i]);
//    }
//    QList<IoNode*>itemIos;
//    for(i=0;i<itemsCount;i++)
//    {
//        if(dynamic_cast<IoNode*>(items[i]))
//            itemIos<<dynamic_cast<IoNode*>(items[i]);
//    }
//    QList<BatteryNode*>itemBry;
//    for(i=0;i<itemsCount;i++)
//    {
//        if(dynamic_cast<BatteryNode*>(items[i]))
//            itemBry<<dynamic_cast<BatteryNode*>(items[i]);
//    }
//    QList<GimbalNode*>itemGim;
//    for(i=0;i<itemsCount;i++)
//    {
//        if(dynamic_cast<GimbalNode*>(items[i]))
//            itemGim<<dynamic_cast<GimbalNode*>(items[i]);
//    }
//    QList<AttitudeNode*>itemAtd;
//    for(i=0;i<itemsCount;i++)
//    {
//        if(dynamic_cast<AttitudeNode*>(items[i]))
//            itemAtd<<dynamic_cast<AttitudeNode*>(items[i]);
//    }
//    QList<ChannelNode*>itemChn;
//    for(i=0;i<itemsCount;i++)
//    {
//        if(dynamic_cast<ChannelNode*>(items[i]))
//            itemChn<<dynamic_cast<ChannelNode*>(items[i]);
//    }
//    QList<RangeFinderNode*>itemRF;
//    for(i=0;i<itemsCount;i++)
//    {
//        if(dynamic_cast<RangeFinderNode*>(items[i]))
//            itemRF<<dynamic_cast<RangeFinderNode*>(items[i]);
//    }
//    QList<Rec*>itemRecs;
//    for(i=0;i<itemsCount;i++)
//    {
//        if(dynamic_cast<Rec*>(items[i]))
//            itemRecs<<dynamic_cast<Rec*>(items[i]);
//    }
//    QList<SomeNode*>itemSomes;
//    for(i=0;i<itemsCount;i++)
//    {
//        if(dynamic_cast<SomeNode*>(items[i]))
//            itemSomes<<dynamic_cast<SomeNode*>(items[i]);
//    }
//    QList<VarInstanceNode*>itemVarInstances;
//    for(i=0;i<itemsCount;i++)
//    {
//        if(dynamic_cast<VarInstanceNode*>(items[i]))
//            itemVarInstances<<dynamic_cast<VarInstanceNode*>(items[i]);
//    }
//    QList<VarNode*>itemVars;
//    for(i=0;i<itemsCount;i++)
//    {
//        if(dynamic_cast<VarNode*>(items[i]))
//            itemVars<<dynamic_cast<VarNode*>(items[i]);
//    }
//    QList<VarNode*>itemVariables;
//    for(i=0;i<itemsCount;i++)
//    {
//        if(dynamic_cast<VarNode*>(items[i]))
//            itemVariables<<dynamic_cast<VarNode*>(items[i]);
//    }
//    foreach (Link* item, itemLinks) {
////        typename QMap<QString, LOGIC_Help*>::iterator iter;
////        typename QList<Link*>::iterator it;
////        LOGIC_Help* lh;
////        Link* link;
////        for(iter=LHM->begin();iter!=LHM->end();iter++){
////            lh = iter.value();
////            for(int i=0;i<lh->LOG->tlink.length();i++){
////                link = lh->LOG->tlink[i];
////                if(link->name==item->name){
////                    lh->LOG->tlink.removeOne(link);
////                }
////            }
////            for(int i=0;i<lh->LOG->flink.length();i++){
////                link = lh->LOG->flink[i];
////                if(link->name==item->name){
////                    lh->LOG->flink.removeOne(link);
////                }
////            }

////        }
//        WidgetWrap tmp(item);
//        wm->del(tmp);
//        delete item;
//    }
//    foreach (TakeOffNode* item, itemTakeoffs) {
//        /*qDebug()<<"In del():\n"<<"TakeOff: ";
//        qDebug()<<"type: "<<item->identifier;
//        qDebug()<<"id: "<<item->controlsId;
//        qDebug()<<"location_x: "<<item->pos().x();
//        qDebug()<<"location_y: "<<item->pos().y();*/
//        WidgetWrap tmp(item);
//        wm->del(tmp);
//        scene->check_in_Logic(&tmp,"del",0);
//        delete item;
//    }
//    foreach (LandNode* item, itemLandons) {
//        WidgetWrap tmp(item);
//        wm->del(tmp);
//        scene->check_in_Logic(&tmp,"del",0);
//        delete item;
//    }
//    foreach (GoNode* item, itemTranslations) {
//        WidgetWrap tmp(item);
//        wm->del(tmp);
//        scene->check_in_Logic(&tmp,"del",0);
//        delete item;
//    }
//    foreach (TurnNode* item, itemTurn) {
//        WidgetWrap tmp(item);
//        wm->del(tmp);
//        scene->check_in_Logic(&tmp,"del",0);
//        delete item;
//    }
//    foreach (HoverNode* item, itemHover) {
//        WidgetWrap tmp(item);
//        wm->del(tmp);
//        scene->check_in_Logic(&tmp,"del",0);
//        delete item;
//    }
//    foreach (DelayNode* item, itemDelay) {
//        WidgetWrap tmp(item);
//        wm->del(tmp);
//        scene->check_in_Logic(&tmp,"del",0);
//        delete item;
//    }
//    foreach (ComputeNode* item, itemComputes) {
//        WidgetWrap tmp(item);
//        wm->del(tmp);
//        scene->check_in_Logic(&tmp,"del",0);
//        delete item;
//    }
//    foreach (IoNode* item, itemIos) {
//        WidgetWrap tmp(item);
//        wm->del(tmp);
//        scene->check_in_Logic(&tmp,"del",0);
//        delete item;
//    }
//    foreach (BatteryNode* item, itemBry) {
//        WidgetWrap tmp(item);
//        wm->del(tmp);
//        scene->check_in_Logic(&tmp,"del",0);
//        delete item;
//    }
//    foreach (GimbalNode* item, itemGim) {
//        WidgetWrap tmp(item);
//        wm->del(tmp);
//        scene->check_in_Logic(&tmp,"del",0);
//        delete item;
//    }
//    foreach (AttitudeNode* item, itemAtd) {
//        WidgetWrap tmp(item);
//        wm->del(tmp);
//        scene->check_in_Logic(&tmp,"del",0);
//        delete item;
//    }
//    foreach (ChannelNode* item, itemChn) {
//        WidgetWrap tmp(item);
//        wm->del(tmp);
//        scene->check_in_Logic(&tmp,"del",0);
//        delete item;
//    }
//    foreach (RangeFinderNode* item, itemRF) {
//        WidgetWrap tmp(item);
//        wm->del(tmp);
//        scene->check_in_Logic(&tmp,"del",0);
//        delete item;
//    }
//    foreach (Rec* item, itemRecs) {
//        WidgetWrap tmp(item);
//        wm->del(tmp);
//        delete item;
//    }
//    foreach (VarInstanceNode* item, itemVarInstances) {
//        WidgetWrap tmp(item);
//        wm->del(tmp);
//        scene->check_in_Logic(&tmp,"del",0);
//        delete item;
//    }
//    foreach (VarNode* item, itemVars) {
//        WidgetWrap tmp(item);
//        wm->del(tmp);
//        scene->check_in_Logic(&tmp,"del",0);
//        delete item;
//    }
//    foreach (SomeNode* item, itemSomes) {
//        delete item;
//    }
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
void DiagramWindow::copyItems( QList<QGraphicsItem *> &items)//const?
{
    QByteArray copiedItems;
    QDataStream out(&copiedItems,QIODevice::WriteOnly);
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
 * Function name: openDocumentation()
 * Description:This funciton open the help documentation.
 * Callee:
 * Inputs:
 * Outputs:
******************************************************************/
void DiagramWindow::openDocumentation()
{
    oDocument *w;
    w = new oDocument;
    w->show();
}

/*******************************************************************
 * Function name: systemInformation()
 * Description:
 * Callee:
 * Inputs:
 * Outputs:
******************************************************************/
void DiagramWindow::systemInformation()
{

    ODescription *description;
    description = new ODescription;
    description->show();
}

void DiagramWindow::help()
{
    //QDesktopServices::openUrl(QUrl("file:///C:/Users/ASUS/learngit/DroneVPL/help/help.chm"));
    //打包就用下面这个方法，在编译情况下将文件放在debug或release文件夹下也可用下面的方法，或者用上面指定具体路径
    QString strUrl=QCoreApplication::applicationDirPath();
    strUrl=QString("file:///%1/help.chm").arg(strUrl);
    QUrl url(strUrl);
    qDebug()<<QDesktopServices::openUrl(url);
}

/*******************************************************************
 * Function name: updateActions()
 * Description: This function changes the state of actions according
 *              to the selected items
 * Callee:
 * Inputs:
 * Outputs:
******************************************************************/
void DiagramWindow::updateActions()
{
    bool isNode = (selectedNode() != 0||selectedNewNode()!=0);
    bool isYuanPair = (selectedYuanPair() != YuanPair());
    bool isRec = (selectedRec() != 0);

    fileSaveAction->setEnabled(isWindowModified());
    bool hasItems=sceneHasItems();
    fileSaveAsAction->setEnabled(hasItems);
    fileExportAction->setEnabled(hasItems);
    filePrintAction->setEnabled(hasItems);
    int selected=scene->selectedItems().count();
    cutAction->setEnabled(selected>=1);
    copyAction->setEnabled(selected>=1);
    deleteAction->setEnabled(selected>=1);
    //pasteAction未写  以下为参考
    /*QClipboard *clipboard = QApplication::clipboard();
    const QMimeData *mimeData = clipboard->mimeData();
    editPasteAction->setEnabled(mimeData &&
            (mimeData->hasFormat(MimeType) || mimeData->hasHtml() ||
             mimeData->hasText()));*/
    addLinkAction->setEnabled(isYuanPair);
    bringToFrontAction->setEnabled(isNode||isRec);
    sendToBackAction->setEnabled(isNode||isRec);
    //更新view菜单中的动作
    //showEditToolBarAction->setChecked(editToolBar->isVisible());
    showNodeBarAction->setChecked(aToolBar->isVisible());
    showNodeStatusBarAction->setChecked(widgetCondition->isVisible());

    bool hasAltitudeProperty;
    bool hasTimeProperty;
    bool hasSpeedProperty;
    bool hasGroundSpeedProperty;
    bool hasDirectionProperty;
    getSelectionProperties(&hasAltitudeProperty,&hasTimeProperty,
                           &hasSpeedProperty,&hasGroundSpeedProperty,&hasDirectionProperty);
    mutableWidget->altitudeLineEdit->setEnabled(hasAltitudeProperty);
    mutableWidget->timeLineEdit->setEnabled(hasTimeProperty);
    mutableWidget->speedLineEdit->setEnabled(hasSpeedProperty);
    mutableWidget->groundSpeedLineEdit->setEnabled(hasGroundSpeedProperty);
    colorWidget->directionLabel->setEnabled(hasDirectionProperty);

    foreach (QAction *action, view->actions())
        view->removeAction(action);

    foreach (QAction *action, sceneMenu->actions()) {
        if (action->isEnabled())
        view->addAction(action);
    }
}

void DiagramWindow::getSelectionProperties(bool *hasAltitudeProperty,bool *hasTimeProperty,
                            bool *hasSpeedProperty,bool *hasGroundSpeedProperty,
                                           bool *hasDirectionProperty) const
{
    *hasAltitudeProperty = false;
    *hasTimeProperty = false;
    *hasSpeedProperty = false;
    *hasGroundSpeedProperty = false;
    *hasDirectionProperty = false;
    foreach (QGraphicsItem *item, scene->selectedItems()) {
        if(QObject *object = dynamic_cast<QObject *>(item))
        {
            const QMetaObject *metaObject = object->metaObject();
            if(metaObject->indexOfProperty("myAltitude")>-1)
                *hasAltitudeProperty = true;
            if(metaObject->indexOfProperty("myTime")>-1)
                *hasTimeProperty = true;
            if(metaObject->indexOfProperty("mySpeed")>-1)
                *hasSpeedProperty = true;
            if(metaObject->indexOfProperty("myGroundSpeed")>-1)
                *hasGroundSpeedProperty = true;
            if(metaObject->indexOfProperty("myDirection")>-1)
                *hasDirectionProperty = true;
            if(*hasAltitudeProperty&&*hasGroundSpeedProperty&&
                    *hasSpeedProperty&&*hasTimeProperty&&*hasDirectionProperty)
                break;
        }
    }
}

bool DiagramWindow::sceneHasItems() const
{
    foreach (QGraphicsItem *item, scene->items()) {
        if(item!=gridGroup && item->group()!=gridGroup)
            return true;
    }
    return false;
}

/*******************************************************************
 * Function name:showNodeBar()
 * Description: This function changes the state of node Bar according
 *     to the state of the "NodeBar" checkbox.
 * Callee:
 * Inputs:
 * Outputs:
******************************************************************/
void DiagramWindow::showNodeBar()
{
    if(showNodeBarAction->isChecked()) {
       aToolBar->show();
    }
    else
    {aToolBar->hide();}
}

/*******************************************************************
 * Function name:showNodeStatusBar()
 * Description: This function changes the state of status bar according
 *     to the state of the "NodeStatusBar" checkbox.
 * Callee:
 * Inputs:
 * Outputs:
******************************************************************/
void DiagramWindow::showNodeStatusBar()
{
    if(showNodeStatusBarAction->isChecked()) {
        widgetCondition->show();
    }
    else
        widgetCondition->hide();
}

/*******************************************************************
 * Function name:viewShowGrid()
 * Description: This function changes the background of scene
 * Callee:
 * Inputs:
 * Outputs:
******************************************************************/
void DiagramWindow::viewShowGrid(bool on)
{
    /*if(viewShowGridAction->isChecked())
        scene->setBackgroundBrush(QPixmap(":/images/background1.png"));
    else
        scene->setBackgroundBrush(QPixmap(":/images/background2.png"));
    scene->update();
    view->update();*/
    if(!gridGroup)
    {
        const int GridSize = 40;
        QPen pen(QColor(175,175,175,127));
        gridGroup = new QGraphicsItemGroup;
        const int MaxX = static_cast<int>(std::ceil(scene->width())
                                          /GridSize)*GridSize;
        const int MaxY = static_cast<int>(std::ceil(scene->height())
                                          /GridSize)*GridSize;
        /*for(int x = 0;x <= MaxX;x += GridSize)
        {
            QGraphicsLineItem *item = new QGraphicsLineItem(x,0,x,MaxY);
            item->setPen(pen);
            item->setZValue(-101);
            item->setEnabled(false);
            gridGroup->addToGroup(item);
        }
        for(int y = 0;y <= MaxY;y +=GridSize)
        {
            QGraphicsLineItem *item = new QGraphicsLineItem(0,y,MaxX,y);
            item->setPen(pen);
            item->setZValue(-101);
            item->setEnabled(false);
            gridGroup->addToGroup(item);
        }*/
        QGraphicsLineItem *item1 = new QGraphicsLineItem(0,0,0,MaxY);
        item1->setPen(pen);
        item1->setZValue(-101);
        //item1->setEnabled(false);
        gridGroup->addToGroup(item1);
        QGraphicsLineItem *item2 = new QGraphicsLineItem(MaxX,0,MaxX,MaxY);
        item2->setPen(pen);
        item2->setZValue(-101);
        //item2->setEnabled(false);
        gridGroup->addToGroup(item2);
        QGraphicsLineItem *item3 = new QGraphicsLineItem(0,0,MaxX,0);
        item3->setPen(pen);
        item3->setZValue(-101);
        //item3->setEnabled(false);
        gridGroup->addToGroup(item3);
        QGraphicsLineItem *item4 = new QGraphicsLineItem(0,MaxY,MaxX,MaxY);
        item4->setPen(pen);
        item4->setZValue(-101);
        //item4->setEnabled(false);
        gridGroup->addToGroup(item4);
        scene->addItem(gridGroup);
    }
    gridGroup->setVisible(on);
}

/*******************************************************************
 * Function name:canvas()
 * Description: This function is used to set the properties of the canvers.
 * Callee:
 * Inputs:
 * Outputs:
******************************************************************/
void DiagramWindow::canvas()
{
    CanvasDialog canvas(view,this);
    canvas.exec();
}

/*******************************************************************
 * Function name: checkup()
 * Description: This function discovers errors in the biock diagram before compile.
 * Callee:
 * Inputs:
 * Outputs:
******************************************************************/
void DiagramWindow::checkup()
{
    //得到当前程序工作目录
    QString path;
    QString newPath;
    QDir dir;
    path = dir.currentPath();
    //将格式改为D:\\path\\Compile.py，即将“/”改为“\\”
    QStringList list;
    list = path.split('/');
    int lenth = list.length();
    for(int i=0;i<lenth;i++)
        newPath = newPath + list[i] + "\\\\";
    newPath = newPath + "Compile.py";

    QString command;
    command = "pscp -pw apsync " + newPath + " apsync@10.0.1.128:FlightController";

    QStringList arguments;
    arguments<<"pscp"<<"-pw"<<"apsync"<<newPath
            <<QString("%1@%2:%3").arg("apsync").arg("10.0.1.128").arg("FlihtController");

    QProcess process(this);
    //process.setProcessChannelMode(QProcess::SeparateChannels);
    //process.setReadChannel(QProcess::StandardOutput);
    process.start("cmd.exe",arguments,QIODevice::ReadWrite);
    process.waitForStarted();
    process.waitForFinished();
    QString strResult = QString::fromLocal8Bit(process.readAllStandardOutput());
    qDebug()<<strResult;
    char *data;
    process.read(data,10);
    qDebug()<<data;
    process.close();

}

void DiagramWindow::compile()
{
    QMap<QString,WidgetWrap*> *m = &(wm->get_map());
    format formater;
    formater.set_map(m);
    formater.set_digraph(m,LHM);
    //formater.SavePyFile("../Compile.py");
    QString path;
    QDir dir;
    path = dir.currentPath();
    path += "/Compile.py";
    qDebug()<<path;
    formater.SavePyFile(path);

//    digraph digrapher(m);
//    std::stack<widget*> stk = digrapher.get_topology();
//    WidgetWrap* tmp;
//    while(!stk.empty()){
//        tmp = stk.top();
//        stk.pop();
//        qDebug()<<"compile():";
//        qDebug()<<"name :"<<tmp->name;
//        qDebug()<<"identifier :"<<tmp->identifier;
//        qDebug()<<"controlsId :"<<tmp->controlsId;
//    }
}

void DiagramWindow::upload()
{
    //得到当前程序工作目录
    QString path;
    QString newPath;
    QDir dir;
    path = dir.currentPath();
    //将格式改为D:\\path\\Compile.py，即将“/”改为“\\”
    QStringList list;
    list = path.split('/');
    int lenth = list.length();
    for(int i=0;i<lenth;i++)
        newPath = newPath + list[i] + "\\\\";
    newPath = newPath + "Compile.py";
    qDebug()<<newPath;
    string command;
    command = "pscp -pw apsync " + newPath.toStdString() + " apsync@10.0.1.128:FlightController";
    system(command.c_str());
    //char *chararguments;
    //chararguments = "pscp -pw apsync " + newPath + " apsync@10.0.1.128:FlightController";
    //system("pscp -pw apsync D:\\path\\Compile.py apsync@10.0.1.128:FlightController");

    /*QProcess p(0);
    p.start(command);
    p.waitForStarted();
    p.waitForFinished();
    QString str = QString::fromLocal8Bit(p.readAllStandardError());
    processWidget->setTextEdit(str);
    qDebug()<<QString::fromLocal8Bit(p.readAllStandardOutput());
    qDebug()<<str;*/
}

void DiagramWindow::run()
{
    system("plink -pw apsync apsync@10.0.1.128 python FlightController/Compile.py");
    /*QProcess p(0);
    p.start("plink -pw fff19970210 ryanfeng@192.168.80.129 python Documents/2017.txt");
    p.waitForStarted();
    p.waitForFinished();*/
    /*if(p.isReadable())
    {
        qDebug()<<true;
        QString str = QString::fromLocal8Bit(p.read(32));
        qDebug()<<str;
        processWidget->setTextEdit(str);
    }*/
    /*QString str = QString::fromLocal8Bit(p.readAllStandardOutput());
    if(!str.isEmpty())
        processWidget->setTextEdit(str);
    str = QString::fromLocal8Bit(p.readAllStandardError());
    if(!str.isEmpty())
        processWidget->setTextEdit(str);
    qDebug()<<str;*/
}

void DiagramWindow::checkupAndCompile()
{
    QProcess more;
    more.start("more");
    more.write("Text to display");
    more.closeWriteChannel();
    qDebug()<<more.readAllStandardOutput();
    qDebug()<<more.errorString();
    qDebug()<<more.environment();
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
    fileNewAction = new QAction(tr("新建"),this);
    //fileNewAction->setShortcut(QKeySequence::New);
    //fileNewAction->setIcon(QIcon(":/images/icon/打钩.png"));
    connect(fileNewAction, SIGNAL(triggered()), this, SLOT(fileNew()));
    fileNewAction->setIcon(QIcon(":/images/filenew.png"));

    fileOpenAction = new QAction(tr("打开"),this);
    fileOpenAction->setShortcut(QKeySequence::Open);
    connect(fileOpenAction, SIGNAL(triggered()), this, SLOT(fileOpen()));
    fileOpenAction->setIcon(QIcon(":/images/fileopen.png"));

   for (int i = 0; i < MaxRecentFiles; ++i) {
        recentFileActions[i] = new QAction(this);
        recentFileActions[i]->setVisible(false);
        connect(recentFileActions[i], SIGNAL(triggered()),
                this, SLOT(openRecentFile()));
    }

    fileSaveAction = new QAction(tr("保存"),this);
    fileSaveAction->setShortcut(QKeySequence::Save);
    connect(fileSaveAction, SIGNAL(triggered()), this, SLOT(fileSave()));
    fileSaveAction->setIcon(QIcon(":/images/filesave.png"));

    fileSaveAsAction = new QAction(tr("另存为"),this);
    connect(fileSaveAsAction, SIGNAL(triggered()), this, SLOT(fileSaveAs()));

    fileExportAction = new QAction(tr("导出"),this);
    connect(fileExportAction, SIGNAL(triggered()), this, SLOT(fileExport()));
    fileExportAction->setIcon(QIcon(":/images/fileexport.png"));

    filePrintAction = new QAction(tr("打印"),this);
    connect(filePrintAction, SIGNAL(triggered()), this, SLOT(filePrint()));
    filePrintAction->setIcon(QIcon(":/images/fileprint.png"));

    closeAction = new QAction(tr("关闭"),this);
    closeAction->setShortcut(tr("Ctrl+W"));
    connect(closeAction,SIGNAL(triggered()),this,SLOT(close()));

    exitAction = new QAction(tr("退出"), this);
    exitAction->setShortcut(tr("Ctrl+Q"));
    connect(exitAction, SIGNAL(triggered()), qApp, SLOT(closeAllWindows()));

    addActionNodeAction = new QAction(tr("Action"),this);

    addTakeoffNodeAction = new QAction(tr("起飞"), this);
    addTakeoffNodeAction->setIcon(QIcon(":/images/icon/take off copy.png"));
    connect(addTakeoffNodeAction, SIGNAL(triggered()), this, SLOT(addTakeoffNode()));
    addLandonNodeAction = new QAction(tr("LandOn"),this);
    addLandonNodeAction->setIcon(QIcon(":/images/icon/land on copy.png"));
    connect(addLandonNodeAction, SIGNAL(triggered()), this, SLOT(addLandonNode()));

    addTranslationNodeAction = new QAction(tr("移动"),this);
    addTranslationNodeAction->setIcon(QIcon("://images/icon/motion.png"));

    addRiseNodeAction = new QAction(tr("向上"),this);
    addRiseNodeAction->setIcon(QIcon(":/images/icon/up copy.png"));
    connect(addRiseNodeAction, SIGNAL(triggered()), this, SLOT(addRiseNode()));
    addFallNodeAction = new QAction(tr("向下"),this);
    addFallNodeAction->setIcon(QIcon(":/images/icon/down copy.png"));
    connect(addFallNodeAction, SIGNAL(triggered()), this, SLOT(addFallNode()));
    addAdvanceNodeAction = new QAction(tr("向前"),this);
    addAdvanceNodeAction->setIcon(QIcon(":/images/icon/forward copy.png"));//这里的图片要换
    connect(addAdvanceNodeAction, SIGNAL(triggered()), this, SLOT(addAdvanceNode()));
    addBackNodeAction = new QAction(tr("向后"),this);
    addBackNodeAction->setIcon(QIcon(":/images/icon/backward copy.png"));//这里的图片也要换
    connect(addBackNodeAction, SIGNAL(triggered()), this, SLOT(addBackNode()));
    addRightNodeAction = new QAction(tr("向右"),this);
    addRightNodeAction->setIcon(QIcon(":/images/icon/right copy.png"));
    connect(addRightNodeAction, SIGNAL(triggered()), this, SLOT(addRightNode()));
    addLeftNodeAction = new QAction(tr("向左"),this);
    addLeftNodeAction->setIcon(QIcon(":/images/icon/left copy.png"));
    connect(addLeftNodeAction, SIGNAL(triggered()), this, SLOT(addLeftNode()));

    addSomeNodeAction = new QAction(tr("Add Some..."),this);
    connect(addSomeNodeAction,SIGNAL(triggered()),this,SLOT(addSomeNode()));
    addTurnLeftNodeAction = new QAction(tr("左转"),this);
    addTurnLeftNodeAction->setIcon(QIcon(":/images/icon/turn left copy.png"));
    connect(addTurnLeftNodeAction, SIGNAL(triggered()), this, SLOT(addTurnLeftNode()));
    addTurnRightNodeAction = new QAction(tr("右转"),this);
    addTurnRightNodeAction->setIcon(QIcon(":/images/icon/turn right copy.png"));
    connect(addTurnRightNodeAction, SIGNAL(triggered()), this, SLOT(addTurnRightNode()));
    addHangingNodeAction = new QAction(tr("悬停"),this);
    addHangingNodeAction->setIcon(QIcon(":/images/icon/hover copy.png"));
    connect(addHangingNodeAction, SIGNAL(triggered()), this, SLOT(addHangingNode()));
    addDelayNodeAction = new QAction(tr("延时"),this);
    addDelayNodeAction->setIcon(QIcon(":/images/icon/delay copy.png"));
    connect(addDelayNodeAction, SIGNAL(triggered()), this, SLOT(addDelayNode()));

    addVarNodeAction = new QAction(tr("变量"),this);
    addVarNodeAction->setIcon(QIcon(":/images/icon/add variable copy.png"));
    connect(addVarNodeAction,SIGNAL(triggered()),this,SLOT(addVarNode()));
    addVarInstanceNodeAction = new QAction(tr("VarInstance"),this);
    addVarInstanceNodeAction->setIcon(QIcon(":/images/icon/add variable copy.png"));
    connect(addVarInstanceNodeAction,SIGNAL(triggered()),this,SLOT(addVarInstanceNode()));

    addComputeNodeAction = new QAction(tr("计算"),this);
    addComputeNodeAction->setIcon(QIcon(":/images/icon/compute copy.png"));
    addCompareNodeAction = new QAction(tr("比较"),this);
    addCompareNodeAction->setIcon(QIcon(":/images/icon/compare copy.png"));
    connect(addCompareNodeAction,SIGNAL(triggered()),this,SLOT(addEqualNode()));
    addAddNodeAction = new QAction(this);
    addAddNodeAction->setIcon(QIcon(":/images/icon/compute copy.png"));
    connect(addAddNodeAction,SIGNAL(triggered()),this,SLOT(addAddNode()));
    addSubNodeAction = new QAction(tr("-"),this);
    connect(addSubNodeAction,SIGNAL(triggered()),this,SLOT(addSubNode()));
    addMulNodeAction = new QAction(tr("*"),this);
    connect(addMulNodeAction,SIGNAL(triggered()),this,SLOT(addMulNode()));
    addDivNodeAction = new QAction(tr("/"),this);
    connect(addDivNodeAction,SIGNAL(triggered()),this,SLOT(addDivNode()));
    addCosNodeAction = new QAction(this);
    addCosNodeAction->setIcon(QIcon(":/images/icon/三角.png"));
    connect(addCosNodeAction,SIGNAL(triggered()),this,SLOT(addCosNode()));
    addSinNodeAction = new QAction(tr("sin"),this);
    connect(addSinNodeAction,SIGNAL(triggered()),this,SLOT(addSinNode()));
    addTanNodeAction = new QAction(tr("tan"),this);
    connect(addTanNodeAction,SIGNAL(triggered()),this,SLOT(addTanNode()));
    addLogNodeAction = new QAction(this);
    addLogNodeAction->setIcon(QIcon(":/images/icon/log.png"));
    connect(addLogNodeAction,SIGNAL(triggered()),this,SLOT(addLogNode()));
    addENodeAction = new QAction(this);
    addENodeAction->setIcon(QIcon(":/images/icon/e.png"));
    connect(addENodeAction,SIGNAL(triggered()),this,SLOT(addENode()));
    addEqualNodeAction = new QAction(this);
    addEqualNodeAction->setIcon(QIcon(":/images/icon/compare copy.png"));
    connect(addEqualNodeAction,SIGNAL(triggered()),this,SLOT(addEqualNode()));
    addMoreNodeAction = new QAction(tr(">"),this);
    connect(addMoreNodeAction,SIGNAL(triggered()),this,SLOT(addMoreNode()));
    addLessNodeAction = new QAction(tr("<"),this);
    connect(addLessNodeAction,SIGNAL(triggered()),this,SLOT(addLessNode()));

    addIONodeAction = new QAction(tr("传感器"),this);
    addIONodeAction->setIcon(QIcon(":/images/icon/IO.png"));
    addBatteryNodeAction = new QAction(tr("电池传感器"),this);
    addBatteryNodeAction->setIcon(QIcon(":/images/icon/Battery.png"));
    connect(addBatteryNodeAction,SIGNAL(triggered()),this,SLOT(addBatteryNode()));
    addAttitudeNodeAction = new QAction(tr("姿态传感器"),this);
    addAttitudeNodeAction->setIcon(QIcon(":/images/icon/alttitude.png"));
    connect(addAttitudeNodeAction,SIGNAL(triggered()),this,SLOT(addAttitudeNode()));
    addRangeFinderNodeAction = new QAction(tr("测高传感器"),this);
    addRangeFinderNodeAction->setIcon(QIcon(":/images/icon/rangerfinder.png"));
    connect(addRangeFinderNodeAction,SIGNAL(triggered()),this,SLOT(addRangeFinderNode()));
    addChannelNodeAction = new QAction(tr("遥控器通道"),this);
    addChannelNodeAction->setIcon(QIcon(":/images/icon/channel.png"));
    connect(addChannelNodeAction,SIGNAL(triggered()),this,SLOT(addChannelNode()));
    addGimbalNodeAction = new QAction(tr("云台"),this);
    addGimbalNodeAction->setIcon(QIcon(":/images/icon/gimbal.png"));
    connect(addGimbalNodeAction,SIGNAL(triggered()),this,SLOT(addGimbalNode()));

    addLinkAction = new QAction(tr("连线"), this);
    addLinkAction->setIcon(QIcon(":/images/link.png"));
    addLinkAction->setShortcut(tr("Ctrl+L"));
    connect(addLinkAction, SIGNAL(triggered()), this, SLOT(addLink()));

    addRecAction = new QAction(tr("逻辑框"), this);
    connect(addRecAction, SIGNAL(triggered()), this, SLOT(addRec()));
    addIfAction = new QAction(tr("If"),this);
    addIfAction->setIcon(QIcon(":/images/icon/if copy.png"));
    connect(addIfAction,SIGNAL(triggered()),this,SLOT(addIf()));
    addElseAction = new QAction(tr("Else"),this);
    addElseAction->setIcon(QIcon(":/images/icon/else copy.png"));
    connect(addElseAction,SIGNAL(triggered()),this,SLOT(addElse()));
    addWhileAction = new QAction(tr("While"),this);
    addWhileAction->setIcon(QIcon(":/images/icon/while copy.png"));
    connect(addWhileAction,SIGNAL(triggered()),this,SLOT(addWhile()));


    uploadAction = new QAction(tr("上传"),this);
    connect(uploadAction,SIGNAL(triggered()),this,SLOT(upload()));
    runAction = new QAction(tr("运行"),this);
    runAction->setIcon(QIcon(":/images/icon/运行.png"));
    connect(runAction,SIGNAL(triggered()),this,SLOT(run()));


    deleteAction = new QAction(tr("删除"), this);
    deleteAction->setIcon(QIcon(":/images/icon/delete copy.png"));
    deleteAction->setShortcut(tr("Del"));
    connect(deleteAction, SIGNAL(triggered()), this, SLOT(del()));

    cutAction = new QAction(tr("剪切"), this);
    cutAction->setIcon(QIcon(":/images/icon/cut copy.png"));
    cutAction->setShortcut(tr("Ctrl+X"));
    connect(cutAction, SIGNAL(triggered()), this, SLOT(cut()));

    copyAction = new QAction(tr("复制"), this);
    copyAction->setIcon(QIcon(":/images/icon/copy copy.png"));
    copyAction->setShortcut(tr("Ctrl+C"));
    connect(copyAction, SIGNAL(triggered()), this, SLOT(copy()));

    pasteAction = new QAction(tr("粘贴"), this);
    pasteAction->setIcon(QIcon(":/images/icon/paste copy.png"));
    pasteAction->setShortcut(tr("Ctrl+V"));
    connect(pasteAction, SIGNAL(triggered()), this, SLOT(paste()));

    bringToFrontAction = new QAction(tr("前移"), this);
    bringToFrontAction->setIcon(QIcon(":/images/bringtofront.png"));
    connect(bringToFrontAction, SIGNAL(triggered()),
            this, SLOT(bringToFront()));

    sendToBackAction = new QAction(tr("后移"), this);
    sendToBackAction->setIcon(QIcon(":/images/sendtoback.png"));
    connect(sendToBackAction, SIGNAL(triggered()),
            this, SLOT(sendToBack()));

    /*showEditToolBarAction = new QAction(tr("EditToolBar"), this);
    showEditToolBarAction->setStatusTip(tr("show or hide the edit toolbar"));
    showEditToolBarAction->setCheckable(true);
    showEditToolBarAction->setChecked(true);
    connect(showEditToolBarAction, SIGNAL(triggered()),
            this, SLOT(showEditToolBar()));*/
    showNodeBarAction = new QAction(tr("NodeBar"), this);
    showNodeBarAction->setStatusTip(tr("show or hide the node bar"));
    showNodeBarAction->setCheckable(true);
    showNodeBarAction->setChecked(true);
    connect(showNodeBarAction, SIGNAL(triggered()),
            this, SLOT(showNodeBar()));
    showNodeStatusBarAction = new QAction(tr("ToolStatusBar"), this);
    showNodeStatusBarAction->setStatusTip(tr("show or hide the tool status bar"));
    showNodeStatusBarAction->setCheckable(true);
    showNodeStatusBarAction->setChecked(true);
    connect(showNodeStatusBarAction, SIGNAL(triggered()),
            this, SLOT(showNodeStatusBar()));
    viewShowGridAction = new QAction(tr("show grid"),this);
    viewShowGridAction->setIcon(QIcon(":/images/showgrid.png"));
    viewShowGridAction->setStatusTip((tr("show or hide grid")));
    viewShowGridAction->setCheckable(true);
    viewShowGridAction->setChecked(false);
    connect(viewShowGridAction,SIGNAL(toggled(bool)),
             this,SLOT(viewShowGrid(bool)));

    propertiesAction = new QAction(tr("P&roperties..."), this);
    connect(propertiesAction, SIGNAL(triggered()),
            this, SLOT(properties()));

    canvasAction = new QAction(tr("canvas..."), this);
    connect(canvasAction, SIGNAL(triggered()),
            this, SLOT(canvas()));

    checkupAction = new QAction(tr("检查"),this);
    connect(checkupAction,SIGNAL(triggered()),
            this, SLOT(checkup()));

    compileAction = new QAction(tr("编译"),this);
    connect(compileAction,SIGNAL(triggered()),this,SLOT(compile()));

    checkupAndCompileAction = new QAction(tr("检查&编译"),this);
    connect(checkupAndCompileAction,SIGNAL(triggered()),this,SLOT(checkupAndCompile()));

    openDocumentationAction = new QAction(tr("&Documentation"),this);
    connect(openDocumentationAction,SIGNAL(triggered()),this,SLOT(openDocumentation()));

    systemInformationAction = new QAction(tr("&System information"),this);
    connect(systemInformationAction,SIGNAL(triggered()),this,SLOT(systemInformation()));

    openHelpAction = new QAction(tr("帮助"),this);
    connect(openHelpAction,SIGNAL(triggered()),this,SLOT(help()));

    viewZoomInAction = new QAction(QIcon(":/images/icon/zoom in copy.png"),
                                   tr("Zoom In"),this);
    viewZoomInAction->setShortcut(tr("+"));
    connect(viewZoomInAction,SIGNAL(triggered()),view,SLOT(zoomIn()));
    viewZoomOutAction = new QAction(QIcon(":/images/icon/zoom out copy.png"),
                                   tr("Zoom out"),this);
    viewZoomOutAction->setShortcut(tr("-"));
    connect(viewZoomOutAction,SIGNAL(triggered()),view,SLOT(zoomOut()));
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
    QMenuBar *bar = menuBar();
    bar->setStyleSheet("spacing: 52px");
    //bar->setStyleSheet("color:rgba(230,230,230,255)");
    //bar->setStyleSheet("background-color:rgba(230,230,230,255)");

    //bar->setStyleSheet("background:rgba(230,230,230,255)");
    //bar->setStyleSheet("item:background-color:rgba(230,230,230,255)");
    QFont font("MicrosoftYaHei");
    font.setPixelSize(14);
    bar->setFont(font);
    fileMenu = bar->addMenu(tr("文件"));
    editMenu = bar->addMenu(tr("编辑"));
    actionMenu = bar->addMenu(tr("动作"));
    computeMenu = bar->addMenu(tr("运算"));
    viewMenu = bar->addMenu(tr("视图"));
    compileMenu = bar->addMenu(tr("编译"));
    helpMenu = bar->addMenu(tr("帮助"));
    //fileMenu->setStyleSheet("background-color:rgba(230,230,230,255)");
    /*editMenu->setStyleSheet("background-color:rgba(230,230,230,255)");
    actionMenu->setStyleSheet("background-color:rgba(230,230,230,255)");
    computeMenu->setStyleSheet("background-color:rgba(230,230,230,255)");
    viewMenu->setStyleSheet("background-color:rgba(230,230,230,255)");
    compileMenu->setStyleSheet("background-color:rgba(230,230,230,255)");*/

    //filemenu
    fileMenu->addAction(fileNewAction);
    fileMenu->addAction(fileOpenAction);
    fileMenu->addAction(fileSaveAction);
    //fileMenu->addAction(fileSaveAsAction);
    fileMenu->addSeparator();
    fileMenu->addAction(fileExportAction);
    fileMenu->addAction(filePrintAction);
    separatorAction=fileMenu->addSeparator();
    for(int i=0;i<MaxRecentFiles;++i)
        fileMenu->addAction(recentFileActions[i]);
    fileMenu->addSeparator();
    fileMenu->addAction(closeAction);
    fileMenu->addAction(exitAction);
    //helpmenu
    helpMenu->addAction(openDocumentationAction);
    helpMenu->addAction(systemInformationAction);
    helpMenu->addAction(openHelpAction);
    //editmenu
    //editMenu->addAction(cutAction);
    editMenu->addAction(deleteAction);
    //editMenu->addAction(copyAction);
    //editMenu->addAction(pasteAction);
    editMenu->addSeparator();
    editMenu->addAction(bringToFrontAction);
    editMenu->addAction(sendToBackAction);

    //actionMenu
    QMenu *translationMenu = new QMenu(tr("movement"),this);
    foreach(QAction *action,QList<QAction*>()
            <<addRiseNodeAction<<addFallNodeAction
            <<addAdvanceNodeAction<<addBackNodeAction
            <<addRightNodeAction<<addLeftNodeAction)
        translationMenu->addAction(action);
    addTranslationNodeAction->setMenu(translationMenu);

    QMenu *aMenu = new QMenu(tr("action"),this);
    foreach (QAction *action,QList<QAction*>()
             <<addTakeoffNodeAction<<addLandonNodeAction
             <<addTranslationNodeAction
             <<addTurnLeftNodeAction<<addTurnRightNodeAction
             <<addHangingNodeAction<<addDelayNodeAction)
        aMenu->addAction(action);
    addActionNodeAction->setMenu(aMenu);
    QMenu *IOMenu = new QMenu(tr("IO"),this);
    foreach(QAction *action,QList<QAction*>()
            <<addBatteryNodeAction
            <<addGimbalNodeAction
            <<addAttitudeNodeAction
            <<addChannelNodeAction
            <<addRangeFinderNodeAction)
        IOMenu->addAction(action);
    addIONodeAction->setMenu(IOMenu);

    actionMenu->addAction(addActionNodeAction);
    actionMenu->addAction(addVarNodeAction);
    actionMenu->addAction(addIONodeAction);
    //computeMenu
    QMenu *comMenu = new QMenu(tr("Compute"),this);
    foreach (QAction *action, QList<QAction*>()
             <<addAddNodeAction
             <<addCosNodeAction
             <<addLogNodeAction
             <<addENodeAction)
        comMenu->addAction(action);
    addComputeNodeAction->setMenu(comMenu);

    /*QMenu *compareMenu = new QMenu(tr("Compare"),this);
    foreach (QAction *action, QList<QAction*>()
             <<addEqualNodeAction<<addMoreNodeAction
             <<addLessNodeAction)
        compareMenu->addAction(action);
    addCompareNodeAction->setMenu(compareMenu);*/
    QMenu *logicMenu = new QMenu(tr("Logic"),this);
    foreach (QAction *action, QList<QAction*>()
             <<addIfAction<<addElseAction
             <<addWhileAction)
        logicMenu->addAction(action);
    addRecAction->setMenu(logicMenu);

    computeMenu->addAction(addComputeNodeAction);
    computeMenu->addAction(addCompareNodeAction);
    computeMenu->addAction(addRecAction);

    //viewmenu
    /*viewMenu->addAction(showEditToolBarAction);*/
    //viewMenu->addAction(showNodeBarAction);
    //viewMenu->addAction(showNodeStatusBarAction);
    viewMenu->addAction(viewShowGridAction);
    //viewMenu->addSeparator();
    viewMenu->addAction(viewZoomInAction);
    viewMenu->addAction(viewZoomOutAction);
    viewMenu->addSeparator();
    //viewMenu->addAction(propertiesAction);
    //viewMenu->addAction(canvasAction);
    //compilemenu
    //compileMenu->addAction(checkupAction);
    compileMenu->addAction(compileAction);
    compileMenu->addAction(checkupAndCompileAction);
    compileMenu->addAction(uploadAction);
    compileMenu->addAction(runAction);

    //sceneMenu
    sceneMenu = new QMenu;
    sceneMenu->addAction(fileSaveAction);
    sceneMenu->addAction(deleteAction);
    //sceneMenu->addAction(cutAction);
    //sceneMenu->addAction(copyAction);
    //sceneMenu->addAction(pasteAction);
    sceneMenu->addAction(bringToFrontAction);
    sceneMenu->addAction(sendToBackAction);
    sceneMenu->addAction(viewZoomInAction);
    sceneMenu->addAction(viewZoomOutAction);
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
    aToolBar = addToolBar(tr("a"));
    aToolBar->addAction(addTakeoffNodeAction);
    aToolBar->addAction(addLandonNodeAction);
    aToolBar->addAction(addHangingNodeAction);
    aToolBar->addAction(addDelayNodeAction);
    aToolBar->addAction(addTranslationNodeAction);
    aToolBar->addAction(addTurnLeftNodeAction);
    aToolBar->addAction(addTurnRightNodeAction);

    addToolBar(Qt::LeftToolBarArea,aToolBar);

    bToolBar = addToolBar(tr("b"));
    bToolBar->addAction(addIfAction);
    bToolBar->addAction(addElseAction);
    bToolBar->addAction(addWhileAction);
    bToolBar->addAction(addVarNodeAction);
    bToolBar->addAction(addComputeNodeAction);
    bToolBar->addAction(addCompareNodeAction);
    bToolBar->addAction(addIONodeAction);

    addToolBar(Qt::LeftToolBarArea,bToolBar);

    cToolBar = addToolBar(tr("c"));
    //cToolBar->addAction(cutAction);
    cToolBar->addAction(deleteAction);
    //cToolBar->addAction(copyAction);
    //cToolBar->addAction(pasteAction);
    cToolBar->addAction(bringToFrontAction);
    cToolBar->addAction(sendToBackAction);
    cToolBar->addAction(viewZoomInAction);
    cToolBar->addAction(viewZoomOutAction);

    addToolBar(Qt::LeftToolBarArea,cToolBar);
}

/*******************************************************************
 * Function name: createWidgetConditionBar()
 * Description: This function creates a widgetConditionBar.
 * Callee:
 * Inputs:
 * Outputs:
******************************************************************/
void DiagramWindow::createWidgetConditionBar(WidgetCondition *widgetCondition)
{
    setDockOptions(DiagramWindow::AnimatedDocks);

    QDockWidget::DockWidgetFeatures features=
            QDockWidget::DockWidgetClosable;

    QDockWidget *rightside = new QDockWidget(this);
    //WidgetCondition *widgetCondition = new WidgetCondition();
    rightside->setWidget(widgetCondition);
    rightside->setFeatures(features);
    rightside->setAllowedAreas(Qt::RightDockWidgetArea);
    addDockWidget(Qt::RightDockWidgetArea, rightside);
}

void DiagramWindow::createDockWidgets()
{
    setDockOptions(QMainWindow::AnimatedDocks);
    QDockWidget::DockWidgetFeatures features =
            QDockWidget::DockWidgetMovable|
            QDockWidget::DockWidgetFloatable;
    colorWidget = new ColorWidget;
    QDockWidget *colorDockWidget = new QDockWidget(
                tr("Color"),this);
    colorDockWidget->setFeatures(features);
    colorDockWidget->setWidget(colorWidget);
    addDockWidget(Qt::RightDockWidgetArea,colorDockWidget);
    colorDockWidget->setVisible(false);

    positionWidget = new PositionWidget;
    QDockWidget *positionDockWidget = new QDockWidget(
                tr("Position"),this);
    positionDockWidget->setWidget(positionWidget);
    //addDockWidget(Qt::RightDockWidgetArea,positionDockWidget);
    positionDockWidget->setVisible(false);

    mutableWidget = new MutableWidget;
    QDockWidget *mutableDockWidget = new QDockWidget(
                tr("Mutable"),this);
    mutableDockWidget->setWidget(mutableWidget);
    addDockWidget(Qt::RightDockWidgetArea,mutableDockWidget);
    mutableDockWidget->setVisible(false);

    processWidget = new ProcessOutputWidget;
    QDockWidget *processDockWidget = new QDockWidget(
                tr("upload and run"),this);
    processDockWidget->setWidget(processWidget);
    addDockWidget(Qt::RightDockWidgetArea,processDockWidget);
    processDockWidget->setVisible(false);
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

bool DiagramWindow::conditionChanged(){
    QList<QGraphicsItem *> items = scene->selectedItems();
    if (items.count() >= 1) {
            QGraphicsItem *item = dynamic_cast<QGraphicsItem*>(items.first());
            emit passWidget(item);
    }
    return true;
}


void DiagramWindow::selectionChanged()
{
    QList<QGraphicsItem*>items = scene->selectedItems();
    if(items.count()==1)
    {
        if(QObject *item = dynamic_cast<QObject*>(items.at(0)))
        {
            if(item->property("textColor").isValid())
                colorWidget->setTextColor(
                            item->property("textColor").value<QColor>());
            if(item->property("outlineColor").isValid())
                colorWidget->setOutlineColor(
                            item->property("outlineColor").value<QColor>());
            if(item->property("backgroundColor").isValid())
                colorWidget->setBackgroundColor(
                            item->property("backgroundColor").value<QColor>());
            if(item->property("myIdentifier").isValid())
                colorWidget->setIdentifier(
                            item->property("myIdentifier").value<QString>());
            if(item->property("myDirection").isValid())
                colorWidget->setDirection(
                            item->property("myDirection").value<QString>());
            else
                colorWidget->setDirection(QString("None"));
            if(item->property("position").isValid())
                positionWidget->setPosition(item->property("position").value<QPoint>());
            //特有的属性
            if(item->property("myAltitude").isValid())
                mutableWidget->setAltitude(item->property("myAltitude").value<double>());
            if(item->property("myTime").isValid())
                mutableWidget->setTime(item->property("myTime").value<double>());
            if(item->property("myGroundSpeed").isValid())
                mutableWidget->setGroundSpeed(item->property("myGroundSpeed").value<double>());
            if(item->property("mySpeed").isValid())
                mutableWidget->setSpeed(item->property("mySpeed").value<double>());
        }
    }
}
