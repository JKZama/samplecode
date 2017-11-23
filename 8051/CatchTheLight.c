; Jesse Zamazanuk
; This program is an implementation of the "Catch the light" game featured in many arcades
; The Simon development board consists of nine LEDs arranged in a 3x3 square with a push-button
;  switch below each of the LEDs. The LEDs on the perimeter will light up in a counterclockwise
;  sequence, with the speed depending on the difficulty chosen. The point of the game is to try
;  to "catch" the LED by pressing the switch below the top middle LED when it is lit up. Ten
;  sequential catches results in a win and plays an LED animation and victory song.
;  An unsuccessful catch results in a reset of the score.

#include <reg932.h>
#include <uart.c>
#include <uart.h>


sbit SPKR	= P1^7;                         //Creating variables to access the
sbit SW_1	= P2^0;                         //  switches, LEDs, and speaker
sbit SW_2	= P0^2;
sbit SW_3	= P2^1;
sbit SW_4	= P0^1;
sbit SW_5	= P1^4;
sbit SW_6	= P0^3;
sbit SW_7	= P2^3;
sbit SW_8	= P0^0;
sbit SW_9	= P2^2;
sbit LED_1	= P2^4;		
sbit LED_2	= P0^6;
sbit LED_3	= P2^5;
sbit LED_4	= P0^5;
sbit LED_5	= P1^6;
sbit LED_6	= P0^7;
sbit LED_7	= P2^7;
sbit LED_8	= P0^4;
sbit LED_9	= P2^6;
const unsigned int MAX_DELAY=65536;
const unsigned int EASY_DELAY=250;
const unsigned int MED_DELAY=100;
const unsigned int HARD_DELAY=50;

void init();                                //function declarations
char chooseLevel();
void runGame(char);
void endSequence(char);
void updateScore(char);
void ledToggle(unsigned char);
void playC5(unsigned int count);
void playflatA4(unsigned int count);
void playflatB4(unsigned int count);
void delay(unsigned int count);
void buttonyes();
void buttonno();
void song();

unsigned char title[]={67,97,116,99,104,32,116,104,101,32,76,105,103,104,116,33,32};
char o;
void main()
{	
	P0M1=0;	                            //Setting modes for ports
	P1M1=0;
	P2M1=0;
	TMOD=0x10;                          //setting timer mode
    uart_init();                        //function call the establish the uart
    EA=1;                               //needed to enable the uart interrupt
   
	for(o=0;o<17;o++)
		uart_transmit(title[o]);        //Transmitting the title to the screen
	while(1)
	{
		init();
		runGame(chooseLevel());
	}
}

void init()                         // Initial LED animation to run on startup
{	                                // or restart            
	unsigned char i,j,k;    
	char spiral[]={1,2,3,6,9,8,7,4,5};
	for(k=1;k<3;k++)
	{
		for(i=0;i<9;i++)
		{
			ledToggle(spiral[i]);
			for(j=0;j<12;j++)
			{	
				TH1=0x6F;
				TL1=0xFF;
				TR1=1;
				while(TF1==0);
				TR1=0;
				TF1=0;
			}
		}
	}

	return;
}

char chooseLevel()
{
	char i,j;
	char difficulty=1;
	for(i=1;i<6;i++)
	{							//Display difficulty option (easy is default)
		ledToggle(7);
		for(j=0;j<12;j++)	
		{	
			TH1=0xA5;
			TL1=0xFF;
			TR1=1;
			while(TF1==0);
			TR1=0;
			TF1=0;
		}	
	}
	while(1)						//Cycles through difficulties
    {

		if(SW_5==0&&SW_4==1)
			break;				
		if(SW_4==0)
		{
			while(SW_4==0);
			difficulty++;
			if(difficulty==4)
				difficulty=1;
			if(difficulty==1)
			{
				ledToggle(1);
				ledToggle(7);
			}
			if(difficulty==2)
			{
				ledToggle(4);
				ledToggle(7);
			}
			if(difficulty==3)
			{
				ledToggle(1);
				ledToggle(4);
			}
		}
		if(SW_5==0&&SW_4==0)
			song();				//Play song option
		delay(MAX_DELAY);
		delay(MAX_DELAY);
		delay(MAX_DELAY);
	
	}

	return difficulty;
}

