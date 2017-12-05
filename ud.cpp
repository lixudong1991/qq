#include "ud.h"
#include<QGridLayout>
#include<QHBoxLayout>
#include<QVBoxLayout>
#include<QMessageBox>
UD::UD( QWidget *parent) :
    QDialog(parent)
{
QGridLayout *gl=new QGridLayout();
gl->addWidget(la,0,0);
ea->setMaxLength(10);
gl->addWidget(ea,0,1);
gl->addWidget(lb,1,0);
eb->setEchoMode(QLineEdit::Password);
eb->setMaxLength(20);
gl->addWidget(eb,1,1);
//gl->addWidget(ba,4,0);
//gl->addWidget(bb,4,1);
QHBoxLayout *hb=new QHBoxLayout();

hb->addWidget(ba);
hb->addWidget(bb);
hb->setSpacing(20);
QVBoxLayout *vb=new QVBoxLayout(this);
vb->addLayout(gl);
vb->addLayout(hb);
vb->setMargin(20);
setWindowTitle("登陆");
this->setFixedSize(220,150);

connect(ba,SIGNAL(clicked()),this,SLOT(udba()));
connect(bb,SIGNAL(clicked()),this,SLOT(udbb()));

islogin=false;
 setWindowIcon(QPixmap(":ima/0.png"));
  la->setFocus();

//setWindowFlags(Qt::FramelessWindowHint);
this->setAutoFillBackground(true);
QPalette pap;
pap.setBrush(QPalette::Background,QBrush(QPixmap(":/ima/b1.jpg")));
setPalette(pap);
}

void UD::udba()
{
    if(ea->text().isEmpty()){
       QMessageBox::information(this,"错误","号码不能为空");
       ea->setFocus();
       return;
    }
    if(eb->text().isEmpty()){
       QMessageBox::information(this,"错误","密码不能为空");
       eb->setFocus();
       return;
    }
    bool b;
    usid=ea->text().toInt(&b);
    if(!b){
        QMessageBox::information(this,"错误","号码只能是数字");
        ea->clear();
        ea->setFocus();
        return;
    }
    pass=eb->text();
    islogin=true;
    close();
}

void UD::udbb()
{
    close();
}

