//Register Configuration for PortF.
# define SYSCTL_RCGCGPIO_R (*(( volatile unsigned long *)0x400FE608))
# define GPIO_PORTF_DATA_WR (*(( volatile unsigned long *)0x40025020)) 
# define GPIO_PORTF_DIR_R (*(( volatile unsigned long *)0x40025400)) 
# define GPIO_PORTF_DEN_R (*(( volatile unsigned long *)0x4002551C))
//SysTic Register Definition.
#define ST_CTRL_R (*((volatile unsigned long*)0xE000E010)) //Systick control and status register
#define ST_RELOAD_R (*((volatile unsigned long*)0xE000E014)) // Systick reload value register
#define ST_CURRENT_R (*((volatile unsigned long *)0xE000E018)) // Systick reload Current register

int main()
{
 SYSCTL_RCGCGPIO_R  |= 0x20; // turn on bus clock for GPIOF
  GPIO_PORTF_DIR_R  |= 0x08; //set GREEN pin as a digital output pin
  GPIO_PORTF_DEN_R  |= 0x08;  // Enable PF3 pin as a digital pin
	

	ST_RELOAD_R = 1600000; // one second delay relaod value.
	ST_CTRL_R  = 7 ; // enable counter, interrupt and select system bus clock 
	ST_CURRENT_R = 0; //initialize current value register 
   
	while (1)
	{
		//do nothing here
	}
}

// this routine will execute after every one second

void SysTick_Handler(void)
{
   GPIO_PORTF_DATA_WR  ^= 8;  //toggle PF3 pin
  
}