#include <p18f452.h>
#include <delays.h>
#include "xlcd.h"
#include <stdio.h>
#include <stdlib.h>
#include <timers.h>
#include <pwm.h>


#pragma config WDT = OFF //Disable watchdog timer
#pragma config LVP = OFF //Disable low voltage programming

#define _XTAL_FREQ 4000000UL

/*************Global Variable Declarations*********************/

volatile int heartBeats =0;
volatile int heartBeatz ;
volatile int heartrate;
volatile int allowCount=0;
volatile int countsamples=0;
volatile float avg=0;
volatile int Avg=0;
volatile int sum;
volatile char out[30];
volatile int pulse;
volatile unsigned int pulse1;
volatile unsigned int pulse2;
volatile float Interval;
volatile int interval;
volatile int NN;
volatile int NN50;
volatile float Hrv;
volatile int HRv;

/*********************Function Declarations*************************/
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
     
    PORTD = 0x00;
    TRISD = 0x00;
    OpenXLCD(FOUR_BIT & LINES_5X7);
    while(BusyXLCD());
    SetDDRamAddr(0x00);
    WriteCmdXLCD( SHIFT_DISP_LEFT );
    while(BusyXLCD());
    WriteCmdXLCD( BLINK_ON );
    while(BusyXLCD());
    return;
 }

void init_Timer0(void){
 
    OpenTimer0(TIMER_INT_ON & T0_16BIT & T0_SOURCE_INT & T0_PS_1_256);
}

void startHeartBeatSample(void){
    
    INTCONbits.TMR0IE = 1;          //Enable Timer0 Interrupt
    INTCONbits.TMR0IF = 0;          //Clear Timer0 Interrupt Flag
    INTCON2bits.TMR0IP = 1;          //Timer0 High Priority
    WriteTimer0(0x676A);            //Timer interrupts at 10s w/256 prescaler
    //WriteTimer0(57224); 
    TRISDbits.RD3 = 0;
    PORTDbits.RD3 =1;
    allowCount = 0;

}

void HRV(void){
    if(pulse==1){
        pulse1= ReadTimer0();
    }
    else if(pulse >1){
        pulse=0;
        pulse2= ReadTimer0();
        NN++;
        Interval= ((pulse2- pulse1)*256)/100;
        interval= Interval;
        if(interval>50){
            NN50++;
            
        }
         while(BusyXLCD());
        SetDDRamAddr(0x40); //Set cursor to top line
        while(BusyXLCD());
        Delay1KTCYx(110); //Give time for LCD to refresh, writing to it too quickly causes flicker issues
        while(BusyXLCD());
        sprintf(out,"HR interval: %d ms",interval);
        putsXLCD(out); 
    }
}

void stopHeartBeatSample(void){
    allowCount=1;
    countsamples++;
    PORTDbits.RD3 =0;
    INTCON3bits.INT2IE = 0;
    CloseTimer0();
    while(BusyXLCD());
    SetDDRamAddr(0x00); //Set cursor to top line
    while(BusyXLCD());
    Delay1KTCYx(110); //Give time for LCD to refresh, writing to it too quickly causes flicker issues
    while(BusyXLCD());
    Hrv = ((float)NN50/(float)NN)*100;
    HRv=Hrv;
    sprintf(out,"HRV:%d %",HRv);
    putsXLCD(out);
    if(allowCount==1){
    while(BusyXLCD());
    SetDDRamAddr(0x14); //Set cursor to top line
    while(BusyXLCD());
    Delay1KTCYx(110); //Give time for LCD to refresh, writing to it too quickly causes flicker issues
    while(BusyXLCD());
    heartBeats = (6*heartBeats);
    heartBeatz = heartBeats;
    sum = sum + heartBeats;
    avg = sum/countsamples;
    Avg=avg;
    sprintf(out,"Heartrate:%d BPM",heartBeats);
    putsXLCD(out);
    while(BusyXLCD());
    SetDDRamAddr(0x54); //Set cursor to top line
    while(BusyXLCD());
    Delay1KTCYx(110); //Give time for LCD to refresh, writing to it too quickly causes flicker issues
    while(BusyXLCD());
    sprintf(out,"Avg HR: %d BPM",Avg);
    putsXLCD(out); 
     if(heartBeatz>100)
         {
          OpenTimer2(  TIMER_INT_OFF &    T2_PS_1_16 & T2_POST_1_1  );
          OpenPWM1(2000);
          SetDCPWM1(511);
          
         
         }
    if(heartBeatz<=100)
         {
          CloseTimer1();
          ClosePWM1();
                   
         }
    }
    
}

void heartBeatCount(void){

    if(allowCount==0){
    heartBeats++;
    while(BusyXLCD());
    SetDDRamAddr(0x14); //Set cursor to top line
    while(BusyXLCD());
    Delay1KTCYx(110); //Give time for LCD to refresh, writing to it too quickly causes flicker issues
    while(BusyXLCD());
    sprintf(out,"Heartbeat: %d BPM",heartBeats);
    putsXLCD(out); 
    }    
 } 

void resetcount(void){
    INTCON3bits.INT2IP = 1;
    INTCON2bits.INTEDG2 = 0;
    INTCON3bits.INT2IE = 1;
    INTCONbits.RBIF = 0;
    
    
    OpenTimer0(TIMER_INT_ON & T0_16BIT & T0_SOURCE_INT & T0_PS_1_256);
    WriteTimer0(0x676A);            //Timer interrupts at 10s w/256 prescaler
    heartBeats=0;
    allowCount=0;
    NN50=0;
    NN=0;
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
    
    if(INTCON3bits.INT2IF==1){
        INTCON3bits.INT2IE = 0;
        heartBeatCount();
        pulse++;
        HRV();
        INTCON3bits.INT2IF = 0;
        INTCON3bits.INT2IE = 1;
    }
    
    if(INTCONbits.TMR0IF==1){
        INTCONbits.TMR0IF=0;
        INTCONbits.TMR0IE= 0;
        stopHeartBeatSample();
        
    }
    if(INTCON3bits.INT1IF==1){
        INTCON3bits.INT1IE = 0;
        resetcount();
        INTCON3bits.INT1IF = 0;
        INTCON3bits.INT1IE = 1;
        INTCON3bits.INT2IE = 1;
    }
    INTCONbits.GIE = 1;
    return;
}

/******************MAIN CODE******************/
 
 void main(){
     heartBeatz = 0;
     sum=0;
     heartrate=0;
       pulse=0;
     pulse1=0;
     pulse2=0;
     NN=0;
     NN50=0;
     Hrv=0;
     HRv=0;
     init_LCD();
     //init_Timer0();
     RCONbits.IPEN = 1;              //Enable Priority Levels
     
     INTCON3bits.INT1IP = 1;
     INTCON2bits.INTEDG1 = 1;
     INTCON3bits.INT1IE = 1;
     
     INTCONbits.GIE = 1;
     INTCONbits.TMR0IE = 1;          //Enable Timer0 Interrupt
     INTCONbits.TMR0IF = 0;          //Clear Timer0 Interrupt Flag
     INTCON2bits.TMR0IP = 1;          //Timer0 High Priority
     
     //startHeartBeatSample();
     while(1){
        
     
     }
 }