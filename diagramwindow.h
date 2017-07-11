#ifndef DIAGRAMWINDOW_H
#define DIAGRAMWINDOW_H

#include <QMainWindow>
#include <QPair>
#include <QEvent>

class QAction;
class QGraphicsItem;
class QGraphicsScene;
class QGraphicsView;
class QFile;
class Link;
class Node;
class NewNode;
class TranslationNode;
class SomeNode;
class Yuan;
class Rec;
class TakeoffNode;

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


public slots:
    void setDirty(bool on=true);
    void selectAllItems();
   // void set_new_line();

private slots:
    void fileNew();
    void fileOpen();
    bool fileSave();
    bool fileSaveAs();
    void fileExport();
    void filePrint();
    void loadFile();


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
    void addVardefNode();
    void addComputeNode();
    void addIoNode();
    void addLink();
    void addRec();
    void del();
    void cut();
    void copy();
    void paste();
    void bringToFront();
    void sendToBack();
    void properties();
    void updateActions();

private:
    typedef QPair<Yuan *, Yuan *> YuanPair;

    void createActions();
    void createMenus();
    void createToolBars();
    void setZValue(int z);
    void setupNode(Node *node);
    void setupNewNode(NewNode *newnode);

    void selectItems(const QSet<QGraphicsItem*>&items);
    void copyItems(const QList<QGraphicsItem*>&items);

    void readItems(QDataStream &in, int offset=0, bool select=false);
    void writeItems(QDataStream &out,
                    const QList<QGraphicsItem*> &items);
    bool okToClearData();
    bool openPageDesignerFile(QFile *file, QDataStream &in);

    void addTranslation(TranslationNode* node);//addTranslationNode()的帮助函数
    void addSome(SomeNode* node);//addSomeNode()的帮助函数

    Link *selectedLink() const;
    Yuan *selectedYuan() const;
    YuanPair selectedYuanPair() const;
    Rec *selectedRec() const;

    QMenu *fileMenu;
    QMenu *editMenu;
    QToolBar *editToolBar;
    QToolBar *actionToolBar;
    QAction *fileNewAction;
    QAction *fileOpenAction;
    QAction *fileSaveAction;
    QAction *fileSaveAsAction;
    QAction *fileExportAction;
    QAction *filePrintAction;
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
    QAction *addVardefNodeAction;
    QAction *addComputeNodeAction;
    QAction *addIoNodeAction;
    QAction *addTextNodeAction;   //ceshi
    QAction *addLinkAction;
    QAction *addRecAction;
    QAction *deleteAction;
    QAction *cutAction;
    QAction *copyAction;
    QAction *pasteAction;
    QAction *bringToFrontAction;
    QAction *sendToBackAction;
    QAction *propertiesAction;

    QPrinter *printer;
    QGraphicsScene *scene;
    QGraphicsView *view;
    int pasteOffset;

    int minZ;
    int maxZ;
    int seqNumber;
    int varNodeNum;  //计数varNode,命名每个varNode,下同
    int takeoffNodeNum;
    int landonNodeNum;
    int vardefNodeNum;
    int computeNodeNum;
    int ioNodeNum;
    int recNodeNum;
    int linkNodeNum;
};

#endif
