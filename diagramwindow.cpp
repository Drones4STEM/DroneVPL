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
#include "canvasdialog.h"
#include "itemtypes.h"
#include "widgetcondition.h"
#include "propertywidget.h"
#include "oDocument.h"
#include "odescription.h"
#include "format.h"
#include "digraph.h"
#include <QProcess>



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
    //scene->setSceneRect(0,0,pageSize.width(),pageSize.height());
    scene->setSceneRect(0,0,2*pageSize.width(),2*pageSize.height());
    scene->setBackgroundBrush(Qt::white);
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
    vardefNodeNum=0;
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
    //setCursor(Qt::CrossCursor);//设置鼠标为十字星
    //TranslationNode *node=new TranslationNode;
    //addTranslation(node);

    //setDirty(true);
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
    //setCursor(Qt::CrossCursor);//设置鼠标为十字星
    //TranslationNode *node=new TranslationNode;
    //addTranslation(node);
    //node->box->setCurrentIndex(0);

    //setDirty(true);
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
    //setCursor(Qt::CrossCursor);//设置鼠标为十字星
    //TranslationNode *node=new TranslationNode;
    //addTranslation(node);
    //node->box->setCurrentIndex(1);

    //setDirty();
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
    //setCursor(Qt::CrossCursor);//设置鼠标为十字星
    //TranslationNode *node=new TranslationNode;
    //addTranslation(node);
    //node->box->setCurrentIndex(2);

    //setDirty();
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
    //setCursor(Qt::CrossCursor);
    scene->need_to_set=1;
    scene->selected_Index = 106;
    //TranslationNode *node=new TranslationNode;
    //addTranslation(node);
    //node->box->setCurrentIndex(3);

    //setDirty();
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
    //setCursor(Qt::CrossCursor);
    scene->need_to_set=1;
    scene->selected_Index = 107;
    //TranslationNode *node=new TranslationNode;
    //addTranslation(node);
    //node->box->setCurrentIndex(4);

    //setDirty();
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
    //setCursor(Qt::CrossCursor);
    scene->need_to_set=1;
    scene->selected_Index = 108;
    //TranslationNode *node=new TranslationNode;
    //addTranslation(node);
    //node->box->setCurrentIndex(5);

    //setDirty();
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
    //setCursor(Qt::CrossCursor);
    scene->need_to_set=1;
    scene->selected_Index = 10;
    //SomeNode *node=new SomeNode;
    //addSome(node);
    //node->box->setCurrentIndex(0);

    //setDirty();
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
    //setCursor(Qt::CrossCursor);
    scene->need_to_set=1;
    scene->selected_Index = 110;
    //SomeNode *node=new SomeNode;
    //addSome(node);
    //node->box->setCurrentIndex(0);

    //setDirty();
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
    //setCursor(Qt::CrossCursor);
    scene->need_to_set=1;
    scene->selected_Index = 111;
    //SomeNode *node=new SomeNode;
    //addSome(node);
    //node->box->setCurrentIndex(1);

    //setDirty();
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
    //setCursor(Qt::CrossCursor);
    scene->need_to_set=1;
    scene->selected_Index = 112;
    //SomeNode *node=new SomeNode;
    //addSome(node);
    //node->box->setCurrentIndex(2);

    //setDirty();
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
 * Function name: addVardefNod()
 * Description: This function add a VardefNod on the scene.
 * Callee:
 * Inputs:
 * Outputs:
