#include <xinu.h>

handler_list* rising_list[32];
handler_list* falling_list[32];

const uint32 pin[][47] ={ 

{0,0,0,GPIO_1_START+ 6,GPIO_1_START+ 7,GPIO_1_START+ 2,GPIO_1_START+ 3,0,
	0,0,0,GPIO_1_START+ 13,GPIO_1_START+ 12,0,
GPIO_0_START+ 26,GPIO_1_START+ 15,GPIO_1_START+ 14,GPIO_0_START+ 27,GPIO_2_START+ 1,0,
GPIO_1_START+ 31,GPIO_1_START+ 30,GPIO_1_START+ 5,GPIO_1_START+ 4,GPIO_1_START+ 1,GPIO_1_START+ 0,
GPIO_1_START+ 29,GPIO_2_START+ 22,GPIO_2_START+ 24,GPIO_2_START+ 23,GPIO_2_START+ 25,0,
0,0,0,0,0,0,
0,GPIO_2_START+ 12,GPIO_2_START+ 13,GPIO_2_START+ 10,GPIO_2_START+ 11,GPIO_2_START+ 8,
GPIO_2_START+ 9,GPIO_2_START+ 6,GPIO_2_START+ 7},

{0,0,0,0,0,0,0,0,0,0,0,0,GPIO_1_START+ 28,0,0,GPIO_1_START+ 16,0,
	0,0,0,0,0,0,GPIO_1_START+ 17,0,GPIO_3_START+ 21,0,
GPIO_3_START+ 19,0,0,0,0,0,0,0,0,0,0,0,0,0,0,GPIO_0_START+ 7,0,0,0,0}

};


const uint32 control_module[][32] = {
	{0,0,0,0,0,0,0,0x964,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0x828,0x82C,0,0,0,0},
	{0x800,0x804,0x808,0x80C,0x810,0x814,0x818,0x81C,0,0,0,0,0X830,0X834,0X838,0x83C,0x840,0x844,0,0,0,0,0,0,0,0,0,0,0x878,0x87C,0x880,0x884},
	{0,0x88C,0,0,0,0,0x8A0,0x8A4,0x8A8,0x8AC,0x8B0,0x8B4,0x8B8,0x8BC,0,0,0,0,0,0,0,0,0x8E0,0x8E4,0x8E8,0x8EC,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0x9A4,0,0x9AC,0,0,0,0,0,0,0,0,0,0}
};

void gpio_init(){
		int i=0;
		for(i=0;i<NUM_GPIO_MODULES;i++){
			gpio_module_init(module[i]);
			control_module_init(i);
		}
		

		// Set Interrupt
		set_evec(INTC_GPIO_0A,(uint32)gpio_handler);
		set_evec(INTC_GPIO_0B,(uint32)gpio_handler);
		set_evec(INTC_GPIO_1A,(uint32)gpio_handler);
		set_evec(INTC_GPIO_1B,(uint32)gpio_handler);
		set_evec(INTC_GPIO_2A,(uint32)gpio_handler);
		set_evec(INTC_GPIO_2B,(uint32)gpio_handler);
		set_evec(INTC_GPIO_3A,(uint32)gpio_handler);
		set_evec(INTC_GPIO_3B,(uint32)gpio_handler);



}

void control_module_init(gpio_module module){
	 
	 int i =0;
	 uint32 value;

	 for(i=0; i<32;i++){
	 	if(control_module[module][i] != 0){
	 		dprintf("Accessing register in control mode: 0x%x \n",CONTROL_MODULE_BASE + control_module[module][i]);
	 		value = MM_READ(CONTROL_MODULE_BASE + control_module[module][i]);
	 		value |= 7;
	 		MM_WRITE(CONTROL_MODULE_BASE + control_module[module][i],value);
			dprintf("control module data is 0x%x \n",MM_READ(CONTROL_MODULE_BASE + control_module[module][i]));

			}
		}
}


void gpio_module_init(gpio_module module) {


	int i=0;
	struct	gpio_csreg *csrptr = (struct gpio_csreg *)module;

	dprintf("Accessing register in init: 0x%x \n",csrptr);

   // Perform software reset
    dprintf("GPIO_SYSCONFIG Register content %d \n",MM_READ(&csrptr->sys_config));
    dprintf("GPIO_SYSSTATUS Register content %d \n",MM_READ(&csrptr->sys_status));
    dprintf("GPIO_CTRL Register content %d \n",MM_READ(&csrptr->ctrl));
//	BIT_SET(csrptr->sys_config,1);

	//wait till the reset is done
	dprintf("SYSSTATUS address is 0x%x \n",&csrptr->sys_status);
//	while(BIT_READ(csrptr->sys_status,0) == 0);

	dprintf("GPIO_SYSSTATUS Register content %d",BIT_READ(csrptr->sys_status,0));

	for(i=0;i<32;i++){
		rising_list[i] = NULL;
	}
}






bool8 gpio_set_mode(uint32 p,bool8 data){
	if(p != 0){


	struct	gpio_csreg *csrptr = (struct gpio_csreg *)(((uint32)p) & (~0x1F));
	dprintf("Accessing register in set mode: 0x%x \n",csrptr);

	// Pin is 0 to 31
	uint8 pin_no = (p & 0x1F);

	if(data){
		// input
		BIT_SET(csrptr->oe,pin_no);

		// PUll UP/DOWN disabled


	}
	else{
		// input
		BIT_CLEAR(csrptr->oe,pin_no);

		// Pull UP enabled
	}
}
else
{
	dprintf("Pin does not exist \n");
	return FALSE;
}
	return TRUE;
}


