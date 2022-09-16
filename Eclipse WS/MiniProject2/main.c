/*
=========================================================================================================================================
 Program		: Main.c
 Author			: Erini G
 Description 	: Stop Watch MT MiniProject2
 Date			: 15/09/2022
=========================================================================================================================================
*/

/* Linking Section */
#include "main.h"


/*Global Variables Declaration*/
uint8 g_TimerFlag = 0;
uint8 Time [3] = {0};


/* Start of -- Main Section -- */
int main(void)
{
	 GPIO();

	 Timer1_CTC_Init();
	 INT0_Init();
	 INT1_Init();
	 INT2_Init();

	while(1)
	{

		DisplayTimer();
		/*if 1 second passed and the timer is triggered*/
		if(g_TimerFlag == 1)
		{
			/*CountUp the Timer*/
			CountUp();
			/*Reset the flag till it's triggered again*/
			g_TimerFlag = 0;
		}

	}
}
/* End of -- Main Section -- */


/*************************************************************************************************************************
/*	Function Name : GPIO.

/*	Function Info : setting the direction of the pins and initializing them

/*	Function Input: void
*************************************************************************************************************************/
void GPIO(void)
{
	/*Set the first 4 pins of Port C as an output pins*/
	DDRC |= (0x0F);
	/*initialize the first 4 pins of Port C with 0*/
	PORTC &= ~ (0X0F);

	/*Set the first 6 pins of Port A as an output pins*/
	DDRA |= (0x3F);
	/*initialize the first 6 pins of Port C with 0*/
	PORTA &= ~(0x3F);

	/*Set the GIE*/
	SREG |= (1<<7);
}



/*************************************************************************************************************************
/*	Function Name : Timer1_CTC_Init.

/*	Function Info : enables Timer1 in compare match mode

/*	Function Input: void
*************************************************************************************************************************/
void Timer1_CTC_Init(void)
{
	TCNT1 = 0;		/* Set timer1 initial count to zero */

	OCR1A = 1000;    /* Set the Compare value to 1000 */

	TIMSK |= (1<<OCIE1A); /* Enable Timer1 Compare A Interrupt */

	/* Configure timer control register TCCR1A
	 * 1. Non-PWM Mode -> FOC1A=1
	 */
	TCCR1A = (1<<FOC1A);

	/* Configure timer control register TCCR1B
	 * 1. CTC Mode WGM12=1 (Mode Number 4)
	 * 2. Prescaler = F_CPU/1024 CS10=1 CS11=0 CS12=1
	 * 			FCPU = 1Mhz, FTimer = 1/1024
	 * 			TTimer = 1/(1/1024) -> 1024
	 * 			Ticks = 1000
	 * 			TCompare = 1024 * 1000 ->1024000us
	 * 			1/(1024000 * 10^(-6)) = 0.976 -> 1 sec
	 * 			1 sec takes 1000 ticks
	 */
	TCCR1B = (1<<WGM12) | (1<<CS10) | (1<<CS12);
}

/*if Timer1 is triggered as a compare match*/
ISR(TIMER1_COMPA_vect)
{
	/*set the timer flag*/
	g_TimerFlag = 1;
}



/*************************************************************************************************************************
/*	Function Name : INT0_Init.

/*	Function Info : enables External Interrupt 0 to reset the timer

/*	Function Input: void
*************************************************************************************************************************/
void INT0_Init(void)
{
	DDRD &= ~(1<<INT0); //I/P
	MCUCR = (1<<ISC01); //Falling Edge
	GICR |= (1<<INT0); //ENABLE MIE

}

/*if INT0 is triggered*/
ISR (INT0_vect)
{
	for(int i = 0; i < 3; i++)
	{
		/*reset the stop watch*/
		Time[i] = 0;
	}
}



/*************************************************************************************************************************
/*	Function Name : INT1_Init.

/*	Function Info : enables External Interrupt 1 to pause the timer

/*	Function Input: void
*************************************************************************************************************************/
void INT1_Init(void)
{
	DDRD &= ~(1<<INT1); //I/P
	MCUCR = (1<<ISC10) | (1<<ISC11); //Rising Edge
	GICR |= (1<<INT1); //ENABLE MIE
}

/*if INT1 is triggered*/
ISR(INT1_vect)
{
	/*turn off the source clock to stop disconnect the timer*/
	TCCR1B = 0;
}



/*************************************************************************************************************************
/*	Function Name : INT2_Init.

/*	Function Info : enables External Interrupt 2 to resume the timer

/*	Function Input: void
*************************************************************************************************************************/
void INT2_Init(void)
{
	DDRB &= ~(1<<INT2); //I/P
	MCUCSR &= ~(1<<ISC2); //Falling Edge
	GICR |= (1<<INT2); //ENABLE MIE
}

/*if INT2 is triggered*/
ISR(INT2_vect)
{
	/*turn on the source clock to stop reconncet the timer*/
	TCCR1B |= (1<<WGM12) | (1<<CS10) | (1<<CS12);
}



/*************************************************************************************************************************
/*	Function Name : CountUp.

/*	Function Info : function that acts as a counter up for the timer

/*	Function Input: void
*************************************************************************************************************************/
void CountUp(void)
{
	Time[SECS]++;
	/*if the seconds reaches 60 reset it to 0 and increment the minutes*/
	if(Time[SECS]>=60)
	{
		Time[SECS] = 0;
		Time[MINS]++;
	}
	/*if the minutes reaches 60 reset it to 0 and increment the hours*/
	if(Time[MINS]>=60)
	{
		Time[MINS] = 0;
		Time[HRS]++;
	}
}



/*************************************************************************************************************************
/*	Function Name : DisplayTimer.

/*	Function Info : Enables the 7 segments and displays the time digits on each one of them

/*	Function Input: void
*************************************************************************************************************************/
void DisplayTimer(void)
{
	/*Enable the 1st 7 segment*/
	PORTA = (1<<PA0);
	/*Display the 1st digit of the seconds*/
	PORTC = (PORTC & (0xF0)) | (Time[SECS] % 10);

	_delay_us(10);

	/*Enable the 2nd 7 segment*/
	PORTA = (1<<PA1);
	/*Display the 2nd digit of the seconds*/
	PORTC = (PORTC & (0xF0)) | (Time[SECS]/10);

	_delay_us(10);

	/*Enable the 3rd 7 segment*/
	PORTA = (1<<PA2);
	/*Display the 1st digit of the minutes*/
	PORTC = (PORTC & (0xF0)) | (Time[MINS] % 10);

	_delay_us(10);

	/*Enable the 4th 7 segment*/
	PORTA = (1<<PA3);
	/*Display the 2nd digit of the minutes*/
	PORTC = (PORTC & (0xF0)) | (Time[MINS]/10);

	_delay_us(10);

	/*Enable the 5th 7 segment*/
	PORTA = (1<<PA4);
	/*Display the 1st digit of the hours*/
	PORTC = (PORTC & (0xF0)) | (Time[HRS] %10);

	_delay_us(10);

	/*Enable the 6th 7 segment*/
	PORTA = (1<<PA5);
	/*Display the 2nd digit of the hours*/
	PORTC = (PORTC & (0xF0)) | (Time[HRS]/10);

	_delay_ms(1);
}
