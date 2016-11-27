#include <xinu.h>

bool8 gpio_read(uint32 p) {
	if(p != 0){
		struct	gpio_csreg *csrptr = (struct gpio_csreg *)(((uint32)p) & (~0x1F));

	 // Pin is 0 to 31
		uint8 pin_no = (p & 0x1F);

		if(BIT_READ(csrptr->oe,pin_no) == 1){

			dprintf("Bit value is %d",BIT_READ(csrptr->datain,pin_no));
    
			return BIT_READ(csrptr->datain,pin_no);
		}
		else {
		
			return BIT_READ(csrptr->dataout,pin_no);
		}
	}
	else
	{
		dprintf("Pin does not exist \n");
		return FALSE;
	}
	
}

bool8 gpio_subscribe_high(uint32 p,handler h) {
	if(p != 0){
		struct	gpio_csreg *csrptr = (struct gpio_csreg *)(((uint32)p) & (~0x1F));

		 // Pin is 0 to 31
		uint8 pin_no = (p & 0x1F);

	if(BIT_READ(csrptr->oe,pin_no)) {
	dprintf("risingdetect address is 0x%x \n",&csrptr->risingdetect);
	BIT_SET(csrptr->risingdetect,pin_no);
	dprintf("irq_status_set0 address is 0x%x \n",&csrptr->irq_status_set0);
	BIT_SET(csrptr->irq_status_set0,pin_no);

	//Clock Divsion
		MM_WRITE(&csrptr->ctrl,6);
		dprintf("GPIO_CTRL Register content %d \n",MM_READ(&csrptr->ctrl));

	dprintf("Handler registering \n");
    handler_list *temp = (handler_list *)getmem(sizeof(handler_list));
    temp->h = h;
    temp->next = rising_list[pin_no];
	rising_list[pin_no] = temp;

	dprintf("Handler registered successfully \n");
	}
	else
	{
		dprintf("Cannot subsrcibe to output pin \n");
		return FALSE;
	}
}
else
{
	dprintf("Pin does not exist \n");
	return FALSE;
}

	return TRUE;



}

bool8 gpio_unsubscribe_high(uint32 p,handler h) {
	if(p != 0){
	struct	gpio_csreg *csrptr = (struct gpio_csreg *)(((uint32)p) & (~0x1F));

		 // Pin is 0 to 31
	uint8 pin_no = (p & 0x1F);

	if(BIT_READ(csrptr->oe,pin_no)) {

		dprintf("Handler unsubscribing \n");
    	handler_list *temp = rising_list[pin_no];
    	handler_list *temp2 = NULL;
    	
    	while(temp!= NULL && (temp->h != h)){ temp2 = temp;
    		temp = temp->next;}

    	if(temp2 != NULL && temp != NULL){
    	temp2->next = temp->next;
    	freemem((char*)temp,sizeof(handler_list));
    	dprintf("Handler unregistered successfully \n");
    	}
	
		// Do any clean operation if the handler list is empty
	}
	else
	{
		dprintf("Cannot unsubsrcibe to output pin \n");
		return FALSE;
	}
}
else
{
	dprintf("Pin does not exist \n");
	return FALSE;
}

	return TRUE;



}

bool8 gpio_subscribe_low(uint32 p, handler h) {
	if(p != 0){
	struct	gpio_csreg *csrptr = (struct gpio_csreg *)(((uint32)p) & (~0x1F));

		 // Pin is 0 to 31
	uint8 pin_no = (p & 0x1F);

	if(BIT_READ(csrptr->oe,pin_no)) {
	dprintf("fallingdetect address is 0x%x \n",&csrptr->risingdetect);
	BIT_SET(csrptr->fallingdetect,pin_no);
	dprintf("irq_status_set0 address is 0x%x \n",&csrptr->irq_status_set0);
	BIT_SET(csrptr->irq_status_set0,pin_no);

	//Clock Divsion
	MM_WRITE(&csrptr->ctrl,6);
	dprintf("GPIO_CTRL Register content %d \n",MM_READ(&csrptr->ctrl));


	dprintf("Handler registering \n");
    handler_list *temp = (handler_list *)getmem(sizeof(handler_list));
    temp->h = h;
    temp->next = falling_list[pin_no];
	falling_list[pin_no] = temp;

	dprintf("Handler registered successfully \n");
	}
	else
	{
		dprintf("Cannot subsrcibe to output pin \n");
		return FALSE;
	}
}
else
{
	dprintf("Pin does not exist \n");
	return FALSE;
}

	return TRUE;

} 



bool8 gpio_unsubscribe_low(uint32 p,handler h) {
	if(p != 0){
	struct	gpio_csreg *csrptr = (struct gpio_csreg *)(((uint32)p) & (~0x1F));

		 // Pin is 0 to 31
	uint8 pin_no = (p & 0x1F);

	if(BIT_READ(csrptr->oe,pin_no)) {

		dprintf("Handler unsubscribing \n");
    	handler_list *temp = falling_list[pin_no];
    	handler_list *temp2 = NULL;
    	
    	while(temp!= NULL && (temp->h != h)){ temp2 = temp;
    		temp = temp->next;}

    	if(temp2 != NULL && temp != NULL){
    	temp2->next = temp->next;
    	freemem((char*)temp,sizeof(handler_list));
    	dprintf("Handler unregistered successfully \n");
    	}
	
		// Do any clean operation if the handler list is empty
	}
	else
	{
		dprintf("Cannot unsubsrcibe to output pin \n");
		return FALSE;
	}
}
else
{
	dprintf("Pin does not exist \n");
	return FALSE;
}

	return TRUE;



}