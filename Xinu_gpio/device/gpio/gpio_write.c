#include <xinu.h>

bool8 gpio_write(uint32 p,bool8 data) {
	if(p != 0){
	struct	gpio_csreg *csrptr = (struct gpio_csreg *)(((uint32)p) & (~0x1F));
	dprintf("Accessing register in write: 0x%x \n",csrptr);

    // Pin is 0 to 31
	uint8 pin_no = (p & 0x1F);

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
}
else
{
	dprintf("Pin does not exist \n");
	return FALSE;
}

	return TRUE;
}