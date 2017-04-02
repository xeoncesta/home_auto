
/*HOME-AUTOMATION USING USART COMM. [BLUETOOTH]*/

#include<avr/io.h> /*Includes io.h header file where all the Input/Output Registers and its Bits are defined for all AVR microcontrollers*/
#include<util/delay.h> /*Includes delay.h header file which defines two functions, _delay_ms (millisecond delay) and _delay_us (microsecond delay)*/ 
#include<string.h>

//#define F_CPU 8000000
/*Defines a macro for the delay.h header file. F_CPU is the microcontroller frequency value for the delay.h header file. Default value of F_CPU in delay.h header file is 1000000(1MHz)*/
//LCD FUNC.:cmd(),data() lcd(),string(),num()

void cmd(char c)
{
PORTB=c;
PORTC=0x04;
_delay_ms(10);
PORTC=0x00;

}
void data(char c)
{
PORTB=c;
PORTC=0x05;
_delay_ms(10);
PORTC=0x01;

}

void lcd()
{

cmd(0x38);    //selecting 8-bit matrix
_delay_ms(10);   
cmd(0x01);        //clear
_delay_ms(10);
cmd(0x0e);       //display on ,cursor on
_delay_ms(10);
cmd(0x80);         //selecting first row first column
_delay_ms(10);

}

void string(unsigned char *p)    //function for displaying strings
{

while(*p!='\0')
{data(*p);
p++;}

}


void num(unsigned int p)    //functions for displaying ASCII values
{

unsigned int k,w,b=1;
k=p;
while(k>=10)
{
b=b*10;
k=k/10;
}
while(b>0)
{
w=p/b;
p=p%b;
b=b/10;
data(w+48);

}


}
unsigned char x;
unsigned char arr[6]="000000";    //temp array
//APPLIANCES-Enter the following in virtual terminal.
unsigned char card1[6]="LIGHT1";  
unsigned char card2[6]="LIGHT2";
unsigned char card3[6]="SOUND ";
unsigned char card4[6]="FAN   ";


void main()
{
//Ports init.
DDRC=0xff;
DDRB=0xff;
DDRD=0x00;
DDRA=0xff;
PORTA=0x00;
lcd();
string("WELCOME To NSITAUTO          __[ver-1.0]");

_delay_ms(3000);

//cmd(0x1c);
//registers initialization ,see datasheet
UBRRL=51;
UCSRB=(1<<TXEN)|(1<<RXEN);
UCSRC=(1<<URSEL)|(1<<UCSZ0)|(1<<UCSZ1);
_delay_ms(800);

while(1)
{
lcd();
cmd(0x80);
string("Enter your command:");
_delay_ms(400);
cmd(0xc0);
for(int i=0;i<=5;i++)
{
while((UCSRA&(1<<RXC))==0);//polling for recieving data in UDR buffer

x=UDR;
arr[i]=x;
data(x);

}
cmd(0x01);
if((strncmp(arr,card1,6))==0)
{
 cmd(0x80);
 string("LIGHT1");
 PORTA=0x01;
 _delay_ms(40000);
//break; 

}
else if((strncmp(arr,card2,6))==0)
{
 cmd(0x80);
 
 string("LIGHT2");
 PORTA=0x02;
_delay_ms(4000);
//break;
}
else if((strncmp(arr,card3,6))==0)
{
 cmd(0x80);
 
 string("SPEAKERS");
 PORTA=0x04;
_delay_ms(40000);
//break;
}
else if((strncmp(arr,card4,6))==0)
{
 cmd(0x80);
 
 string("FAN");
 PORTA=0x08;
_delay_ms(4000);
//break;
}

else
{
//cmd(0x01);
cmd(0x80);
 
string("SORRY!!  Wrong cmd:");
PORTA=0x00;
_delay_ms(4000);

}


}}
