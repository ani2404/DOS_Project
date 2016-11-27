#include <xinu.h>


extern sid32 readSem;

/**************************************************************************
function name:  adcread
description:    Read function will enable ADC, then wait for semaphore which
                which will be signaled by ADC interrupt. When semaphore is
                got, read function can read the data.

input:          devptr -------> base address of ADC register				 
**************************************************************************/
devcall	adcread(uint8 pin,char* buff, int32 count)
{

	if(!isvalidanalogpin(pin)) {
		dprintf("The pin is not a valid analog pin \n");
		return SYSERR;
	}

	if((pin_map[1][pin-1] & (1<<6)) == 0){
		dprintf("Not a valid analog pin \n");
		return SYSERR;
	}

	uint8 step = (pin_map[1][pin-1] & 7)+1;
	//kprintf("--------------ADC Read-------------\r\n");
	struct adc_csreg* pReg = (struct adc_csreg*)0x44e0d000;

	//I use ADC with continuous mode, and in the interrupt handler, the step module
	//will be disable.Thus everytime when user want to read the ADC, this function will
	//enable the step module.
	dprintf("Enabling step %d\n",step);
	ADCStepEnable(pReg,step);
	enabled_steps[step] = TRUE;

	//wait for semaphore signaled by interrupt handler.
	wait(readSem);

	unsigned int data = 0;
	// float temperature;
	// unsigned int integer = 0;
	// unsigned int decimal = 0;
	int sampleNum = pReg->fifoInfo[0].fifoCount;
	int i;
	dprintf("Total no of samples %d\n",sampleNum);
	for(i = 0; i < sampleNum; i++)
	{
		data = pReg->fifoData0 &(0xFFF) ;
		//dprintf("data is 0x%x \n",data);
	}
	buff[3] = (data&(0xff000000))>>24;
	buff[2] = (data&(0x00ff0000))>>16;	
	buff[1] = (data&(0x0000ff00))>>8;
	buff[0] = (data&(0x000000ff));
//	temperature = (((data*1.8)/4095.0)-0.5)*100;
//	integer = (unsigned int)temperature;
//	decimal = (unsigned int)((temperature - integer)*100);
//	kprintf("temperature = %d.%d\r\n",integer,decimal);
	return 1;
}
