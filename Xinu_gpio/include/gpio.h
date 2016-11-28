//#define GPIO Configuration
#define GPIO_0_START 0x44E07000
#define GPIO_1_START 0x4804C000
#define GPIO_2_START 0x481AC000
#define GPIO_3_START 0x481AE000
#define CONTROL_MODULE_BASE 0x44E10000

#define NUM_GPIO_MODULES 4
#define PINS_PER_MODULE 32

//namespace A{}
typedef enum gpio_module {
module0 = 0x44E07000,
module1 = 0x4804C000,
module2 = 0x481AC000,
module3 = 0x481AE000
}gpio_module;


#define GPIO_WRITE 0
#define GPIO_READ 1
 


#define INTC_GPIO_0A 96
#define INTC_GPIO_0B 97
#define INTC_GPIO_1A 98
#define INTC_GPIO_1B 99
#define INTC_GPIO_2A 32
#define INTC_GPIO_2B 33
#define INTC_GPIO_3A 62
#define INTC_GPIO_3B 63


typedef struct gpio_csreg {
	uint32	revision;
	uint32	res1[3];
	uint32	sys_config;
	uint32  res2[3];
	uint32	eoi;
	uint32	irq_status_raw0;
	uint32	irq_status_raw1;
	uint32	irq_status_0;
	uint32	irq_status_1;
	uint32	irq_status_set0;
	uint32	irq_status_set1;
	uint32	irq_status_clr0;
	uint32	irq_status_clr1;
	uint32	irq_waken_0;
	uint32	irq_waken_1;
	uint32  res3[50];
	uint32  sys_status;
	uint32  res4[6];
	uint32	ctrl;
	uint32  oe;
	uint32  datain;
	uint32  dataout;
	uint32  leveldetect_0;
	uint32  leveldetect_1;
	uint32  risingdetect;
	uint32  fallingdetect;
	uint32  debounce_enable;
	uint32  debounce_time;
	uint32  res5[14];
	uint32  clear_data_out;
	uint32  set_data_out;
}gpio_csreg;

typedef void (* handler) (void);
typedef struct handler_list
{
	handler h;
	struct handler_list *next;
}handler_list;

extern handler_list* rising_list[][32];
extern handler_list* falling_list[][32];
extern const uint32 pin_map[][46];
extern const uint32 control_module[][32];
extern const gpio_module module[];


#define	MM_WRITE(addr,val)	(*((uint32 *)(addr))) = (val)
#define MM_READ(addr) (*((uint32 *)(addr)))

#define BIT_SET(val,bit)     val |= (1 << bit)
#define BIT_CLEAR(val,bit)   val &= ~(1 << bit)
#define BIT_READ(val,bit)    ((val >> bit) & 1)


#define NUM_EXPANSION_HEADERS 2
#define PINS_PER_HEADER 46
#define isvalidpin(exp,pin) ((exp < NUM_EXPANSION_HEADERS) && (pin > 0) && (pin <= PINS_PER_HEADER))

/* Function declarations */
void gpio_init();
void gpio_handler(uint8);
void gpio_handler_mod0(void);
void gpio_handler_mod1(void);
void gpio_handler_mod2(void);
void gpio_handler_mod3(void);
bool8 gpio_read(uint8,uint8);
bool8 gpio_subscribe(uint8,uint8,handler,bool8);
bool8 gpio_unsubscribe(uint8,uint8, handler,bool8);
bool8 gpio_write(uint8,uint8,bool8);
void control_module_init(gpio_module module);
void gpio_module_init(gpio_module module);
bool8 gpio_set_mode(uint8,uint8,bool8);


