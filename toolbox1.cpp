#include "toolbox1.h"
#include<QGroupBox>
#include<QVBoxLayout>
#include<QMessageBox>
#include<QDateTime>
#include<QHostAddress>
#include"ud.h"
#include<QFile>
#include<QTextStream>

toolbox1::toolbox1(QWidget *parent) :
    QToolBox(parent)
{
    QFile fi("D:/temp/a/QQ/record/ip.txt");
    if(!fi.open(QIODevice::ReadOnly)) exit(0);
    QTextStream qs(&fi);
    uip=qs.readLine();
    upt=qs.readLine().toInt();
    fi.close();
    setWindowIcon(QPixmap(":/ima/0.png"));
    log();
   connect(sk,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(sockerror(QAbstractSocket::SocketError)));
   connect(sk,SIGNAL(connected()),this,SLOT(sockcon()));
   connect(sk,SIGNAL(readyRead()),this,SLOT(sockrecv()));
}
void toolbox1::log(){
    UD u;
    u.exec();
    if(!u.islogin){
        sk->close();
        exit(0);
    }
    uid=u.usid;
    ups=u.pass;
   sk->connectToHost(QHostAddress(uip),upt);
}

void toolbox1::sockerror(QAbstractSocket::SocketError serr)
{
    if(serr==QAbstractSocket::RemoteHostClosedError){
    QMessageBox::information(this,"登录失败",err);
    }else{
   QMessageBox::information(this,"登录失败",sk->errorString());
    }
     sk->close();
     exit(0);
 }
void toolbox1::sockcon()
{
    char buf[1024];
    buf[0]=1;
    buf[1]=uid;
    for(int i=0;i!=ups.size();++i){
        buf[2+i]=ups[i].unicode();
    }
    sk->write(buf,2+ups.size());
}

void setuse(QVector<user> &v,const QString&s){
QStringList sl=s.split(".");
for(int i=0;i!=sl.size();++i){
    QStringList ssl=sl[i].split(",");
    v.push_back(user(ssl[0].toInt(),ssl[1],ssl[2].toInt()));
}
}
void toolbox1::sockrecv(){
    QByteArray ba=sk->readAll();
    int t=ba.at(0),t1 ;
    switch (t) {
    case 0:
        err="密码错误";
        return;
    case 1:
        err="账号不存在";
       return;
    case 2:
    {
         ba.remove(0,1);
         setuse(map,QString(ba));
         int_bt();
        return;
    }
    case 3:{
       t1=ba[1];
      auto itw=wid.find(t1);
      if(itw==wid.cend()) return;
      Widget* w=itw.value();
      if(w->ishow){
          QString stm=w->ur.getname()+"["+QDateTime::currentDateTime().toString("yyyy-MM-dd hh:ss:ss")+"]:"
          +"\n"+QString(ba.remove(0,2));
        w->settx(stm);
        return;
       }
        break;
    }
    case 4:
    {
        t1=ba[1];
        wid.find(t1).value()->ur.setT(1);
        break;
    }
    case 5:
        t1=ba[1];
         wid.find(t1).value()->ur.setT(0);
        break;
    default:
        return;
    }
    settip(t,t1);
}
toolbox1::~toolbox1(){
}

bool toolbox1::eventFilter(QObject *wa, QEvent *ev)
{
    if(ev->type()==QEvent::MouseButtonPress){
        int i=wa->objectName().toInt();
        Widget *tem=wid.find(i).value();
        tem->ishow=true;
        toolbt.find(i).value()->setToolTip((tem->ur.getT()==0?tr("离线"):tr("在线")));  
       tem->showNormal();
    }
    return QToolBox::eventFilter(wa,ev);
}

/*void toolbox1::bt(toolbox1::TB b)
{
    QRect qr=b->geometry();
    b->setGeometry(qr.x()+j,qr.y(),qr.width(),qr.height());
    j=-j;
}*/
void toolbox1::settip(int T,int T1){
   QString s;
   if(T==3) {
       s="有消息";
      }
   else if(T==4) s="在线";
     else if(T==5) s="离线";
   toolbt.find(T1).value()->setToolTip(s);
}

void toolbox1::closeEvent(QCloseEvent *evt)
{
    if(QMessageBox::question(this,"提示","是否退出?",QMessageBox::Yes|QMessageBox::No,QMessageBox::No)
              ==QMessageBox::Yes)
     {
       auto it=wid.begin();
       while(it!=wid.end()){
           delete it.value();
           ++it;
       }
          sk->close();
     evt->accept();
  }else{
      evt->ignore();
  }
}

void toolbox1::int_bt(){
     auto itb=map.cbegin();
     setWindowTitle(itb ->getname());
     ++itb;
     int x=1;
      while(itb!=map.cend()){
         QToolButton *tem=new QToolButton();
        tem->setObjectName(QString::number(itb->getID()));
        tem->installEventFilter(this);
        tem->setText(itb->getname());
        QString tm=QString(":/ima/")+QString::number((x++)%8)+QString(".png");
       tem->setIcon(QPixmap(tm));
        tem->setIconSize(QPixmap(tm).size());
        tem->setAutoRaise(true);
       tem->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
        tem->setToolTip((itb->getT()==0?tr("离线"):tr("在线")));
        toolbt.insert(itb->getID(),tem);
        wid.insert(itb->getID(),new Widget(QPixmap(tm) ,user(itb->getID(),itb->getname(),itb->getT()),sk));
        itb++;
    }
   QGroupBox *gb[4];
   QVBoxLayout *vl[4];
   QString tem[4]={"好友","陌生人","家人","同学"};
   for(int i=0;i!=4;++i){
       gb[i]=new QGroupBox;
       vl[i]=new QVBoxLayout(gb[i]);
       vl[i]->setMargin(10);
       vl[i]->setAlignment(Qt::AlignHCenter);
       if(i==0){
         auto itm=toolbt.cbegin();
         while(itm!=toolbt.cend()){
           vl[0]->addWidget(itm.value());
           ++itm;
         }
       }
       vl[i]->addStretch();
       addItem((QWidget*)gb[i],tem[i]);
   }
   setFixedSize(200,500);
}