******************************************************************/
void DiagramWindow::addVardefNode()
{
    //setCursor(Qt::CrossCursor);
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
    //setCursor(Qt::CrossCursor);
    scene->need_to_set=1;
    scene->selected_Index = 301;
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

void DiagramWindow::addCosNode()
{
    scene->need_to_set=1;
    scene->selected_Index = 305;
}

void DiagramWindow::addSinNode()
{
    scene->need_to_set=1;
    scene->selected_Index = 306;
}

void DiagramWindow::addTanNode()
{
    scene->need_to_set=1;
    scene->selected_Index = 307;
}

void DiagramWindow::addLogNode()
{
    scene->need_to_set=1;
    scene->selected_Index = 308;
}

void DiagramWindow::addENode()
{
    scene->need_to_set=1;
    scene->selected_Index = 309;
}

void DiagramWindow::addEqualNode()
{
    scene->need_to_set=1;
    scene->selected_Index = 310;
}

void DiagramWindow::addMoreNode()
{
    scene->need_to_set=1;
    scene->selected_Index = 311;
}

void DiagramWindow::addLessNode()
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
    //setCursor(Qt::CrossCursor);
    scene->need_to_set=1;
    scene->selected_Index = 501;
    //Rec *rec=new Rec;
    //QGraphicsItem* item= scene->addWidget(rec->box);
    //rec->item=item;
    //rec->setPos(100,100);
    //scene->addItem(rec);
    //scene->clearSelection();
    //rec->setSelected(true);

    //rec->yuan2->setPos(QPointF(rec->pos().x() - rec->outlineRect().height()/2 + item->boundingRect().width()/2,
    //                          rec->pos().y() - rec->outlineRect().height()/2 +item->boundingRect().height()*1.5));
    //scene->addItem(rec->yuan2);

    //item->setPos(QPointF(rec->pos().x()-rec->outlineRect().width()/2,
    //                    (rec->pos().y() - rec->outlineRect().height()/2)));
    //item->setZValue(rec->zValue()+1);
    //rec->box->addItem(tr("if"));
    //rec->box->addItem(tr("else"));
    //rec->box->addItem(tr("while"));

    //recNodeNum++;
    //rec->controlsId=recNodeNum;

    //setDirty();
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
    QList<TakeOffNode*>itemTakeoffs;
    for(i=0;i<itemsCount;i++)
    {
        if(dynamic_cast<TakeOffNode*>(items[i]))
            itemTakeoffs<<dynamic_cast<TakeOffNode*>(items[i]);
    }
    QList<LandNode*>itemLandons;
    for(i=0;i<itemsCount;i++)
    {
        if(dynamic_cast<LandNode*>(items[i]))
            itemLandons<<dynamic_cast<LandNode*>(items[i]);
    }
    QList<GoNode*>itemTranslations;
    for(i=0;i<itemsCount;i++)
    {
        if(dynamic_cast<GoNode*>(items[i]))
            itemTranslations<<dynamic_cast<GoNode*>(items[i]);
    }
    QList<TurnNode*>itemTurn;
    for(i=0;i<itemsCount;i++)
    {
        if(dynamic_cast<TurnNode*>(items[i]))
            itemTurn<<dynamic_cast<TurnNode*>(items[i]);
    }
    QList<HoverNode*>itemHover;
    for(i=0;i<itemsCount;i++)
    {
        if(dynamic_cast<HoverNode*>(items[i]))
            itemHover<<dynamic_cast<HoverNode*>(items[i]);
    }
    QList<DelayNode*>itemDelay;
    for(i=0;i<itemsCount;i++)
    {
        if(dynamic_cast<DelayNode*>(items[i]))
            itemDelay<<dynamic_cast<DelayNode*>(items[i]);
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
    QList<BatteryNode*>itemBry;
    for(i=0;i<itemsCount;i++)
    {
        if(dynamic_cast<BatteryNode*>(items[i]))
            itemBry<<dynamic_cast<BatteryNode*>(items[i]);
    }
    QList<GimbalNode*>itemGim;
    for(i=0;i<itemsCount;i++)
    {
        if(dynamic_cast<GimbalNode*>(items[i]))
            itemGim<<dynamic_cast<GimbalNode*>(items[i]);
    }
    QList<AttitudeNode*>itemAtd;
    for(i=0;i<itemsCount;i++)
    {
        if(dynamic_cast<AttitudeNode*>(items[i]))
            itemAtd<<dynamic_cast<AttitudeNode*>(items[i]);
    }
    QList<ChannelNode*>itemChn;
    for(i=0;i<itemsCount;i++)
    {
        if(dynamic_cast<ChannelNode*>(items[i]))
            itemChn<<dynamic_cast<ChannelNode*>(items[i]);
    }
    QList<RangeFinderNode*>itemRF;
    for(i=0;i<itemsCount;i++)
    {
        if(dynamic_cast<RangeFinderNode*>(items[i]))
            itemRF<<dynamic_cast<RangeFinderNode*>(items[i]);
    }
    QList<Rec*>itemRecs;
    for(i=0;i<itemsCount;i++)
    {
        if(dynamic_cast<Rec*>(items[i]))
            itemRecs<<dynamic_cast<Rec*>(items[i]);
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
        typename QMap<QString, LOGIC_Help*>::iterator iter;
        //typename QList<Link*>::iterator it;
        LOGIC_Help* lh;
        Link* link;
        for(iter=LHM->begin();iter!=LHM->end();iter++){
            lh = iter.value();
            for(int i=0;i<lh->LOG->tlink.length();i++){
                link = lh->LOG->tlink[i];
                if(link->name==item->name){
                    lh->LOG->tlink.removeOne(link);
                }
            }
            for(int i=0;i<lh->LOG->flink.length();i++){
                link = lh->LOG->flink[i];
                if(link->name==item->name){
                    lh->LOG->flink.removeOne(link);
                }
            }

        }
        delete item;
    }
    foreach (TakeOffNode* item, itemTakeoffs) {
        /*qDebug()<<"In del():\n"<<"TakeOff: ";
        qDebug()<<"type: "<<item->identifier;
        qDebug()<<"id: "<<item->controlsId;
        qDebug()<<"location_x: "<<item->pos().x();
        qDebug()<<"location_y: "<<item->pos().y();*/
        WidgetWrap tmp(item);
        wm->del(tmp);
        scene->check_in_Logic(&tmp,"del",0);
        delete item;
    }
    foreach (LandNode* item, itemLandons) {
        WidgetWrap tmp(item);
        wm->del(tmp);
        scene->check_in_Logic(&tmp,"del",0);
        delete item;
    }
    foreach (GoNode* item, itemTranslations) {
        WidgetWrap tmp(item);
        wm->del(tmp);
        scene->check_in_Logic(&tmp,"del",0);
        delete item;
    }
    foreach (TurnNode* item, itemTurn) {
        WidgetWrap tmp(item);
        wm->del(tmp);
        scene->check_in_Logic(&tmp,"del",0);
        delete item;
    }
    foreach (HoverNode* item, itemHover) {
        WidgetWrap tmp(item);
        wm->del(tmp);
        scene->check_in_Logic(&tmp,"del",0);
        delete item;
    }
    foreach (DelayNode* item, itemDelay) {
        WidgetWrap tmp(item);
        wm->del(tmp);
        scene->check_in_Logic(&tmp,"del",0);
        delete item;
    }
    foreach (ComputeNode* item, itemComputes) {
        WidgetWrap tmp(item);
        wm->del(tmp);
        scene->check_in_Logic(&tmp,"del",0);
        delete item;
    }
    foreach (IoNode* item, itemIos) {
        WidgetWrap tmp(item);
        wm->del(tmp);
        scene->check_in_Logic(&tmp,"del",0);
        delete item;
    }
    foreach (BatteryNode* item, itemBry) {
        WidgetWrap tmp(item);
        wm->del(tmp);
        scene->check_in_Logic(&tmp,"del",0);
        delete item;
    }
    foreach (GimbalNode* item, itemGim) {
        WidgetWrap tmp(item);
        wm->del(tmp);
        scene->check_in_Logic(&tmp,"del",0);
        delete item;
    }
    foreach (AttitudeNode* item, itemAtd) {
        WidgetWrap tmp(item);
        wm->del(tmp);
        scene->check_in_Logic(&tmp,"del",0);
        delete item;
    }
    foreach (ChannelNode* item, itemChn) {
        WidgetWrap tmp(item);
        wm->del(tmp);
        scene->check_in_Logic(&tmp,"del",0);
        delete item;
    }
    foreach (RangeFinderNode* item, itemRF) {
        WidgetWrap tmp(item);
        wm->del(tmp);
        scene->check_in_Logic(&tmp,"del",0);
        delete item;
    }
    foreach (Rec* item, itemRecs) {
        WidgetWrap tmp(item);
        typename QMap<QString, LOGIC_Help*>::iterator iter;
        LOGIC_Help* lh;
        for(iter=LHM->begin();iter!=LHM->end();){
            lh = iter.value();
            if(lh->LOG->name==tmp.name){
                iter++;     //因为删除以后就没法访问下一个元素，所以手动在删除前访问
                LHM->remove(lh->LOG->name);
            }else iter++;
        }
        wm->del(tmp);
        delete item;
    }
    foreach (VardefNode* item, itemVardefs) {
        WidgetWrap tmp(item);
        wm->del(tmp);
        scene->check_in_Logic(&tmp,"del",0);
        delete item;
    }
    foreach (VarNode* item, itemVars) {
        WidgetWrap tmp(item);
        wm->del(tmp);
        scene->check_in_Logic(&tmp,"del",0);
        delete item;
    }
    foreach (SomeNode* item, itemSomes) {

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
    showEditToolBarAction->setChecked(editToolBar->isVisible());
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

    foreach (QAction *action, editMenu->actions()) {
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
 * Function name:showEditToolBar()
 * Description: This function changes the state of edit toolBar according
 *     to the state of the "EditToolBar" checkbox.
 * Callee:
 * Inputs:
 * Outputs:
******************************************************************/
void DiagramWindow::showEditToolBar()
{
    if(showEditToolBarAction->isChecked()) {
       editToolBar->show();
    }
    else
    {editToolBar->hide();}
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

}

void DiagramWindow::compile()
{
    QMap<QString,WidgetWrap*> *m = &(wm->get_map());
    format formater;
    formater.set_map(m);
    formater.set_digraph(m,LHM);
    formater.SavePyFile("../Compile.py");
    /*
    digraph digrapher(m);
    std::stack<widget*> stk = digrapher.get_topology();
    WidgetWrap* tmp;
    while(!stk.empty()){
        tmp = stk.top();
        stk.pop();
        qDebug()<<"compile():";
        qDebug()<<"name :"<<tmp->name;
        qDebug()<<"identifier :"<<tmp->identifier;
        qDebug()<<"controlsId :"<<tmp->controlsId;
    }*/
}

void DiagramWindow::upload()
{
    system("pscp -pw apsync D:\\path\\2017.txt apsync@10.0.1.128:FlightController");
}

void DiagramWindow::run()
{
    system("plink -pw apsync apsync@10.0.1.128 python FlightController/VehicleProperties.py");
}

void DiagramWindow::checkupAndCompile()
{

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
    fileNewAction->setShortcut(QKeySequence::New);
    connect(fileNewAction, SIGNAL(triggered()), this, SLOT(fileNew()));
    fileNewAction->setIcon(QIcon(":/images/filenew.png"));

    fileOpenAction = new QAction(tr("Open"),this);
    fileOpenAction->setShortcut(QKeySequence::Open);
    connect(fileOpenAction, SIGNAL(triggered()), this, SLOT(fileOpen()));
    fileOpenAction->setIcon(QIcon(":/images/fileopen.png"));

   for (int i = 0; i < MaxRecentFiles; ++i) {
        recentFileActions[i] = new QAction(this);
        recentFileActions[i]->setVisible(false);
        connect(recentFileActions[i], SIGNAL(triggered()),
                this, SLOT(openRecentFile()));
    }

    fileSaveAction = new QAction(tr("Save"),this);
    fileSaveAction->setShortcut(QKeySequence::Save);
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

    closeAction = new QAction(tr("&Close"),this);
    closeAction->setShortcut(tr("Ctrl+W"));
    connect(closeAction,SIGNAL(triggered()),this,SLOT(close()));

    exitAction = new QAction(tr("E&xit"), this);
    exitAction->setShortcut(tr("Ctrl+Q"));
    connect(exitAction, SIGNAL(triggered()), qApp, SLOT(closeAllWindows()));

    addActionNodeAction = new QAction(tr("Action"),this);
    addIONodeAction = new QAction(tr("IO"),this);

    addTakeoffNodeAction = new QAction(tr("TakeOff"), this);
    connect(addTakeoffNodeAction, SIGNAL(triggered()), this, SLOT(addTakeoffNode()));
    addLandonNodeAction = new QAction(tr("LandOn"),this);
    connect(addLandonNodeAction, SIGNAL(triggered()), this, SLOT(addLandonNode()));

    addTranslationNodeAction = new QAction(tr("Translation"),this);

    addRiseNodeAction = new QAction(tr("GoUp"),this);
    connect(addRiseNodeAction, SIGNAL(triggered()), this, SLOT(addRiseNode()));
    addFallNodeAction = new QAction(tr("GoDown"),this);
    connect(addFallNodeAction, SIGNAL(triggered()), this, SLOT(addFallNode()));
    addAdvanceNodeAction = new QAction(tr("Forward"),this);
    connect(addAdvanceNodeAction, SIGNAL(triggered()), this, SLOT(addAdvanceNode()));
    addBackNodeAction = new QAction(tr("Backward"),this);
    connect(addBackNodeAction, SIGNAL(triggered()), this, SLOT(addBackNode()));
    addRightNodeAction = new QAction(tr("GoRight"),this);
    connect(addRightNodeAction, SIGNAL(triggered()), this, SLOT(addRightNode()));
    addLeftNodeAction = new QAction(tr("GoLeft"),this);
    connect(addLeftNodeAction, SIGNAL(triggered()), this, SLOT(addLeftNode()));

    addSomeNodeAction = new QAction(tr("Add Some..."),this);
    connect(addSomeNodeAction,SIGNAL(triggered()),this,SLOT(addSomeNode()));
    addTurnLeftNodeAction = new QAction(tr("TurnLeft"),this);
    connect(addTurnLeftNodeAction, SIGNAL(triggered()), this, SLOT(addTurnLeftNode()));
    addTurnRightNodeAction = new QAction(tr("TurnRight"),this);
    connect(addTurnRightNodeAction, SIGNAL(triggered()), this, SLOT(addTurnRightNode()));
    addHangingNodeAction = new QAction(tr("Hover"),this);
    connect(addHangingNodeAction, SIGNAL(triggered()), this, SLOT(addHangingNode()));
    addDelayNodeAction = new QAction(tr("Delay"),this);
    connect(addDelayNodeAction, SIGNAL(triggered()), this, SLOT(addDelayNode()));

    addVarNodeAction = new QAction(tr("VarType"),this);
    connect(addVarNodeAction,SIGNAL(triggered()),this,SLOT(addVarNode()));
    addVardefNodeAction = new QAction(tr("VarDef"),this);
    connect(addVardefNodeAction,SIGNAL(triggered()),this,SLOT(addVardefNode()));

    addComputeNodeAction = new QAction(tr("Compute"),this);
    //connect(addComputeNodeAction,SIGNAL(triggered()),this,SLOT(addComputeNode()));
    addAddNodeAction = new QAction(tr("+"),this);
    connect(addAddNodeAction,SIGNAL(triggered()),this,SLOT(addAddNode()));
    addSubNodeAction = new QAction(tr("-"),this);
    connect(addSubNodeAction,SIGNAL(triggered()),this,SLOT(addSubNode()));
    addMulNodeAction = new QAction(tr("*"),this);
    connect(addMulNodeAction,SIGNAL(triggered()),this,SLOT(addMulNode()));
    addDivNodeAction = new QAction(tr("/"),this);
    connect(addDivNodeAction,SIGNAL(triggered()),this,SLOT(addDivNode()));
    addCosNodeAction = new QAction(tr("cos"),this);
    connect(addCosNodeAction,SIGNAL(triggered()),this,SLOT(addCosNode()));
    addSinNodeAction = new QAction(tr("sin"),this);
    connect(addSinNodeAction,SIGNAL(triggered()),this,SLOT(addSinNode()));
    addTanNodeAction = new QAction(tr("tan"),this);
    connect(addTanNodeAction,SIGNAL(triggered()),this,SLOT(addTanNode()));
    addLogNodeAction = new QAction(tr("log"),this);
    connect(addLogNodeAction,SIGNAL(triggered()),this,SLOT(addLogNode()));
    addENodeAction = new QAction(tr("e"),this);
    connect(addENodeAction,SIGNAL(triggered()),this,SLOT(addENode()));
    addEqualNodeAction = new QAction(tr("="),this);
    connect(addEqualNodeAction,SIGNAL(triggered()),this,SLOT(addEqualNode()));
    addMoreNodeAction = new QAction(tr(">"),this);
    connect(addMoreNodeAction,SIGNAL(triggered()),this,SLOT(addMoreNode()));
    addLessNodeAction = new QAction(tr("<"),this);
    connect(addLessNodeAction,SIGNAL(triggered()),this,SLOT(addLessNode()));

    addBatteryNodeAction = new QAction(tr("Battery"),this);
    connect(addBatteryNodeAction,SIGNAL(triggered()),this,SLOT(addBatteryNode()));
    addAttitudeNodeAction = new QAction(tr("Attitude"),this);
    connect(addAttitudeNodeAction,SIGNAL(triggered()),this,SLOT(addAttitudeNode()));
    addRangeFinderNodeAction = new QAction(tr("RangeFinder"),this);
    connect(addRangeFinderNodeAction,SIGNAL(triggered()),this,SLOT(addRangeFinderNode()));
    addChannelNodeAction = new QAction(tr("Channel"),this);
    connect(addChannelNodeAction,SIGNAL(triggered()),this,SLOT(addChannelNode()));
    addGimbalNodeAction = new QAction(tr("Gimbal"),this);
    connect(addGimbalNodeAction,SIGNAL(triggered()),this,SLOT(addGimbalNode()));

    addLinkAction = new QAction(tr("&Link"), this);
    addLinkAction->setIcon(QIcon(":/images/link.png"));
    addLinkAction->setShortcut(tr("Ctrl+L"));
    connect(addLinkAction, SIGNAL(triggered()), this, SLOT(addLink()));

    addRecAction = new QAction(tr("Logic"), this);
    connect(addRecAction, SIGNAL(triggered()), this, SLOT(addRec()));

    uploadAction = new QAction(tr("upload"),this);
    connect(uploadAction,SIGNAL(triggered()),this,SLOT(upload()));
    runAction = new QAction(tr("run"),this);
    connect(runAction,SIGNAL(triggered()),this,SLOT(run()));


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

    showEditToolBarAction = new QAction(tr("EditToolBar"), this);
    showEditToolBarAction->setStatusTip(tr("show or hide the edit toolbar"));
    showEditToolBarAction->setCheckable(true);
    showEditToolBarAction->setChecked(true);
    connect(showEditToolBarAction, SIGNAL(triggered()),
            this, SLOT(showEditToolBar()));
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

    checkupAction = new QAction(tr("check up"),this);
    connect(checkupAction,SIGNAL(triggered()),
            this, SLOT(checkup()));

    compileAction = new QAction(tr("compile"),this);
    connect(compileAction,SIGNAL(triggered()),this,SLOT(compile()));

    checkupAndCompileAction = new QAction(tr("check up and compile"),this);
    connect(checkupAndCompileAction,SIGNAL(triggered()),this,SLOT(checkupAndCompile()));

    openDocumentationAction = new QAction(tr("&Documentation"),this);
    connect(openDocumentationAction,SIGNAL(triggered()),this,SLOT(openDocumentation()));

    systemInformationAction = new QAction(tr("&System information"),this);
    connect(systemInformationAction,SIGNAL(triggered()),this,SLOT(systemInformation()));

    openHelpAction = new QAction(tr("Help"),this);
    connect(openHelpAction,SIGNAL(triggered()),this,SLOT(help()));

    viewZoomInAction = new QAction(QIcon(":/images/zoom-in.png"),
                                   tr("Zoom In"),this);
    viewZoomInAction->setShortcut(tr("+"));
    connect(viewZoomInAction,SIGNAL(triggered()),view,SLOT(zoomIn()));
    viewZoomOutAction = new QAction(QIcon(":/images/zoom-out.png"),
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
    fileMenu = menuBar()->addMenu(tr("&File"));
    editMenu = menuBar()->addMenu(tr("&Edit"));
    viewMenu = menuBar()->addMenu(tr("&View"));
    compileMenu = menuBar()->addMenu(tr("&Compile"));
    helpMenu = menuBar()->addMenu(tr("&Help"));
    //filemenu
    fileMenu->addAction(fileNewAction);
    fileMenu->addAction(fileOpenAction);
    fileMenu->addAction(fileSaveAction);
    fileMenu->addAction(fileSaveAsAction);
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

    QMenu *IOMenu = new QMenu(tr("IO"),this);
    foreach(QAction *action,QList<QAction*>()
            <<addBatteryNodeAction
            <<addGimbalNodeAction
            <<addAttitudeNodeAction
            <<addChannelNodeAction
            <<addRangeFinderNodeAction)
        IOMenu->addAction(action);
    addIONodeAction->setMenu(IOMenu);
    QMenu *computeMenu = new QMenu(tr("Compute"),this);
    foreach (QAction *action, QList<QAction*>()
             <<addAddNodeAction<<addSubNodeAction
             <<addMulNodeAction<<addDivNodeAction
             <<addCosNodeAction<<addSinNodeAction
             <<addTanNodeAction<<addLogNodeAction
             <<addENodeAction<<addEqualNodeAction
             <<addMoreNodeAction<<addLessNodeAction)
        computeMenu->addAction(action);
    addComputeNodeAction->setMenu(computeMenu);

    editMenu->addAction(addActionNodeAction);
    editMenu->addAction(addVarNodeAction);
    editMenu->addAction(addVardefNodeAction);
    editMenu->addAction(addComputeNodeAction);
    editMenu->addAction(addIONodeAction);
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
    //viewmenu
    viewMenu->addAction(showEditToolBarAction);
    viewMenu->addAction(showNodeBarAction);
    viewMenu->addAction(showNodeStatusBarAction);
    viewMenu->addAction(viewShowGridAction);
    viewMenu->addSeparator();
    viewMenu->addAction(viewZoomInAction);
    viewMenu->addAction(viewZoomOutAction);
    viewMenu->addSeparator();
    viewMenu->addAction(propertiesAction);
    viewMenu->addAction(canvasAction);
    //compilemenu
    compileMenu->addAction(checkupAction);
    compileMenu->addAction(compileAction);
    compileMenu->addAction(checkupAndCompileAction);
    compileMenu->addAction(uploadAction);
    compileMenu->addAction(runAction);
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
    editToolBar->addAction(viewZoomInAction);
    editToolBar->addAction(viewZoomOutAction);
    editToolBar->addAction(viewShowGridAction);

    aToolBar = addToolBar(tr("action"));
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
    aToolBar->addAction(addIONodeAction);
    aToolBar->addAction(addRecAction);
    aToolBar->addAction(addLinkAction);
    addToolBar(Qt::LeftToolBarArea,aToolBar);
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

    positionWidget = new PositionWidget;
    QDockWidget *positionDockWidget = new QDockWidget(
                tr("Position"),this);
    positionDockWidget->setWidget(positionWidget);
    addDockWidget(Qt::RightDockWidgetArea,positionDockWidget);

    mutableWidget = new MutableWidget;
    QDockWidget *mutableDockWidget = new QDockWidget(
                tr("Mutable"),this);
    mutableDockWidget->setWidget(mutableWidget);
    addDockWidget(Qt::RightDockWidgetArea,mutableDockWidget);
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
