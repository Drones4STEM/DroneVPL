
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

    for(iter=Map.begin(); iter!=Map.end(); iter++){   //先存Var
        if(iter.value()->identifier=="Var")
            widget_convert_to_xml(iter,stream);
    }
    for(iter=Map.begin(); iter!=Map.end(); iter++){   //存剩下的控件
        if(iter.value()->identifier!="Var" && iter.value()->identifier!="Link")
            widget_convert_to_xml(iter,stream);
    }
    //注意这里的link保存一定要放在最后，因为在打开函数中，并没有规定读取顺序，即如果link先保存，就会被先打开，若link要连的控件还未被读取，则必然报错！
    for(iter=Map.begin(); iter!=Map.end(); iter++){   //存link
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
            stream.writeTextElement("direction",ww->mGoNode->direction);
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
            stream.writeTextElement("direction",ww->mTurnNode->direction);
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
//            QString Time = QString::number((long)ww->mDelayNode->lineEdit->text(),10);
            stream.writeTextElement("Time",ww->mDelayNode->lineEdit->text());
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
        if(identifier == "Var"){
            x = QString::number((long)ww->mVarNode->pos().x(),10);
            stream.writeTextElement("location_x",x);
            y = QString::number((long)ww->mVarNode->pos().y(),10);
            stream.writeTextElement("location_y",y);
            stream.writeTextElement("amount",QString::number((long)ww->mVarNode->getvarnum(),10));
            for(int i=1;i<=ww->mVarNode->getvarnum();i++){
                if(i==1){
                    stream.writeTextElement("data_type1",
                                            ww->mVarNode->typeBox[0]->currentText());
                    stream.writeTextElement("name",ww->mVarNode->nameEdit[0]->text());
                    stream.writeTextElement("init_value",ww->mVarNode->valueEdit[0]->text());
                }
                if(i==2){
                    stream.writeTextElement("data_type2",
                                            ww->mVarNode->typeBox[1]->currentText());
                    stream.writeTextElement("name",ww->mVarNode->nameEdit[1]->text());
                    stream.writeTextElement("init_value",ww->mVarNode->valueEdit[1]->text());
                }
                if(i==3){
                    stream.writeTextElement("data_type3",
                                            ww->mVarNode->typeBox[2]->currentText());
                    stream.writeTextElement("name",ww->mVarNode->nameEdit[2]->text());
                    stream.writeTextElement("init_value",ww->mVarNode->valueEdit[2]->text());
                }
                if(i==4){
                    stream.writeTextElement("data_type4",
                                            ww->mVarNode->typeBox[3]->currentText());
                    stream.writeTextElement("name",ww->mVarNode->nameEdit[3]->text());
                    stream.writeTextElement("init_value",ww->mVarNode->valueEdit[3]->text());
                }
            }


        }
        if(identifier == "VarInstance"){
            x = QString::number((long)ww->mVarInstanceNode->pos().x(),10);
            stream.writeTextElement("location_x",x);
            y = QString::number((long)ww->mVarInstanceNode->pos().y(),10);
            stream.writeTextElement("location_y",y);
            stream.writeTextElement("VarName",ww->mVarInstanceNode->varName);
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

            stream.writeStartElement("math");
            if(ww->mComputeNode->box->currentText()=="+"){ //如果加法
                stream.writeAttribute("operator","+");
                stream.writeTextElement("op_left",ww->mComputeNode->lineEdit1->text());
                stream.writeTextElement("op_right",ww->mComputeNode->lineEdit2->text());
            }
            if(ww->mComputeNode->box->currentText()=="-"){ //如果减法
                stream.writeAttribute("operator","-");
                stream.writeTextElement("op_left",ww->mComputeNode->lineEdit1->text());
                stream.writeTextElement("op_right",ww->mComputeNode->lineEdit2->text());
            }
            if(ww->mComputeNode->box->currentText()=="*"){ //如果乘法
                stream.writeAttribute("operator","*");
                stream.writeTextElement("op_left",ww->mComputeNode->lineEdit1->text());
                stream.writeTextElement("op_right",ww->mComputeNode->lineEdit2->text());
            }
            if(ww->mComputeNode->box->currentText()=="/"){ //如果除法
                stream.writeAttribute("operator","/");
                stream.writeTextElement("op_left",ww->mComputeNode->lineEdit1->text());
                stream.writeTextElement("op_right",ww->mComputeNode->lineEdit2->text());
            }
            if(ww->mComputeNode->box->currentText()=="="){ //如果等于
                stream.writeAttribute("operator","=");
                stream.writeTextElement("op_left",ww->mComputeNode->lineEdit1->text());
                stream.writeTextElement("op_right",ww->mComputeNode->lineEdit2->text());

            }
            if(ww->mComputeNode->box->currentText()=="<"){ //如果小于
                stream.writeAttribute("operator","<");
                stream.writeTextElement("op_left",ww->mComputeNode->lineEdit1->text());
                stream.writeTextElement("op_right",ww->mComputeNode->lineEdit2->text());
            }
            if(ww->mComputeNode->box->currentText()==">"){ //如果大于
                stream.writeAttribute("operator",">");
                stream.writeTextElement("op_left",ww->mComputeNode->lineEdit1->text());
                stream.writeTextElement("op_right",ww->mComputeNode->lineEdit2->text());
            }
        }else{//下面代码很大重复，本来应该通过widgetwrap实时拷贝控件位置，来避免指针调用的，没时间优化了
            if(ww->identifier=="E"){ //如果幂
                x = QString::number((long)ww->mENode->pos().x(),10);
                stream.writeTextElement("location_x",x);
                y = QString::number((long)ww->mENode->pos().y(),10);
                stream.writeTextElement("location_y",y);

                stream.writeStartElement("math");
                stream.writeAttribute("operator","e");
                stream.writeTextElement("op_left","");
                stream.writeTextElement("op_right",ww->mENode->lineEdit2->text());
            }
            if(ww->identifier=="Sin"){ //如果正弦
                x = QString::number((long)ww->mSinNode->pos().x(),10);
                stream.writeTextElement("location_x",x);
                y = QString::number((long)ww->mSinNode->pos().y(),10);
                stream.writeTextElement("location_y",y);

                stream.writeStartElement("math");
                QString tmp = ww->mSinNode->box->currentText();
                stream.writeAttribute("operator",tmp);
                stream.writeTextElement("op_left","");
                stream.writeTextElement("op_right",ww->mSinNode->lineEdit2->text());
            }
            if(ww->identifier=="Log"){ //如果对数
                x = QString::number((long)ww->mLogNode->pos().x(),10);
                stream.writeTextElement("location_x",x);
                y = QString::number((long)ww->mLogNode->pos().y(),10);
                stream.writeTextElement("location_y",y);

                stream.writeStartElement("math");
                stream.writeAttribute("operator","log");
                stream.writeTextElement("op_left",ww->mLogNode->lineEdit1->text());
                stream.writeTextElement("op_right",ww->mLogNode->lineEdit2->text());
            }
        }

        stream.writeEndElement();
        stream.writeEndElement();   //correspond to writeStartElement("Compute")
        qDebug()<<"category: "<<ww->category;
        qDebug()<<"type: "<<identifier;
        qDebug()<<"id: "<<controlsId;
        qDebug()<<"location_x: "<<x;
        qDebug()<<"location_y: "<<y;
//        qDebug()<<"operator: "<<ww->mComputeNode->box->currentText();
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
        if(identifier == "If"||
                identifier == "While"||
                identifier == "Else"){
            x = QString::number((long)ww->mLogicNode->pos().x(),10);
            stream.writeTextElement("location_x",x);
            y = QString::number((long)ww->mLogicNode->pos().y(),10);
            stream.writeTextElement("location_y",y);
            QString width = QString::number((long)ww->mLogicNode->boundingRect().width(),10);
            QString height = QString::number((long)ww->mLogicNode->boundingRect().height(),10);
            stream.writeTextElement("width",width);
            stream.writeTextElement("height",height);
        }
        stream.writeEndElement();   //correspond to writeStartElement("Logic")
        qDebug()<<"category: "<<ww->category;
        qDebug()<<"type: "<<identifier;
        qDebug()<<"id: "<<controlsId;
        qDebug()<<"location_x: "<<x;
        qDebug()<<"location_y: "<<y;
    }

    if(ww->category == "Link"){
        stream.writeStartElement("Link");
        stream.writeAttribute("type",identifier);
        stream.writeTextElement("id",controlsId);
        if(identifier == "Link"){ //连线
//             x = QString::number((long)ww->mLinkNode->pos().x(),10);
//             stream.writeTextElement("location_x",x);
//             y = QString::number((long)ww->mLinkNode->pos().y(),10);
//             stream.writeTextElement("location_y",y);
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
                        QString direction;
                        if(stream.name().toString()=="direction"){
                            direction = stream.readElementText();
                            qDebug()<<"direction: "<<direction;
                        }
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
                        CreateGo(point,id,direction,GroundSpeed,Time); //在窗口中生成takeoff控件
                    }
                    if(type=="Turn"){
                        stream.readNext();
                        stream.readNext();
                        QString direction;
                        if(stream.name().toString()=="direction"){
                            direction = stream.readElementText();
                            qDebug()<<"direction: "<<direction;
                        }
                        stream.readNext();
                        stream.readNext();
                        double Angel;
                        if(stream.name().toString()=="Angel"){
                            Angel = stream.readElementText().toDouble();
                            qDebug()<<"Angel: "<<Angel;
                        }
                        CreateTurn(point,id,direction,Angel); //在窗口中生成takeoff控件
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
                        stream.readNext();
                        stream.readNext();
                        double Time;
                        if(stream.name().toString()=="Time"){
                            Time = stream.readElementText().toDouble();
                            qDebug()<<"Time: "<<Time;
                        }
                        CreateDelay(point,id,Time); //在窗口中生成takeoff控件
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
                    if(type=="Var"){
                        QString data_type[4],varname[4],init_value[4];
                        int amount;
                        stream.readNext();
                        stream.readNext();
                        if(stream.name().toString()=="amount"){
                            amount = stream.readElementText().toInt();
                            for(int i=0;i<amount;i++){
                                stream.readNext();
                                stream.readNext();
                                //本来要检查读到的是什么，但是太麻烦，代码重复度又非常高，冒险不查了
                                data_type[i] = stream.readElementText();
                                stream.readNext();
                                stream.readNext();
                                varname[i] = stream.readElementText();
                                stream.readNext();
                                stream.readNext();
                                init_value[i] = stream.readElementText();
                            }
                            qDebug()<<"location_y: "<<location_y;
                        }
                        CreateVar(point,id,amount,data_type,varname,init_value); //在窗口中生成takeoff控件
                    }
                    if(type=="VarInstance"){
                        stream.readNext();
                        stream.readNext();
                        if(stream.name().toString()=="VarName"){
                            QString name = stream.readElementText();
                            stream.readNext();  stream.readNext();
                            CreateVarInstance(point,id,name); //在窗口中生成VarInstance控件
                        }
                    }
                }catch(exception e){
                   ;
                }
            }
            if(stream.name().toString()=="Compute"){
                QString left,right;
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
                    math = stream.attributes().value("operator").toString();
                    qDebug()<<"math: "<<math;
                }
                stream.readNext();
                stream.readNext();
                if(stream.name().toString()=="op_left"){
                    left = stream.readElementText();
                    qDebug()<<"left: "<<left;
                }
                stream.readNext();
                stream.readNext();
                if(stream.name().toString()=="op_right"){
                    right = stream.readElementText();
                    qDebug()<<"right: "<<right;
                }

                QPointF point(location_x,location_y);
                if(type=="Compute"){
                    CreateCompute(point,id,math,left,right,type); //在map中生成compute控件
                }else if(type=="E"){
                    CreateE(point,id,math,right,type);
                }else if(type=="Log"){
                    CreateLog(point,id,math,left,right,type);
                }else if(type=="Sin"){
                    CreateSin(point,id,math,right,type);
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
                int width,height;
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
                if(stream.name().toString()=="width"){
                    width = stream.readElementText().toInt();
                    qDebug()<<"width: "<<width;
                }
                stream.readNext();
                stream.readNext();
                if(stream.name().toString()=="height"){
                    height = stream.readElementText().toInt();
                    qDebug()<<"height: "<<height;
                }

                QPointF point(location_x,location_y);
                try{
                    if(type=="If" || type=="Else" || type=="While"){
                        CreateLogic(point,id,type,width,height); //在窗口中生成Logic控件
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
//                if(stream.name().toString()=="location_x"){
//                    location_x = stream.readElementText().toInt();
//                    qDebug()<<"location_x: "<<location_x;
//                }
//                stream.readNext();
//                stream.readNext();
//                if(stream.name().toString()=="location_y"){
//                    location_y = stream.readElementText().toInt();
//                    qDebug()<<"location_y: "<<location_y;
//                }
//                stream.readNext();  stream.readNext();
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
                    CreateLink(point,id,from,to,fyuan,tyuan);
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
        if(tmp->identifier=="While" ||
                tmp->identifier=="If" ||
                tmp->identifier=="Else" ){
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

    QString path;
    QDir dir;
    path = dir.currentPath();
    path = path + "/prepare.py";
    qDebug()<<"file copy "
            <<QFile::copy(path,filename);

    qDebug()<<filename;
    file.open(QIODevice::Append);
    QTextStream in(&file);
    in<<"\n\n\nfrom math import *\n";
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

    if(w->identifier=="Var"){    //如果传入的控件是Var
        VarNode* tmp = w->mVarNode;
        for(int i=0;i<tmp->getvarnum();i++){
            if(tmp->typeBox[i]->currentText()=="int"){
                stream<<tmp->nameEdit[i]->text()<<"="<<tmp->valueEdit[i]->text().toInt();
            }else if(tmp->typeBox[i]->currentText()=="double"){
                stream<<tmp->nameEdit[i]->text()<<"="<<tmp->valueEdit[i]->text().toDouble();
            }else if(tmp->typeBox[i]->currentText()=="float"){
                stream<<tmp->nameEdit[i]->text()<<"="<<tmp->valueEdit[i]->text().toFloat();
            }

        }
        stream<<"\n";
        qDebug()<<"format::widget_convert_to_py()\n"<<w->name;
    }
    if(w->identifier=="VarInstance"){
//        VarInstance 会用到的地方，在原则上只能是compute和io，考虑到它不是一个操作而是一个变量，
//          不能在拓扑遍历到时执行（会损失和其他控件的联系），所以改成：如果一个控件可能连varinstance，
//          则在它被遍历到时查看是否“附带了”varinstance
        //所以原则上这个if不会执行
    }

    if(w->identifier=="Battery"){    //如果传入的控件是Battery
        BatteryNode* tmp = w->mBatteryNode;
        if(tmp->node1!=0){
            for(int i=1;i<=tabs;i++) stream<<"   ";
            stream<<tmp->node1->yuan->myLinks.toList()[0]->fromYuan()->master->mVarInstanceNode->varName<<"=";
            stream<<"myCopter.battery.voltage\n";
        }
        if(tmp->node2!=0){
            for(int i=1;i<=tabs;i++) stream<<"   ";
            stream<<tmp->node2->yuan->myLinks.toList()[0]->fromYuan()->master->mVarInstanceNode->varName<<"=";
            stream<<"(myCopter.battery.current * 10)\n";
        }
        if(tmp->node3!=0){
            for(int i=1;i<=tabs;i++) stream<<"   ";
            stream<<tmp->node3->yuan->myLinks.toList()[0]->fromYuan()->master->mVarInstanceNode->varName<<"=";
            stream<<"myCopter.battery.level\n";
        }
        qDebug()<<"format::widget_convert_to_py()\n"<<w->name;
    }

    if(w->identifier=="Attitude"){    //如果传入的控件是Attitude
        AttitudeNode* tmp = w->mAttitudeNode;
        if(tmp->node1!=0){
            for(int i=1;i<=tabs;i++) stream<<"   ";
            stream<<tmp->node1->yuan->myLinks.toList()[0]->fromYuan()->master->mVarInstanceNode->varName<<"=";
            stream<<"myCopter.attitude.roll\n";
        }
        if(tmp->node2!=0){
            for(int i=1;i<=tabs;i++) stream<<"   ";
            stream<<tmp->node2->yuan->myLinks.toList()[0]->fromYuan()->master->mVarInstanceNode->varName<<"=";
            stream<<"myCopter.attitude.pitch\n";
        }
        if(tmp->node3!=0){
            for(int i=1;i<=tabs;i++) stream<<"   ";
            stream<<tmp->node3->yuan->myLinks.toList()[0]->fromYuan()->master->mVarInstanceNode->varName<<"=";
            stream<<"myCopter.attitude.yaw\n";
        }
        qDebug()<<"format::widget_convert_to_py()\n"<<w->name;
    }
    if(w->identifier=="Channel"){    //如果传入的控件是Channel
        ChannelNode* tmp = w->mChannelNode;
        if(tmp->node1!=0){
            for(int i=1;i<=tabs;i++) stream<<"   ";
            stream<<tmp->node1->yuan->myLinks.toList()[0]->fromYuan()->master->mVarInstanceNode->varName<<"=";
            stream<<"myCopter.channels[str(1)]\n";
        }
        if(tmp->node2!=0){
            for(int i=1;i<=tabs;i++) stream<<"   ";
            stream<<tmp->node2->yuan->myLinks.toList()[0]->fromYuan()->master->mVarInstanceNode->varName<<"=";
            stream<<"myCopter.channels[str(2)]\n";
        }
        if(tmp->node3!=0){
            for(int i=1;i<=tabs;i++) stream<<"   ";
            stream<<tmp->node3->yuan->myLinks.toList()[0]->fromYuan()->master->mVarInstanceNode->varName<<"=";
            stream<<"myCopter.channels[str(3)]\n";
        }
        if(tmp->node4!=0){
            for(int i=1;i<=tabs;i++) stream<<"   ";
            stream<<tmp->node4->yuan->myLinks.toList()[0]->fromYuan()->master->mVarInstanceNode->varName<<"=";
            stream<<"myCopter.channels[str(4)]\n";
        }
        if(tmp->node5!=0){
            for(int i=1;i<=tabs;i++) stream<<"   ";
            stream<<tmp->node5->yuan->myLinks.toList()[0]->fromYuan()->master->mVarInstanceNode->varName<<"=";
            stream<<"myCopter.channels[str(5)]\n";
        }
        if(tmp->node6!=0){
            for(int i=1;i<=tabs;i++) stream<<"   ";
            stream<<tmp->node6->yuan->myLinks.toList()[0]->fromYuan()->master->mVarInstanceNode->varName<<"=";
            stream<<"myCopter.channels[str(6)]\n";
        }
        if(tmp->node7!=0){
            for(int i=1;i<=tabs;i++) stream<<"   ";
            stream<<tmp->node7->yuan->myLinks.toList()[0]->fromYuan()->master->mVarInstanceNode->varName<<"=";
            stream<<"myCopter.channels[str(7)]\n";
        }
        if(tmp->node8!=0){
            for(int i=1;i<=tabs;i++) stream<<"   ";
            stream<<tmp->node8->yuan->myLinks.toList()[0]->fromYuan()->master->mVarInstanceNode->varName<<"=";
            stream<<"myCopter.channels[str(8)]\n";
        }
        if(tmp->node9!=0){
            for(int i=1;i<=tabs;i++) stream<<"   ";
            stream<<tmp->node9->yuan->myLinks.toList()[0]->fromYuan()->master->mVarInstanceNode->varName<<"=";
            stream<<"myCopter.channels[str(9)]\n";
        }
        qDebug()<<"format::widget_convert_to_py()\n"<<w->name;
    }
    if(w->identifier=="RangeFinder"){    //如果传入的控件是RangeFinder
        RangeFinderNode* tmp = w->mRangeFinderNode;
        if(tmp->node2!=0){
            for(int i=1;i<=tabs;i++) stream<<"   ";
            stream<<tmp->node2->yuan->myLinks.toList()[0]->toYuan()->master->mVarInstanceNode->varName<<"=";
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
        if(w->mGoNode->direction=="GoUp"){
//            stream<<"print \"Going upward\"\n";
            for(int i=1;i<=tabs;i++) stream<<"   ";
            stream<<"myCopter.send_nav_velocity(0, 0,"<<(-(w->mGoNode->myGroundSpeed()))<<")\n";
            for(int i=1;i<=tabs;i++) stream<<"   ";
            stream<<"time.sleep("<<w->mGoNode->myTime()<<")\n";
        }
        if(w->mGoNode->direction=="GoDown"){
//            stream<<"print \"Going down\"\n";
            for(int i=1;i<=tabs;i++) stream<<"   ";
            stream<<"myCopter.send_nav_velocity(0, 0,"<<w->mGoNode->myGroundSpeed()<<")\n";
            for(int i=1;i<=tabs;i++) stream<<"   ";
            stream<<"time.sleep("<<w->mGoNode->myTime()<<")\n";
        }
        if(w->mGoNode->direction=="GoRight"){
//            stream<<"print \"Going rightward at 1m/s for 5s\"\n";
            for(int i=1;i<=tabs;i++) stream<<"   ";
            stream<<"myCopter.send_nav_velocity("<<w->mGoNode->myGroundSpeed()<<", 0, 0)\n";
            for(int i=1;i<=tabs;i++) stream<<"   ";
            stream<<"time.sleep("<<w->mGoNode->myTime()<<")\n";
        }
        if(w->mGoNode->direction=="GoLeft"){
//            stream<<"print \"Going leftward at 1m/s for 5s\"\n";
            for(int i=1;i<=tabs;i++) stream<<"   ";
            stream<<"myCopter.send_nav_velocity("<<(-(w->mGoNode->myGroundSpeed()))<<", 0, 0)\n";
            for(int i=1;i<=tabs;i++) stream<<"   ";
            stream<<"time.sleep("<<w->mGoNode->myTime()<<")\n";
        }
        if(w->mGoNode->direction=="Forward"){
//            stream<<"print \"Going forward at 1m/s for 5s\"\n";
            for(int i=1;i<=tabs;i++) stream<<"   ";
            stream<<"myCopter.send_nav_velocity(0, "<<(-(w->mGoNode->myGroundSpeed()))<<", 0)\n";
            for(int i=1;i<=tabs;i++) stream<<"   ";
            stream<<"time.sleep("<<w->mGoNode->myTime()<<")\n";
        }
        if(w->mGoNode->direction=="Backward"){
//            stream<<"print \"Going backward at 1m/s for 5s\"\n";
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
        stream<<"time.sleep("<<w->mDelayNode->lineEdit->text()<<")"<<"\n";
    }

    if(w->identifier=="Compute"){    //如果传入的控件是Compute
        //除logic外，compute控件必须连接一个变量控件
        ComputeNode* tmp = w->mComputeNode;
        VarInstanceNode* vn = tmp->yuan3->myLinks.toList()[0]->toYuan()->master->mVarInstanceNode;
        for(int i=1;i<=tabs;i++) stream<<"   ";
        stream<<vn->varName<<"="<<tmp->lineEdit1->text()<<tmp->box->currentText()<<tmp->lineEdit2->text()<<"\n";
        qDebug()<<"format::widget_convert_to_py()\n"<<w->name;
    }
    if(w->identifier=="Log"){    //如果传入的控件是Compute
        //除logic外，compute控件必须连接一个变量控件
        logNode* tmp = w->mLogNode;
        VarInstanceNode* vn = tmp->yuan3->myLinks.toList()[0]->toYuan()->master->mVarInstanceNode;
        for(int i=1;i<=tabs;i++) stream<<"   ";
        stream<<vn->varName<<"="<<"log("<<tmp->lineEdit2->text()<<","<<tmp->lineEdit2->text()<<")";
        qDebug()<<"format::widget_convert_to_py()\n"<<w->name;
    }
    if(w->identifier=="E"){    //如果传入的控件是Compute
        //除logic外，compute控件必须连接一个变量控件
        eNode* tmp = w->mENode;
        VarInstanceNode* vn = tmp->yuan3->myLinks.toList()[0]->toYuan()->master->mVarInstanceNode;
        for(int i=1;i<=tabs;i++) stream<<"   ";
//        stream<<vn->text()<<"="<<tmp->lineEdit1->text()<<"**"<<tmp->lineEdit2->text();
        qDebug()<<"format::widget_convert_to_py()\n"<<w->name;
    }
    if(w->identifier=="Sin"){    //如果传入的控件是Compute
        //除logic外，compute控件必须连接一个变量控件
        sinNode* tmp = w->mSinNode;
        VarInstanceNode* vn = tmp->yuan3->myLinks.toList()[0]->toYuan()->master->mVarInstanceNode;
        for(int i=1;i<=tabs;i++) stream<<"   ";
        stream<<vn->varName<<"="<<tmp->box->currentText()<<"("<<tmp->lineEdit2->text()<<")";
        qDebug()<<"format::widget_convert_to_py()\n"<<w->name;
    }
    if(w->identifier=="If"||
            w->identifier=="Else"||
            w->identifier=="While"){    //如果传入的控件是Logic
        for(int i=1;i<=tabs;i++) stream<<"   ";
        QString tmp;
        if(w->identifier=="If") tmp = "if";
        if(w->identifier=="Else") tmp = "else";
        if(w->identifier=="While") tmp = "while";
        stream<<tmp<<" ";
        if(w->identifier!="Else"){
            ComputeNode* tmp = w->mLogicNode->yuan3->myLinks.toList()[0]->fromYuan()->master->mComputeNode;
            if(tmp->box->currentText()=="=")
                stream<<tmp->lineEdit1->text()<<"=="<<tmp->lineEdit2->text();
            else{
                stream<<tmp->lineEdit1->text()<<tmp->box->currentText()<<tmp->lineEdit2->text();
            }
        }
        stream<<":\n";
    }
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
    node->yuan->name = "yuan";
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
    node->yuan2->name = "yuan2";
    return true;
}

bool format::CreateGo(QPointF point, int id, QString direction, double Speed, double Time)
{
    GoNode *node=new GoNode;
    node->lx = point.x();
    node->ly = point.y();
    node->direction = direction;
    node->controlsId=id;
    node->identifier="Go";
    QString cid = QString::number(node->controlsId,10);
    node->name = node->identifier + cid;
    node->groundspeed = Speed;
    node->Time = Time;
    node->direction = direction;
    qDebug()<<"Create():";
    qDebug()<<"name :"<<node->name;
    qDebug()<<"identifier :"<<node->identifier;
    qDebug()<<"controlsId :"<<node->controlsId;

    WidgetWrap* tmp = new WidgetWrap(node);   //包装节点
    node->yuan2->master = tmp;
    node->yuan2->name = "yuan2";
    node->yuan->master = tmp;
    node->yuan->name = "yuan";
    Map.insert(tmp->name,tmp);            //添加到widgetmap中

    return true;
}

bool format::CreateTurn(QPointF point, int id, QString direction, double Angel)
{
    TurnNode *node=new TurnNode;

    node->lx = point.x();
    node->ly = point.y();
    node->controlsId=id;
    node->identifier="Turn";
    QString cid = QString::number(node->controlsId,10);
    node->name = node->identifier + cid;
    node->Angel = Angel;
    node->direction = direction;

    qDebug()<<"Create():";
    qDebug()<<"name :"<<node->name;
    qDebug()<<"identifier :"<<node->identifier;
    qDebug()<<"controlsId :"<<node->controlsId;

    WidgetWrap* tmp = new WidgetWrap(node);   //包装节点
    node->yuan2->master = tmp;
    node->yuan2->name = "yuan2";
    node->yuan->master = tmp;
    node->yuan->name = "yuan";
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
    node->yuan2->master = tmp;
    node->yuan2->name = "yuan2";
    node->yuan->master = tmp;
    node->yuan->name = "yuan";
    return true;
}

bool format::CreateDelay(QPointF point, int id, double time)
{
    DelayNode *node=new DelayNode;

    node->lx = point.x();
    node->ly = point.y();
    node->controlsId=id;
    node->identifier="Delay";
    QString cid = QString::number(node->controlsId,10);
    node->name = node->identifier + cid;
    node->time = time;

    qDebug()<<"Create():";
    qDebug()<<"name :"<<node->name;
    qDebug()<<"identifier :"<<node->identifier;
    qDebug()<<"controlsId :"<<node->controlsId;

    WidgetWrap* tmp = new WidgetWrap(node);   //包装节点
    node->yuan2->master = tmp;
    node->yuan2->name = "yuan2";
    node->yuan->master = tmp;
    node->yuan->name = "yuan";
    Map.insert(tmp->name,tmp);            //添加到widgetmap中
    return true;
}

bool format::CreateVar(QPointF point, int id, int amount, QString* data_type, QString* varname, QString* init_value)
{
    VarNode* node=new VarNode;
    node->lx = point.x();
    node->ly = point.y();

    node->controlsId=id;
    node->identifier="Var";
    QString cid = QString::number(node->controlsId,10);
    node->name = node->identifier + cid;
    for(int i=0;i<amount;i++){
        node->type[i] = data_type[i];
        node->vname[i] = varname[i];
        node->value[i] = init_value[i];
    }
    node->setvarnum(amount);
    qDebug()<<"format::Create():";
    qDebug()<<"name :"<<node->name;
    qDebug()<<"identifier :"<<node->identifier;
    qDebug()<<"controlsId :"<<node->controlsId;

    WidgetWrap* tmp = new WidgetWrap(node);   //包装节点
    Map.insert(tmp->name,tmp);            //添加到widgetmap中
    return true;
}
/*********
 * QString name - the associated Var name.
 * int num - the VarInstance rank num in all the VarInstance widgets of Var
 ********/
bool format::CreateVarInstance(QPointF point, int id, QString name)//varnode内部有个成员叫num，所以形参不能和它重名
{
    VarInstanceNode *vdn = new VarInstanceNode;
    vdn->lx = point.x();
    vdn->ly = point.y();
    vdn->varName = name;

    vdn->controlsId=id;
    vdn->identifier="VarInstance";
    QString cid = QString::number(vdn->controlsId,10);
    vdn->name = vdn->identifier + cid;
    qDebug()<<"format::Create():";
    qDebug()<<"name :"<<vdn->name;
    qDebug()<<"identifier :"<<vdn->identifier;
    qDebug()<<"controlsId :"<<vdn->controlsId;

    WidgetWrap* tmp = new WidgetWrap(vdn);   //包装节点
    Map.insert(tmp->name,tmp);            //添加到widgetmap中
    vdn->yuan2->master=tmp;
    vdn->yuan2->name = "yuan2";
    return true;
}

bool format::CreateCompute(QPointF point, int id, QString math ,QString left, QString right,QString type)
{
    ComputeNode *node=new ComputeNode;
    node->lx = point.x();
    node->ly = point.y();

    node->oprt = math;
    node->left = left;
    node->right = right;
    node->controlsId=id;
    node->identifier=type;
    QString cid = QString::number(node->controlsId,10);
    node->name = node->identifier + cid;
    qDebug()<<"Create():";
    qDebug()<<"name :"<<node->name;
    qDebug()<<"identifier :"<<node->identifier;
    qDebug()<<"controlsId :"<<node->controlsId;

    WidgetWrap* tmp = new WidgetWrap(node);   //包装节点
    node->yuan2->master = tmp;
    node->yuan2->name = "yuan2";
    node->yuan->master = tmp;
    node->yuan->name = "yuan";
    node->yuan3->master = tmp;
    node->yuan3->name = "yuan3";
    Map.insert(tmp->name,tmp);            //添加到widgetmap中
   /* }else if(type=="E"){
        eNode *node=new eNode;
        node->lx = point.x();
        node->ly = point.y();

        node->
        node->controlsId=id;
        node->identifier=type;
        QString cid = QString::number(node->controlsId,10);
        node->name = node->identifier + cid;
        qDebug()<<"Create():";
        qDebug()<<"name :"<<node->name;
        qDebug()<<"identifier :"<<node->identifier;
        qDebug()<<"controlsId :"<<node->controlsId;

        WidgetWrap* tmp = new WidgetWrap(node);   //包装节点
        Map.insert(tmp->name,tmp);            //添加到widgetmap中
    }else if(type=="Log"){
        logNode *node=new logNode;
        node->lx = point.x();
        node->ly = point.y();

        node->
        node->controlsId=id;
        node->identifier=type;
        QString cid = QString::number(node->controlsId,10);
        node->name = node->identifier + cid;
        qDebug()<<"Create():";
        qDebug()<<"name :"<<node->name;
        qDebug()<<"identifier :"<<node->identifier;
        qDebug()<<"controlsId :"<<node->controlsId;

        WidgetWrap* tmp = new WidgetWrap(node);   //包装节点
        Map.insert(tmp->name,tmp);            //添加到widgetmap中
    }else if(type=="E"){
        sinNode *node=new sinNode;
        node->lx = point.x();
        node->ly = point.y();

        node->
        node->controlsId=id;
        node->identifier=type;
        QString cid = QString::number(node->controlsId,10);
        node->name = node->identifier + cid;
        qDebug()<<"Create():";
        qDebug()<<"name :"<<node->name;
        qDebug()<<"identifier :"<<node->identifier;
        qDebug()<<"controlsId :"<<node->controlsId;

        WidgetWrap* tmp = new WidgetWrap(node);   //包装节点
        Map.insert(tmp->name,tmp);            //添加到widgetmap中
    }*/



    return true;
}

bool format::CreateE(QPointF point,int id,QString math,QString right,QString type){
    eNode *node=new eNode;
    node->lx = point.x();
    node->ly = point.y();

    node->oprt = math;
    node->right = right;
    node->controlsId=id;
    node->identifier=type;
    QString cid = QString::number(node->controlsId,10);
    node->name = node->identifier + cid;
    qDebug()<<"Create():";
    qDebug()<<"name :"<<node->name;
    qDebug()<<"identifier :"<<node->identifier;
    qDebug()<<"controlsId :"<<node->controlsId;

    WidgetWrap* tmp = new WidgetWrap(node);   //包装节点
    node->yuan->master = tmp;
    node->yuan->name = "yuan";
    node->yuan2->master = tmp;
    node->yuan2->name = "yuan2";
    node->yuan3->master = tmp;
    node->yuan3->name = "yuan3";
    Map.insert(tmp->name,tmp);            //添加到widgetmap中
}

bool format::CreateSin(QPointF point,int id,QString math,QString right,QString type){
    sinNode *node=new sinNode;
    node->lx = point.x();
    node->ly = point.y();

    node->oprt = math;
    node->right = right;
    node->controlsId=id;
    node->identifier=type;
    QString cid = QString::number(node->controlsId,10);
    node->name = node->identifier + cid;
    qDebug()<<"Create():";
    qDebug()<<"name :"<<node->name;
    qDebug()<<"identifier :"<<node->identifier;
    qDebug()<<"controlsId :"<<node->controlsId;

    WidgetWrap* tmp = new WidgetWrap(node);   //包装节点
    Map.insert(tmp->name,tmp);            //添加到widgetmap中
    node->yuan2->master = tmp;
    node->yuan2->name = "yuan2";
    node->yuan->master = tmp;
    node->yuan->name = "yuan";
    node->yuan3->master = tmp;
    node->yuan3->name = "yuan3";
}

bool format:: CreateLog(QPointF point,int id,QString math,QString left,QString right,QString type){
    logNode *node=new logNode;
    node->lx = point.x();
    node->ly = point.y();

    node->oprt = math;
    node->left = left;
    node->right = right;
    node->controlsId=id;
    node->identifier=type;
    QString cid = QString::number(node->controlsId,10);
    node->name = node->identifier + cid;
    qDebug()<<"Create():";
    qDebug()<<"name :"<<node->name;
    qDebug()<<"identifier :"<<node->identifier;
    qDebug()<<"controlsId :"<<node->controlsId;

    WidgetWrap* tmp = new WidgetWrap(node);   //包装节点
    Map.insert(tmp->name,tmp);            //添加到widgetmap中
    node->yuan->master = tmp;
    node->yuan->name = "yuan";
    node->yuan2->master = tmp;
    node->yuan2->name = "yuan2";
    node->yuan3->master = tmp;
    node->yuan3->name = "yuan3";
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
    node->yuan2->master = tmp;
    node->yuan2->name = "yuan2";
    node->yuan->master = tmp;
    node->yuan->name = "yuan";
    node->node1->yuan->master = tmp;
    node->node1->yuan->name = "n1yuan";
    node->node2->yuan->master = tmp;
    node->node2->yuan->name = "n2yuan";
    node->node3->yuan->master = tmp;
    node->node3->yuan->name = "n3yuan";
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
    node->yuan2->master = tmp;
    node->yuan2->name = "yuan2";
    node->yuan->master = tmp;
    node->yuan->name = "yuan";
    node->node1->yuan->master = tmp;
    node->node1->yuan->name = "n1yuan";
    node->node2->yuan->master = tmp;
    node->node2->yuan->name = "n2yuan";
    node->node3->yuan->master = tmp;
    node->node3->yuan->name = "n3yuan";
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
    node->yuan2->master = tmp;
    node->yuan2->name = "yuan2";
    node->yuan->master = tmp;
    node->yuan->name = "yuan";
    node->node1->yuan->master = tmp;
    node->node1->yuan->name = "n1yuan";
    node->node2->yuan->master = tmp;
    node->node2->yuan->name = "n2yuan";
    node->node3->yuan->master = tmp;
    node->node3->yuan->name = "n3yuan";
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
    node->node1->yuan->name = "n1yuan";
    node->node2->yuan->master = tmp;
    node->node2->yuan->name = "n2yuan";
    node->node3->yuan->master = tmp;
    node->node3->yuan->name = "n3yuan";
    node->node4->yuan->master = tmp;
    node->node4->yuan->name = "n4yuan";
    node->node5->yuan->master = tmp;
    node->node5->yuan->name = "n5yuan";
    node->node6->yuan->master = tmp;
    node->node6->yuan->name = "n6yuan";
    node->node7->yuan->master = tmp;
    node->node7->yuan->name = "n7yuan";
    node->node8->yuan->master = tmp;
    node->node8->yuan->name = "n8yuan";
    node->node9->yuan->master = tmp;
    node->node9->yuan->name = "n9yuan";
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
    node->yuan2->master = tmp;
    node->yuan2->name = "yuan2";
    node->yuan->master = tmp;
    node->yuan->name = "yuan";
    node->node2->yuan->master = tmp;
    node->node2->yuan->name = "n2yuan";
    return true;
}

bool format::CreateLogic(QPointF point, int id, QString type,int width,int height)
{
    Rec *node=new Rec;
    node->lx = point.x();
    node->ly = point.y();
    node->wide = width;
    node->high = height;

    node->controlsId=id;
    node->identifier=type;
    QString cid = QString::number(node->controlsId,10);
    node->name = node->identifier + cid;
    qDebug()<<"Create():";
    qDebug()<<"name :"<<node->name;
    qDebug()<<"identifier :"<<node->identifier;
    qDebug()<<"controlsId :"<<node->controlsId;

    WidgetWrap* tmp = new WidgetWrap(node);   //包装节点
    node->yuan->master = tmp;
    node->yuan->name = "yuan";
    node->yuan2->master = tmp;
    node->yuan2->name = "yuan2";
    node->yuan3->master = tmp;
    node->yuan3->name = "yuan3";
    node->yuan4->master = tmp;
    node->yuan4->name = "yuan4";
    node->yuan5->master = tmp;
    node->yuan5->name = "yuan5";
    node->yuan6->master = tmp;
    node->yuan6->name = "yuan6";
    node->yuan7->master = tmp;
    node->yuan7->name = "yuan7";
    Map.insert(tmp->name,tmp);            //添加到widgetmap中
//    if(type=="Else"){

//    }else if(type=="If"){
//        node->yuan->master = tmp;
//        node->yuan->name = "yuan";
//        node->yuan2->master = tmp;
//        node->yuan2->name = "yuan2";
//        node->yuan3->master = tmp;
//        node->yuan3->name = "yuan3";
//        node->yuan4->master = tmp;
//        node->yuan4->name = "yuan4";
//        node->yuan5->master = tmp;
//        node->yuan5->name = "yuan5";
//        node->yuan6->master = tmp;
//        node->yuan6->name = "yuan6";
//        node->yuan7->master = tmp;
//        node->yuan7->name = "yuan7";
//    }
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
    if(wfrom.identifier == "Var"){
    }
    if(wfrom.identifier == "VarInstance"){
//        first = dynamic_cast<Yuan*> (wfrom.mVarInstanceNode->yuan);
    }
    if(wfrom.identifier == "Compute"){
        if(fyuan==wfrom.mComputeNode->yuan->name)
            first = dynamic_cast<Yuan*> (wfrom.mComputeNode->yuan);
        if(fyuan==wfrom.mComputeNode->yuan3->name)
            first = dynamic_cast<Yuan*> (wfrom.mComputeNode->yuan3);
    }
    if(wfrom.identifier == "E"){
        if(fyuan==wfrom.mENode->yuan->name)
            first = dynamic_cast<Yuan*> (wfrom.mENode->yuan);
        if(fyuan==wfrom.mENode->yuan3->name)
            first = dynamic_cast<Yuan*> (wfrom.mENode->yuan3);
    }
    if(wfrom.identifier == "Log"){
        if(fyuan==wfrom.mLogNode->yuan->name)
            first = dynamic_cast<Yuan*> (wfrom.mLogNode->yuan);
        if(fyuan==wfrom.mLogNode->yuan3->name)
            first = dynamic_cast<Yuan*> (wfrom.mLogNode->yuan3);
    }
    if(wfrom.identifier == "Sin"){
        if(fyuan==wfrom.mSinNode->yuan->name)
            first = dynamic_cast<Yuan*> (wfrom.mSinNode->yuan);
        if(fyuan==wfrom.mSinNode->yuan3->name)
            first = dynamic_cast<Yuan*> (wfrom.mSinNode->yuan3);
    }
    if(wfrom.identifier == "RangeFinder"){
        if(fyuan == "yuan")
            first = dynamic_cast<Yuan*> (wfrom.mRangeFinderNode->yuan);
        else if(fyuan == "n2yuan")
            first = dynamic_cast<Yuan*> (wfrom.mRangeFinderNode->node2->yuan);
    }
    if(wfrom.identifier == "Channel"){
        if(fyuan == "yuan")
            first = dynamic_cast<Yuan*> (wfrom.mChannelNode->yuan);
        else if(fyuan == "n1yuan")
            first = dynamic_cast<Yuan*> (wfrom.mChannelNode->node1->yuan);
        else if(fyuan == "n2yuan")
            first = dynamic_cast<Yuan*> (wfrom.mChannelNode->node2->yuan);
        else if(fyuan == "n3yuan")
            first = dynamic_cast<Yuan*> (wfrom.mChannelNode->node3->yuan);
        else if(fyuan == "n4yuan")
            first = dynamic_cast<Yuan*> (wfrom.mChannelNode->node4->yuan);
        else if(fyuan == "n5yuan")
            first = dynamic_cast<Yuan*> (wfrom.mChannelNode->node5->yuan);
        else if(fyuan == "n6yuan")
            first = dynamic_cast<Yuan*> (wfrom.mChannelNode->node6->yuan);
        else if(fyuan == "n7yuan")
            first = dynamic_cast<Yuan*> (wfrom.mChannelNode->node7->yuan);
        else if(fyuan == "n8yuan")
            first = dynamic_cast<Yuan*> (wfrom.mChannelNode->node8->yuan);
        else if(fyuan == "n9yuan")
            first = dynamic_cast<Yuan*> (wfrom.mChannelNode->node9->yuan);
    }
    if(wfrom.identifier == "Attitude"){
        if(fyuan == "yuan")
            first = dynamic_cast<Yuan*> (wfrom.mAttitudeNode->yuan);
        else if(fyuan == "n1yuan")
            first = dynamic_cast<Yuan*> (wfrom.mAttitudeNode->node1->yuan);
        else if(fyuan == "n2yuan")
            first = dynamic_cast<Yuan*> (wfrom.mAttitudeNode->node2->yuan);
        else if(fyuan == "n3yuan")
            first = dynamic_cast<Yuan*> (wfrom.mAttitudeNode->node3->yuan);
    }
    if(wfrom.identifier == "Gimbal"){
        if(fyuan == "yuan")
            first = dynamic_cast<Yuan*> (wfrom.mGimbalNode->yuan);
        else if(fyuan == "n1yuan")
            first = dynamic_cast<Yuan*> (wfrom.mGimbalNode->node1->yuan);
        else if(fyuan == "n2yuan")
            first = dynamic_cast<Yuan*> (wfrom.mGimbalNode->node2->yuan);
        else if(fyuan == "n3yuan")
            first = dynamic_cast<Yuan*> (wfrom.mGimbalNode->node3->yuan);
    }
    if(wfrom.identifier == "Battery"){
        if(fyuan == "yuan")
            first = dynamic_cast<Yuan*> (wfrom.mBatteryNode->yuan);
        else if(fyuan == "n1yuan")
            first = dynamic_cast<Yuan*> (wfrom.mBatteryNode->node1->yuan);
        else if(fyuan == "n2yuan")
            first = dynamic_cast<Yuan*> (wfrom.mBatteryNode->node2->yuan);
        else if(fyuan == "n3yuan")
            first = dynamic_cast<Yuan*> (wfrom.mBatteryNode->node3->yuan);
    }
    if(wfrom.identifier == "If"||
            wfrom.identifier == "Else"||
            wfrom.identifier == "While"){
        if(fyuan==wfrom.mLogicNode->yuan->name)
            first = dynamic_cast<Yuan*> (wfrom.mLogicNode->yuan);
        else if(fyuan==wfrom.mLogicNode->yuan4->name)
            first = dynamic_cast<Yuan*> (wfrom.mLogicNode->yuan4);
        else if(fyuan==wfrom.mLogicNode->yuan7->name)
            first = dynamic_cast<Yuan*> (wfrom.mLogicNode->yuan7);
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
    if(wto.identifier == "Var"){
        //Var没有指入和指出
    }
    if(wto.identifier == "VarInstance"){
        second = wto.mVarInstanceNode->yuan2;
    }
    if(wto.identifier == "Compute"){
        if(tyuan == "yuan2")
            second = wto.mComputeNode->yuan2;
    }
    if(wto.identifier == "E"){
        if(tyuan == "yuan2")
            second = wto.mENode->yuan2;
    }
    if(wto.identifier == "Log"){
        if(tyuan == "yuan2")
            second = wto.mLogNode->yuan2;
    }
    if(wto.identifier == "Sin"){
        if(tyuan == "yuan2")
            second = wto.mSinNode->yuan2;
    }
    if(wto.identifier == "IO"){
        second = wto.mIONode->yuan2;
    }
    if(wto.identifier == "RangeFinder"){
        second = wto.mRangeFinderNode->yuan2;
    }
    if(wto.identifier == "Channel"){
        second = wto.mChannelNode->yuan2;
    }
    if(wto.identifier == "Attitude"){
        second = wto.mAttitudeNode->yuan2;
    }
    if(wto.identifier == "Gimbal"){
        second = wto.mGimbalNode->yuan2;
    }
    if(wto.identifier == "Battery"){
        second = wto.mBatteryNode->yuan2;
    }
    if(wto.identifier == "If"||
            wto.identifier == "Else"||
            wto.identifier == "While"){
        if(tyuan==wto.mLogicNode->yuan2->name)
            second = wto.mLogicNode->yuan2;
        else if(tyuan==wto.mLogicNode->yuan3->name)
            second = wto.mLogicNode->yuan3;
        else if(tyuan==wto.mLogicNode->yuan5->name)
            second = wto.mLogicNode->yuan5;
        else if(tyuan==wto.mLogicNode->yuan6->name)
            second = wto.mLogicNode->yuan6;
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


