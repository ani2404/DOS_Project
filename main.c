/*  main.c  - main */

#include <xinu.h>

uint16 my_getnbr(char *str)
{
  uint16           result;
  uint16           puiss;

  result = 0;
  puiss = 1;
  while (('-' == (*str)) || ((*str) == '+'))
    {
      if (*str == '-')
        puiss = puiss * -1;
      str++;
    }
  while ((*str >= '0') && (*str <= '9'))
    {
      result = (result * 10) + ((*str) - '0');
      str++;
    }
  return (result * puiss);
}

//--------------------------------------------------------------------------------------------------------------------------------------------------
process main(void)
{
	uid32	slot;			/* Slot in UDP table		*/
	uint32 remote_ip=0;
	uint16 remote_port=0,local_port = 1234;
	int32 retval=0;
	char buffer[65535];
   	int len;
   	int spcCount = 0;
   	int ptrIndex = 0;
   	int i,j,k;
   
 	slot = udp_register(remote_ip, remote_port, local_port);
	
	if (slot == SYSERR) {
		fprintf(stderr,"getutime: cannot register a udp port %d\n",
					local_port);
	}
	kprintf("UDP slot obtained");
    while(1) {
	retval = udp_recvaddr(slot,&remote_ip, &remote_port,buffer, sizeof(buffer),5000);
		printf("message received of length: %d",retval);
					 
// ==========================================================================================================================
	if ( (retval == SYSERR) ) {
		
		break;
	}

	if(retval == TIMEOUT){
		continue;
	}



// added the c -tokenizer
		
   len = strlen(buffer);
  spcCount = 0;
  ptrIndex = 0;

   for(i = 0; i < len; i++)
   {
      if(i != 0 && buffer[i] == ' ' && buffer[i-1] != ' ')
      {
         spcCount++;
      }
   }

   char** ptr = (char**) getmem(sizeof(char*)*(spcCount+2));
   printf("memory allocated");
   //Used to differentiate separating spaces from unnecessary ones
   char temp;

   for(j = 0; j < len; j++){
      if(j == 0){
         ptr[ptrIndex] = &buffer[j];
         temp = buffer[j];
          ptrIndex++;
      }
      else{
         if(buffer[j] == '|'){
            temp = buffer[j];
            buffer[j] = '\0';
         }
         else if(buffer[j] != '|' && buffer[j] != '\0' && temp == '|'){
            ptr[ptrIndex] = &buffer[j];
            temp = buffer[j];
            ptrIndex++;

         }
      }
   }
 
   for (k = 0; k < ptrIndex; ++k){
      printf("%s \n", ptr[k]);
   }

	if(strncmp("gpio_write",ptr[0],strlen(ptr[0]))==0)
	 gpio_write(my_getnbr(ptr[1]),my_getnbr(ptr[2]),my_getnbr(ptr[3]));
	else
	printf("unknown function name");
 
  freemem((char*)ptr,sizeof(char*)*(spcCount+2));
// =======================================================================================================================


	
	printf("sending back reply ");
	udp_sendto (
	 slot,			/* UDP table slot to use	*/
	 remote_ip,			/* Remote IP address to use	*/
	 remote_port,		/* Remote protocol port to use	*/
	 buffer,			/* Buffer of UDP data		*/
	 retval			/* Length of data in buffer	*/
	);
	kprintf("Received messg %s \n",buffer);
}
   
	udp_release(slot);

	return OK;
}
