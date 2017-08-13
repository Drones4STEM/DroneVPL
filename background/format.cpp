
/********************************************************
 * File: format.cpp
 * Author: Gao Jiefeng
 * Description:This file realizes two functions about save and compile.
 *    It can save block diagram of widgets and write it into xml file.
 *    It can also write python code according to topological sequence
 *    of block diagram.
*********************************************************/
#include <QDebug>
#include <QPointF>

#include "format.h"
#include "link.h"

using namespace std;

/*****************************************************
 * Function name: format
 * Description: This is the constructor.
 * Calle: none
 * Input: QMap<QString, widget *> - widgets exits now
 * Output: none
 *****************************************************/
format::format(QMap<QString, WidgetWrap>& m)
{
    if(m.isEmpty()) qDebug()<<"format(QMap<QString, WidgetWrap>& m)   m is empty";
    Map = m;
}


/*****************************************************
 * Function name: save_frame_file
 * Description: This is the funtion that save the block
 *    diagram in xml file.
 * Calle: widget_convert_to_xml
 * Input: QString filename = "FrameGraph.xml" - name of xml file
 * Output: true - to tell if saving is successful.
 *****************************************************/
bool format::save_frame_file(QString filename = "FrameGraph.xml")
{
    qDebug()<<filename;
    typename QMap<QString, widget>::iterator iter;
    QFile file(filename);
    file.open(QIODevice::WriteOnly);
    QXmlStreamWriter stream(&file);

    stream.setAutoFormatting(true);
    stream.writeStartDocument("1.0");
    stream.writeStartElement("WidgetsAndLinks");
    //这个标签本身没有什么意义，但是在读取xml文件的时候需要有一个总标签包含所有子标签，所以用它占位

    if(Map.isEmpty()) qDebug()<<"format map is empty";
    else
        for(iter=Map.begin(); iter!=Map.end(); iter++){   //遍历控件指针
            widget_convert_to_xml(iter,stream);
        }

    stream.writeEndElement();
    stream.writeEndDocument();
    file.close();
    return true;
}


/*****************************************************
 * Function name: widget_convert_to_xml
 * Description: This is the funtion that save one widget
 *    in xml file. All its properties will be described
 *    in xml.
 * Calle: none
 * Input: widget* w - the widget to be saved
 *        QXmlStreamWriter stream - the file stream to operate the xml file
 * Output: none
 *****************************************************/

