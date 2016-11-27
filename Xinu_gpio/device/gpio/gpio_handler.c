#include <xinu.h>

void gpio_handler(uint8 module_no)
{
	uint8 pin = 0;
	handler_list *temp;

	struct	gpio_csreg *csrptr = (struct gpio_csreg *)module[module_no];

	dprintf("irq_status_raw0 address is 0x%x \n",&csrptr->irq_status_raw0);
	while(BIT_READ(csrptr->irq_status_raw0,pin) == 0){pin++;}

	if(BIT_READ(csrptr->risingdetect,pin) == 1){
		temp = rising_list[module_no][pin];
	}
	else
	{
		temp = falling_list[module_no][pin];
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

void gpio_handler_mod0(void) {
	gpio_handler(0);
}
void gpio_handler_mod1(void) {
	gpio_handler(1);
}
void gpio_handler_mod2(void) {
	gpio_handler(2);
}
void gpio_handler_mod3(void) {
	gpio_handler(3);
}