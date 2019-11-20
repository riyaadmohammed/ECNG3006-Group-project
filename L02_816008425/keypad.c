#include <p18f452.h>
#include <delays.h>
#include "xlcd.h"
#include <string.h>
#include <stdio.h>
#include <adc.h>
#include <stdlib.h>
#include <float.h>
#include <pwm.h>
#include <timers.h> 



#pragma config WDT = OFF //Disable watchdog timer
#pragma config LVP = OFF //Disable low voltage programming

#define _XTAL_FREQ 4000000UL
#define bitmask 240

 void DelayFor18TCY(void){
     Nop(); Nop(); Nop(); Nop(); 
     Nop(); Nop(); Nop(); Nop(); 
     Nop(); Nop(); Nop(); Nop(); 
     Nop(); Nop();
     return;
}

void DelayXLCD(void){
    Delay1KTCYx(5);	
    return;
}

void DelayPORXLCD(void){
    Delay1KTCYx(15);
    return;
}

void init_LCD(void){
    
    TRISD = 0x00;
    PORTD = 0x00;
    OpenXLCD(FOUR_BIT & LINES_5X7);
    while(BusyXLCD());
    SetDDRamAddr(0x00);
    WriteCmdXLCD( SHIFT_DISP_LEFT );
    while(BusyXLCD());
    WriteCmdXLCD( BLINK_ON );
    while(BusyXLCD());
    return;
 }


void readAndDisplayKey(void){

    int output;    
    char y[20];
    char out[20];
    //output = 0;
    //output = output | PORTBbits.RB7;
    //output = output << 1;
    //output = output | PORTBbits.RB6;
    //output = output << 1;
    //output = output | PORTBbits.RB5;
    //output = output << 1;
    //output = output | PORTBbits.RB4;
	output =(PORTC&bitmask);  
    switch (output){
        case 0:
          sprintf(y,"1");
          break;
        case 16:
          sprintf(y,"4");
          break;
        case 32:
          sprintf(y,"7");
          break;
        case 48:
          sprintf(y,"0");
          break;
        case 64:
          sprintf(y,"2");
          break;
        case 80:
          sprintf(y,"5");
          break;
        case 96:
          sprintf(y,"8");
          break;
        case 112:
          sprintf(y,"F");
          break;
        case 128:
          sprintf(y,"3");
          break;
        case 144:
          sprintf(y,"6");
          break;
        case 160:
          sprintf(y,"9");
          break;
        case 176:
          sprintf(y,"E");
          break;
        case 192:
          sprintf(y,"A");
          break;
        case 208:
          sprintf(y,"B");
          break;
        case 224:
          sprintf(y,"C");
          break;
        case 240:
          sprintf(y,"D");
          break;
          
    }
        
        while(BusyXLCD());
        SetDDRamAddr(0x00); //Set cursor to top line
        while(BusyXLCD());
        Delay1KTCYx(110); //Give time for LCD to refresh, writing to it too quickly causes flicker issues
        while(BusyXLCD());
        sprintf(out,"Key : %s",y);
        //sprintf(out,"%04x",output);
        putsXLCD(out);      
 } 

    
 
/******************INTERRUPT THINGS******************/
void high_ISR(void);
 
#pragma code high_vector = 0x08
 void high_interrupt_vector(void){
     _asm 
     GOTO high_ISR
     _endasm
 }
#pragma code 
 
#pragma interrupt high_ISR
void high_ISR(void){
    INTCONbits.GIE = 0;
    
    if(INTCONbits.INT0F==1){

     
     readAndDisplayKey();
     
     
     INTCONbits.INT0E = 0;
     INTCONbits.INT0F = 0;
     
    }
    INTCONbits.INT0E = 1;
    INTCONbits.GIE = 1;
    return;
}

/******************MAIN CODE******************/
 
 void main(){
     
    TRISCbits.RC7=1;
    TRISCbits.RC6=1;
    TRISCbits.RC5=1;
    TRISCbits.RC4=1;
     INTCONbits.GIE = 1;
     INTCONbits.INT0IE = 1;
     INTCON2bits.INTEDG0 = 0;
     init_LCD();
     while(1){}
 }