void format::widget_convert_to_xml(QMap<QString, widget>::iterator& iter, QXmlStreamWriter& stream)
{
    /* 类别 category
     * 型号 type
     * 名字 name
     * 坐标 location
     */
    QString identifier = iter->identifier;
    QString controlsId = QString::number(iter->controlsId);
    QString x,y;
    //---------Action------------------
    if( iter->category == "Action" ){
        //stream.writeStartElement("widget");
        //stream.writeAttribute("category", "Action");

        stream.writeStartElement("Action");
        stream.writeAttribute("type",identifier);
        stream.writeTextElement("id",controlsId);
        if(identifier == "TakeOff"){ //起飞动作
             x = QString::number((long)iter->mTakeoffNode->pos().x(),10);
             stream.writeTextElement("location_x",x);
             y = QString::number((long)iter->mTakeoffNode->pos().y(),10);
             stream.writeTextElement("location_y",y);
             //stream.writeStartElement("arrow_out");
        }
        if(identifier == "Land"){ //降落动作
            x = QString::number((long)iter->mLandNode->pos().x(),10);
            stream.writeTextElement("location_x",x);
            y = QString::number((long)iter->mLandNode->pos().y(),10);
            stream.writeTextElement("location_y",y);
        }
        if(identifier == "Go"){ //六个方向的直线移动动作
            x = QString::number((long)iter->mGoNode->pos().x(),10);
            stream.writeTextElement("location_x",x);
            y = QString::number((long)iter->mGoNode->pos().y(),10);
            stream.writeTextElement("location_y",y);
        }
        if(identifier == "Turn"){ //转向动作
            x = QString::number((long)iter->mTurnNode->pos().x(),10);
            stream.writeTextElement("location_x",x);
            y = QString::number((long)iter->mTurnNode->pos().y(),10);
            stream.writeTextElement("location_y",y);
        }
        if(identifier == "Hover"){ //悬停动作
            x = QString::number((long)iter->mHoverNode->pos().x(),10);
            stream.writeTextElement("location_x",x);
            y = QString::number((long)iter->mHoverNode->pos().y(),10);
            stream.writeTextElement("location_y",y);
        }
        if(identifier == "Delay"){ //延时动作
            x = QString::number((long)iter->mDelayNode->pos().x(),10);
            stream.writeTextElement("location_x",x);
            y = QString::number((long)iter->mDelayNode->pos().y(),10);
            stream.writeTextElement("location_y",y);
        }
        /*
          if(identifier == "GoLeft"){ //左移动作
              stream.writeTextElement("motion","move_left");
              stream.writeStartElement("speed");
              stream.writeAttribute("unit","m/s");
              //stream.writeCharacters();
              stream.writeEndElement();
          }
          if(identifier == "GoRight"){ //右移动作
              stream.writeTextElement("motion","move_right");
              stream.writeStartElement("speed");
              stream.writeAttribute("unit","m/s");
              //stream.writeCharacters();
              stream.writeEndElement();
          }
          if(identifier == "GoUp"){ //上移动作
              stream.writeTextElement("motion","move_up");
              stream.writeStartElement("speed");
              stream.writeAttribute("unit","m/s");
             // stream.writeCharacters();
              stream.writeEndElement();
          }
          if(identifier == "GoDown"){ //下移动作
              stream.writeTextElement("motion","move_down");
              stream.writeStartElement("speed");
              stream.writeAttribute("unit","m/s");
             // stream.writeCharacters();
              stream.writeEndElement();
          }
          if(identifier == "Forward"){ //前移动作
              stream.writeTextElement("motion","move_forward");
              stream.writeStartElement("speed");
              stream.writeAttribute("unit","m/s");
              //stream.writeCharacters();
              stream.writeEndElement();
          }
          if(identifier == "Backward"){ //后移动作
              stream.writeTextElement("motion","move_back");
              stream.writeStartElement("speed");
              stream.writeAttribute("unit","m/s");
              //stream.writeCharacters();
              stream.writeEndElement();
          }
          if(identifier == "TurnLeft"){ //左转动作
              stream.writeTextElement("motion","turn_left");
              stream.writeStartElement("angle");
              stream.writeAttribute("unit","angle");
              //stream.writeCharacters();
              stream.writeEndElement();
          }
          if(identifier == "TurnRight"){ //右转动作
              stream.writeTextElement("motion","turn_right");
              stream.writeStartElement("angle");
              stream.writeAttribute("unit","angle");
             // stream.writeCharacters();
              stream.writeEndElement();
          }*/

        qDebug()<<"category: "<<iter->category;
        qDebug()<<"type: "<<identifier;
        qDebug()<<"id: "<<controlsId;
        qDebug()<<"location_x: "<<x;
        qDebug()<<"location_y: "<<y;
        //stream.writeEndElement();
        stream.writeEndElement();   //correspond to writeStartElement("Action")
    }
    //--------VAR--------------
    if( iter->category == "VAR" ){    //是VAR控件
        stream.writeStartElement("VAR");
        stream.writeAttribute("type",identifier);
        stream.writeTextElement("id",controlsId);
        if(identifier == "VarType"){
            x = QString::number((long)iter->mVarTypeNode->pos().x(),10);
            stream.writeTextElement("location_x",x);
            y = QString::number((long)iter->mVarTypeNode->pos().y(),10);
            stream.writeTextElement("location_y",y);
            //stream.writeTextElement("data_type",iter->);
            //stream.writeStartElement("arrow_out");
        }
        if(identifier == "VarDef"){
            x = QString::number((long)iter->mVarDefNode->pos().x(),10);
            stream.writeTextElement("location_x",x);
            y = QString::number((long)iter->mVarDefNode->pos().y(),10);
            stream.writeTextElement("location_y",y);
            //if(iter->mVarDefNode->node!=0)
                //stream.writeTextElement("data_type",iter->mVarDefNode->node->name); //  添加对应的VarType的编号
            //stream.writeStartElement("arrow_out");
        }
        stream.writeEndElement();   //correspond to writeStartElement("VAR")
        qDebug()<<"category: "<<iter->category;
        qDebug()<<"type: "<<identifier;
        qDebug()<<"id: "<<controlsId;
        qDebug()<<"location_x: "<<x;
        qDebug()<<"location_y: "<<y;
    }
    //---------Compute------------------
    if(iter->category == "Compute"){
        stream.writeStartElement("Compute");
        stream.writeAttribute("type",identifier);
        stream.writeTextElement("id",controlsId);
        if(identifier == "Compute"){
            x = QString::number((long)iter->mComputeNode->pos().x(),10);
            stream.writeTextElement("location_x",x);
            y = QString::number((long)iter->mComputeNode->pos().y(),10);
            stream.writeTextElement("location_y",y);
            //stream.writeStartElement("arrow_out");
        }
        stream.writeStartElement("math");
        if(iter->mComputeNode->box->currentText()=="+"){ //如果加法
            stream.writeAttribute("operator","add");
        }
        stream.writeEndElement();
        stream.writeEndElement();   //correspond to writeStartElement("Compute")
        qDebug()<<"category: "<<iter->category;
        qDebug()<<"type: "<<identifier;
        qDebug()<<"id: "<<controlsId;
        qDebug()<<"location_x: "<<x;
        qDebug()<<"location_y: "<<y;
        qDebug()<<"operator: "<<iter->mComputeNode->box->currentText();
    }
    //---------IO------------------
    if(iter->category == "IO"){
        stream.writeStartElement("IO");
        stream.writeAttribute("type",identifier);
        stream.writeTextElement("id",controlsId);
        if(identifier == "IO"){
            x = QString::number((long)iter->mIONode->pos().x(),10);
            stream.writeTextElement("location_x",x);
            y = QString::number((long)iter->mIONode->pos().y(),10);
            stream.writeTextElement("location_y",y);
            //stream.writeStartElement("arrow_out");
        }/*
          if(){ //有输出
              stream.writeStartElement("arrow_out");
               stream.writeStartElement("a_to");
               stream.writeAttribute("name", iter->value->yuan->myLinks->yuan->node->name);
               //指到node的名字，还未完成
               stream.writeCharacters(iter->value->yuan->myLinks->yuan);
               //指到yuan的名字，还未完成
               stream.writeTextElement("goto",iter->value->yuan->myLinks->yuan->node->name);
               stream.writeEndElement();
              stream.writeEndElement();
          }*/
        stream.writeEndElement();   //correspond to writeStartElement("IO")
        qDebug()<<"category: "<<iter->category;
        qDebug()<<"type: "<<identifier;
        qDebug()<<"id: "<<controlsId;
        qDebug()<<"location_x: "<<x;
        qDebug()<<"location_y: "<<y;
    }
    //---------LOGIC------------------
    if(iter->category == "Logic"){
        stream.writeStartElement("Logic");
        stream.writeAttribute("type",identifier);
        stream.writeTextElement("id",controlsId);
        if(identifier == "Logic"){
            x = QString::number((long)iter->mLogicNode->pos().x(),10);
            stream.writeTextElement("location_x",x);
            y = QString::number((long)iter->mLogicNode->pos().y(),10);
            stream.writeTextElement("location_y",y);
            //stream.writeStartElement("arrow_out");
        }
        stream.writeEndElement();   //correspond to writeStartElement("Logic")
        qDebug()<<"category: "<<iter->category;
        qDebug()<<"type: "<<identifier;
        qDebug()<<"id: "<<controlsId;
        qDebug()<<"location_x: "<<x;
        qDebug()<<"location_y: "<<y;
    }
        /*
        QString t;
        t = ;//判断是if还是else还是while
         SWmap::iterator WILiter;
         SWmap* WIL = iter->value->li->WidgetsInLOGIC;
         for(WILiter=WIL->begin(); WILiter!=WIL->end(); WILiter++){
            save_widget(WILiter,stream);
         }
         if(){ //控件if
            stream.writeTextElement("else",);
         }
         stream.writeEndElement();
        stream.writeEndElement();
    }*/
        /*
        if(){ //如果减法
            stream.writeAttribute("subtraction");
        }
        if(){ //如果乘法
            stream.writeAttribute("multiple");
        }
        if(){ //如果除法
            stream.writeAttribute("divison");
        }
        if(){ //如果幂
            stream.writeAttribute("power");
        }
        if(){ //如果正弦
            stream.writeAttribute("sin");
            stream.writeStartElement("angle");
            stream.writeAttribute("unit","angle");
            stream.writeTextElement();
        }
        if(){ //如果余弦
            stream.writeAttribute("cos");
            stream.writeStartElement("angle");
            stream.writeAttribute("unit","angle");
            stream.writeTextElement();
        }
        if(){ //如果正切
            stream.writeAttribute("tan");
            stream.writeStartElement("angle");
            stream.writeAttribute("unit","angle");
            stream.writeTextElement();
        }
        if(){ //如果对数
            stream.writeAttribute("log");
        }
        if(){ //如果等于
            stream.writeAttribute("equal");
        }
        if(){ //如果小于
            stream.writeAttribute("less");
        }
        if(){ //如果大于
            stream.writeAttribute("greater");
        }
        stream.writeEndElement();
        stream.writeEndElement();
    }*/
          /*
          if(){   //判断VAR里面是否有保存变量
              stream.writeTextElement("variable_a",iter->);
              stream.writeTextElement("variable_b",iter->);
          }
          if(){ //有constant
              stream.writeTextElement("constant",);
          }
          if(){ //有输出
              stream.writeStartElement("arrow_out");
               stream.writeStartElement("a_to");
               stream.writeAttribute("name", iter->value->yuan->myLinks->yuan->node->name);
               //指到node的名字，还未完成
                stream.writeCharacters(iter->value->yuan->myLinks->yuan);
                //指到yuan的名字，还未完成
               stream.writeEndElement();
              stream.writeEndElement();
          }
         stream.writeEndElement();
        stream.writeEndElement();
    }
*/
}

