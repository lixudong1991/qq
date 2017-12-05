#ifndef USER_H
#define USER_H
#include<QString>
class user
{
public:
    user();
    user(int x,QString n,int y):id(x),name(n),t(y){}
    int getID()const{
        return id;
    }
   QString getname()const{
       return name;
   }
   int getT()const{
       return t;
   }
   void setname(QString n){
       name=n;
   }
 void setT(int x){
     t=x;
 }
void setid(int i){
    id=i;
}
user(const user& u):id(u.getID()),name(u.getname()),t(u.getT()){}
user& operator=(const user&u);
private:
int id=0;
QString name;
int t=0;
};

#endif // USER_H
