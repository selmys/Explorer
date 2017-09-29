#include <sys/types.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>

void tolowercase(char *ch){
   int i;
   for(i=0;i<strlen(ch);i++)
     ch[i]=tolower(ch[i]);
}

void parse(char *ch){
   char cc[60],cd[60],*ptr;
   int gotone=0;
   strcpy(cc,ch);
   cc[strlen(cc)-1]='\0';
   ptr=strtok(cc," ");
   if(ptr){
      if(!strcmp(ptr,"north")){strcpy(cd,"n"); gotone=1;}
      if(!strcmp(ptr,"n")){strcpy(cd,"n"); gotone=1;}
      if(!strcmp(ptr,"south")){strcpy(cd,"s"); gotone=1;}
      if(!strcmp(ptr,"s")){strcpy(cd,"s"); gotone=1;}
      if(!strcmp(ptr,"east")){strcpy(cd,"e"); gotone=1;}
      if(!strcmp(ptr,"e")){strcpy(cd,"e"); gotone=1;}
      if(!strcmp(ptr,"west")){strcpy(cd,"w"); gotone=1;}
      if(!strcmp(ptr,"w")){strcpy(cd,"w"); gotone=1;}
      if(!strcmp(ptr,"up")){strcpy(cd,"u"); gotone=1;}
      if(!strcmp(ptr,"u")){strcpy(cd,"u"); gotone=1;}
      if(!strcmp(ptr,"down")){strcpy(cd,"d"); gotone=1;}
      if(!strcmp(ptr,"d")){strcpy(cd,"d"); gotone=1;}
      if(!strcmp(ptr,"value")){strcpy(cd,"v"); gotone=1;}
      if(!strcmp(ptr,"v")){strcpy(cd,"v"); gotone=1;}
      if(!strcmp(ptr,"score")){strcpy(cd,"v"); gotone=1;}
      if(!strcmp(ptr,"help")){strcpy(cd,"h"); gotone=1;}
      if(!strcmp(ptr,"h")){strcpy(cd,"h"); gotone=1;}
      if(!strcmp(ptr,"look")){strcpy(cd,"l"); gotone=1;}
      if(!strcmp(ptr,"l")){strcpy(cd,"l"); gotone=1;}
      if(!strcmp(ptr,"quit")){strcpy(cd,"q"); gotone=1;}
      if(!strcmp(ptr,"q")){strcpy(cd,"q"); gotone=1;}
      if(!strcmp(ptr,"get")){strcpy(cd,"g "); gotone=2;}
      if(!strcmp(ptr,"g")){strcpy(cd,"g "); gotone=2;}
      if(!strcmp(ptr,"put")){strcpy(cd,"p "); gotone=2;}
      if(!strcmp(ptr,"p")){strcpy(cd,"p "); gotone=2;}
      if(!strcmp(ptr,"talk")){strcpy(cd,"t "); gotone=2;}
      if(!strcmp(ptr,"t")){strcpy(cd,"t "); gotone=2;}
   }
   if(gotone==2){
      ptr=strtok(NULL,"\0");
      if(ptr)
         strcat(cd,ptr);
   }
   if(gotone)
      strcpy(ch,cd);
   else
      strcpy(ch,"?");
} 
int main()
{
    int sockfd;
    int len;
    struct sockaddr_in address;
    int result,nread;
    fd_set inputs, testfds;
    struct timeval timeout;
    char ch[1024]="";

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    //sockfd = socket(AF_I2.7NET, SOCK_STREAM, 0);
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr("192.168.2.7");
    address.sin_port = htons(4663);
    len = sizeof(address);
    result = connect(sockfd, (struct sockaddr *)&address, len);

    if(result == -1) {
        perror("oops: client7");
        exit(1);
    }
    FD_ZERO(&inputs);
    FD_SET(0,&inputs);
    FD_SET(sockfd,&inputs);
    printf("You are standing outside an enormous structure,\n");
    printf("filled with rooms, interconnected by passages.\n");
    fflush(stdout); 
    while (ch[0] != 'Q') {
       testfds = inputs;
       timeout.tv_sec = 0;
       timeout.tv_usec = 0;
       result = select(FD_SETSIZE, &testfds, (fd_set *)0, (fd_set *)0, &timeout);
       switch (result){
          case 0: break;
          case -1: perror("select"); break;
          default:
              if(FD_ISSET(0, &testfds)) {
                 ioctl(0,FIONREAD, &nread);
                 if(nread == 0) {
                    printf("got eof on input\n");
                    close(sockfd);
                    exit(0);
                 }
                 else {
                    nread=read(0,ch,nread);
                    ch[nread]=0;  
                    tolowercase(ch); 
                    parse(ch);
                    if(ch[0]!='h')
                       if(ch[0]!='?')
                          write(sockfd, ch, strlen(ch));
                       else
                          printf("Huh?\n");
                    else {
                       printf("You are exploring a gigantic structure located somewhere\n");
                       printf("in the north-western part of what once was the United States.\n");
                       printf("It is the year 2525. Zager and Evans have been\n");
                       printf("dead for half a millennium. You have found this place after\n");
                       printf("studying historical records you have discovered while browsing\n");
                       printf("abandand web sites in your research.\n");
                       printf("You can move about by entering directions such as north,\n");
                       printf("south, east, or west as well as up and down.\n");
                       printf("If you find an object you want, use get or put to take\n");
                       printf("it with you or drop it where you stand.\n");
                       printf("Type in value to see your score or look to see what's\n");
                       printf("arround you. To speak to other explorers type talk followed\n");
                       printf("whatever you want to say, but keep it to one line.\n");
                       printf("When you've had enough enter quit.\n");
                    }
                 }
              }
              if(FD_ISSET(sockfd, &testfds)) {
                 ioctl(sockfd,FIONREAD, &nread);
                 if(nread == 0) {
                    printf("got eof on sockfd\n");
                    exit(0);
                 }
                 else {
                    nread=read(sockfd,ch,nread);
                    ch[nread]=0;  
                    if(ch[0]!='Q'){
		       printf("%s", ch);
		       fflush(stdout);
		    }
                 }
              }
              break; 
       }
    }
    close(sockfd);
    exit(0);
}