void runGame(char d)
{	
	char i,j;
	char difficulty=d;
	char fail=0;
	char points=0;			
	char spinOrder[]={1,2,3,6,9,8,7,4};	//Order of led animation
	unsigned int ledDelay;


	updateScore(points);
	switch(d)							//Switch to assign delay value based on
	{                                   // difficulty
		case 1: 
			for(i=1;i<6;i++)			//Display chosen difficulty
			{
				ledToggle(7);			
				for(j=0;j<12;j++)	
				{	
					TH1=0xA5;
					TL1=0xFF;
					TR1=1;
					while(TF1==0);
					TR1=0;
					TF1=0;
				}	
			}
			ledDelay=EASY_DELAY;	
			break;

			case 2: 
				for(i=1;i<6;i++)		//Display chosen difficulty
				{
					ledToggle(4);
					for(j=0;j<12;j++)	
					{	
						TH1=0xA5;
						TL1=0xFF;
						TR1=1;
						while(TF1==0);
						TR1=0;
						TF1=0;
					}
				}
				ledDelay=MED_DELAY;
				break;
			case 3: 
				for(i=1;i<6;i++)		//Display chosen difficulty
				{
					ledToggle(1);
					for(j=0;j<12;j++)	
					{	
						TH1=0xA5;
						TL1=0xFF;
						TR1=1;
						while(TF1==0);
						TR1=0;
						TF1=0;
					}
				 }
					ledDelay=HARD_DELAY;
					break;
		}

			for(i=1;i<9;i++)					//Displays arrow towards the top
	{											// middle LED (this is the one
		ledToggle(3);                           // used in the game)
		ledToggle(5);
		ledToggle(9);
		for(j=0;j<15;j++)	
		{	
			TH1=0x95;
			TL1=0xFF;
			TR1=1;
			while(TF1==0);
			TR1=0;
			TF1=0;
		}	
	}
		do
		{
			for(i=0;i<8;i++)        //Cycles through the outer ring of LEDs
			{
				ledToggle(spinOrder[i]);
			
				for(j=0;j<100;j++)
				{
					if(i==7&&SW_5==0)	//button is pressed while top LED is lit
					{
						while(SW_5==0);
						points++;
						buttonyes();
						buttonyes();
						updateScore(points);

					}

					if(i!=7&&SW_5==0)//button is pressed while other LED is lit 
					{
						fail=1;
						buttonno();
						break;

					}

					delay(ledDelay);
				}
				ledToggle(spinOrder[i]);
				for(j=0;j<25;j++)
				{
					delay(ledDelay);
				}
			}

			if(points==10)
				break;					
		}while(fail==0);
		endSequence(points);
					
	return;
}

void updateScore(char points)		//updates seven segment display with points
{
	switch(points)
	{
		case 0:
			SW_1=1;
			SW_2=1;
			SW_3=1;
			SW_6=1;
			SW_7=1;
			SW_8=1;
			SW_9=0;
			break;
		case 1:
			SW_1=0;
			SW_2=1;
			SW_3=1;
			SW_6=0;
			SW_7=0;
			SW_8=0;
			SW_9=0;
			break;
		case 2:
			SW_1=1;
			SW_2=1;
			SW_3=0;
			SW_6=1;
			SW_7=1;
			SW_8=0;
			SW_9=1;
			break;
		case 3:
			SW_1=1;
			SW_2=1;
			SW_3=1;
			SW_6=1;
			SW_7=0;
			SW_8=0;
			SW_9=1;
			break;
		case 4:
			SW_1=0;
			SW_2=1;
			SW_3=1;
			SW_6=0;
			SW_7=0;
			SW_8=1;
			SW_9=1;
			break;
		case 5:
			SW_1=1;
			SW_2=0;
			SW_3=1;
			SW_6=1;
			SW_7=0;
			SW_8=1;
			SW_9=1;
			break;
		case 6:
			SW_1=0;
			SW_2=0;
			SW_3=1;
			SW_6=1;
			SW_7=1;
			SW_8=1;
			SW_9=1;
			break;
		case 7:
			SW_1=1;
			SW_2=1;
			SW_3=1;
			SW_6=0;
			SW_7=0;
			SW_8=0;
			SW_9=0;
			break;
		case 8:
			SW_1=1;
			SW_2=1;
			SW_3=1;
			SW_6=1;
			SW_7=1;
			SW_8=1;
			SW_9=1;
			break;
		case 9:
			SW_1=1;
			SW_2=1;
			SW_3=1;
			SW_6=0;
			SW_7=0;
			SW_8=1;
			SW_9=1;
			break;

	}
			
}
void endSequence(char p)		//Displays ending message serially
{
	char i;
	char win[]={13,89,111,117,32,87,105,110,33};
	char lose[]={13,89,111,117,32,76,111,115,101,46,46,46};
	if(p==10)
	{
		song();
		for(i=0;i<9;i++)
		{
			uart_transmit(win[i]);
		}
	}
	else
	{
		for(i=0;i<12;i++)
		{
			uart_transmit(lose[i]);
		}
	}

	return;
}

