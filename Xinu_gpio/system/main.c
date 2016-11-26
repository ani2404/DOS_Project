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

process	main(void)
{
	int register_value,pin =12;
	int i=0;

	/* Run the Xinu shell */

	//recvclr();
	//resume(create(shell, 8192, 50, "shell", 1, CONSOLE));

	/* Wait for shell to exit and recreate it */

					//Set the mode as GPIO
				register_value = am355x_read(CONTROL_MODULE + GPI01_12);
				kprintf("mode value is: %d \n",register_value & 0x7);
				register_value |= 0x7;
				am335x_padctl(CONTROL_MODULE + GPI01_6,register_value);

				//Set the Pin as output
				register_value = am355x_read(GPIO_1_START + GPIO_OE);
				kprintf("GPIO_OE value is: 0x%x \n",register_value);
				register_value &= (~(1<<pin));
				am335x_padctl(GPIO_1_START + GPIO_OE,register_value);
				kprintf("GPIO_OE value after operation is: 0x%x \n",register_value);


	while(i++ < 20) {
		//kprintf("LED operations- \n ON: Press 1 \n  OFF: Press 2 \n :");
		//scanf("%d",value);

		{
				kprintf("Setting the LED to on \n");
				//ON the pin
				register_value = am355x_read(GPIO_1_START + GPIO_DATAOUT);
				kprintf("GPIO_DATAOUT value is: 0x%x \n",register_value);
				register_value |= (1<<pin);
				am335x_padctl(GPIO_1_START + GPIO_DATAOUT,register_value);
				kprintf("GPIO_DATAOUT value after operation is: 0x%x \n",register_value);
		}

		sleep(1);

		
		{
			kprintf("Setting the LED off \n");
				//off the pin
				register_value = am355x_read(GPIO_1_START + GPIO_DATAOUT);
				kprintf("GPIO_DATAOUT value is: 0x%x \n",register_value);
				register_value &= ~(1<<pin);
				am335x_padctl(GPIO_1_START + GPIO_DATAOUT,register_value);
				kprintf("GPIO_DATAOUT value after operation is: 0x%x \n",register_value);
		}
		sleep(1);

	}

	
	return OK;
    
}
