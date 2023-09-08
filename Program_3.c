//clock, Data, Direction, Digital Enable, Pull Up resistor _ Redistor Enabling for the portF
# define SYSCTL_RCGCGPIO_R (*(( volatile unsigned long *)0x400FE608))
# define GPIO_PORTF_DATA_RD (*(( volatile unsigned long *)0x40025040)) 
# define GPIO_PORTF_DATA_WR (*(( volatile unsigned long *)0x40025020)) 
# define GPIO_PORTF_DIR_R (*(( volatile unsigned long *)0x40025400)) 
# define GPIO_PORTF_DEN_R (*(( volatile unsigned long *)0x4002551C))
# define GPIO_PORTF_PUR_R (*(( volatile unsigned long *)0x40025510 ))
#define GPIO_PORTB_DEN_R *(( volatile unsigned long *)0x4000551C )
#define GPIO_PORTB_DATA_R *(( volatile unsigned long *)0x400053FC )
#define GPIO_PORTB_DIR_R *(( volatile unsigned long *)0x40005400 )

// IRQ 0 to 31 Set Enable Register
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
// PortF an PortB pin definitions.
# define SYSCTL_RCGC2_GPIOF 0x0022
# define GPIO_PORTF_PIN3_EN 0x08
# define GPIO_PORTF_PIN4_EN 0x10
// variable declaration for display on 7_segments.
unsigned char CC_7Seg_Table[10] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F};

// Default clock frequency and delay definition
# define SYSTEM_CLOC_FREQUENCY  16000000
#define DELAY_DEBOUNCE		SYSTEM_CLOC_FREQUENCY/10000
void Delay(unsigned long counter)
{
	unsigned long i = 0;
	for(i = 0; i<counter; i++);	
}
// Locals Variables
	unsigned long volatile j;
	unsigned short i = 0;
// MAin User Application Program
int main ()
{
	
// Enable the clock for port F
SYSCTL_RCGCGPIO_R |= SYSCTL_RCGC2_GPIOF;

GPIO_PORTF_DEN_R|= GPIO_PORTF_PIN3_EN+ GPIO_PORTF_PIN4_EN ; //Digitally Enabling both the Pins Of PortF.
GPIO_PORTF_DIR_R |=GPIO_PORTF_PIN3_EN ; // PF3 as output.
GPIO_PORTF_DIR_R &= (~GPIO_PORTF_PIN4_EN); // PF4 as input.
GPIO_PORTF_PUR_R |= GPIO_PORTF_PIN4_EN ; // Enable pulp rasistor for portfp4.
	//configure PORTF4, for falling edge trigger interrupt
GPIO_PORTF_IS_R &= (~GPIO_PORTF_PIN4_EN); // PF4 is edge trigred
GPIO_PORTF_IBE_R &= (~GPIO_PORTF_PIN4_EN); // PF4 is not both edge sensitive
GPIO_PORTF_IEV_R &= (~GPIO_PORTF_PIN4_EN); // PF4 falling edge sensitive

GPIO_PORTF_ICR_R |= GPIO_PORTF_PIN4_EN ; // Clearing interrupt Flag in status Register
GPIO_PORTF_IM_R |= GPIO_PORTF_PIN4_EN ; // Masking the interrupt for PortF4
NVIC_PRI7_R |= 0x00300000 ; // Priority is 3 (0000 0000 0011 0000 0000 0000 0000 0000)
NVIC_EN0_R |= NVIC_EN0_INT30; // Enable INT 30 in NVIC
	// Configure Port B direction and digital enable functions
	GPIO_PORTB_DIR_R = 0xFF ;
	GPIO_PORTB_DEN_R = 0xFF ;
	// initial value on the display.
		GPIO_PORTB_DATA_R = 0xFF ;
while (1)
{
	// Do Nothing.
}

} 
 // Interrupt service routine for Port F
void GPIOF_Handler(void)
{
// Insert delay for switch debouncing.
Delay (DELAY_DEBOUNCE);
if(GPIO_PORTF_MIS_R &= 0x10)
{
GPIO_PORTF_DATA_WR ^= GPIO_PORTF_PIN3_EN;
	     i++;
			for (j=0; j< DELAY_DEBOUNCE; j++);
				
			// Check if counter need to reset
			if (i>9)
				{
				i = 0;
			  }
GPIO_PORTF_ICR_R = 0x10 ; // acknowledge flag4
}
      // dispaly on 7_segment.
			// Update the display on 7-Segment
			GPIO_PORTB_DATA_R = CC_7Seg_Table[i];
			// Delay
			Delay (DELAY_DEBOUNCE);	
	}
// End Of The Program.