/*****************************************************
 * Function name: read_frame_file
 * Description: This funtion reads a xml file and
 *              redraw the frame.
 * Calle:
 * Input: QString filename - the xml file name
 * Output: true - to tell if reading is successful
 *****************************************************/
bool format::read_frame_file(QString filename)
{
    QFile file(filename);
    file.open(QIODevice::ReadOnly);
    QXmlStreamReader stream(&file);
    qDebug()<<"read frame file";
    QString category,type;
    qint16 location_x,location_y,id;

    qDebug()<<stream.hasError();
    while(!stream.atEnd()){ //exit if at end or has error
        //qDebug()<<"not end";
        stream.readNext();
        if (stream.isStartElement()){
            /*if(stream.name().toString() == "widget"){   //根据xml的嵌套关系构建嵌套的if语句，维护属性包装。widget属性暂时无用
                category = stream.attributes().value("category").toString();
                qDebug()<<"category: "<<category;
                stream.readNext();  //不知为何，在读新值之前会先读一个空值
                stream.readNext();*/
            if(stream.name().toString()=="Action"){
                    type = stream.attributes().value("type").toString();
                    qDebug()<<"type: "<<type;
                    stream.readNext();
                    stream.readNext();
                    if(stream.name().toString()=="id"){
                        id = stream.readElementText().toInt();
                        qDebug()<<"id: "<<id;
                    }
                    stream.readNext();
                    stream.readNext();
                    if(stream.name().toString()=="location_x"){
                        location_x = stream.readElementText().toInt();
                        qDebug()<<"location_x: "<<location_x;
                    }
                    stream.readNext();
                    stream.readNext();
                    if(stream.name().toString()=="location_y"){
                        location_y = stream.readElementText().toInt();
                        qDebug()<<"location_y: "<<location_y;
                    }

                    QPointF point(location_x,location_y);
                    try{
                        if(type=="TakeOff"){
                            scene->CreateTakeOff(point,id); //在窗口中生成takeoff控件
                        }
                        if(type=="Land"){
                            scene->CreateLand(point,id); //在窗口中生成takeoff控件
                        }
                        if(type=="Go"){
                            scene->CreateGo(point,id); //在窗口中生成takeoff控件
                        }
                        if(type=="Turn"){
                            scene->CreateTurn(point,id); //在窗口中生成takeoff控件
                        }
                        if(type=="Hover"){
                            scene->CreateHover(point,id); //在窗口中生成takeoff控件
                        }
                        if(type=="Delay"){
                            scene->CreateDelay(point,id); //在窗口中生成takeoff控件
                        }
                    }catch(exception e){
                       ;
                    }
                }
            //}
            if(stream.name().toString()=="VAR"){
                type = stream.attributes().value("type").toString();
                qDebug()<<"type: "<<type;
                stream.readNext();
                stream.readNext();
                if(stream.name().toString()=="id"){
                    id = stream.readElementText().toInt();
                    qDebug()<<"id: "<<id;
                }
                stream.readNext();
                stream.readNext();
                if(stream.name().toString()=="location_x"){
                    location_x = stream.readElementText().toInt();
                    qDebug()<<"location_x: "<<location_x;
                }
                stream.readNext();
                stream.readNext();
                if(stream.name().toString()=="location_y"){
                    location_y = stream.readElementText().toInt();
                    qDebug()<<"location_y: "<<location_y;
                }

                QPointF point(location_x,location_y);
                try{
                    if(type=="VarType"){
                        scene->CreateVarType(point,id); //在窗口中生成takeoff控件
                    }
                    if(type=="VarDef"){
                        //stream.readNext();
                        //stream.readNext();
                        //if(stream.name().toString()=="data_type")
                            //CreateVarDef(point,id,data_type);
                        scene->CreateVarDef(point,id); //在窗口中生成VarDef控件
                    }
                }catch(exception e){
                   ;
                }
            }
            if(stream.name().toString()=="Compute"){
                type = stream.attributes().value("type").toString();
                qDebug()<<"type: "<<type;
                stream.readNext();
                stream.readNext();
                if(stream.name().toString()=="id"){
                    id = stream.readElementText().toInt();
                    qDebug()<<"id: "<<id;
                }
                stream.readNext();
                stream.readNext();
                if(stream.name().toString()=="location_x"){
                    location_x = stream.readElementText().toInt();
                    qDebug()<<"location_x: "<<location_x;
                }
                stream.readNext();
                stream.readNext();
                if(stream.name().toString()=="location_y"){
                    location_y = stream.readElementText().toInt();
                    qDebug()<<"location_y: "<<location_y;
                }

                QPointF point(location_x,location_y);
                try{
                    if(type=="Compute"){
                        scene->CreateCompute(point,id); //在窗口中生成compute控件
                    }
                }catch(exception e){
                   ;
                }
            }
            if(stream.name().toString()=="IO"){
                type = stream.attributes().value("type").toString();
                qDebug()<<"type: "<<type;
                stream.readNext();
                stream.readNext();
                if(stream.name().toString()=="id"){
                    id = stream.readElementText().toInt();
                    qDebug()<<"id: "<<id;
                }
                stream.readNext();
                stream.readNext();
                if(stream.name().toString()=="location_x"){
                    location_x = stream.readElementText().toInt();
                    qDebug()<<"location_x: "<<location_x;
                }
                stream.readNext();
                stream.readNext();
                if(stream.name().toString()=="location_y"){
                    location_y = stream.readElementText().toInt();
                    qDebug()<<"location_y: "<<location_y;
                }

                QPointF point(location_x,location_y);
                try{
                    if(type=="IO"){
                        scene->CreateIO(point,id); //在窗口中生成IO控件
                    }
                }catch(exception e){
                   ;
                }
            }
            if(stream.name().toString()=="Logic"){
                type = stream.attributes().value("type").toString();
                qDebug()<<"type: "<<type;
                stream.readNext();
                stream.readNext();
                if(stream.name().toString()=="id"){
                    id = stream.readElementText().toInt();
                    qDebug()<<"id: "<<id;
                }
                stream.readNext();
                stream.readNext();
                if(stream.name().toString()=="location_x"){
                    location_x = stream.readElementText().toInt();
                    qDebug()<<"location_x: "<<location_x;
                }
                stream.readNext();
                stream.readNext();
                if(stream.name().toString()=="location_y"){
                    location_y = stream.readElementText().toInt();
                    qDebug()<<"location_y: "<<location_y;
                }

                QPointF point(location_x,location_y);
                try{
                    if(type=="Logic"){
                        scene->CreateLogic(point,id); //在窗口中生成IO控件
                    }
                }catch(exception e){
                   ;
                }
            }
        }
     }


    file.close();
    return true;

}


