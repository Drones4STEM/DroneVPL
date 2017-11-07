#ifndef FORMAT_H
#define FORMAT_H

#include <QObject>
#include <QMap>
#include <QString>
#include <QFile>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include <QTextStream>

#include "widgetwrap.h"
#include "scene.h"
#include "diagramwindow.h"
#include "digraph.h"

typedef WidgetWrap widget ;

/***************************************************
 * Class name: format
 * Base class: QObject
 * Description: This class is designed for
 *    save digraph, which is not in topological
 *    sequence. It also compile the topological
 *    widgets into python code that will be carried
 *    out by drones.
****************************************************/


class format
{
public:
    //~format(){;}
    //format(){set_scene(view->scene);}
    format();
    bool save_frame_file(QString filename);    //保存框图文件
    bool SavePyFile(QString filename = "compile.py");
    bool save_py_file(std::stack<widget *> *stk, QTextStream &in, int tabs);     //保存py可执行文件
    bool read_frame_file(QString filename);

    bool set_map(QMap<QString,WidgetWrap*>* m){Map = *m; return !Map.isEmpty();}
    bool set_digraph(QMap<QString,WidgetWrap*>* m,QMap<QString,LOGIC_Help*>* lhm)
        {digrapher = new digraph(m,lhm); return true;}
    QMap<QString, widget*> get_map(){return Map;}
    //bool set_scene(newscene* s){scene = s;return true;}

    QMap<QString, widget*> Map;
private:
    digraph* digrapher;
    //SSmap用于保存需要接受变量的控件的名字和对应的变量
    //因为每一个前置控件都知道其后置控件需要什么参数并定义该参数，但是后置控件需要事先与参数绑定才能在转化时找到它
    QMap<QString,QString> SSmap;
    //newscene* scene;

    //控件转化成代码块。参数1是要写入的目标文件名，2是要被转化的控件的指针
    void widget_convert_to_py(WidgetWrap *w, QTextStream& stream, int tabs);


    //控件转化成xml格式代码。参数1是指向要转化的控件的迭代器，2是xml文件的写入流
    void widget_convert_to_xml(QMap<QString, widget*>::iterator& iter, QXmlStreamWriter& stream);

    //These are create……() functions like those in newscene,h, but they have one important point that
    //they accept parameters from xml.
    bool CreateTakeOff(QPointF point, int id, double Altitude);
    bool CreateLand(QPointF point, int id);
    bool CreateGo(QPointF point, int id, QString direction, double Speed,double Time);
    bool CreateTurn(QPointF point, int id, QString direction, double Angel);
    bool CreateHover(QPointF point, int id, double Time);
    bool CreateDelay(QPointF point, int id, double time);
    bool CreateVar(QPointF point, int id, int amount, QString* data_type, QString* varname, QString* init_value);
    bool CreateVarInstance(QPointF point, int id, QString name, int seq);   //num表示VarInstance是Var的第几个节点
    bool CreateCompute(QPointF point, int id, QString math, QString left, QString right, QString type);
    bool CreateE(QPointF point,int id,QString math,QString right,QString type);
    bool CreateSin(QPointF point,int id,QString math,QString right,QString type);
    bool CreateLog(QPointF point,int id,QString math,QString left,QString right,QString type);
    bool CreateIO(QPointF point, int id);
    bool CreateBattery(QPointF point, int id);
    bool CreateGimbal(QPointF point, int id);
    bool CreateAttitude(QPointF point, int id);
    bool CreateChannel(QPointF point, int id);
    bool CreateRangeFinder(QPointF point, int id);

    bool CreateLogic(QPointF point, int id, QString type);

    bool CreateLink(QPointF point, int id, QString from, QString to, QString fyuan, QString tyuan);
signals:

public slots:
};

#endif // FORMAT_H
