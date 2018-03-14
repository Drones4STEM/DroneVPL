#ifndef DIAGRAMWINDOW_H
#define DIAGRAMWINDOW_H

#include <QMainWindow>
#include <QPair>

#include<qapplication.h>
#include<qwidget.h>
#include<qfile.h>
#include<qtextstream.h>
#include<qstring.h>
#include <QEvent>
#include <QMouseEvent>

#include "scene.h"
#include "view.h"
#include "logic_help.h"


class QAction;
class QGraphicsItem;
class QGraphicsScene;
class QGraphicsView;
class QFile;
class Link;
class Node;
class NewNode;
class GoNode;
class SomeNode;
class Yuan;
class Rec;
class TakeOffNode;
class WidgetCondition;
class ColorWidget;
class PositionWidget;
class MutableWidget;
class ProcessOutputWidget;
/*******************************************************************
 * Class name: DiagramWindow
 * Base class: QMainWindow
 * Description: This is the declaration of class DiagramWindow.
 *         DiagramWindow is the main window of DroneVPL.

******************************************************************/
class DiagramWindow : public QMainWindow
{
    Q_OBJECT

public:
    DiagramWindow();

    QSize sizeHint() const;

    Node *selectedNode() const;
    NewNode *selectedNewNode() const;
    newscene* get_scene(){return scene;}

    int pasteOffset;

    int minZ;
    int maxZ;
    int seqNumber;
    int varNodeNum;  //计数varNode,命名每个varNode,下同
    int takeoffNodeNum;
    int landonNodeNum;
    int VarInstanceNodeNum;
    int computeNodeNum;
    int ioNodeNum;
    int recNodeNum;
    int linkNodeNum;

    WidgetMap* wm;
    QMap<QString,LOGIC_Help*>* LHM;
    QThread t;  //用于后台运行的线程
    newscene* scene;

    static QStringList recentFiles;

    WidgetCondition *widgetCondition;
signals:
    bool passWidget(QGraphicsItem *);
public slots:
    void setDirty(bool on);
    void setDirty();
    void selectAllItems();
    bool conditionChanged();
    void changeNodeNum(int num);

protected:
    void closeEvent(QCloseEvent *event);

private slots:
    DiagramWindow* fileNew();
    void fileOpen();
    void openRecentFile();
    bool fileSave();
    bool fileSaveAs();
    void fileExport();
    void filePrint();
    void loadFile(DiagramWindow* mainWin);

    void addTakeoffNode();
    void addLandonNode();

    void addTranslationNode();
    void addRiseNode();
    void addFallNode();
    void addAdvanceNode();
    void addBackNode();
    void addRightNode();
    void addLeftNode();

    void addSomeNode();
    void addTurnLeftNode();
    void addTurnRightNode();
    void addHangingNode();
    void addDelayNode();

    void addVarNode();
    void addVarInstanceNode();
    void addComputeNode();
    void addCompareNode();
    void addAddNode();
    void addSubNode();
    void addMulNode();
    void addDivNode();
    void addCosNode();
    void addSinNode();
    void addTanNode();
    void addLogNode();
    void addENode();
    void addEqualNode();
    void addMoreNode();
    void addLessNode();
    void addIoNode();
    void addBatteryNode();
    void addGimbalNode();
    void addAttitudeNode();
    void addChannelNode();
    void addRangeFinderNode();
    void addLink();
    void addRec();
    void addIf();
    void addElse();
    void addWhile();
    void upload();
    void run();
    void del();
    void cut();
    void copy();
    void paste();
    void bringToFront();
    void sendToBack();
    void properties();
    void updateActions();
    //void showEditToolBar();
    void showNodeBar();
    void showNodeStatusBar();
    void viewShowGrid(bool on);   //显示网格
    void canvas();
    void checkup();
    void compile();
    void checkupAndCompile();
    void openDocumentation();
    void systemInformation();
    void help();

    void connectItem(QObject *item);//建立属性框与item之间的连接
    void selectionChanged();//确保工具箱窗口部件正确显示选中项的属性

private:
    typedef QPair<Yuan *, Yuan *> YuanPair;

    void createActions();
    void createMenus();
    void createToolBars();
    void createWidgetConditionBar(WidgetCondition *widgetCondition);
    void createDockWidgets();//创建可停靠的的悬浮属性框
    void setZValue(int z);

    void setupNode(Node *node);
    void setupNewNode(NewNode *newnode);

    void selectItems( QSet<QGraphicsItem*>&items);
    void copyItems( QList<QGraphicsItem*>&items);

