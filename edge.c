#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>

int main(){
  
  int xinuSocket,edgeSocket,portNum, nBytes;
  char buffer[1024]="cool";
  char buffer1[1024];
  struct sockaddr_storage serverStorage;
  struct sockaddr_in edgeAddr;
  struct sockaddr_in xinuAddr;
  socklen_t edgeAddr_size;
  socklen_t xinuAddr_size;

  /*Create UDP socket*/
  edgeSocket = socket(PF_INET, SOCK_DGRAM, 0);
  xinuSocket = socket(PF_INET, SOCK_DGRAM, 0);
  
  /*Configure settings in address struct*/
  
  edgeAddr.sin_family = AF_INET;
  edgeAddr.sin_port = htons(8000);
  edgeAddr.sin_addr.s_addr = inet_addr("192.168.0.14");
  memset(edgeAddr.sin_zero, '\0', sizeof edgeAddr.sin_zero);  
 
 if( bind(edgeSocket, (struct sockaddr *) &edgeAddr, sizeof(edgeAddr))<0)
 printf("Failed binding edge socket \n");

  xinuAddr.sin_family = AF_INET;
  xinuAddr.sin_port = htons(1234);
  xinuAddr.sin_addr.s_addr = inet_addr("192.168.0.55");
  memset(xinuAddr.sin_zero, '\0', sizeof xinuAddr.sin_zero);  
	
  //if(bind(xinuSocket, (struct sockaddr *) &xinuAddr, sizeof(xinuAddr))<0)
  
  /*Initialize size variable to be used later on*/
  xinuAddr_size = sizeof xinuAddr;
  edgeAddr_size = sizeof edgeAddr;
  //cloudAddr_size = sizeof cloudAddr;
  printf("entering message exchange code \n");

  while(1)
  { /*Receive message from cloud*/
    nBytes = recvfrom(edgeSocket,buffer,1024,0,(struct sockaddr *)&serverStorage, &edgeAddr_size);

    struct sockaddr_in *sin = (struct sockaddr_in *)&serverStorage;
   
 unsigned char *ip_cloud = (unsigned char *)&sin->sin_addr.s_addr;
    unsigned char *ip_xinu = (unsigned char *)xinuAddr.sin_addr.s_addr;
    if(&sin->sin_addr.s_addr != xinuAddr.sin_addr.s_addr) 
      printf("message not from xinu");

	  //printf("received the message %s from %s \n",buffer,ip_xinu);
       sendto(xinuSocket,buffer,strlen(buffer)+1,0,(struct sockaddr *)&xinuAddr,xinuAddr_size);
    printf("message sent to xinu which was received from cloud\n");
    sleep(1);

   /*Receive message from xinu*/
   nBytes = recvfrom(xinuSocket,buffer1,1024,0,NULL, NULL);
   printf("received reply from xinu %s\n",buffer1);
   
   /* send message to cloud*/	
    sendto(edgeSocket,buffer1,strlen(buffer1)+1,0,(struct sockaddr *)&serverStorage,edgeAddr_size);
    printf("message sent to cloud which was received from xinu is: %s\n",buffer1);
    sleep(1);
  }

  return 0;
  }
