/*  main.c  - main */

#include <xinu.h>

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
