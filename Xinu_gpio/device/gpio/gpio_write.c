#include <xinu.h>

bool8 gpio_write(uint8 exp, uint8 pin,bool8 data) {
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

    if(BIT_READ(csrptr->oe,pin_no) == 0) {
	dprintf("dataout address is 0x%x \n",&csrptr->dataout);

    if(data){
		BIT_SET(csrptr->dataout,pin_no);
	}
	else
		BIT_CLEAR(csrptr->dataout,pin_no);

	dprintf("Bit value is %d \n",BIT_READ(csrptr->dataout,pin_no));
	}
	else
	{
		dprintf("Cannot write to the pin %d\n",pin_no);
		return FALSE;
	}
	return TRUE;
}
