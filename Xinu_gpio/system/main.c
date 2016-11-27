/*  main.c  - main */

#include <xinu.h>

process edge_data(void)
{
	uid32	slot;			/* Slot in UDP table		*/
	uint32 remote_ip=0;
	uint16 remote_port=0,local_port = 1234;
	int32 retval=0;
	char buffer[65535];

	/* Run the Xinu shell */

	//recvclr();
	//resume(create(shell, 8192, 50, "shell", 1, CONSOLE));

	/* Wait for shell to exit and recreate it *
	} */
	//char ip_addr[256] = "192.168.0.";
   // dot2ip(ip_addr, &remote_ip); 
 	slot = udp_register(remote_ip, remote_port, local_port);
	if (slot == SYSERR) {
		fprintf(stderr,"getutime: cannot register a udp port %d\n",
					local_port);
	}

    while(1) {
	retval = udp_recvaddr(slot,&remote_ip, &remote_port,buffer, sizeof(buffer),
							 5000);

	if ( (retval == SYSERR) ) {
		
		break;
	}

	if(retval == TIMEOUT){
		continue;
	}
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

void button(void)
{
	static bool8 data = FALSE;
	data = !data;
	gpio_write(pin[0][5],data);
}
process main()
{
	int16 i=0;
	gpio_init();
	
	gpio_set_mode(pin[0][3],1);
	gpio_subscribe_high(pin[0][3],button);
	for (i=1;i<47;i++)
	{
	if(i != 3) {
	gpio_set_mode(pin[0][i],0);
	gpio_set_mode(pin[1][i],0);
	}	
	gpio_write(pin[0][i],1);
	gpio_write(pin[1][i],1);
	
}
}
	