    bool okToClearData();
    bool openPageDesignerFile(QFile *file, QDataStream &in);

    void setCurrentFile(const QString &fileName);
    void updateRecentFileActions();//update recent files
    QString strippedName( QString &fullFileName);//updateRecentFileActions()的帮助函数

    void addTranslation(GoNode* node);//addTranslationNode()的帮助函数
    void addSome(SomeNode* node);//addSomeNode()的帮助函数

    void readSettings();
    void writeSettings();

    Link *selectedLink() const;
    Yuan *selectedYuan() const;
    YuanPair selectedYuanPair() const;
    Rec *selectedRec() const;
    bool sceneHasItems() const;
    void getSelectionProperties(bool *hasAltitudeProperty,bool *hasTimeProperty,
                                bool *hasSpeedProperty,bool *hasGroundSpeedProperty,
                                bool *hasDirectionProperty) const;

    QMenu *fileMenu;
    QMenu *editMenu;
    QMenu *actionMenu;
    QMenu *computeMenu;
    QMenu *viewMenu;
    QMenu *compileMenu;
    QMenu *helpMenu;
    QMenu *sceneMenu;   //右键呼出菜单


    //QToolBar *editToolBar;
    QToolBar *actionToolBar;
    QToolBar *aToolBar;
    QToolBar *bToolBar;
    QToolBar *cToolBar;


    QAction *fileNewAction;
    QAction *fileOpenAction;

    enum { MaxRecentFiles = 5 };
    QAction *recentFileActions[MaxRecentFiles];
    QAction *separatorAction;

    QAction *fileSaveAction;
    QAction *fileSaveAsAction;
    QAction *fileExportAction;
    QAction *filePrintAction;
    QAction *closeAction;
    QAction *exitAction;

    QAction *addActionNodeAction;
    QAction *addTakeoffNodeAction;
    QAction *addLandonNodeAction;

    QAction *addTranslationNodeAction;
    QAction *addRiseNodeAction;
    QAction *addFallNodeAction;
    QAction *addAdvanceNodeAction;
    QAction *addBackNodeAction;
    QAction *addRightNodeAction;
    QAction *addLeftNodeAction;

    QAction *addSomeNodeAction;
    QAction *addTurnLeftNodeAction;
    QAction *addTurnRightNodeAction;
    QAction *addHangingNodeAction;
    QAction *addDelayNodeAction;

    QAction *addVarNodeAction;
    QAction *addVarInstanceNodeAction;

    QAction *addComputeNodeAction;
    QAction *addCompareNodeAction;
    QAction *addAddNodeAction;
    QAction *addSubNodeAction;
    QAction *addMulNodeAction;
    QAction *addDivNodeAction;
    QAction *addCosNodeAction;
    QAction *addSinNodeAction;
    QAction *addTanNodeAction;
    QAction *addLogNodeAction;
    QAction *addENodeAction;
    QAction *addEqualNodeAction;
    QAction *addMoreNodeAction;
    QAction *addLessNodeAction;

    QAction *addIONodeAction;
    QAction *addBatteryNodeAction;
    QAction *addGimbalNodeAction;
    QAction *addAttitudeNodeAction;
    QAction *addChannelNodeAction;
    QAction *addRangeFinderNodeAction;

    QAction *addTextNodeAction;

    QAction *addLinkAction;
    QAction *addRecAction;
    QAction *addIfAction;
    QAction *addElseAction;
    QAction *addWhileAction;

    QAction *uploadAction;
    QAction *runAction;

    QAction *deleteAction;
    QAction *cutAction;
    QAction *copyAction;
    QAction *pasteAction;
    QAction *bringToFrontAction;
    QAction *sendToBackAction;
    QAction *propertiesAction;
    //QAction *showEditToolBarAction;
    QAction *showNodeBarAction;
    QAction *showNodeStatusBarAction;
    QAction *canvasAction;
    QAction *checkupAction;
    QAction *compileAction;
    QAction *checkupAndCompileAction;
    QAction *openDocumentationAction;
    QAction *systemInformationAction;
    QAction *openHelpAction;

    QAction *viewZoomInAction;
    QAction *viewZoomOutAction;
    QAction *viewShowGridAction;

    ColorWidget *colorWidget;
    PositionWidget *positionWidget;
    MutableWidget *mutableWidget;
    ProcessOutputWidget *processWidget;


    QPrinter *printer;
    QString curFile;

    View *view;

    QGraphicsItemGroup *gridGroup;
    QMouseEvent *e;

};

#endif
