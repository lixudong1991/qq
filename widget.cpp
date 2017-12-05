#include "widget.h"
#include<QPalette>
#include<QHBoxLayout>
#include<QFile>
#include<QDataStream>
#include<QColorDialog>
#include<QScrollBar>
#include<QDateTime>
Widget::Widget(const QIcon ico ,user u,PS p,QWidget *parent )
    : QWidget(parent)
{
    ur=u;
    ps=p;
    setWindowTitle(ur.getname());
   setWindowIcon(ico);
    setAutoFillBackground(true);
    QPalette pa;
    pa.setColor(QPalette::Background,QColor(Qt::darkCyan));
    setPalette(pa);
    int_wid();
    ishow=false;

}

Widget::~Widget()
{

}

void Widget::settx(QString &s)
{
    tx->append(s);
      tx->verticalScrollBar()->setValue(tx->verticalScrollBar()->maximum());
}
void Widget::txapend()
{
     tx->verticalScrollBar()->setValue(tx->verticalScrollBar()->maximum());
    if(ur.getT()==0){
        QColor t=tx->textColor();
        tx->setTextColor(QColor("red"));
        tx->append("系统消息:"+ur.getname()+"不在线");
         tx->setTextColor(t);
        return;
    }
    QString stm="["+QDateTime::currentDateTime().toString("yyyy-MM-dd hh:ss:ss")+"]"
    +"\n",mes=le->toPlainText();//      ->    text();
            tx->append(stm+mes);
      char buf[4096];
      memset(buf,0,4096);
      buf[0]=0;
      buf[1]=(int)ur.getID();
      std::string s=mes.toStdString();
      for(unsigned i=0;i!=s.size();++i){
          buf[2+i]=s[i];
      }
         ps->write(buf,s.size()+2);
         le ->clear();
}
void Widget::save()
{
     QFile fi("E:/c/qtpro/QQ/record/"+QString::number(ur.getID()));
     if (!fi.open(QIODevice::Append|QIODevice::ReadOnly|QIODevice::WriteOnly))
            return;
      QDataStream ts(&fi);
      QString str(tx->toPlainText());
      ts<<str;
      fi.close();
}

void Widget::setfont(const QFont &f)
{
    tx->setCurrentFont(f);
    tx->setFontPointSize(cb->currentText().toDouble());
    if(tb1->isChecked()) tx->setFontWeight(QFont::Bold);
    else  tx->setFontWeight(QFont::Normal);
    if(tb2->isChecked())  tx->setFontItalic(true);
    else   tx->setFontItalic(false);
    if(tb3->isChecked()) tx->setFontUnderline(true);
    else tx->setFontUnderline(false);
    tx->setTextColor(cr);
    le->setFocus();
}

void Widget::setcb(const QString &s)
{
    tx->setFontPointSize(s.toDouble());
     le->setFocus();
}

void Widget::settb1(bool b)
{
    if(b) tx->setFontWeight(QFont::Bold);
    else  tx->setFontWeight(QFont::Normal);
    le->setFocus();
}

void Widget::settb2(bool b)
{
    tx->setFontItalic(b);
  le->setFocus();
}

void Widget::settb3(bool b)
{
    tx->setFontUnderline(b);
      le->setFocus();
}

void Widget::settb4()
{
    cr=QColorDialog::getColor(cr,this);
    if(cr.isValid()){
        tx->setTextColor(cr);
        le->setFocus();
    }
}

void Widget::int_wid()
{
    tx=new QTextBrowser();
    tx->setStyleSheet("background-image:url(:/ima/b2.jpg)");
    QHBoxLayout *hte=new QHBoxLayout;
    hte->addWidget(tx);

    fc=new QFontComboBox;
    fc->setToolTip(tr("字体"));

    cb=new QComboBox;
    cb->setToolTip(tr("字号"));
    cb->setEditable(true);
    for(int i=8;i!=23;++i){
        cb->addItem(QString::number(i));
    }
    cb->setCurrentIndex(4);

    tb1=new QToolButton;
    tb1->setToolTip(tr("加粗"));
    tb1->setText(tr("加粗"));
    tb1->setCheckable(true);
    tb1->setAutoRaise(true);
    tb1->setIcon(QPixmap(":/ima/bold.png"));

    tb2=new QToolButton;
    tb2->setToolTip(tr("斜体"));
    tb2->setText(tr("斜体"));
    tb2->setCheckable(true);
    tb2->setAutoRaise(true);
    tb2->setIcon(QPixmap(":/ima/italic.png"));

    tb3=new QToolButton;
    tb3->setToolTip(tr("下划线"));
    tb3->setText(tr("下划线"));
    tb3->setCheckable(true);
    tb3->setAutoRaise(true);
    tb3->setIcon(QPixmap(":/ima/under.png"));

    tb4=new QToolButton;
    tb4->setToolTip(tr("颜色"));
    tb4->setText(tr("颜色"));
    tb4->setCheckable(true);
    tb4->setAutoRaise(true);
    tb4->setIcon(QPixmap(":/ima/color.png"));

    tb5=new QToolButton;
    tb5->setText(tr("保存"));
     tb5->setToolTip(tr("保存聊天信息"));
    tb5->setAutoRaise(true);
    tb5->setAutoRaise(true);
    tb5->setIcon(QPixmap(":/ima/save.png"));
    QHBoxLayout *htm=new QHBoxLayout();
    htm->addWidget(fc);
    htm->addWidget(cb);
    htm->addWidget(tb1);
    htm->addWidget(tb2);
    htm->addWidget(tb3);
    htm->addWidget(tb4);
    htm->addWidget(tb5);

    le=new QTextEdit;
    le->setFixedHeight(100);
    pb=new QPushButton(tr("发送"));
   pb->setIcon(QPixmap(":/ima/send.png"));
    QHBoxLayout *lhb=new QHBoxLayout;
    lhb->addWidget(le);
    lhb->addWidget(pb);
    QVBoxLayout *mainlhb=new QVBoxLayout;
    mainlhb->addLayout(hte);
    mainlhb->addLayout(htm);
    mainlhb->addLayout(lhb);
    setLayout(mainlhb);
    tx->setCurrentFont(fc->font());
    tx->setFontPointSize(cb->currentText().toDouble());
    le->setFocus();
   connect(pb,SIGNAL(clicked()),this,SLOT(txapend()));
   connect(tb5,SIGNAL(clicked()),this,SLOT(save()));
   connect(tb1,SIGNAL(clicked(bool)),this,SLOT(settb1(bool)));
   connect(tb2,SIGNAL(clicked(bool)),this,SLOT(settb2(bool)));
   connect(tb3,SIGNAL(clicked(bool)),this,SLOT(settb3(bool)));
   connect(tb4,SIGNAL(clicked()),this,SLOT(settb4()));
   connect(fc,SIGNAL(currentFontChanged(QFont)),this,SLOT(setfont(QFont)));
   connect(cb,SIGNAL(currentIndexChanged(QString)),this,SLOT(setcb(QString)));

       QFile fi("/home/lxd/c++/qt/QQ/record/"+QString::number(ur.getID()));
        if (!fi.open(QIODevice::ReadOnly))return;
        QDataStream ts(&fi);
        QString str;
        ts>>str;
        tx->setText(str);
        fi.close();
}

void Widget::closeEvent(QCloseEvent *ev)
{
    ishow=false;
    ev->accept();
}
