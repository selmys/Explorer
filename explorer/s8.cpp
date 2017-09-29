#include <sys/types.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <string.h>

#include "globals.h"
#include "classes.h"

int main()
{
    int lead[6];
    cave *c=new cave;
    explorer *e;
    lead[0]=1;lead[1]=0;lead[2]=0;lead[3]=0;lead[4]=0;lead[5]=0;
    cavern *w=new cavern(caverns[0], (char *)"e", lead, 0, 0);
    c->add(w);
    lead[0]=0;lead[1]=1;lead[2]=1;lead[3]=2;lead[4]=0;lead[5]=0;
    w=new cavern(caverns[1], (char *)"wnes", lead, 1, 0);
    c->add(w);
    lead[0]=1;lead[1]=2;lead[2]=2;lead[3]=3;lead[4]=0;lead[5]=0;
    w=new cavern(caverns[2], (char *)"nsew", lead, 2, 0);
    c->add(w);
    lead[0]=2;lead[1]=4;lead[2]=0;lead[3]=0;lead[4]=0;lead[5]=0;
    w=new cavern(caverns[3], (char *)"eu", lead, 3, 8);
    c->add(w);
    lead[0]=3;lead[1]=5;lead[2]=7;lead[3]=0;lead[4]=0;lead[5]=0;
    w=new cavern(caverns[4], (char *)"dns", lead, 4, 0);
    c->add(w);
    lead[0]=4;lead[1]=6;lead[2]=0;lead[3]=0;lead[4]=0;lead[5]=0;
    w=new cavern(caverns[5], (char *)"we", lead, 5, 5);
    c->add(w);
    lead[0]=5;lead[1]=7;lead[2]=0;lead[3]=0;lead[4]=0;lead[5]=0;
    w=new cavern(caverns[6], (char *)"ns", lead, 6, 16);
    c->add(w);
    lead[0]=4;lead[1]=6;lead[2]=8;lead[3]=0;lead[4]=0;lead[5]=0;
    w=new cavern(caverns[7], (char *)"wes", lead, 7, 0);
    c->add(w);
    lead[0]=7;lead[1]=9;lead[2]=10;lead[3]=0;lead[4]=0;lead[5]=0;
    w=new cavern(caverns[8], (char *)"wen", lead, 8, 0);
    c->add(w);
    lead[0]=8;lead[1]=10;lead[2]=11;lead[3]=0;lead[4]=0;lead[5]=0;
    w=new cavern(caverns[9], (char *)"nsw", lead, 9, 0);
    c->add(w);
    lead[0]=8;lead[1]=9;lead[2]=11;lead[3]=13;lead[4]=0;lead[5]=0;
    w=new cavern(caverns[10], (char *)"enws", lead, 10, 512);
    c->add(w);
    lead[0]=9;lead[1]=10;lead[2]=12;lead[3]=0;lead[4]=0;lead[5]=0;
    w=new cavern(caverns[11], (char *)"esw", lead, 11, 0);
    c->add(w);
    lead[0]=11;lead[1]=0;lead[2]=0;lead[3]=0;lead[4]=0;lead[5]=0;
    w=new cavern(caverns[12], (char *)"n", lead, 12, 0);
    c->add(w);
    lead[0]=10;lead[1]=14;lead[2]=0;lead[3]=0;lead[4]=0;lead[5]=0;
    w=new cavern(caverns[13], (char *)"nw", lead, 13, 0);
    c->add(w);
    lead[0]=13;lead[1]=15;lead[2]=16;lead[3]=0;lead[4]=0;lead[5]=0;
    w=new cavern(caverns[14], (char *)"new", lead, 14, 0);
    c->add(w);
    lead[0]=14;lead[1]=16;lead[2]=0;lead[3]=0;lead[4]=0;lead[5]=0;
    w=new cavern(caverns[15], (char *)"en", lead , 15, 0);
    c->add(w);
    lead[0]=14;lead[1]=15;lead[2]=0;lead[3]=0;lead[4]=0;lead[5]=0;
    w=new cavern(caverns[16], (char *)"wn", lead , 16, 0);
    c->add(w);

    int server_sockfd, client_sockfd, socks[10];
    int server_len, client_len,n=0,pos,i;
    struct sockaddr_in server_address;
    struct sockaddr_in client_address;
    int result;
    fd_set readfds, testfds;
    server_sockfd = socket(AF_INET, SOCK_STREAM, 0);

    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    server_address.sin_port = htons(4663);
    server_len = sizeof(server_address);

    bind(server_sockfd, (struct sockaddr *)&server_address, server_len);
    listen(server_sockfd, 5);

    FD_ZERO(&readfds);
    FD_SET(server_sockfd, &readfds);
    while(1) {
        char ch[60]="";
        int fd;
        int nread;

        testfds = readfds;

        printf("server waiting\n");
        result = select(FD_SETSIZE, &testfds, (fd_set *)0, 
            (fd_set *)0, (struct timeval *) 0);

        if(result < 1) {
            perror("server7");
            exit(1);
        }
        for(fd = 0; fd < FD_SETSIZE; fd++) {
            if(FD_ISSET(fd,&testfds)) {
                if(fd == server_sockfd) {
                    client_sockfd = accept(server_sockfd, 
                        (struct sockaddr *)&client_address, (socklen_t *)&client_len);
                    FD_SET(client_sockfd, &readfds);
                    printf("adding client on fd %d\n", client_sockfd);
                    socks[n++]=client_sockfd;
                    e=new explorer(client_sockfd,0);
                    c->add(e);
                }
                else {
                    ioctl(fd, FIONREAD, &nread);

                    if(nread == 0) {
                        close(fd);
                        FD_CLR(fd, &readfds);
                        printf("removing client on fd %d\n", fd);
                        for(i=0;i<n;i++)
                          if(socks[i]==fd)pos=i;
                        for(i=pos;i<n-1;i++)
                          socks[i]=socks[i+1];
                        n--;
                        e=c->get_explorer(fd);
                        c->remove(e);
                    }

                    else {
                        read(fd, ch, 60);
                        e=c->get_explorer(fd);
                        switch (ch[0])
                        {
                           case 'n':
                           case 's':
                           case 'e':
                           case 'w':
                           case 'u':
                           case 'd': c->move(ch[0],e); break;
                           case 'l': c->look(e); break;
                           case 'g': c->get(ch,e); break;
                           case 'p': c->put(ch,e); break;
                           case 't': c->talk(ch,e); break;
                           case 'v': c->value(e); break;
                           case 'q': c->quit(e); break;
                        }
                    }
                }
            }
        }
    }
}
