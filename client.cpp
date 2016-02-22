#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <netdb.h>
#include <math.h>
#include <sys/time.h>
#include <stdlib.h>

using namespace std;

int fd,reqLen;
sockaddr_in saddr;
struct hostent *server;
//char data[500000];
char request[1000];
int Length;
char *type;
struct timeval cs,ce,c;

struct HTTP
{
    char *method;
    char *file_name;
    char *protocol;
    char *version;
    char *host;
    char *port;
    char *length;
    char *status;
    char *type;
    char *connection;
}req,res;


int atoi(char a[])
{
    int len = strlen(a);
    int i=0,res=0;
    while(i < len )
    {
	res = res + ( pow(10,len-i-1)*((int)(a[i]-48)));
	i++;
    }
    return res;
}
char* parseResponse(char *data)
{
    char *t;

    char *temp = (char*) malloc(sizeof(char)*strlen(data));

    for(int i = 0; i < strlen(data); i++)
    {
	temp[i] = data[i];
    }
    char a[] = "Content-Length:";
    t = strtok(temp,"\n ");
    while(t != NULL)
    {
	if(memcmp(a,t,14)==0)
	{
		t = strtok(NULL,"\r\n");
		break;
	}
	t = strtok(NULL,"\n ");
    }

    Length = atoi(t);

    char b[]= "\r\n\r\n";
    t = strstr(data,b);

    return &t[3];
}

void createRequest(char type[],char file_name[],char host[], char port[],char request[])
{
	bzero(request,1000);
    int len = 0,i=0,k=0,j=0;

    char *resource;
    char protocol[] = "HTTP";
    char version[] = "1.1";
    char method[] = "GET";
    char a[] = "Host: ";
    char b[] = "Accept: text/html\r\n";
    char c[] = "Connection: ";
    char d[4];

    while(k < strlen(method))
    {
        request[i++] = method[k++];
    }
    k=0;
    request[i++] = ' ' ;
    request[i++] = '/';
    while(k < strlen(file_name))
    {
        request[i++] = file_name[k++];
    }
    k=0;
    request[i++] = ' ';
    while(k < strlen(protocol))
    {
        request[i++] = protocol[k++];
    }
    k=0;
    request[i++] = '/';
    while(k < strlen(version))
    {
        request[i++] = version[k++];
    }
    k=0;
    request[i++] = '\r';
    request[i++] = '\n';
    while(k < strlen(a))
    {
        request[i++] = a[k++];
    }
    k=0;
    while(k < strlen(host))
    {
        request[i++] = host[k++];
    }
    k=0;
    request[i++] = ':';
    while(k < strlen(port))
    {
        request[i++] = port[k++];
    }
    k=0;
    request[i++] = '\r';
    request[i++] = '\n';
    while(k < strlen(b))
    {
        request[i++] = b[k++];
    }
    k=0;
    while(k < strlen(c))
    {
        request[i++] = c[k++];
    }
    k=0;
    while(k < strlen(type))
    {
        request[i++] = type[k++];
    }
    k=0;
    request[i++] = '\r';
    request[i++] = '\n';
    request[i++] = '\r';
    request[i++] = '\n';

    reqLen = i;

        while(k < reqLen )
        {
           cout << request[k++];
        }

}

void read_write()
{
    struct timeval s,e;
    char ch,ch2;
    char file[30];
    char f2[]="no";
    char a[] = "close";
    char b[] = "keep-alive";
    char *f,*c;
    char *d;
    char data[1000];
    unsigned int fp=0;
    do
    {
	gettimeofday(&s,NULL);
	send(fd,request,1000,0);
	bzero(data,1000);
    recv(fd,data,1000,0);
    d=parseResponse(data);
    puts(d);
    fp = strlen(d);
    while(fp < Length)
    {
        bzero(data,1000);
        recv(fd,data,1000,0);
        puts(data);
        fp += 1000;
    }

	gettimeofday(&e,NULL);
	cout << "\nRetrieval Time : " << (e.tv_sec*1000000 + e.tv_usec)-(s.tv_sec*1000000 + s.tv_usec)+(::c.tv_sec*1000000+::c.tv_usec);
	::c.tv_sec = 0;
	::c.tv_usec =0;
	if(strncmp(type,"close",5)==0)
        break;

    cout << "want to retrieve  more ? ";
    cin >> ch;
    if(ch != 'y' && ch != 'Y' )
	{

        close(fd);
        exit(0);
	}
	else
	{
        cout << "\nEnter File to be retrieved : ";
        cin >> file;
        c=b;
        f = file;
        bzero(request,1000);
        createRequest(c,f,req.host,req.port,request);
        puts(request);
	}

    }while(true);

}

void startClient(char a[],char b[])
{
    fd = socket(AF_INET,SOCK_STREAM,0);

    if(fd < 0)
    {
        cout << "Invalid Socket";
    }

    server = gethostbyname(a);

    saddr.sin_family = AF_INET;
    bcopy((char *)server->h_addr,(char *)&saddr.sin_addr.s_addr,server->h_length);



    int k = atoi(b);
    saddr.sin_port = htons(k);
    gettimeofday(&cs,NULL);
    if(connect(fd,(sockaddr *)&saddr,sizeof(saddr)) < 0)
    {
        cout << "\nCould not connect!!\n";
    }
    gettimeofday(&ce,NULL);
    c.tv_sec = ce.tv_sec-cs.tv_sec;
    c.tv_usec = ce.tv_usec-cs.tv_usec;
    //c=(ce.tv_sec*1000000 + ce.tv_usec)-(cs.tv_sec*1000000 + cs.tv_usec);

}
int main(int argc, char *argv[])
{
    if (argc == 1)
    {
	cout << "\nInsufficient Arguments\n";
                return 0;
    }
    for (int i =0; i < argc; i++)
    {
	if(argv[i] == NULL)
	{
		cout << "\nInsufficient Arguments\n";
		return 0;
	}
    }
    req.host = argv[1];
    req.port = argv[2];
    startClient(req.host,req.port);
    bzero(request,1000);
    type=argv[3];
    bzero(request,1000);
    createRequest(argv[3],argv[4],req.host,req.port,request);
    read_write();
    return 0;
}
