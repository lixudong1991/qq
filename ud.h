#ifndef UD_H
#define UD_H

#include <QDialog>
#include<QLabel>
#include<QLineEdit>
#include<QPushButton>
#include"user.h"
class UD : public QDialog
{
    Q_OBJECT
public:
    explicit UD( QWidget *parent = 0);
    int usid;
    QString pass;
    bool islogin;
signals:

public slots:
void udba();
void udbb();
  private:
    QLabel *la=new QLabel(tr("号码:")),*lb=new QLabel(tr("密码:"));
    QLineEdit *ea=new QLineEdit,*eb=new QLineEdit;
    QPushButton *ba=new QPushButton(tr("登陆")),*bb=new QPushButton(tr("取消"));
};


#endif // UD_H