/*****************************************************
 * Function name: save_pyfile
 * Description: This is the funtion that save topological
 *    widgets in python code.
 * Calle: widget_convert_to_py()
 * Input: QString filename = "Perform.py" - the python file name
 * Output: true - to tell if saving is successful
 *****************************************************/
bool format::save_pyfile(QString filename = "Perform.py")
{
    typename QMap<QString, widget>::iterator iter;
    QFile file("E:/QT_project/workspace/" + filename);
    QTextStream in(&file);

    for(iter=Map.begin();iter!=Map.end();iter++){
        //iter->
        widget_convert_to_py(iter, in);
    }

    file.close();
    return true;
}


/*****************************************************
 * Function name: widget_convert_to_py
 * Description: This is the funtion that saves the python code
 *    based on one widget.
 * Calle: none
 * Input: QFile* file - the python file
 *        widget* w - the widget to be saved
 * Output: none
 *****************************************************/

void format::widget_convert_to_py(QMap<QString, widget>::iterator& iter, QTextStream& stream)
{
    /*
     * 判断传入的控件是什么
     * -判断传入的控件有没有in
     * --有的话从SSmap调用名字对应的变量名
     * -判断传入的控件是否有out
     * -根据控件的类别和参数形成代码字符，写入file
    if(iter->key->name.compare(0,2,"VAR") == 0){    //如果传入的控件是VAR

    }

    if(iter->key->name.compare(0,1,"IO") == 0){    //如果传入的控件是IO

    }

    if(iter->key->name.compare(0,6,"Action") == 0){    //如果传入的控件是Action

    }

    if(iter->key->name.compare(0,6,"Compute") == 0){    //如果传入的控件是Compute

    }

    if(iter->key->name.compare(0,4,"Logic") == 0){    //如果传入的控件是Logic

    }

    if(iter->value->yuan->myLinks->mytoyuan != nullptr){    //有后置控件

    }
*/
}

