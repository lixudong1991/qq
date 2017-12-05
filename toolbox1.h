#ifndef TOOLBOX1_H
#define TOOLBOX1_H

#include <QToolBox>
#include<QToolButton>
#include<QVector>
#include<QEvent>
#include<QVector>
#include<QMap>
#include<QTcpSocket>
#include<QCloseEvent>
#include"widget.h"
#include"user.h"
class toolbox1 : public QToolBox
{
    Q_OBJECT
    typedef QToolButton* TB;
public:
    explicit toolbox1(QWidget *parent = 0);
    ~toolbox1();

signals:

public slots:
bool eventFilter(QObject *wa, QEvent *ev);
//void bt(TB b);
void sockerror(QAbstractSocket::SocketError);
void sockrecv();
void sockcon();
private:
int uid;
QString ups;
QString uip;
int upt;
QString err;
  toolbox1(const toolbox1& tb);
   toolbox1& operator =(const toolbox1& tb);
QMap<int,TB> toolbt;
QVector<user> map;
QMap<int,Widget*> wid;
 QTcpSocket* sk=new QTcpSocket();
void int_bt();
void log();
void settip(int T,int T1);
 void closeEvent(QCloseEvent *);
};

#endif // TOOLBOX1_H
