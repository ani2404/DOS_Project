#include <xinu.h>

void gpio_handler(void)
{
	uint8 pin = 0;
	handler_list *temp;

	struct	gpio_csreg *csrptr = (struct gpio_csreg *)GPIO_1_START;

	dprintf("irq_status_raw0 address is 0x%x \n",&csrptr->irq_status_raw0);
	while(BIT_READ(csrptr->irq_status_raw0,pin) == 0){pin++;}

	if(BIT_READ(csrptr->risingdetect,pin) == 1){
		temp = rising_list[pin];
	}
	else
	{
		temp = falling_list[pin];
	}

	while(temp != NULL)
	{
		dprintf("HANDLERS EXIST \n");
		temp->h();
		temp = temp->next; 
	}

	// Clear the Interrupt

	BIT_SET(csrptr->irq_status_0,pin);

}

void gpio_handler1(void)
{

		uint8 pin = -1;
	handler_list *temp;

	struct	gpio_csreg *csrptr = (struct gpio_csreg *)GPIO_1_START;

	dprintf("irq_status_raw1 address is 0x%x \n",&csrptr->irq_status_raw1);
	while(BIT_READ(csrptr->irq_status_raw1,pin) == 0){pin++;}

	temp = rising_list[pin];

	while(temp != NULL)
	{
		dprintf("HANDLERS EXIST \n");
		temp->h();
		temp = temp->next; 
	}

	//BIT_SET

}