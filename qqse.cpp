#include"unp.h"
#include"user.h"
#include<map>
#include<fstream>
#include<mysql.h>
#include<time.h>
using namespace std;
void setdae()
{
 pid_t pid,sid;
 pid=fork();
 if(pid<0) exit(EXIT_FAILURE);
 if(pid>0) exit(EXIT_SUCCESS);
 if((sid=setsid())<0) exit(EXIT_FAILURE);
 /*//if(chair("/")<0) exit(EXIT_FAILURE);
 umask(0);
close(STDIN_FILENO);
close(STDOUT_FILENO);
close(STDERR_FILENO);*/
}
string gettime(){
  char buf[100];
  time_t t=time(NULL);
  snprintf(buf,sizeof(buf),"%.24s",ctime(&t));
  return string(buf,strlen(buf)); 
}
int crsock(int port)
{
struct sockaddr_in addr;
int st=socket(AF_INET,SOCK_STREAM,0);
memset(&addr,0,sizeof(addr));
addr.sin_family=AF_INET;
addr.sin_port=htons(port);
addr.sin_addr.s_addr=htonl(INADDR_ANY);
if(bind(st,(struct sockaddr*)&addr,sizeof(addr))<0)
 {
   return -1;
 }
if(listen(st,300)<0)
{
 return -1;
}
return st;
}
void setnob(int sck)
{
 int opts=fcntl(sck,F_GETFL);
 if(opts<0)return;
 opts|=O_NONBLOCK;
 fcntl(sck,F_SETFL,opts);
}
int soaccpt(int sk,ofstream &o)
{
struct sockaddr_in addr;
socklen_t len=sizeof(addr);
char ch[16];
memset(&addr,0,len);
int so=accept(sk,(struct sockaddr*)&addr,&len);
if(so==-1) o<<"accpt error"<<endl;
else{
 o<<"IP: "<<inet_ntop(AF_INET,&addr.sin_addr,ch,16)<<" port:"<<ntohs(addr.sin_port)<<endl;
}
return so;
}
int
strecv(int fd,map<int,int>& ma1,map<int,int> &ma2)
{      
        ssize_t		n;
     	char buf[MAXLINE];//buf1[MAXLINE];
        memset(buf,0,sizeof(buf));
    //    memset(buf1,0,sizeof(buf1));
         n = recv(fd,buf, MAXLINE,0);
        if (n <= 0) return -1;
        if(buf[0]==0){
         int id=buf[1],id1=ma2[fd];
    //     cout<<(int)buf[0]<<" "<<id<<" "<<n<<string(buf+2,n-2)<<endl;
         auto it=ma1.find(id);
         if(it!=ma1.end())
         {
          buf[0]=3;buf[1]=id1;
        //  cout<<(int)buf[0]<<" "<<(int)buf[1]<<" "<<string(buf+2,n-2)<<endl;
          send(it->second,buf,n,0);
         }
        }
        return n;
}
bool log(int fd,map<int,user> &ma,map<int,int>& ma1,map<int,int> &ma2,ofstream &o)
{
  char buf[4096];
  memset(buf,0,4096);
  recv(fd,buf,4096,0);
  int te=buf[1];
  if(ma1.count(te)>0) {
   close(fd);
   return false;
   } 
  auto it=ma.find(te);
  if(it==ma.end()){
    buf[0]=1;
  }else if(it->second.getpass()!=string(buf+2)){
     buf[0]=0;
   }else buf[0]=2;
  if(buf[0]==0||buf[0]==1){
    send(fd,buf,1,0);
     close(fd);
     o<<"ID:"<<te<<" 登录失败["<<gettime()<<"]"<<endl;
    return false;
   }
   string msg=to_string(te)+","+it->second.getname()+",1.";
   vector<int> vi=it->second.getvec();
   for(int i=0;i!=vi.size();++i){
     auto it=ma.find(vi[i]);
     if(it!=ma.end()){
        auto it1=ma1.find(vi[i]); 
        string islo="0";
       if(it1!=ma1.end())
        {
         islo="1";
         buf[0]=4;
         buf[1]=te;
         send(it1->second,buf,2,0);
        }
        string tem=to_string(vi[i])+","+it->second.getname()+","+islo+".";
        msg+=tem;
      }
   }
   memset(buf,0,4096);
   buf[0]=2;
   msg.erase(msg.size()-1);
    for(int i=0;i!=msg.size();++i)
   {
     buf[1+i]=msg[i];
   }
   send(fd,buf,1+msg.size(),0);
   ma1.insert({te,fd});
   ma2.insert({fd,te});
   o<<"ID:"<<te<<" 登录成功["<<gettime()<<"]"<<endl;
   return true;
}
void out(int fd,map<int,user> &ma,map<int,int>& ma1,map<int,int> &ma2,ofstream &o)
{
  char bu[2];
  auto it=ma2.find(fd);
  if(it!=ma2.end())
   {
     int id=it->second;
     bu[0]=5;
     bu[1]=id;
     ma1.erase(id);
     auto it1=ma.find(id);
     o<<"ID:"<<id<<" 退出["<<gettime()<<"]"<<endl;
     vector<int> v=it1->second.getvec();
     for(int i=0;i!=v.size();++i)
     {
       auto it1=ma1.find(v[i]);
       if(it1!=ma1.end())
       {
         send(it1->second,bu,2,0);
       }
     } 
    ma2.erase(fd);
   }

}
bool setmap(map<int,user> &ma,ofstream &o){
 MYSQL ms,*con;
if(mysql_init(&ms)==NULL){
o<<"mysql init error"<<endl;
return false;
}
ifstream in("/home/lxd/c++/w/qqserv/mysqlip.txt");
if(!in){
o<<"读取配置文件失败"<<endl;
return false;
}
string ips,mysqluser,password,database;
getline(in,ips);
getline(in,mysqluser);
getline(in,password);
getline(in,database);
in.close();
if((con=mysql_real_connect(&ms,ips.c_str(),mysqluser.c_str(),password.c_str(),database.c_str(),3306,NULL,0))==NULL)
  {
  o<<"con error"<<mysql_error(&ms)<<endl;
   return false;
  }
string tem="select * from user";
  if(mysql_query(con,tem.c_str())!=0){
     o<<"query error:"<<mysql_error(&ms)<<endl;
    return false;
  }
 MYSQL_RES *rs=mysql_store_result(con);
 if(rs){
  MYSQL_ROW rw;   
  while((rw=mysql_fetch_row(rs))!=NULL)
    {
         user u;
          int id=stoi(rw[0]);
         u.setid(id);
         u.setname(rw[1]);
         u.setpass(rw[2]);
         string txt="select id2 from `use` where id1="; txt.append(rw[0]);
         if(mysql_query(con,txt.c_str())!=0){
           o<<"query error:"<<mysql_error(&ms)<<endl;
             return false;
            }
          MYSQL_RES *rs1=mysql_store_result(con);
          if(rs1){ 
            MYSQL_ROW rw1;
            while((rw1=mysql_fetch_row(rs1))!=NULL)
            u.addvec(stoi(rw1[0]));
           }
          mysql_free_result(rs1);
          ma.insert({id,u});
    }
  mysql_free_result(rs);
}
mysql_close(con);
return true;
}
int main()
{
setdae();
int sock=crsock(9876);
if(sock<0) return -1;
struct epoll_event ev,events[100];
int epfd=epoll_create(100);
setnob(sock);
ev.data.fd=sock;
ev.events=EPOLLIN|EPOLLERR|EPOLLHUP;
epoll_ctl(epfd,EPOLL_CTL_ADD,sock,&ev);
int st=0;
map<int,user> m;
ofstream ou("/home/lxd/c++/w/qqserv/record.txt");
if(!ou){
return 0;
close(epfd);
close(sock);
}
if(!setmap(m,ou)){
ou<<"setmap error"<<endl;
ou.close();
close(epfd);
close(sock);
return 0;
}
map<int,int> m1;
map<int,int> m2;

  while(true)
  {
    int nfds=epoll_wait(epfd,events,100,-1);
    if(nfds==-1) 
    {
      ou<<"epoll wait error"<<endl;
      break;
    }
    for(int i=0;i<nfds;++i)
     {
      if(events[i].data.fd<0) continue;
      if(events[i].data.fd==sock)
      {
       st=soaccpt(sock,ou);
        if(st>=0)
         {
          if(!log(st,m,m1,m2,ou))continue; 
          setnob(st);
          ev.data.fd=st;
          ev.events=EPOLLIN|EPOLLERR|EPOLLHUP;
          epoll_ctl(epfd,EPOLL_CTL_ADD,st,&ev); 
          continue;
         }
      }
       if(events[i].events &EPOLLIN)
       {
        st=events[i].data.fd;
        if(strecv(st,m1,m2)<0)
        {
          close(st);
          out(st,m,m1,m2,ou);
        events[i].data.fd=-1;
        }
       }
       if((events[i].events &EPOLLERR)||(events[i].events &EPOLLHUP))
       {
        st=events[i].data.fd;
        out(st,m,m1,m2,ou);
        close(st);
        events[i].data.fd=-1;
       }
     }
  }
ou.close();
close(epfd);
close(sock);
}
