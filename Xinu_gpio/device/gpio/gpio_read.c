#include <xinu.h>

bool8 gpio_read(uint8 exp,uint8 pin) {
	if(!isvalidpin(exp,pin)){
		dprintf("Pin does not exist \n");
		return FALSE;
	}

	if(pin_map[exp][pin-1] == 0){
		return FALSE;
	}


	uint8 module_no = pin_map[exp][pin-1] >> 5;
	gpio_csreg *csrptr = (gpio_csreg *)(module[module_no]);
	// Pin is 0 to 31
	uint8 pin_no = (pin_map[exp][pin-1] & 0x1F);

	if(BIT_READ(csrptr->oe,pin_no) == 1){

			dprintf("Bit value is %d",BIT_READ(csrptr->datain,pin_no));
    
			return BIT_READ(csrptr->datain,pin_no);
	}
	else {
		
			return BIT_READ(csrptr->dataout,pin_no);
	}
	
}

bool8 gpio_subscribe(uint8 exp, uint8 pin,handler h,bool8 on_rise) {
	if(!isvalidpin(exp,pin)){
		dprintf("Pin does not exist \n");
		return FALSE;
	}

	if(pin_map[exp][pin-1] == 0){
		return FALSE;
	}

	uint8 module_no = pin_map[exp][pin-1] >> 5;
	gpio_csreg *csrptr = (gpio_csreg *)(module[module_no]);
	// Pin is 0 to 31
	uint8 pin_no = (pin_map[exp][pin-1] & 0x1F);

	if(BIT_READ(csrptr->oe,pin_no)) {
		handler_list **head = on_rise ? &rising_list[module_no][pin_no] : &falling_list[module_no][pin_no];
		uint32 *enable_interrupt_addr = on_rise ? &csrptr->risingdetect : &csrptr->fallingdetect;
		if(*head == NULL){
			dprintf("risingdetect address is 0x%x \n",enable_interrupt_addr);
			BIT_SET(*enable_interrupt_addr,pin_no);
			dprintf("irq_status_set0 address is 0x%x \n",&csrptr->irq_status_set0);
			BIT_SET(csrptr->irq_status_set0,pin_no);
					//Clock Divsion
			MM_WRITE(&csrptr->ctrl,6);
			dprintf("GPIO_CTRL Register content %d \n",MM_READ(&csrptr->ctrl));
		}

		dprintf("Handler registering \n");
    		handler_list *temp = (handler_list *)getmem(sizeof(handler_list));
   		temp->h = h;
    		temp->next = *head;
		*head = temp;

		dprintf("Handler registered successfully \n");
	}
	else
	{
		dprintf("Cannot subsrcibe to output pin \n");
		return FALSE;
	}
	return TRUE;

}


bool8 gpio_unsubscribe(uint8 exp, uint8 pin,handler h,bool8 on_rise) {
	if(!isvalidpin(exp,pin)){
		dprintf("Pin does not exist \n");
		return FALSE;
	}

	if(pin_map[exp][pin-1] == 0){
		return FALSE;
	}

	uint8 module_no = pin_map[exp][pin-1] >> 5;
	gpio_csreg *csrptr = (gpio_csreg *)(module[module_no]);
	// Pin is 0 to 31
	uint8 pin_no = (pin_map[exp][pin-1] & 0x1F);

	if(BIT_READ(csrptr->oe,pin_no)) {
		handler_list **head = on_rise ? &rising_list[module_no][pin_no] : &falling_list[module_no][pin_no];

		dprintf("Handler unsubscribing \n");
	    	handler_list *temp = *head;
    		handler_list *temp2 = temp;
    	
    		while(temp!= NULL && (temp->h != h)){ temp2 = temp;
    			temp = temp->next;}

    		if(temp2 != NULL && temp != NULL){
    			temp2->next = temp->next;
    			if(temp == *head) {*head = temp->next;}
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

	return TRUE;



}
