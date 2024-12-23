#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include <arpa/inet.h>
#include<string.h>
#include<errno.h>
#include<stdlib.h>
#include<unistd.h>
#include<iostream>
#include<pthread.h>
#include"Multiplayer.hpp"


int initSocket(int port)
{
  int sockfd;
  
  struct sockaddr_in serv;
  memset(&serv, 0, sizeof(serv));
  
  serv.sin_family = AF_INET;
  serv.sin_port = htons(port);
  serv.sin_addr.s_addr = INADDR_ANY;
  
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if(sockfd < 0)
  {
    perror("Socket");
    exit(-1);
  }
  
  if(bind(sockfd, (struct sockaddr*)&serv, sizeof(serv))<0)
  {
    perror("Bind");
    exit(-1);
  }
  
  if(listen(sockfd,5)<0)
  {
    perror("Listen");
    exit(-1);
  }
  return sockfd;
}

int sendMove(char* ip, int port, int* move)
{
  int sockfd = socket(AF_INET, SOCK_STREAM,0);
  
  if(sockfd < 0)
  {
    perror("socket");
    exit(-1);
  }
  
  struct sockaddr_in serv;
  memset(&serv, 0, sizeof(serv));
  serv.sin_port = htons(port);
  serv.sin_family = AF_INET;
  if(inet_pton(AF_INET, ip,&serv.sin_addr.s_addr)< 0)
  {
    perror("inet_pton");
    exit(-1);
  }
  
  if(connect(sockfd, (struct sockaddr*)&serv, sizeof(serv)) <0)
  {
    perror("connect");
    exit(-1);
  }
  
  char buf[100];
  snprintf(buf,100, "%d#%d", move[0], move[1]);
  
  
  send(sockfd, buf, sizeof(buf),0);
  
  
  return 1;
}


std::vector<uint32_t> getOpMove(int servfd)
{
  std::vector<uint32_t> res(2);
  char buffer[100];
  struct sockaddr_in sender;
  int addrlen = sizeof(sender);
  int newFd;

    if((newFd = accept(servfd, (struct sockaddr*)&sender, (socklen_t*)&addrlen))<0)
    {
      perror("accept");
    }

    recv(newFd, buffer ,100,0);
  
  
  std::string tmp = "";
    for(char c : buffer)
    {
      if(c == '#')
      {
        res[0] = atoi(tmp.c_str());
        tmp.clear();
        continue;
      }
      tmp += c;
    }
  res[1] = atoi(tmp.c_str());
  
  return res;
}
