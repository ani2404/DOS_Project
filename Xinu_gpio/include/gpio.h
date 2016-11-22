// GPIO Configuration, need to push to gpio header file later
#define GPIO_0_START 0x44E07000
#define GPIO_0_END   0x44E07FFF
#define GPIO_1_START 0x4804C000
#define GPIO_1_END   0x4804CFFF
#define GPIO_2_START 0x481AC000
#define GPIO_2_END   0x481ACFFF
#define GPIO_3_START 0x481AE000
#define GPIO_3_END   0x481AEFFF


#define GPIO_OE           0x134
#define GPIO_DATAIN       0x138
#define GPIO_DATAOUT      0x13C
#define GPIO_CLEARDATAOUT 0x190
#define GPIO_SETDATAOUT   0x194

#define CONTROL_MODULE 0x44E10000
#define GPI01_12        0x830
//#define


#define	am335x_padctl(addr,val)	(*((uint32 *)(addr))) = (val)
#define am355x_read(addr) (*((uint32 *)(addr)))