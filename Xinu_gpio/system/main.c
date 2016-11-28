/*  main.c  - main */

#include <xinu.h>

static int16 count=0;
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

void LED_g(void)
{
	static bool8 data = FALSE;
	data = !data;
	gpio_write(0,5,data);
}
void LED_r(void)
{
	static bool8 data = TRUE;
	data = !data;
	gpio_write(0,12,data);
		
}

void buzzer(void)
{
	static bool8 data = FALSE;
	
	
	data = !data;
	gpio_write(0,14,data);
	count++;
	//kprintf("third subscriber \n");
}

process main()
{
	
	int16 i=0;
	gpio_init();
	
	gpio_set_mode(0,3,1);
	gpio_subscribe(0,3,LED_g,TRUE);
	gpio_subscribe(0,3,LED_r,TRUE);
	gpio_subscribe(0,3,buzzer,TRUE);
	for (i=1;i<47;i++)
	{
	if(i != 3) {
	gpio_set_mode(0,i,0);
	gpio_set_mode(1,i,0);
	}	
	gpio_write(0,i,1);
	gpio_write(1,i,1);
	
	}
	kprintf("In main after for loops\n");
	while(1)
	{
		kprintf("");
		if(count>=3){
		kprintf("In unsubscribe\n");
		gpio_write(0,14,FALSE);
		gpio_unsubscribe(0,3,buzzer,TRUE);
			break;
		}
	
	
	
	//analog read
//	kprintf("Start analog read\r\n");
	char data[4];
	unsigned int integer = 0;
	unsigned int decimal = 0;
	float voltage,temp_c;
	
		//adcread(39,(char*)data,4);
		unsigned int vol = *(unsigned int*)data;
		voltage = (vol*1.8/4095)*1000;
		temp_c = (voltage - 500) / 10;
		integer = (unsigned int)temp_c;
		decimal = (unsigned int)((temp_c - integer)*10);
		//kprintf("Voltage = %d mv\r\n",voltage);
		//kprintf("Centigrade = %d.%d  mv\r\n",integer,decimal);
		sleep(3);
	}
	
}

	