/*
bool format::CreateVarDef(QPoint point, int id, QString data_type, int num)
{
    WidgetWrap tmp = map_instrument::find(Map,data_type);
    VardefNode* vdn = new VardefNode();
    vdn->node = tmp.mVarTypeNode;   //使vardefnode知道它属于varnode
    vdn->node->array[node->num]->node=node;

        (node->array[node->num])->setPos(point);
        VardefNode* vdnode = node->array[node->num];    //在这里记录VarDef，最后包装、添加到map
        node->flags[node->num]=true;
        this->addItem(node->array[node->num]);
        node->num=node->num%6+1;

        vdnode->controlsId=id;
        vdnode->identifier="VarDef";
        QString cid = QString::number(vdnode->controlsId,10);
        vdnode->name = vdnode->identifier + cid;
        qDebug()<<"Create():";
        qDebug()<<"name :"<<vdnode->name;
        qDebug()<<"identifier :"<<vdnode->identifier;
        qDebug()<<"controlsId :"<<vdnode->controlsId;
        //qDebug()<<"location_x :"<<QString::number((long)vdnode->pos().x(),10);
        //qDebug()<<"location_y :"<<QString::number((long)vdnode->pos().y(),10);

        WidgetWrap tmp(vdnode);   //包装节点
        wm->add(tmp);            //添加到widgetmap中
}
*/



