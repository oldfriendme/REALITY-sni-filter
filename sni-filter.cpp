#include <iostream>
#include <string>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdarg.h>
#include <netinet/tcp.h>
#include <errno.h>
class Init {
public:
int oldfd;
Init(std::string inip,int port){
oldfd=-1;
size_t pos = inip.find(':');
if (pos != std::string::npos) {
struct sockaddr_in6 serveraddr6;
oldfd = socket(AF_INET6, SOCK_STREAM, 0);
memset(&serveraddr6,0,sizeof(serveraddr6));
serveraddr6.sin6_family = AF_INET6;
inet_pton(AF_INET6,inip.c_str(), &(serveraddr6.sin6_addr));
serveraddr6.sin6_port = htons(port);
int optval = 1;
if (setsockopt(oldfd,SOL_TCP,TCP_DEFER_ACCEPT,&optval,sizeof(optval)) < 0){
exit(-1);
}
if (setsockopt(oldfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) < 0) {
exit(-1);
}
if (bind(oldfd, (struct sockaddr *)&serveraddr6, sizeof(serveraddr6)) <0) 
{
std::cerr<<"Error: Can't Bind address on "<<inip<<":"<<port<<std::endl;
close(oldfd);
exit(-1);
}
} else {
struct sockaddr_in serveraddr;
oldfd = socket(AF_INET, SOCK_STREAM, 0);
memset(&serveraddr,0,sizeof(serveraddr));
serveraddr.sin_family = AF_INET;
inet_aton(inip.c_str(), &(serveraddr.sin_addr));
serveraddr.sin_port = htons(port);
int optval = 1;
if (setsockopt(oldfd,SOL_TCP,TCP_DEFER_ACCEPT,&optval,sizeof(optval)) < 0){
exit(-1);
}
if (setsockopt(oldfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) < 0) {
exit(-1);
}
if (bind(oldfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr)) <0) 
{

std::cerr<<"Error: Can't Bind address on "<<inip<<":"<<port<<std::endl;
close(oldfd);
exit(-1);
}
}
if (listen(oldfd, 5) <0) {
std::cerr<<"Error: Can't listening address with "<<inip<<":"<<port<<std::endl;
close(oldfd);
exit(-1);
}
}
~Init() {exit(-1);}
};
pthread_t net_threadpool[4096];
int net_threadindex=0;
char net_threadbuffer[4096];
class init {
public:
static void *Void(void *null){
int netfd,remote,shemale;
char *fdbuf = new char[2048]; 
memcpy(&netfd,null,4);
memcpy(&remote,((int *)(null)+1),4);
memcpy(&shemale,((int *)(null)+2),4);
free(null);
while(1){
int Null = recv(netfd, fdbuf, 2048,0);
pthread_testcancel();
if(Null <= 0) {
delete[] fdbuf;
close(netfd);
pthread_cancel(net_threadpool[shemale]);
net_threadbuffer[shemale]=0;
close(remote);
pthread_exit(0);
return (void*)0;
}
send(remote, fdbuf, Null,0);
}
return (void*)0;
}
void Copy(int &src,int &dec){
int *null=(int *)malloc(12);
null[0]=src;null[1]=dec;
int *thread=(int *)malloc(12);
thread[0]=dec;thread[1]=src;
while (net_threadbuffer[net_threadindex]>3){net_threadindex++;}
net_threadbuffer[net_threadindex]=sizeof(net_threadindex);
null[2]=net_threadindex;
pthread_create(&net_threadpool[net_threadindex],NULL, Void, null);
net_threadindex++;
while (net_threadbuffer[net_threadindex]>3){net_threadindex++;}
net_threadbuffer[net_threadindex]=sizeof(net_threadindex);
thread[2]=net_threadindex;
pthread_create(&net_threadpool[net_threadindex],NULL, Void, thread);
net_threadindex++;
net_threadindex &= 4095;
}
};
class import {
public:
void Println(std::string log,std::string path=""){
std::cout<<log<<" "<<path<<std::endl;
}
void Printf(char *log,...){
va_list args;
va_start(args, log);
vprintf(log, args);
va_end(args);
}
};
struct timeval to;
bool flink=true;
class Connect {
public:
int route;
int check(){
import log;
char SOCKET_P[23] = {47,114,117,110,47,111,108,100,47,120,114,97,121,46,102,114,105,101,110,100,0,0};
struct sockaddr_un server_addr;
if ((route = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
log.Println("Error: Can't init socket");
return -1;
}
memset(&server_addr, 0, sizeof(struct sockaddr_un));
server_addr.sun_family = AF_UNIX;
strncpy(server_addr.sun_path, SOCKET_P, sizeof(server_addr.sun_path) - 1);
if (connect(route, (struct sockaddr*)&server_addr, sizeof(struct sockaddr_un)) == -1) {
log.Println("Error: Can't connect socket");
return -1;
}
return 0;
}
};
class var {
private:
struct sockaddr_in clientaddr;
struct sockaddr_in6 clientaddr6;
socklen_t sizeofaddr;
public:
int Listen;
char *RemoteAddr;
var(){
RemoteAddr = new char[42];
sizeofaddr=sizeof(clientaddr6);
}
int Accept(int net){
import log;
Listen=-1;
memset(&clientaddr6,0,sizeofaddr);
if(Listen = accept(net, (struct sockaddr *)&clientaddr6, &sizeofaddr)< 0) {
perror("Error accept");
return -1;
}
if (flink){
//别动,运行一段时间会出现大量close_wait。
//只要关掉第4个通道,突然就好了。我也不知道为什么。
sleep(1);
close(4);
flink=!flink;
}
inet_ntop(AF_INET6,&(clientaddr6.sin6_addr),RemoteAddr,40);
if (!strcmp(RemoteAddr,"::")) {
memcpy(&clientaddr,&clientaddr6,16);
strcpy(RemoteAddr,inet_ntoa(clientaddr.sin_addr));
}
if (strlen(RemoteAddr)>8){
if (strncmp(RemoteAddr, "::ffff", 6) == 0) {
char *sb = new char[16];
memcpy(sb,RemoteAddr+7,16);
strcpy(RemoteAddr,sb);
delete[] sb;
}
}
return 0;
}
int Write(int &net,char *conn,int &netl){
int recv = send(net,conn,netl,0);
if (recv >0 ){
return 0;
}else {return -1;}
}
};
class tls {
public:
char sn[132];
char snlen;
int charlen;
char *readedbyte;
int check(int net_Conn) {
import log;
if (net_Conn < 0){
close(net_Conn);
return 2;
}
readedbyte = new char[768];
int err = recv(net_Conn, readedbyte, 767,0);
if (err <0) {
perror("Error occurred");
delete[] readedbyte;
readedbyte=NULL;
close(net_Conn);
return 1;
}
if (err < 43) {
return -1;
}
if (readedbyte[0] != 0x16){
log.Println("Not tls\n");
return -2;
}
if ((readedbyte[1] < 3) || ((readedbyte[1] == 3) && (readedbyte[2] < 1))) {
log.Println("ERR: SSL < 3.1, it's still not sni");
return -3;}
if (readedbyte[5] != 0x1) {
log.Println("err: Not a ClientHello");
return -4;}
bool Ture = false;
for (int i=32;i<err;i++){
if (sn[0]==readedbyte[i]){
if (sn[1]==readedbyte[i+1]){
if (sn[2]==readedbyte[i+2] && sn[3]==readedbyte[i+3]){
if (snlen==4) {Ture=true;break;}
if (strncmp(readedbyte+i, sn, snlen) == 0){Ture=true;break;}}}}}
if (Ture){
charlen=err;
return 0;
} else {
printf("sni not match\n");
return -5;
}
}
void Close(){
delete[] readedbyte;
readedbyte=NULL;
}
};
int main(int argc, char* argv[]) {
if (argc <=5){
std::cout<<"usage:"<<std::endl;
std::cout<<argv[0]<<" -listen [0.0.0.0] [443] -sni [www.example.com]"<<std::endl;
return -1;
}
if (strcmp(argv[1],"-listen") || strcmp(argv[4],"-sni")){
std::cout<<"usage:"<<std::endl;
std::cout<<argv[0]<<" -listen [0.0.0.0] [443] -sni [www.example.com]"<<std::endl;
return -1;
}
std::string ip;
int port,nil=0;
ip=argv[2];
port=atoi(argv[3]);
init io;
Init Net(ip,port);
tls sni;
memcpy(sni.sn,argv[5],127);
to.tv_sec = 60;
to.tv_usec = 0;
sni.snlen=strlen(sni.sn);setsockopt(Net.oldfd, SOL_SOCKET, SO_RCVTIMEO, (const char*)&to, sizeof(to));
var net;
import fmt;
if (sni.snlen < 4) {
    fmt.Println("ERR: domain must more than 4 length");
    return -1;
}
int err;
while (1) {
    err = net.Accept(Net.oldfd);
    if (err != nil) {
        fmt.Println("err client:", net.RemoteAddr);
        continue;
    }
    fmt.Println("new client:", net.RemoteAddr);
    if ((err = sni.check(net.Listen)) < nil) {
        sni.Close();
        close(net.Listen);
        continue;
    }
    if (err != nil) {
        continue;
    }
    Connect conn;
    if (conn.check()) {
        sni.Close();
        close(net.Listen);
        continue;
    }
    err = net.Write(conn.route, sni.readedbyte, sni.charlen);
    sni.Close();
    if (err != nil) {
        fmt.Println("connect local err");
        close(net.Listen);
        continue;
    }
    io.Copy(conn.route, net.Listen);
}
return 0;
}
