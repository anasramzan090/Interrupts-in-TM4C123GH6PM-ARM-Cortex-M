//clock, Data, Direction, Digital Enable, Pull Up resistor _ Redistor Enabling for the portF
# define SYSCTL_RCGCGPIO_R (*(( volatile unsigned long *)0x400FE608))
# define GPIO_PORTF4_DATA_RD (*(( volatile unsigned long *)0x40025040)) 
# define GPIO_PORT0_DATA_RD (*(( volatile unsigned long *)0x40025004)) 
# define GPIO_PORTF_DATA_WR (*(( volatile unsigned long *)0x40025020)) 
# define GPIO_PORTF_DIR_R (*(( volatile unsigned long *)0x40025400)) 
# define GPIO_PORTF_DEN_R (*(( volatile unsigned long *)0x4002551C))
# define GPIO_PORTF_PUR_R (*(( volatile unsigned long *)0x40025510 ))
// IRQ 0 to 31 Set Enable Register.
# define NVIC_EN0_R *(( volatile unsigned long *)0xE000E100 )
# define NVIC_EN0_INT30 0x40000000 // Interrupt 30 enable.
//Interrupt Resiters Configuration For PortF.
# define NVIC_PRI7_R *(( volatile unsigned long *)0xE000E41C ) // IRQ 28 to 31 Priority Register NVIC_PRI7_R.
# define GPIO_PORTF_IS_R *(( volatile unsigned long *)0x40025404 )
# define GPIO_PORTF_IBE_R *(( volatile unsigned long *)0x40025408 )
# define GPIO_PORTF_IEV_R *(( volatile unsigned long *)0x4002540C )
# define GPIO_PORTF_IM_R *(( volatile unsigned long *)0x40025410 )
# define GPIO_PORTF_ICR_R *(( volatile unsigned long *)0x4002541C )
#define GPIO_PORTF_MIS_R  *(( volatile unsigned long *)0x40025418 )
#define GPIO_PORTF_LOCK_R  *(( volatile unsigned long *)0x40025520) // To Unlock the Commit Egister for Taking interrupt from Switch2.
#define GPIO_PORTF_CR_R  *(( volatile unsigned long *)0x40025524) // Commit Register Enabling.
// PortF pin definitions.
# define SYSCTL_RCGC2_GPIOF 0x0020
# define GPIO_PORTF_PIN3_EN 0x08
# define GPIO_PORTF_PIN4_EN 0x10
# define GPIO_PORTF_PIN0_EN 0x01

// Default clock frequency and delay definition
# define SYSTEM_CLOC_FREQUENCY  16000000
#define DELAY_DEBOUNCE		SYSTEM_CLOC_FREQUENCY/10000
void Delay(unsigned long counter)
{
	unsigned long i = 0;
	for(i = 0; i<counter; i++);	
}
//Main Program.
int main(void)
{
	 SYSCTL_RCGCGPIO_R |= SYSCTL_RCGC2_GPIOF; /* Set bit5 of RCGCGPIO to enable clock to PORTF*/
    
	//  /* PORTF0 has special function, need to unlock to modify */
  GPIO_PORTF_LOCK_R |= 0x4C4F434B;   /* unlock commit register */
  GPIO_PORTF_CR_R |= 0x01;           /* make PORTF0 configurable */
  GPIO_PORTF_LOCK_R |= 0;            /* lock commit register */


    /*Initialize PF3 as a digital output, PF0 and PF4 as digital input pins */
	
    GPIO_PORTF_DIR_R &= (~GPIO_PORTF_PIN4_EN)|~(GPIO_PORTF_PIN0_EN );/* Set PF4 and PF0 as a digital input pins */
    GPIO_PORTF_DIR_R |=GPIO_PORTF_PIN3_EN; /* Set PF3 as digital output to control green LED */
    GPIO_PORTF_DEN_R|= GPIO_PORTF_PIN3_EN + GPIO_PORTF_PIN4_EN + GPIO_PORTF_PIN0_EN ; /* make PORTF4-0 digital pins */
    GPIO_PORTF_PUR_R |= GPIO_PORTF_PIN4_EN |(GPIO_PORTF_PIN0_EN );/* enable pull up for PORTF4, 0 */
    
    /* configure PORTF4, 0 for falling edge trigger interrupt */
   GPIO_PORTF_IS_R &= (~GPIO_PORTF_PIN4_EN) |~(GPIO_PORTF_PIN0_EN );/* make bit 4, 0 edge sensitive */
    GPIO_PORTF_IBE_R &= (~GPIO_PORTF_PIN4_EN)|~(GPIO_PORTF_PIN0_EN );/* trigger is controlled by IEV */
   GPIO_PORTF_IEV_R &= (~GPIO_PORTF_PIN4_EN)|~(GPIO_PORTF_PIN0_EN );/* falling edge trigger */
    GPIO_PORTF_ICR_R |= GPIO_PORTF_PIN4_EN|(GPIO_PORTF_PIN0_EN );/* clear any prior interrupt */
   GPIO_PORTF_IM_R |= GPIO_PORTF_PIN4_EN|(GPIO_PORTF_PIN0_EN );/* unmask interrupt */
    
    /* enable interrupt in NVIC and set priority to 3 */
   NVIC_PRI7_R |= 0x00300000 ; /* set interrupt priority to 3 */
   NVIC_EN0_R |= NVIC_EN0_INT30; /* enable IRQ30 (D30 of ISER0) */

    
    while(1)
    {
			// do nothing and wait for the interrupt to occur
    }
}

/* SW1 is connected to PF4 pin, SW2 is connected to PF0. */
/* Both of them trigger PORTF falling edge interrupt */
void GPIOF_Handler(void)
{	
  Delay (DELAY_DEBOUNCE);
  if (GPIO_PORTF_MIS_R & 0x10) /* check if interrupt causes by PF4/SW1*/
    {   
      GPIO_PORTF_DATA_WR|= GPIO_PORTF_PIN3_EN;
     GPIO_PORTF_ICR_R|= 0x10; /* clear the interrupt flag */
     } 
    else if (GPIO_PORTF_MIS_R & 0x01) /* check if interrupt causes by PF0/SW2 */
    {   
     GPIO_PORTF_DATA_WR &= ~GPIO_PORTF_PIN3_EN;
    GPIO_PORTF_ICR_R|= 0x01; /* clear the interrupt flag */
    }
}