void ledToggle(unsigned char ledNum) //Toggles LED. Takes led number as argument
{
	switch(ledNum)
	{
		case 1: 
			if(LED_1==1)
				LED_1=0;
			else
				LED_1=1;
			break;
		case 2: LED_2=~LED_2;
			break;
		case 3: LED_3=~LED_3;
			break;
		case 4: LED_4=~LED_4;
			break;
		case 5: LED_5=~LED_5;
			break;
		case 6: LED_6=~LED_6;
			break;
		case 7: LED_7=~LED_7;
			break;
		case 8: LED_8=~LED_8;
			break;
		case 9: LED_9=~LED_9;
			break;
	}
	return;
}
                                    // Code from here to the bottom is Scott's
void playC5(unsigned int count)     //Function to call for the C5 note.
{                                   // Variable passed determines how long the note plays

 	while(count > 0)
	{
		SPKR=~SPKR;
		TL1=0x4E;
		TH1=0xF2;
		TR1=1;
		while(TF1==0);
		TR1=0;
		TF1=0;
		count--;
	}
}

void playflatA4(unsigned int count) //Function to call for the FlatA4 note.
{                                   // Variable passed determines how long the note plays

 	while(count > 0)
	{
		SPKR=~SPKR;
		TL1=0xA5;
		TH1=0xEE;
		TR1=1;
		while(TF1==0);
		TR1=0;
		TF1=0;
		count--;
	}
}

void playflatB4(unsigned int count) //Function to call for the FlatB4 note.
{                               	// Variable passed determines how long the note plays


 	while(count > 0)
	{
		SPKR=~SPKR;
		TL1=0x8A;
		TH1=0xF0;
		TR1=1;
		while(TF1==0);
		TR1=0;
		TF1=0;
		count--;
	}
}

void delay(unsigned int count) //Function used to help create a gap in the notes
{
	while(count > 0)
	{
		TL1=0xFF;
		TH1=0xFF;
		TR1=1;
		while(TF1==0);
		TR1=0;
		TF1=0;
		count--;
	}
}

void song()                     //Function to call the song for beating the game.
{                               // Based off of Final Fantasy's "Victory Fanfare"
	ledToggle(4);
	ledToggle(5);
	ledToggle(6);
	playC5(250);
	ledToggle(4);
	ledToggle(5);
	ledToggle(6);
	delay(1500);
	ledToggle(4);
	ledToggle(5);
	ledToggle(6);
	playC5(250);
	ledToggle(4);
	ledToggle(5);
	ledToggle(6);
	delay(1500);
	ledToggle(4);
	ledToggle(5);
	ledToggle(6);
	playC5(250);
	ledToggle(4);
	ledToggle(5);
	ledToggle(6);
	delay(1500);
	ledToggle(1);
	ledToggle(2);
	ledToggle(3);
	ledToggle(7);
	ledToggle(8);
	ledToggle(9);
	playflatA4(750);
	ledToggle(1);
	ledToggle(2);
	ledToggle(3);
	ledToggle(7);
	ledToggle(8);
	ledToggle(9);
	playflatB4(750);
	playC5(400);
	playflatB4(400);
	playC5(1800);
}

void buttonyes()               //Function to call for the sound made after 
{                               // pressing a button correctly.
    unsigned int count=50;
    while(count > 0)
	{
	    SPKR=~SPKR;
		TL1=0xF2;
		TH1=0xFC;
		TR1=1;
		while(TF1==0);
		TR1=0;
		TF1=0;
		count--;
	}
    
}

void buttonno()                 //Function to emit the sound made after 
{                               // incorrectly pressing a button
    unsigned int count=50;
	char i;
	for(i=0;i<18;i++)
	{
		ledToggle(1);
		ledToggle(3);
		ledToggle(5);
		ledToggle(7);
		ledToggle(9);
				
	while(count > 0)
	{
	    SPKR=~SPKR;
		TL1=0xF2;
		TH1=0xDC;
		TR1=1;
		while(TF1==0);
		TR1=0;
		TF1=0;
		count--;
	}  
  }

}
