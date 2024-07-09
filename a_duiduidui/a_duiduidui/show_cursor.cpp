#include "show_cursor.h"
#include "ui_show_cursor.h"
#include "math.h"
show_cursor::show_cursor(QWidget *parent) :
    QWidget(parent),
    ui2(new Ui::show_cursor)
{
    ui2->setupUi(this);
}

show_cursor::~show_cursor()
{
    delete ui2;
}

void show_cursor::settext_1(float value)
{
    if(value<2000)
    {
        ui2->lineEdit->setText(tr("%1").arg(value)+"   ms");
    }
    else if(value<4000&&value>=2000)
    {
        ui2->lineEdit->setText(tr("%1").arg(value-2000)+"   us");
    }
    else
        ui2->lineEdit->setText(tr("%1").arg(value-4000)+"   ns");
}
void show_cursor::settext_2(float value)
{
     if(value<2000)
     {
        ui2->lineEdit_2->setText(tr("%1").arg(value)+"   ms");
     }
     else if(value<4000&&value>=2000)
     {
         ui2->lineEdit_2->setText(tr("%1").arg(value-2000)+"   us");
     }
     else
        ui2->lineEdit_2->setText(tr("%1").arg(value-4000)+"   ns");
}
void show_cursor::settext_3(float value)
{
    if(value*value<0.01)
    {
        ui2->lineEdit_4->setText(tr("%1").arg(value*1000)+"   mv");
    }
    else
         ui2->lineEdit_4->setText(tr("%1").arg(value)+"   V");
}
void show_cursor::settext_4(float value)
{
    if(value*value<0.01)
    {
        ui2->lineEdit_5->setText(tr("%1").arg(value*1000)+"   mv");
    }
    else
         ui2->lineEdit_5->setText(tr("%1").arg(value)+"   V");
}
void show_cursor::settext_5(float value,int a)
{
    if(a==0)
    {
        ui2->lineEdit_3->setText(tr("%1").arg(value)+"   ms");
    }
    else if(a==2000)
    {
        ui2->lineEdit_3->setText(tr("%1").arg(value)+"   us");
    }
    else
        ui2->lineEdit_3->setText(tr("%1").arg(value)+"   ns");
}
void show_cursor::settext_6(float value)
{
    if(value*value<0.01)
    {
        ui2->lineEdit_6->setText(tr("%1").arg(value*1000)+"   mv");
    }
    else
        ui2->lineEdit_6->setText(tr("%1").arg(value)+"   V");
}
