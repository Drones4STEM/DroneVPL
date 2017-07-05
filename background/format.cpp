/********************************************************
 * File: format.cpp
 * Author: Gao Jiefeng
 * Description:This file realizes two functions about save and compile.
 *    It can save block diagram of widgets and write it into xml file.
 *    It can also write python code according to topological sequence
 *    of block diagram.
*********************************************************/

#include "format.h"
#include "E:dachuang/QT_project/workspace/diagram1/Link.h"

/*****************************************************
 * Function name: format
 * Description: This is the constructor.
 * Calle: none
 * Input: QMap<QString, widget *> - widgets exits now
 * Output: none
 *****************************************************/
format::format(QObject *parent, QMap<QString, widget *> *m) : QObject(parent)
{
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
    typename QMap<QString, widget*>::iterator iter;
    QFile file("E:/QT_project/workspace/"+filename);
    QXmlStreamWriter stream(&file);

    stream.setAutoFormatting(true);
    stream.writeStartDocument("widgets");

    for(iter=Map->begin(); iter!=Map->end(); iter++){   //遍历控件指针
        widget_convert_to_xml(iter,stream);
    }

    stream.writeEndDocument();
    file.close();
    return true;
}


/*****************************************************
 * Function name: widget_convert_to_xml
 * Description: This is the funtion that save one widget
 *    in xml file. All it's properties will be described
 *    in xml.
 * Calle: none
 * Input: widget* w - the widget to be saved
 *        QXmlStreamWriter stream - the file stream to operate the xml file
 * Output: none
 *****************************************************/
/*
void widget_convert_to_xml(widget* w, QXmlStreamWriter stream)  //保存一个控件，一个lOGIC算一个控件
{
    if( w->compare(0,3,"VAR")==0 ){    //是VAR控件
        stream.writeStartElement("widget");
        stream.writeAttribute("sort", "VAR");
         stream.writeStartElement("VAR");
         stream.writeAttribute("name", iter->key);
          stream.writeTextElement("location_x",iter->value->pos().x());
          stream.writeTextElement("location_y",iter->value->pos().y());
          stream.writeTextElement("type",iter->);
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
    //---------IO------------------
    if( w->key.compare(0,1,"IO")==0 ){
        stream.writeStartElement("widget");
        stream.writeAttribute("sort", "IO");
         stream.writeStartElement("IO");
         stream.writeAttribute("name", iter->key);
          stream.writeTextElement("location_x",iter->value->pos().x());
          stream.writeTextElement("location_y",iter->value->pos().y());
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
          }
         stream.writeEndElement();
        stream.writeEndElement();
    }
    //---------Action------------------
    if( w->key.compare(0,4,"Action")==0 ){
        stream.writeStartElement("widget");
        stream.writeAttribute("sort", "Action");
         stream.writeStartElement("Action");
         stream.writeAttribute("name", iter->key);
          stream.writeTextElement("location_x",iter->value->pos().x());
          stream.writeTextElement("location_y",iter->value->pos().y());
          stream.writeStartElement("arrow_out");
           stream.writeTextElement("goto",);
          stream.writeEndElement();
          if(){ //起飞动作
              stream.writeTextElement("motion","take_off");
              stream.writeTextElement("pattern",);
          }
          if(){ //降落动作
              stream.writeTextElement("motion","land");
              stream.writeTextElement("pattern",);
          }
          if(){ //左移动作
              stream.writeTextElement("motion","move_left");
              stream.writeStartElement("speed");
              stream.writeAttribute("unit","m/s");
              stream.writeCharacters();
              stream.writeEndElement();
          }
          if(){ //右移动作
              stream.writeTextElement("motion","move_right");
              stream.writeStartElement("speed");
              stream.writeAttribute("unit","m/s");
              stream.writeCharacters();
              stream.writeEndElement();
          }
          if(){ //上移动作
              stream.writeTextElement("motion","move_up");
              stream.writeStartElement("speed");
              stream.writeAttribute("unit","m/s");
              stream.writeCharacters();
              stream.writeEndElement();
          }
          if(){ //下移动作
              stream.writeTextElement("motion","move_down");
              stream.writeStartElement("speed");
              stream.writeAttribute("unit","m/s");
              stream.writeCharacters();
              stream.writeEndElement();
          }
          if(){ //前移动作
              stream.writeTextElement("motion","move_forward");
              stream.writeStartElement("speed");
              stream.writeAttribute("unit","m/s");
              stream.writeCharacters();
              stream.writeEndElement();
          }
          if(){ //后移动作
              stream.writeTextElement("motion","move_back");
              stream.writeStartElement("speed");
              stream.writeAttribute("unit","m/s");
              stream.writeCharacters();
              stream.writeEndElement();
          }
          if(){ //左转动作
              stream.writeTextElement("motion","turn_left");
              stream.writeStartElement("angle");
              stream.writeAttribute("unit","angle");
              stream.writeCharacters();
              stream.writeEndElement();
          }
          if(){ //右转动作
              stream.writeTextElement("motion","turn_right");
              stream.writeStartElement("angle");
              stream.writeAttribute("unit","angle");
              stream.writeCharacters();
              stream.writeEndElement();
          }
          if(){ //悬停动作
              stream.writeTextElement("motion","hang_on");
              stream.writeStartElement("pattern");
              stream.writeCharacters();
              stream.writeEndElement();
          }
          if(){ //延时动作
              stream.writeTextElement("motion","none");
          }
          stream.writeStartElement("time");
          stream.writeAttribute("unit","second");
          stream.writeCharacters();
          stream.writeEndElement();
    }
    //---------Compute------------------
    if( w->key.compare(0,6,"Compute")==0 ){
        stream.writeStartElement("widget");
        stream.writeAttribute("sort","Compute");
         stream.writeStartElement("Compute");
         stream.writeAttribute("name",);
          stream.writeTextElement("location_x",iter->value->pos().x());
          stream.writeTextElement("location_x",iter->value->pos().y());
          stream.writeStartElement("arrow_out");
           stream.writeTextElement("goto",);
          stream.writeEndElement();
          stream.writeStartElement("operator");
          if(){ //如果加法
              stream.writeAttribute("add");
          }
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
    }
    //---------LOGIC------------------
    if( w->key.compare(0,4,"LOGIC")==0 ){
        stream.writeStartElement("widget");
        stream.writeAttribute("sort","LOGIC");
        QString t;
        t = ;//判断是if还是else还是while
         stream.writeStartElement("LOGIC");
         stream.writeAttribute("name",t);
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
    }
}
*/

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
    typename QMap<QString, widget*>::iterator iter;
    QFile file("E:/QT_project/workspace/" + filename);
    QTextStream in(&file);

    for(iter=Map->begin();iter!=Map->end();iter++){
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
/*
void format<widget>::widget_convert_to_py(QFile* file, widget* w)
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

}
*/






