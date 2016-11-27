#include <xinu.h>

handler_list* rising_list[4][32];
handler_list* falling_list[4][32];

const gpio_module module[] = { module0, module1, module2, module3 };

const uint32 pin_map[][46] ={ 
//Expansion Header P8
{0,0,(1<<5)+ 6,(1<<5)+ 7,(1<<5)+ 2,(1<<5)+ 3,0,
	0,0,0,(1<<5)+ 13,(1<<5)+ 12,0,
(0<<5)+ 26,(1<<5)+ 15,(1<<5)+ 14,(0<<5)+ 27,(2<<5)+ 1,0,
(1<<5)+ 31,(1<<5)+ 30,(1<<5)+ 5,(1<<5)+ 4,(1<<5)+ 1,(1<<5)+ 0,
(1<<5)+ 29,(2<<5)+ 22,(2<<5)+ 24,(2<<5)+ 23,(2<<5)+ 25,0,
0,0,0,0,0,0,
0,(2<<5)+ 12,(2<<5)+ 13,(2<<5)+ 10,(2<<5)+ 11,(2<<5)+ 8,
(2<<5)+ 9,(2<<5)+ 6,(2<<5)+ 7},
//Expansion Header P9
{0,0,0,0,0,0,0,0,0,0,(1<<5)+ 28,0,0,(1<<5)+ 16,0,
	0,0,0,0,0,0,(1<<5)+ 17,0,(3<<5)+ 21,0,
(3<<5)+ 19,0,0,0,0,0,0,(1<<6) + 4,0,(1<<6) + 6,(1<<6) + 5,(1<<6) + 2,(1<<6) + 3,(1<<6) + 0,(1<<6) + 1,0,(0<<5)+ 7,0,0,0,0}

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
		set_evec(INTC_GPIO_0A,(uint32)gpio_handler_mod0);
	//	set_evec(INTC_GPIO_0B,(uint32)gpio_handler);
		set_evec(INTC_GPIO_1A,(uint32)gpio_handler_mod1);
	//	set_evec(INTC_GPIO_1B,(uint32)gpio_handler);
		set_evec(INTC_GPIO_2A,(uint32)gpio_handler_mod2);
	//	set_evec(INTC_GPIO_2B,(uint32)gpio_handler);
		set_evec(INTC_GPIO_3A,(uint32)gpio_handler_mod3);
	//	set_evec(INTC_GPIO_3B,(uint32)gpio_handler);



}

void control_module_init(gpio_module module){
	 
	 int i =0;
	 uint32 value;

	 //Set all Pins mode to GPIO
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


	int i=0,j=0;
	gpio_csreg *csrptr = (gpio_csreg *)module;

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

	for(j=0;j<NUM_GPIO_MODULES;j++){
		for(i=0;i<32;i++){
				rising_list[j][i] = NULL;
			}
	}
}






bool8 gpio_set_mode(uint8 exp_header, uint8 pin,bool8 data){
	

	if(!isvalidpin(exp_header,pin)){
		dprintf("Pin does not exist \n");
		return FALSE;
	}
	
	if(pin_map[exp_header][pin] == 0){
		return FALSE;
	}

	uint8 module_no = pin_map[exp_header][pin-1] >> 5;
	gpio_csreg *csrptr = (gpio_csreg *)(module[module_no]);
	dprintf("Accessing register in set mode: 0x%x \n",csrptr);

	// Pin is 0 to 31
	uint8 pin_no = (pin_map[exp_header][pin-1] & 0x1F);

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
	return TRUE;
}


