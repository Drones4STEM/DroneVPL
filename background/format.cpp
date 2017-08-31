
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
format::format()
{
    ;
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
    QMap<QString, widget*>::iterator iter;
    QFile file(filename);
    file.open(QIODevice::WriteOnly);
    QXmlStreamWriter stream(&file);

    stream.setAutoFormatting(true);
    stream.writeStartDocument("1.0");
    stream.writeStartElement("WidgetsAndLinks");
    //这个标签本身没有什么意义，但是在读取xml文件的时候需要有一个总标签包含所有子标签，所以用它占位

    for(iter=Map.begin(); iter!=Map.end(); iter++){   //先存VarType
        if(iter.value()->identifier=="VarType")
            widget_convert_to_xml(iter,stream);
    }
    for(iter=Map.begin(); iter!=Map.end(); iter++){   //存剩下的控件
        if(iter.value()->identifier!="VarType" && iter.value()->identifier!="Link")
            widget_convert_to_xml(iter,stream);
    }
    for(iter=Map.begin(); iter!=Map.end(); iter++){   //存剩下的控件
        if(iter.value()->identifier=="Link")
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

void format::widget_convert_to_xml(QMap<QString, widget*>::iterator& iter, QXmlStreamWriter& stream)
{
    /* 类别 category
     * 型号 type
     * 名字 name
     * 坐标 location
     */
    WidgetWrap* ww = iter.value();
    QString identifier = iter.value()->identifier;
    QString controlsId = QString::number(iter.value()->controlsId);
    QString x,y;
    //---------Action------------------
    if( ww->category == "Action" ){
        //stream.writeStartElement("widget");
        //stream.writeAttribute("category", "Action");

        stream.writeStartElement("Action");
        stream.writeAttribute("type",identifier);
        stream.writeTextElement("id",controlsId);
        if(identifier == "TakeOff"){ //起飞动作
             x = QString::number((long)ww->mTakeOffNode->pos().x(),10);
             stream.writeTextElement("location_x",x);
             y = QString::number((long)ww->mTakeOffNode->pos().y(),10);
             stream.writeTextElement("location_y",y);
             //stream.writeStartElement("arrow_out");
        }
        if(identifier == "Land"){ //降落动作
            x = QString::number((long)ww->mLandNode->pos().x(),10);
            stream.writeTextElement("location_x",x);
            y = QString::number((long)ww->mLandNode->pos().y(),10);
            stream.writeTextElement("location_y",y);
        }
        if(identifier == "Go"){ //六个方向的直线移动动作
            x = QString::number((long)ww->mGoNode->pos().x(),10);
            stream.writeTextElement("location_x",x);
            y = QString::number((long)ww->mGoNode->pos().y(),10);
            stream.writeTextElement("location_y",y);
        }
        if(identifier == "Turn"){ //转向动作
            x = QString::number((long)ww->mTurnNode->pos().x(),10);
            stream.writeTextElement("location_x",x);
            y = QString::number((long)ww->mTurnNode->pos().y(),10);
            stream.writeTextElement("location_y",y);
        }
        if(identifier == "Hover"){ //悬停动作
            x = QString::number((long)ww->mHoverNode->pos().x(),10);
            stream.writeTextElement("location_x",x);
            y = QString::number((long)ww->mHoverNode->pos().y(),10);
            stream.writeTextElement("location_y",y);
        }
        if(identifier == "Delay"){ //延时动作
            x = QString::number((long)ww->mDelayNode->pos().x(),10);
            stream.writeTextElement("location_x",x);
            y = QString::number((long)ww->mDelayNode->pos().y(),10);
            stream.writeTextElement("location_y",y);
        }

        qDebug()<<"category: "<<ww->category;
        qDebug()<<"type: "<<identifier;
        qDebug()<<"id: "<<controlsId;
        qDebug()<<"location_x: "<<x;
        qDebug()<<"location_y: "<<y;
        //stream.writeEndElement();
        stream.writeEndElement();   //correspond to writeStartElement("Action")
    }
    //--------VAR--------------
    if( ww->category == "VAR" ){    //是VAR控件
        stream.writeStartElement("VAR");
        stream.writeAttribute("type",identifier);
        stream.writeTextElement("id",controlsId);
        if(identifier == "VarType"){
            x = QString::number((long)ww->mVarTypeNode->pos().x(),10);
            stream.writeTextElement("location_x",x);
            y = QString::number((long)ww->mVarTypeNode->pos().y(),10);
            stream.writeTextElement("location_y",y);
            //stream.writeTextElement("data_type",iter->);
            //stream.writeStartElement("arrow_out");
        }
        if(identifier == "VarDef"){
            x = QString::number((long)ww->mVarDefNode->pos().x(),10);
            stream.writeTextElement("location_x",x);
            y = QString::number((long)ww->mVarDefNode->pos().y(),10);
            stream.writeTextElement("location_y",y);
            if(ww->mVarDefNode->node!=0){
                stream.writeTextElement("data_type",ww->mVarDefNode->node->name);
            }else{
                QString s = "none";
                stream.writeTextElement("data_type",s);
            }
            QString seq;
            seq = QString::number((long)ww->mVarDefNode->seq,10);
            stream.writeTextElement("sequence",seq);
            //stream.writeStartElement("arrow_out");
        }
        stream.writeEndElement();   //correspond to writeStartElement("VAR")
        qDebug()<<"category: "<<ww->category;
        qDebug()<<"type: "<<identifier;
        qDebug()<<"id: "<<controlsId;
        qDebug()<<"location_x: "<<x;
        qDebug()<<"location_y: "<<y;
    }
    //---------Compute------------------
    if(ww->category == "Compute"){
        stream.writeStartElement("Compute");
        stream.writeAttribute("type",identifier);
        stream.writeTextElement("id",controlsId);
        if(identifier == "Compute"){
            x = QString::number((long)ww->mComputeNode->pos().x(),10);
            stream.writeTextElement("location_x",x);
            y = QString::number((long)ww->mComputeNode->pos().y(),10);
            stream.writeTextElement("location_y",y);
            //stream.writeStartElement("arrow_out");
        }
        stream.writeStartElement("math");
        if(ww->mComputeNode->box->currentText()=="+"){ //如果加法
            stream.writeAttribute("operator","add");
        }
        stream.writeEndElement();
        stream.writeEndElement();   //correspond to writeStartElement("Compute")
        qDebug()<<"category: "<<ww->category;
        qDebug()<<"type: "<<identifier;
        qDebug()<<"id: "<<controlsId;
        qDebug()<<"location_x: "<<x;
        qDebug()<<"location_y: "<<y;
        qDebug()<<"operator: "<<ww->mComputeNode->box->currentText();
    }
    //---------IO------------------
    if(ww->category == "IO"){
        stream.writeStartElement("IO");
        stream.writeAttribute("type",identifier);
        stream.writeTextElement("id",controlsId);
        if(identifier == "Battery"){
            x = QString::number((long)ww->mBatteryNode->pos().x(),10);
            stream.writeTextElement("location_x",x);
            y = QString::number((long)ww->mBatteryNode->pos().y(),10);
            stream.writeTextElement("location_y",y);
            //stream.writeStartElement("arrow_out");
        }
        if(identifier == "Gimbal"){
            x = QString::number((long)ww->mGimbalNode->pos().x(),10);
            stream.writeTextElement("location_x",x);
            y = QString::number((long)ww->mGimbalNode->pos().y(),10);
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
        qDebug()<<"category: "<<ww->category;
        qDebug()<<"type: "<<identifier;
        qDebug()<<"id: "<<controlsId;
        qDebug()<<"location_x: "<<x;
        qDebug()<<"location_y: "<<y;
    }
    //---------LOGIC------------------
    if(ww->category == "Logic"){
        stream.writeStartElement("Logic");
        stream.writeAttribute("type",identifier);
        stream.writeTextElement("id",controlsId);
        if(identifier == "Logic"){
            x = QString::number((long)ww->mLogicNode->pos().x(),10);
            stream.writeTextElement("location_x",x);
            y = QString::number((long)ww->mLogicNode->pos().y(),10);
            stream.writeTextElement("location_y",y);
            //stream.writeStartElement("arrow_out");
        }
        stream.writeEndElement();   //correspond to writeStartElement("Logic")
        qDebug()<<"category: "<<ww->category;
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
    if(ww->category == "Link"){
        stream.writeStartElement("Link");
        stream.writeAttribute("type",identifier);
        stream.writeTextElement("id",controlsId);
        if(identifier == "Link"){ //连线
             x = QString::number((long)ww->mLinkNode->pos().x(),10);
             stream.writeTextElement("location_x",x);
             y = QString::number((long)ww->mLinkNode->pos().y(),10);
             stream.writeTextElement("location_y",y);
             QString from = ww->mLinkNode->from_master_name();
             QString to = ww->mLinkNode->to_master_name();
             QString fyuan = ww->mLinkNode->fromYuan()->name;
             QString tyuan = ww->mLinkNode->toYuan()->name;
             stream.writeStartElement("from");
             stream.writeTextElement("widget",from);
             stream.writeTextElement("yuan",fyuan);
             stream.writeEndElement();
             stream.writeStartElement("to");
             stream.writeTextElement("widget",to);
             stream.writeTextElement("yuan",tyuan);
             stream.writeEndElement();

        }
        qDebug()<<"category: "<<ww->category;
        qDebug()<<"type: "<<identifier;
        qDebug()<<"id: "<<controlsId;
        qDebug()<<"location_x: "<<x;
        qDebug()<<"location_y: "<<y;
        stream.writeEndElement();   //correspond to writeStartElement("Link")
    }
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
    QString category,type;
    qint16 location_x,location_y,id;

    //qDebug()<<stream.hasError();
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
                            CreateTakeOff(point,id); //在窗口中生成takeoff控件
                        }
                        if(type=="Land"){
                            CreateLand(point,id); //在窗口中生成takeoff控件
                        }
                        if(type=="Go"){
                            CreateGo(point,id); //在窗口中生成takeoff控件
                        }
                        if(type=="Turn"){
                            CreateTurn(point,id); //在窗口中生成takeoff控件
                        }
                        if(type=="Hover"){
                            CreateHover(point,id); //在窗口中生成takeoff控件
                        }
                        if(type=="Delay"){
                            CreateDelay(point,id); //在窗口中生成takeoff控件
                        }
                    }catch(exception e){
                       ;
                    }
                }
            //}
            if(stream.name().toString()=="VAR"){
                qDebug()<<"read frame file():";
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
                        CreateVarType(point,id); //在窗口中生成takeoff控件
                    }
                    if(type=="VarDef"){
                        stream.readNext();
                        stream.readNext();
                        if(stream.name().toString()=="data_type"){
                            QString name = stream.readElementText();
                            stream.readNext();  stream.readNext();
                            QString seq = stream.readElementText();
                            int s = seq.toInt();
                            qDebug()<<"data_type: "<<name;
                            qDebug()<<"sequence: "<<seq;
                            CreateVarDef(point,id,name,s); //在窗口中生成VarDef控件
                        }
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
                        CreateCompute(point,id); //在窗口中生成compute控件
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
                    if(type=="Battery"){
                        CreateBattery(point,id); //在窗口中生成IO控件
                    }
                    if(type=="Gimbal"){
                        CreateGimbal(point,id); //在窗口中生成IO控件
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
                        CreateLogic(point,id); //在窗口中生成IO控件
                    }
                }catch(exception e){
                   ;
                }
            }
            if(stream.name().toString()=="Link"){
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
                stream.readNext();  stream.readNext();
                QString from,fyuan;
                if(stream.name().toString()=="from"){
                    stream.readNext();  stream.readNext();
                    if(stream.name().toString()=="widget")
                        from = stream.readElementText();
                    stream.readNext();  stream.readNext();
                    if(stream.name().toString()=="yuan")
                        fyuan = stream.readElementText();
                    qDebug()<<"from: "<<from<<" "<<fyuan;
                }
                stream.readNextStartElement();
                stream.readNextStartElement();
                QString to,tyuan;
                if(stream.name().toString()=="to"){
                    stream.readNext();  stream.readNext();
                    if(stream.name().toString()=="widget")
                        to = stream.readElementText();
                    stream.readNext();  stream.readNext();
                    if(stream.name().toString()=="yuan")
                        tyuan = stream.readElementText();
                    qDebug()<<"to: "<<to<<" "<<tyuan;
                }
                QPointF point(location_x,location_y);
                if(type=="Link"){
                    CreateLink(point,id,from,to,fyuan,tyuan); //在窗口中生成IO控件
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
bool format::save_py_file(std::stack<widget*>* stk, QTextStream& in)
{
    WidgetWrap* tmp = new widget();

    while(!stk->empty()){
        tmp = stk->top();    stk->pop();
        if(tmp->identifier=="Logic"){
            std::stack<widget*>* s = digrapher->get_topology(tmp->mLogicNode);
            save_py_file(s,in);
        }else{  //Logic转化成代码的过程不在这里进行
            widget_convert_to_py(tmp, in);
        }

    }


    return true;
}


/*****************************************************
 * Function name: SavePyfile
 * Description: This is the funtion that save topological
 *    widgets in python code.
 * Calle: widget_convert_to_py()
 * Input: QString filename = "Perform.py" - the python file name
 * Output: true - to tell if saving is successful
 *****************************************************/
bool format::SavePyFile(QString filename)
{

    QFile file(filename);
    if(file.exists()){
        qDebug()<<"fiel exists";
        file.open(QIODevice::WriteOnly);
        QTextStream tmp(&file);
        tmp<<"";
        qDebug()<<"file remove "<<file.remove();
    }
    qDebug()<<"file copy "
            <<QFile::copy("../DroneVPL/pycode/prepare.py",filename);

    qDebug()<<filename;
    file.open(QIODevice::Append);
    QTextStream in(&file);
    in<<"\n\n\n#========specified code=============\n";

    QMap<QString, widget*>* m = &(Map);
    std::stack<widget*>* stk = digrapher->get_topology(0);
    save_py_file(stk,in);

    in<<"\nmyCopter.exit()\n";
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

void format::widget_convert_to_py(WidgetWrap* w, QTextStream& stream)
{

    if(w->identifier=="VarType"){    //如果传入的控件是VAR
        qDebug()<<"format::widget_convert_to_py()\n"<<w->name;
    }

    if(w->identifier=="Battery"){    //如果传入的控件是Battery
        qDebug()<<"format::widget_convert_to_py()\n"<<w->name;
    }

    if(w->identifier=="Gimbal"){    //如果传入的控件是Battery
        qDebug()<<"format::widget_convert_to_py()\n"<<w->name;
    }

    if(w->identifier=="TakeOff"){    //如果传入的控件是Action
        qDebug()<<"format::widget_convert_to_py()\n"<<w->name;
        stream<<"if not myCopter.takeoff(1):\n"
              <<"   sys.exit(1)\n";
    }
    if(w->identifier=="Land"){    //如果传入的控件是Action
        qDebug()<<"format::widget_convert_to_py()\n"<<w->name;
        stream<<"timeoutCounter = 0\n"
              <<"while not myCopter.land():\n"
              <<"    timeoutCounter += 1\n"
              <<"    if timeoutCounter > 3:\n"
              <<"        print \"Critical: Cannot land the vehicle after 3 retries.\"\n"
              <<"        sys.exit(1)";
    }
    if(w->identifier=="Go"){    //如果传入的控件是Action
        qDebug()<<"format::widget_convert_to_py()\n"<<w->name;
        if(w->mGoNode->direction=="GoUp"){
            stream<<"print \"Going upward for 0.2m/s for 5 seconds\""
                  <<"myCopter.send_nav_velocity(0, 0, -0.2)"
                  <<"time.sleep(5)";
        }
        if(w->mGoNode->direction=="GoDown"){
            stream<<"print \"Going upward for 0.2m/s for 5 seconds\""
                  <<"myCopter.send_nav_velocity(0, 0, 0.2)"
                  <<"time.sleep(5)";
        }
        if(w->mGoNode->direction=="GoRight"){
            stream<<"print \"Going rightward at 1m/s for 5s\""
                  <<"myCopter.send_nav_velocity(1, 0, 0)"
                  <<"time.sleep(5)";
        }
        if(w->mGoNode->direction=="GoLeft"){
            stream<<"print \"Going rightward at 1m/s for 5s\""
                  <<"myCopter.send_nav_velocity(-1, 0, 0)"
                  <<"time.sleep(5)";
        }
        if(w->mGoNode->direction=="Forward"){
            stream<<"print \"Going forward at 1m/s for 5s\""
                  <<"myCopter.send_nav_velocity(0, -1, 0)"
                  <<"time.sleep(5)";
        }
        if(w->mGoNode->direction=="Backward"){
            stream<<"Going forward at 1m/s for 5s\""
                  <<"myCopter.send_nav_velocity(0, 1, 0)"
                  <<"time.sleep(5)";
        }
    }
    if(w->identifier=="Turn"){    //如果传入的控件是Action
        qDebug()<<"format::widget_convert_to_py()\n"<<w->name;
        //turn目前没有要对应的代码

    }
    if(w->identifier=="Hover"){    //如果传入的控件是Action
        qDebug()<<"format::widget_convert_to_py()\n"<<w->name;
        stream<<"time.sleep(5)\n";
    }
    if(w->identifier=="Delay"){    //如果传入的控件是Action
        qDebug()<<"format::widget_convert_to_py()\n"<<w->name;
        //目前没有要对应的代码
    }

    if(w->identifier=="Compute"){    //如果传入的控件是Compute
        qDebug()<<"format::widget_convert_to_py()\n"<<w->name;
    }

    if(w->identifier=="Logic"){    //如果传入的控件是Logic
        //理论上讲Logic不会被传到这个函数里执行
        qDebug()<<"format::widget_convert_to_py()\n"<<w->name<<"(if you see logic is print like now,then this is a bug)";
    }
/*
    if(iter->value->yuan->myLinks->mytoyuan != nullptr){    //有后置控件

    }
*/
}



/*******************************************************************************************
 * Function name: creat……()
 * Description: This is the description for the following functions.
 *              Following funtions are used to create widgets that satisfy the xml file request,
 *              but they only set properites except graphic ones. To draw these widgets, scene
 *              will help. The widgets created here are saved in a map and convey to widgetmap
 *              of scene, thus scene can draw widgets according to those properties.
 * Calle: none
 * Input: QPointF point - widget location
 *        int id - widget id
 * Output: bool
 *******************************************************************************************/
bool format::CreateTakeOff(QPointF point,int id)
{
    TakeOffNode *node=new TakeOffNode;
    node->lx = point.x();
    node->ly = point.y();
    node->controlsId=id;
    node->identifier="TakeOff";
    QString cid = QString::number(node->controlsId,10);
    node->name = node->identifier + cid;

    WidgetWrap* tmp = new WidgetWrap(node);   //包装节点
    node->yuan->master = tmp;
    Map.insert(tmp->name,tmp);            //添加到widgetmap中
    return true;
}

bool format::CreateLand(QPointF point, int id)
{
    LandNode *node=new LandNode;
    node->lx = point.x();
    node->ly = point.y();
    node->controlsId=id;
    node->identifier="Land";
    QString cid = QString::number(node->controlsId,10);
    node->name = node->identifier + cid;

    WidgetWrap* tmp = new WidgetWrap(node);   //包装节点
    node->yuan2->master = tmp;
    Map.insert(tmp->name,tmp);            //添加到widgetmap中
    return true;
}

bool format::CreateGo(QPointF point, int id)
{
    GoNode *node=new GoNode;
    node->lx = point.x();
    node->ly = point.y();
    node->controlsId=id;
    node->identifier="Go";
    QString cid = QString::number(node->controlsId,10);
    node->name = node->identifier + cid;
    qDebug()<<"Create():";
    qDebug()<<"name :"<<node->name;
    qDebug()<<"identifier :"<<node->identifier;
    qDebug()<<"controlsId :"<<node->controlsId;

    WidgetWrap* tmp = new WidgetWrap(node);   //包装节点
    Map.insert(tmp->name,tmp);            //添加到widgetmap中

}

bool format::CreateTurn(QPointF point, int id)
{
    TurnNode *node=new TurnNode;

    node->lx = point.x();
    node->ly = point.y();
    node->controlsId=id;
    node->identifier="Turn";
    QString cid = QString::number(node->controlsId,10);
    node->name = node->identifier + cid;
    qDebug()<<"Create():";
    qDebug()<<"name :"<<node->name;
    qDebug()<<"identifier :"<<node->identifier;
    qDebug()<<"controlsId :"<<node->controlsId;

    WidgetWrap* tmp = new WidgetWrap(node);   //包装节点
    Map.insert(tmp->name,tmp);            //添加到widgetmap中
}

bool format::CreateHover(QPointF point, int id)
{
    HoverNode *node=new HoverNode;
    node->lx = point.x();
    node->ly = point.y();
    node->controlsId=id;
    node->identifier="Hover";
    QString cid = QString::number(node->controlsId,10);
    node->name = node->identifier + cid;
    qDebug()<<"Create():";
    qDebug()<<"name :"<<node->name;
    qDebug()<<"identifier :"<<node->identifier;
    qDebug()<<"controlsId :"<<node->controlsId;

    WidgetWrap* tmp = new WidgetWrap(node);   //包装节点
    Map.insert(tmp->name,tmp);            //添加到widgetmap中
    return true;
}

bool format::CreateDelay(QPointF point, int id)
{
    DelayNode *node=new DelayNode;

    node->lx = point.x();
    node->ly = point.y();
    node->controlsId=id;
    node->identifier="Delay";
    QString cid = QString::number(node->controlsId,10);
    node->name = node->identifier + cid;
    qDebug()<<"Create():";
    qDebug()<<"name :"<<node->name;
    qDebug()<<"identifier :"<<node->identifier;
    qDebug()<<"controlsId :"<<node->controlsId;

    WidgetWrap* tmp = new WidgetWrap(node);   //包装节点
    Map.insert(tmp->name,tmp);            //添加到widgetmap中
    return true;
}

bool format::CreateVarType(QPointF point, int id)
{
    VarNode* node=new VarNode;
    node->lx = point.x();
    node->ly = point.y();

    node->controlsId=id;
    node->identifier="VarType";
    QString cid = QString::number(node->controlsId,10);
    node->name = node->identifier + cid;
    qDebug()<<"format::Create():";
    qDebug()<<"name :"<<node->name;
    qDebug()<<"identifier :"<<node->identifier;
    qDebug()<<"controlsId :"<<node->controlsId;

    WidgetWrap* tmp = new WidgetWrap(node);   //包装节点
    Map.insert(tmp->name,tmp);            //添加到widgetmap中
    return true;
}
/*********
 * QString name - the associated VarType name.
 * int num - the VarDef rank num in all the VarDef widgets of VarType
 ********/
bool format::CreateVarDef(QPointF point, int id, QString name, int seq)//varnode内部有个成员叫num，所以形参不能和它重名
{
    VardefNode *vdn = new VardefNode;
    VarNode *vn = new VarNode;
    vdn->lx = point.x();
    vdn->ly = point.y();
    if(name!="none" && seq!=-1){
        qDebug()<<"3";
        QMap<QString,WidgetWrap*>* m = &Map;
        qDebug()<<"2";
        WidgetWrap t = map_instrument::find(m,name);
        if(t.mVarTypeNode==0) qDebug()<<"mVarTypeNode==0";
        qDebug()<<"0";
        vdn->node = t.mVarTypeNode;   //使vardefnode知道它属于varnode
        qDebug()<<"1";
        vdn->seq = seq;
        qDebug()<<"4";
        vn = vdn->node;
        qDebug()<<"5";
        vn->array[seq]=vdn; //使varnode知道属于它的vardefnode
        qDebug()<<"6";
        vn->flags[seq]=true;
    }else{
        vdn->node = 0;
        vdn->seq = -1;
    }

    vdn->controlsId=id;
    vdn->identifier="VarDef";
    QString cid = QString::number(vdn->controlsId,10);
    vdn->name = vdn->identifier + cid;
    qDebug()<<"format::Create():";
    qDebug()<<"name :"<<vdn->name;
    qDebug()<<"identifier :"<<vdn->identifier;
    qDebug()<<"controlsId :"<<vdn->controlsId;

    WidgetWrap* tmp = new WidgetWrap(vdn);   //包装节点
    Map.insert(tmp->name,tmp);            //添加到widgetmap中
    return true;
}

bool format::CreateCompute(QPointF point, int id)
{
    ComputeNode *node=new ComputeNode;
    node->lx = point.x();
    node->ly = point.y();

    node->controlsId=id;
    node->identifier="Compute";
    QString cid = QString::number(node->controlsId,10);
    node->name = node->identifier + cid;
    qDebug()<<"Create():";
    qDebug()<<"name :"<<node->name;
    qDebug()<<"identifier :"<<node->identifier;
    qDebug()<<"controlsId :"<<node->controlsId;

    WidgetWrap* tmp = new WidgetWrap(node);   //包装节点
    Map.insert(tmp->name,tmp);            //添加到widgetmap中
    return true;
}

bool format::CreateIO(QPointF point, int id)
{
    IoNode* node=new IoNode;
    node->lx = point.x();
    node->ly = point.y();

    node->controlsId=id;
    node->identifier="IO";
    QString cid = QString::number(node->controlsId,10);
    node->name = node->identifier + cid;
    qDebug()<<"Create():";
    qDebug()<<"name :"<<node->name;
    qDebug()<<"identifier :"<<node->identifier;
    qDebug()<<"controlsId :"<<node->controlsId;

    WidgetWrap* tmp = new WidgetWrap(node);   //包装节点
    Map.insert(tmp->name,tmp);            //添加到widgetmap中
    return true;
}

bool format::CreateBattery(QPointF point, int id)
{
    BatteryNode* node=new BatteryNode;
    node->lx = point.x();
    node->ly = point.y();

    node->controlsId=id;
    node->identifier="Battery";
    QString cid = QString::number(node->controlsId,10);
    node->name = node->identifier + cid;
    qDebug()<<"Create():";
    qDebug()<<"name :"<<node->name;
    qDebug()<<"identifier :"<<node->identifier;
    qDebug()<<"controlsId :"<<node->controlsId;

    WidgetWrap* tmp = new WidgetWrap(node);   //包装节点
    Map.insert(tmp->name,tmp);            //添加到widgetmap中
    return true;
}

bool format::CreateGimbal(QPointF point, int id)
{
    GimbalNode* node=new GimbalNode;
    node->lx = point.x();
    node->ly = point.y();

    node->controlsId=id;
    node->identifier="Gimbal";
    QString cid = QString::number(node->controlsId,10);
    node->name = node->identifier + cid;
    qDebug()<<"Create():";
    qDebug()<<"name :"<<node->name;
    qDebug()<<"identifier :"<<node->identifier;
    qDebug()<<"controlsId :"<<node->controlsId;

    WidgetWrap* tmp = new WidgetWrap(node);   //包装节点
    Map.insert(tmp->name,tmp);            //添加到widgetmap中
    return true;
}

bool format::CreateLogic(QPointF point, int id)
{
    Rec *node=new Rec;
    node->lx = point.x();
    node->ly = point.y();

    node->controlsId=id;
    node->identifier="Logic";
    QString cid = QString::number(node->controlsId,10);
    node->name = node->identifier + cid;
    qDebug()<<"Create():";
    qDebug()<<"name :"<<node->name;
    qDebug()<<"identifier :"<<node->identifier;
    qDebug()<<"controlsId :"<<node->controlsId;

    WidgetWrap* tmp = new WidgetWrap(node);   //包装节点
    Map.insert(tmp->name,tmp);            //添加到widgetmap中
    return true;
}

bool format::CreateLink(QPointF point, int id, QString from, QString to, QString fyuan, QString tyuan)
{
    QMap<QString,WidgetWrap*>* m = &Map;
    WidgetWrap wfrom = map_instrument::find(m,from);
    WidgetWrap wto = map_instrument::find(m,to);
    Yuan *first,*second;
    if(wfrom.identifier == "TakeOff"){
        first = dynamic_cast<Yuan*> (wfrom.mTakeOffNode->yuan);//triYuan->Yuan
    }
    if(wfrom.identifier == "Land"){
        //Land不可能是指出的点
    }
    if(wfrom.identifier == "Go"){
        first = dynamic_cast<Yuan*> (wfrom.mGoNode->yuan);
    }
    if(wfrom.identifier == "Turn"){
        first = dynamic_cast<Yuan*> (wfrom.mTurnNode->yuan);
    }
    if(wfrom.identifier == "Hover"){
        first = dynamic_cast<Yuan*> (wfrom.mHoverNode->yuan);
    }
    if(wfrom.identifier == "Delay"){
        first = dynamic_cast<Yuan*> (wfrom.mDelayNode->yuan);
    }
    if(wfrom.identifier == "VarType"){
    }
    if(wfrom.identifier == "VarDef"){
        first = dynamic_cast<Yuan*> (wfrom.mVarDefNode->yuan);
    }
    if(wfrom.identifier == "Compute"){
        first = dynamic_cast<Yuan*> (wfrom.mComputeNode->yuan);
    }
    if(wfrom.identifier == "IO"){
        if(fyuan == "n1yuan")
            first = dynamic_cast<Yuan*> (wfrom.mIONode->node1->yuan);
        else if(fyuan == "n2yuan")
            first = dynamic_cast<Yuan*> (wfrom.mIONode->node2->yuan);
        else if(fyuan == "n3yuan")
            first = dynamic_cast<Yuan*> (wfrom.mIONode->node3->yuan);
        else if(fyuan == "yuan")
            first = dynamic_cast<Yuan*> (wfrom.mIONode->yuan);
    }
    if(wfrom.identifier == "Logic"){
        //Logci不可能是指出的点，if和else的情况暂时不算
    }
    if(wto.identifier == "TakeOff"){
        //TakeOff不可能是指入的点
    }
    if(wto.identifier == "Land"){
        second = wto.mLandNode->yuan2;
    }
    if(wto.identifier == "Go"){
        second = wto.mGoNode->yuan2;
    }
    if(wto.identifier == "Turn"){
        second = wto.mTurnNode->yuan2;
    }
    if(wto.identifier == "Hover"){
        second = wto.mHoverNode->yuan2;
    }
    if(wto.identifier == "Delay"){
        second = wto.mDelayNode->yuan2;
    }
    if(wto.identifier == "VarType"){
        //VarType没有指入和指出
    }
    if(wto.identifier == "VarDef"){
        second = wto.mVarDefNode->yuan2;
    }
    if(wto.identifier == "Compute"){
        if(tyuan == "yuan2")
            second = wto.mComputeNode->yuan2;
        else if(tyuan == "yuan3")
            second = wto.mComputeNode->yuan3;
    }
    if(wto.identifier == "IO"){
        second = wto.mIONode->yuan2;
    }
    if(wto.identifier == "Logic"){
        second = wto.mLogicNode->yuan2;
    }
    Link *link = new Link(first,second);

    link->controlsId = id;
    link->identifier = "Link";
    QString cid = QString::number(link->controlsId,10);
    link->name = link->identifier + cid;

    WidgetWrap* tmp = new WidgetWrap(link);   //包装节点
    Map.insert(tmp->name,tmp);            //添加到widgetmap中
    return true;
}


