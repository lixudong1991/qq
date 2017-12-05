#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include<QPushButton>
#include<QTextBrowser>
#include<QFontComboBox>
#include<QComboBox>
#include<QToolButton>
#include<QColor>
#include<QFont>
#include<QTcpSocket>
#include<QCloseEvent>
#include"user.h"
class Widget : public QWidget
{
    Q_OBJECT
   using PS= QTcpSocket* ;
public:
    Widget(const QIcon ico , user u,PS p,QWidget *parent = 0);
    ~Widget();
    void settx(QString &s);
    bool ishow;
     user ur;
private slots:
    void txapend();
    void save();
    void setfont(const QFont &f);
    void setcb(const QString &s);
    void settb1(bool);
    void settb2(bool);
    void settb3(bool);
    void settb4();
private:
    void int_wid();
    QPushButton *pb;

    QTextBrowser *tx;
    QFontComboBox *fc;
    QComboBox *cb;

    QToolButton *tb1;
    QToolButton *tb2;
    QToolButton *tb3;
    QToolButton *tb4;
    QToolButton *tb5;

    QTextEdit *le;
    QColor cr;
    PS ps;
void closeEvent(QCloseEvent *);
};

#endif // WIDGET_H
