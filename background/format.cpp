
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
             QString Altitude = QString::number((long)ww->mTakeOffNode->myAltitude(),10);
             stream.writeTextElement("Altitude",Altitude);
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
            QString GroundSpeed = QString::number((long)ww->mGoNode->myGroundSpeed(),10);
            stream.writeTextElement("GroudSpeed",GroundSpeed);
            QString Time = QString::number((long)ww->mGoNode->myTime(),10);
            stream.writeTextElement("Time",Time);
        }
        if(identifier == "Turn"){ //转向动作
            x = QString::number((long)ww->mTurnNode->pos().x(),10);
            stream.writeTextElement("location_x",x);
            y = QString::number((long)ww->mTurnNode->pos().y(),10);
            stream.writeTextElement("location_y",y);
            QString Angel = QString::number((long)ww->mTurnNode->myAngel(),10);
            stream.writeTextElement("Angel",Angel);
        }
        if(identifier == "Hover"){ //悬停动作
            x = QString::number((long)ww->mHoverNode->pos().x(),10);
            stream.writeTextElement("location_x",x);
            y = QString::number((long)ww->mHoverNode->pos().y(),10);
            stream.writeTextElement("location_y",y);
            QString Time = QString::number((long)ww->mHoverNode->myTime(),10);
            stream.writeTextElement("Time",Time);
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
            stream.writeTextElement("data_type",ww->mVarTypeNode->text());
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
            stream.writeTextElement("Variable",ww->mVarTypeNode->text());
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
        if(ww->mComputeNode->box->currentText()=="-"){ //如果加法
            stream.writeAttribute("operator","substract");
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
        }
        if(identifier == "Attitude"){
            x = QString::number((long)ww->mAttitudeNode->pos().x(),10);
            stream.writeTextElement("location_x",x);
            y = QString::number((long)ww->mAttitudeNode->pos().y(),10);
            stream.writeTextElement("location_y",y);
        }
        if(identifier == "Channel"){
            x = QString::number((long)ww->mChannelNode->pos().x(),10);
            stream.writeTextElement("location_x",x);
            y = QString::number((long)ww->mChannelNode->pos().y(),10);
            stream.writeTextElement("location_y",y);
        }
        if(identifier == "RangeFinder"){
            x = QString::number((long)ww->mRangeFinderNode->pos().x(),10);
            stream.writeTextElement("location_x",x);
            y = QString::number((long)ww->mRangeFinderNode->pos().y(),10);
            stream.writeTextElement("location_y",y);
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
    QString math;

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
                    if(type=="TakeOff"){
                        stream.readNext();
                        stream.readNext();
                        double Altitude;
                        if(stream.name().toString()=="Altitude"){
                            Altitude = stream.readElementText().toDouble();
                            qDebug()<<"Altitude: "<<Altitude;
                        }
                        CreateTakeOff(point,id,Altitude); //在窗口中生成takeoff控件
                    }
                    if(type=="Land"){
                        CreateLand(point,id); //在窗口中生成takeoff控件
                    }
                    if(type=="Go"){
                        stream.readNext();
                        stream.readNext();
                        double GroundSpeed;
                        if(stream.name().toString()=="GroudSpeed"){
                            GroundSpeed = stream.readElementText().toDouble();
                            qDebug()<<"GroudSpeed: "<<GroundSpeed;
                        }
                        stream.readNext();
                        stream.readNext();
                        double Time;
                        if(stream.name().toString()=="Time"){
                            Time = stream.readElementText().toDouble();
                            qDebug()<<"Time: "<<Time;
                        }
                        CreateGo(point,id,GroundSpeed,Time); //在窗口中生成takeoff控件
                    }
                    if(type=="Turn"){
                        stream.readNext();
                        stream.readNext();
                        double Angel;
                        if(stream.name().toString()=="Angel"){
                            Angel = stream.readElementText().toDouble();
                            qDebug()<<"Angel: "<<Angel;
                        }
                        CreateTurn(point,id,Angel); //在窗口中生成takeoff控件
                    }
                    if(type=="Hover"){
                        stream.readNext();
                        stream.readNext();
                        double Time;
                        if(stream.name().toString()=="Time"){
                            Time = stream.readElementText().toDouble();
                            qDebug()<<"Time: "<<Time;
                        }
                        CreateHover(point,id,Time); //在窗口中生成takeoff控件
                    }
                    if(type=="Delay"){
                        CreateDelay(point,id); //在窗口中生成takeoff控件
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
                stream.readNext();
                stream.readNext();
                if(stream.name().toString()=="math"){
                    math = stream.readElementText();
                    qDebug()<<"math: "<<math;
                }

                QPointF point(location_x,location_y);
                try{
                    if(type=="Compute"){
                        CreateCompute(point,id,math); //在map中生成compute控件
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
                        CreateBattery(point,id); //在map中生成IO控件
                    }
                    if(type=="Gimbal"){
                        CreateGimbal(point,id);
                    }
                    if(type=="Attitude"){
                        CreateAttitude(point,id);
                    }
                    if(type=="Channel"){
                        CreateChannel(point,id);
                    }
                    if(type=="RangeFinder"){
                        CreateRangeFinder(point,id);
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
bool format::save_py_file(std::stack<widget*>* stk, QTextStream& in, int tabs)
{
    WidgetWrap* tmp = new widget();
    while(!stk->empty()){
        tmp = stk->top();    stk->pop();
        if(tmp->identifier=="Logic"){
            widget_convert_to_py(tmp,in,tabs);
            std::stack<widget*>* s = digrapher->get_topology(tmp->mLogicNode);
            save_py_file(s,in,++tabs);
            tabs--;
        }else{  //Logic转化成代码的过程不在这里进行
            widget_convert_to_py(tmp, in, tabs);
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
    in<<"\n\n\nimport math\n";
    in<<"\n\n\n#========specified code=============\n";


    QMap<QString, widget*>* m = &(Map);
    std::stack<widget*>* stk = digrapher->get_topology(0);
    save_py_file(stk,in,0);

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

void format::widget_convert_to_py(WidgetWrap* w, QTextStream& stream, int tabs)
{

    if(w->identifier=="VarType"){    //如果传入的控件是VarType
        VarNode* tmp = w->mVarTypeNode;
        stream<<tmp->text()<<" ";
        int i = 0;
        if(i<=tmp->num)  stream<<tmp->array[i]->text();
        for(i=1;i<tmp->num;i++){
            stream<<","<<tmp->array[i]->text();
        }
        stream<<"\n";
        qDebug()<<"format::widget_convert_to_py()\n"<<w->name;
    }
    if(w->identifier=="VarDef"){
        VardefNode* tmp = w->mVarDefNode;
        QString s;
        ComputeNode* cn;
        if(tmp->myStringText!=""){
            s = tmp->myStringText;
        }
        if(!tmp->yuan->myLinks.isEmpty()){
            s = tmp->text() + "=" + tmp->myStringText;
            if(tmp->yuan->myLinks.toList()[0]->toYuan()->master->identifier=="Compute"){
                cn = tmp->yuan->myLinks.toList()[0]->toYuan()->master->mComputeNode;
                if(cn->yuan2==tmp->yuan->myLinks.toList()[0]->toYuan()){
                    cn->rect1text = "(" + s + ")";
                }else{
                    cn->rect2text = "(" + s + ")";
                }
            }else if(tmp->yuan->myLinks.toList()[0]->toYuan()->master->identifier=="VarDef"){
                VardefNode* vn = tmp->yuan->myLinks.toList()[0]->toYuan()->master->mVarDefNode;
                vn->myStringText = "(" + s + ")";
            }
        }else{
            for(int i=1;i<=tabs;i++) stream<<"   ";
            stream<<tmp->text()<<"="<<s<<"\n";
        }
    }

    if(w->identifier=="Battery"){    //如果传入的控件是Battery
        BatteryNode* tmp = w->mBatteryNode;
        if(tmp->node1!=0){
            for(int i=1;i<=tabs;i++) stream<<"   ";
            stream<<tmp->node1->yuan->myLinks.toList()[0]->fromYuan()->master->mVarDefNode->text()<<"=";
            stream<<"myCopter.battery.voltage\n";
        }
        if(tmp->node2!=0){
            for(int i=1;i<=tabs;i++) stream<<"   ";
            stream<<tmp->node2->yuan->myLinks.toList()[0]->fromYuan()->master->mVarDefNode->text()<<"=";
            stream<<"(myCopter.battery.current * 10)\n";
        }
        if(tmp->node3!=0){
            for(int i=1;i<=tabs;i++) stream<<"   ";
            stream<<tmp->node3->yuan->myLinks.toList()[0]->fromYuan()->master->mVarDefNode->text()<<"=";
            stream<<"myCopter.battery.level\n";
        }
        qDebug()<<"format::widget_convert_to_py()\n"<<w->name;
    }

    if(w->identifier=="Attitude"){    //如果传入的控件是Attitude
        AttitudeNode* tmp = w->mAttitudeNode;
        if(tmp->node1!=0){
            for(int i=1;i<=tabs;i++) stream<<"   ";
            stream<<tmp->node1->yuan->myLinks.toList()[0]->fromYuan()->master->mVarDefNode->text()<<"=";
            stream<<"myCopter.attitude.roll\n";
        }
        if(tmp->node2!=0){
            for(int i=1;i<=tabs;i++) stream<<"   ";
            stream<<tmp->node2->yuan->myLinks.toList()[0]->fromYuan()->master->mVarDefNode->text()<<"=";
            stream<<"myCopter.attitude.pitch\n";
        }
        if(tmp->node3!=0){
            for(int i=1;i<=tabs;i++) stream<<"   ";
            stream<<tmp->node3->yuan->myLinks.toList()[0]->fromYuan()->master->mVarDefNode->text()<<"=";
            stream<<"myCopter.attitude.yaw\n";
        }
        qDebug()<<"format::widget_convert_to_py()\n"<<w->name;
    }
    if(w->identifier=="Channel"){    //如果传入的控件是Channel
        ChannelNode* tmp = w->mChannelNode;
        if(tmp->node1!=0){
            for(int i=1;i<=tabs;i++) stream<<"   ";
            stream<<tmp->node1->yuan->myLinks.toList()[0]->fromYuan()->master->mVarDefNode->text()<<"=";
            stream<<"myCopter.channels[str(1)]\n";
        }
        if(tmp->node2!=0){
            for(int i=1;i<=tabs;i++) stream<<"   ";
            stream<<tmp->node2->yuan->myLinks.toList()[0]->fromYuan()->master->mVarDefNode->text()<<"=";
            stream<<"myCopter.channels[str(2)]\n";
        }
        if(tmp->node3!=0){
            for(int i=1;i<=tabs;i++) stream<<"   ";
            stream<<tmp->node3->yuan->myLinks.toList()[0]->fromYuan()->master->mVarDefNode->text()<<"=";
            stream<<"myCopter.channels[str(3)]\n";
        }
        if(tmp->node4!=0){
            for(int i=1;i<=tabs;i++) stream<<"   ";
            stream<<tmp->node4->yuan->myLinks.toList()[0]->fromYuan()->master->mVarDefNode->text()<<"=";
            stream<<"myCopter.channels[str(4)]\n";
        }
        if(tmp->node5!=0){
            for(int i=1;i<=tabs;i++) stream<<"   ";
            stream<<tmp->node5->yuan->myLinks.toList()[0]->fromYuan()->master->mVarDefNode->text()<<"=";
            stream<<"myCopter.channels[str(5)]\n";
        }
        if(tmp->node6!=0){
            for(int i=1;i<=tabs;i++) stream<<"   ";
            stream<<tmp->node6->yuan->myLinks.toList()[0]->fromYuan()->master->mVarDefNode->text()<<"=";
            stream<<"myCopter.channels[str(6)]\n";
        }
        if(tmp->node7!=0){
            for(int i=1;i<=tabs;i++) stream<<"   ";
            stream<<tmp->node7->yuan->myLinks.toList()[0]->fromYuan()->master->mVarDefNode->text()<<"=";
            stream<<"myCopter.channels[str(7)]\n";
        }
        if(tmp->node8!=0){
            for(int i=1;i<=tabs;i++) stream<<"   ";
            stream<<tmp->node8->yuan->myLinks.toList()[0]->fromYuan()->master->mVarDefNode->text()<<"=";
            stream<<"myCopter.channels[str(8)]\n";
        }
        if(tmp->node9!=0){
            for(int i=1;i<=tabs;i++) stream<<"   ";
            stream<<tmp->node9->yuan->myLinks.toList()[0]->fromYuan()->master->mVarDefNode->text()<<"=";
            stream<<"myCopter.channels[str(9)]\n";
        }
        qDebug()<<"format::widget_convert_to_py()\n"<<w->name;
    }
    if(w->identifier=="RangeFinder"){    //如果传入的控件是RangeFinder
        RangeFinderNode* tmp = w->mRangeFinderNode;
        if(tmp->node2!=0){
            for(int i=1;i<=tabs;i++) stream<<"   ";
            stream<<tmp->node2->yuan->myLinks.toList()[0]->toYuan()->master->mVarDefNode->text()<<"=";
            stream<<"myCopter.rangefinder.distance\n";
        }
        qDebug()<<"format::widget_convert_to_py()\n"<<w->name;
    }

    if(w->identifier=="TakeOff"){    //如果传入的控件是Action
        qDebug()<<"format::widget_convert_to_py()\n"<<w->name;
        for(int i=1;i<=tabs;i++) stream<<"   ";
        stream<<"if not myCopter.takeoff("<<w->mTakeOffNode->myAltitude()<<"):\n";
        for(int i=1;i<=tabs;i++) stream<<"   ";
        stream<<"   sys.exit(1)\n";
    }
    if(w->identifier=="Land"){    //如果传入的控件是Action
        qDebug()<<"format::widget_convert_to_py()\n"<<w->name;
        for(int i=1;i<=tabs;i++) stream<<"   ";
        stream<<"timeoutCounter = 0\n";
        for(int i=1;i<=tabs;i++) stream<<"   ";
        stream<<"while not myCopter.land():\n";
        for(int i=1;i<=tabs;i++) stream<<"   ";
        stream<<"    timeoutCounter += 1\n";
        for(int i=1;i<=tabs;i++) stream<<"   ";
        stream<<"    if timeoutCounter > 3:\n";
        for(int i=1;i<=tabs;i++) stream<<"   ";
        stream<<"        print \"Critical: Cannot land the vehicle after 3 retries.\"\n";
        for(int i=1;i<=tabs;i++) stream<<"   ";
        stream<<"        sys.exit(1)";
    }
    if(w->identifier=="Go"){    //如果传入的控件是Action
        qDebug()<<"format::widget_convert_to_py()\n"<<w->name;
        for(int i=1;i<=tabs;i++) stream<<"   ";
        if(w->mGoNode->direction=="GoUp"){
            stream<<"print \"Going upward\"\n";
            for(int i=1;i<=tabs;i++) stream<<"   ";
            stream<<"myCopter.send_nav_velocity(0, 0,"<<(-(w->mGoNode->myGroundSpeed()))<<")\n";
            for(int i=1;i<=tabs;i++) stream<<"   ";
            stream<<"time.sleep("<<w->mGoNode->myTime()<<")\n";
        }
        if(w->mGoNode->direction=="GoDown"){
            stream<<"print \"Going down\"\n";
            for(int i=1;i<=tabs;i++) stream<<"   ";
            stream<<"myCopter.send_nav_velocity(0, 0,"<<w->mGoNode->myGroundSpeed()<<")\n";
            for(int i=1;i<=tabs;i++) stream<<"   ";
            stream<<"time.sleep("<<w->mGoNode->myTime()<<")\n";
        }
        if(w->mGoNode->direction=="GoRight"){
            stream<<"print \"Going rightward at 1m/s for 5s\"\n";
            for(int i=1;i<=tabs;i++) stream<<"   ";
            stream<<"myCopter.send_nav_velocity("<<w->mGoNode->myGroundSpeed()<<", 0, 0)\n";
            for(int i=1;i<=tabs;i++) stream<<"   ";
            stream<<"time.sleep("<<w->mGoNode->myTime()<<")\n";
        }
        if(w->mGoNode->direction=="GoLeft"){
            stream<<"print \"Going leftward at 1m/s for 5s\"\n";
            for(int i=1;i<=tabs;i++) stream<<"   ";
            stream<<"myCopter.send_nav_velocity("<<(-(w->mGoNode->myGroundSpeed()))<<", 0, 0)\n";
            for(int i=1;i<=tabs;i++) stream<<"   ";
            stream<<"time.sleep("<<w->mGoNode->myTime()<<")\n";
        }
        if(w->mGoNode->direction=="Forward"){
            stream<<"print \"Going forward at 1m/s for 5s\"\n";
            for(int i=1;i<=tabs;i++) stream<<"   ";
            stream<<"myCopter.send_nav_velocity(0, "<<(-(w->mGoNode->myGroundSpeed()))<<", 0)\n";
            for(int i=1;i<=tabs;i++) stream<<"   ";
            stream<<"time.sleep("<<w->mGoNode->myTime()<<")\n";
        }
        if(w->mGoNode->direction=="Backward"){
            stream<<"print \"Going backward at 1m/s for 5s\"\n";
            for(int i=1;i<=tabs;i++) stream<<"   ";
            stream<<"myCopter.send_nav_velocity(0, "<<(w->mGoNode->myGroundSpeed())<<", 0)\n";
            for(int i=1;i<=tabs;i++) stream<<"   ";
            stream<<"time.sleep("<<w->mGoNode->myTime()<<")\n";
        }
    }
    if(w->identifier=="Turn"){    //如果传入的控件是Action
        qDebug()<<"format::widget_convert_to_py()\n"<<w->name;
        for(int i=1;i<=tabs;i++) stream<<"   ";
        if(w->mTurnNode->box->currentIndex()==0)
            stream<<"myCopter.condition_yaw("<<w->mTurnNode->myAngel()<<", relative = True, clock_wise = False)\n";
        if(w->mTurnNode->box->currentIndex()==1)
            stream<<"myCopter.condition_yaw("<<w->mTurnNode->myAngel()<<", relative = True, clock_wise = True)\n";

    }
    if(w->identifier=="Hover"){    //如果传入的控件是Action
        qDebug()<<"format::widget_convert_to_py()\n"<<w->name;
        for(int i=1;i<=tabs;i++) stream<<"   ";
        stream<<"time.sleep("<<w->mHoverNode->myTime()<<")\n";
    }
    if(w->identifier=="Delay"){    //如果传入的控件是Action
        qDebug()<<"format::widget_convert_to_py()\n"<<w->name;
        for(int i=1;i<=tabs;i++) stream<<"   ";
        stream<<"\"delay\"\n";
        //目前没有要对应的代码
    }

    if(w->identifier=="Compute"){    //如果传入的控件是Compute
        ComputeNode* tmp = w->mComputeNode;
        QString s;
        QString s1="",s2="";
        if(tmp->rect1text!=""){
            s1 = tmp->rect1text;
        }else{
            s1 = tmp->rect1->text();
        }
        if(tmp->rect2text!=""){
            s2 = tmp->rect2text;
        }else{
            s2 = tmp->rect2->text();
        }
        if(tmp->text()=="cos"){
            s = "cos(" + s2 + ")";
        }else
        if(tmp->text()=="sin"){
            s = "sin(" + s2 + ")";
        }else
        if(tmp->text()=="tan"){
            s = "tan(" + s2 + ")";
        }else
        if(tmp->text()=="log"){
            s = "log10(" + s2 + ")/log10(" + s1 + ")";
        }else
        if(tmp->text()=="e"){
            s = s1 + "**" + s2;
        }else
        if(tmp->text()=="="){
            s = s1 + "==" + s2;
        }else{
            s = s1 + tmp->text() + s2;
        }

        if(!tmp->yuan->myLinks.isEmpty()){
            if(tmp->yuan->myLinks.toList()[0]->toYuan()->master->identifier=="VarDef"){
                VardefNode* vn = tmp->yuan->myLinks.toList()[0]->toYuan()->master->mVarDefNode;
                vn->myStringText = "(" + s + ")";
            }else if(tmp->yuan->myLinks.toList()[0]->toYuan()->master->identifier=="Compute"){
                ComputeNode* cn = tmp->yuan->myLinks.toList()[0]->toYuan()->master->mComputeNode;
                if(cn->yuan2==tmp->yuan->myLinks.toList()[0]->toYuan())
                    cn->rect1text = "(" + s + ")";
                else
                    cn->rect2text = "(" + s + ")";
            }
        }
        for(int i=1;i<=tabs;i++) stream<<"   ";
        stream<<"\"if you see this,it means this compute was compiled independently, the code is wrong.but this does not influence your program.\"\n";
        qDebug()<<"format::widget_convert_to_py()\n"<<w->name;
    }

    if(w->identifier=="Logic"){    //如果传入的控件是Logic
        for(int i=1;i<=tabs;i++) stream<<"   ";
        stream<<w->mLogicNode->box->currentText()<<" ";
        ComputeNode* tmp = w->mLogicNode->yuan2->myLinks.toList()[0]->fromYuan()->master->mComputeNode;
        if(tmp->text()=="cos"){
            stream<<"cos("<<tmp->rect2->text()<<")";
        }else
        if(tmp->text()=="sin"){
            stream<<"sin("<<tmp->rect2->text()<<")";
        }else
        if(tmp->text()=="tan"){
            stream<<"tan("<<tmp->rect2->text()<<")";
        }else
        if(tmp->text()=="log"){
            stream<<"log10("<<tmp->rect2->text()<<")/log10("<<tmp->rect1->text()<<")";
        }else
        if(tmp->text()=="e"){
            stream<<tmp->rect1->text()<<"**"<<tmp->rect2->text();
        }else
        if(tmp->text()=="="){
            stream<<tmp->rect1->text()<<"=="<<tmp->rect2->text();
        }else{
            stream<<tmp->rect1->text()<<tmp->text()<<tmp->rect2->text();
        }
        stream<<":\n";

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
bool format::CreateTakeOff(QPointF point,int id,double Altitude)
{
    TakeOffNode *node=new TakeOffNode;
    node->lx = point.x();
    node->ly = point.y();
    node->controlsId=id;
    node->identifier="TakeOff";
    QString cid = QString::number(node->controlsId,10);
    node->name = node->identifier + cid;
    node->altitude = Altitude;

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

bool format::CreateGo(QPointF point, int id, double Speed, double Time)
{
    GoNode *node=new GoNode;
    node->lx = point.x();
    node->ly = point.y();
    node->controlsId=id;
    node->identifier="Go";
    QString cid = QString::number(node->controlsId,10);
    node->name = node->identifier + cid;
    node->groundspeed = Speed;
    node->Time = Time;
    qDebug()<<"Create():";
    qDebug()<<"name :"<<node->name;
    qDebug()<<"identifier :"<<node->identifier;
    qDebug()<<"controlsId :"<<node->controlsId;

    WidgetWrap* tmp = new WidgetWrap(node);   //包装节点
    Map.insert(tmp->name,tmp);            //添加到widgetmap中
    return true;
}

bool format::CreateTurn(QPointF point, int id, double Angel)
{
    TurnNode *node=new TurnNode;

    node->lx = point.x();
    node->ly = point.y();
    node->controlsId=id;
    node->identifier="Turn";
    QString cid = QString::number(node->controlsId,10);
    node->name = node->identifier + cid;
    node->Angel = Angel;

    qDebug()<<"Create():";
    qDebug()<<"name :"<<node->name;
    qDebug()<<"identifier :"<<node->identifier;
    qDebug()<<"controlsId :"<<node->controlsId;

    WidgetWrap* tmp = new WidgetWrap(node);   //包装节点
    Map.insert(tmp->name,tmp);            //添加到widgetmap中
    return true;
}

bool format::CreateHover(QPointF point, int id, double Time)
{
    HoverNode *node=new HoverNode;
    node->lx = point.x();
    node->ly = point.y();
    node->controlsId=id;
    node->identifier="Hover";
    QString cid = QString::number(node->controlsId,10);
    node->name = node->identifier + cid;
    node->time = Time;
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

bool format::CreateCompute(QPointF point, int id, QString math)
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

bool format::CreateAttitude(QPointF point, int id)
{
    AttitudeNode* node=new AttitudeNode;
    node->lx = point.x();
    node->ly = point.y();

    node->controlsId=id;
    node->identifier="Attitude";
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

bool format::CreateChannel(QPointF point, int id)
{
    ChannelNode* node=new ChannelNode;
    node->lx = point.x();
    node->ly = point.y();

    node->controlsId=id;
    node->identifier="Channel";
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

bool format::CreateRangeFinder(QPointF point, int id)
{
    RangeFinderNode* node=new RangeFinderNode;
    node->lx = point.x();
    node->ly = point.y();

    node->controlsId=id;
    node->identifier="RangeFinder";
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


