#include"toolbox1.h"
#include <QApplication>
#include<QVector>
#include"ud.h"
#include"user.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    toolbox1  w;
    w.show();
    return a.exec();
}
