#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define NUMBER_OF_OVERFLOWS_PER_HALF_SECOND 2
#define second 1
#define min 240
#define hour 14400

unsigned char Flag_0 = 0;
unsigned char Flag_1 = 0;
unsigned char Flag_2 = 0;
unsigned char num = 0;
unsigned char num_2 = 0;
unsigned char min_1 = 0;
unsigned char min_2 = 0;
unsigned char hr_1 = 0;
unsigned char hr_2 = 0;
unsigned char g_tick_sec = 0;
unsigned char pause = 0;
unsigned char sec_var1 = 0;
unsigned char sec_var2 = 0;
unsigned char min_var1 = 0;
unsigned char min_var2 = 0;
unsigned char hr_var1 = 0;
unsigned char hr_var2 = 0;
/*Initialization GPIO*/
void GPIO_INIT(void)
{



/*PORT A*/
DDRA|=(1<<PA0)|(1<<PA1)|(1<<PA2)|(1<<PA3)|(1<<PA4)|(1<<PA5);

PORTA|=(1<<PA0)|(1<<PA1)|(1<<PA2)|(1<<PA3)|(1<<PA4)|(1<<PA5);

/*PORT C*/
DDRC |= 0x0F;
PORTC &= 0xF0;
}

/* ISR */
ISR(INT0_vect)
{
	Flag_0 = 1;
}
ISR(INT1_vect)
{
	pause=0;



}
ISR(INT2_vect)
{

pause=1;

}
ISR(TIMER0_COMP_vect)
{
	g_tick_sec++;

	if(g_tick_sec == second)
	{
		num++;

		g_tick_sec=0;
	}

}

/* External INT0 enable and configuration function */
void INT0_Init(void)
{
	SREG  &= ~(1<<7);                   // Disable interrupts by clearing I-bit
	GICR  |= (1<<INT0);                 // Enable external interrupt pin INT0
	MCUCR &= ~ (1<<ISC00);
	MCUCR |= (1<<ISC01);                 // Trigger INT0 with the falling edge
	SREG  |= (1<<7);                    // Enable interrupts by setting I-bit
	/*PORT D*/
	DDRD &=~(1<<PD2);/*PULL UP*/
	PORTD |= (1<<PD2);

}
void INT1_Init(void)
{
	SREG  &= ~(1<<7);      // Disable interrupts by clearing I-bit
	GICR  |= (1<<INT1);    // Enable external interrupt pin INT1
	// Trigger INT1 with the falling edge
	MCUCR |= (1<<ISC11);
	MCUCR &= ~(1<<ISC10);
	SREG  |= (1<<7);       // Enable interrupts by setting I-bit
	DDRD &=~(1<<PD3);/*PULL DOWN*/
}
void INT2_Init(void)
{
	SREG  &= ~(1<<7);
	DDRB &=~(1<<PB2); /*PULL UP*/
	PORTB |= (1<<PB2);// Disable interrupts by clearing I-bit
	GICR  |= (1<<INT2);    // Enable external interrupt pin INT1
	MCUCSR &= ~ (1<<ISC2);
	SREG  |= (1<<7);       // Enable interrupts by setting I-bit


}
void timer0_init_CTC_mode(unsigned char tick)
{
	TCNT0 = 0;
	OCR0  = tick;
	TIMSK |= (1<<OCIE0);
	TCCR0 = (1<<FOC0) | (1<<WGM01) | (1<<CS02) | (1<<CS00);
}
void state_machine(void)
{
	if(pause==0)
			{
			if(Flag_0==1)
			{
				num = 0;
				num_2=0;
				min_1=0;
				min_2=0;
				hr_1=0;
				hr_2=0;
			   Flag_0=0;
			}

			if(num==10)
			{
				num=0;
				num_2++;
			}
			if(num_2==6)
			{
				num_2=0;
				min_1++;
			}
			if(min_1==10)
			{
				min_1=0;
				min_2++;

			}
			if(min_2==6)
			{
				min_2=0;
				hr_1++;

			}
			if(min_2==6)
			{
				min_2=0;
				hr_1++;

			}
			if(hr_1==10)
			{
				hr_1=0;
				hr_2++;

			}


			PORTC=0;
			PORTC = num;
			PORTA=(1<<PA0);
			PORTA&=~(1<<PA0);
			PORTC = num_2;
			PORTA=(1<<PA1);
			PORTA&=~(1<<PA1);
			PORTC = min_1;
			PORTA=(1<<PA2);
			PORTA&=~(1<<PA2);
			PORTC = min_2;
			PORTA=(1<<PA3);
			PORTA&=~(1<<PA3);
			PORTC = hr_1;
			PORTA=(1<<PA4);
			PORTA&=~(1<<PA4);
			PORTC = hr_2;
			PORTA=(1<<PA5);
			PORTA&=~(1<<PA5);

			sec_var1 = num;
			 sec_var2 = num_2;
			 min_var1 =min_1;
			 min_var2 = min_2;
			 hr_var1 = hr_1;
			 hr_var2 = hr_2;
			}
			 /************************************************/
			else
			{
			        PORTC = sec_var1;
			 		PORTA=(1<<PA0);
			 		PORTA&=~(1<<PA0);
			 		PORTC = sec_var2;
			 		PORTA=(1<<PA1);
			 		PORTA&=~(1<<PA1);
			 		PORTC = min_var1;
			 		PORTA=(1<<PA2);
			 		PORTA&=~(1<<PA2);
			 		PORTC = min_var2;
			 		PORTA=(1<<PA3);
			 		PORTA&=~(1<<PA3);
			 		PORTC =hr_var1;
			 		PORTA=(1<<PA4);
			 		PORTA&=~(1<<PA4);
			 		PORTC = hr_var2;
			 		PORTA=(1<<PA5);
			 		PORTA&=~(1<<PA5);
			}
}

int main(void)
{

	GPIO_INIT();
	INT0_Init();
	INT1_Init();
	INT2_Init();
	timer0_init_CTC_mode(250);

	PORTC = (PORTC & 0xF0) | (num & 0x0F);
	_delay_ms(500);
	while(1)
	{
		state_machine();

	}
}

