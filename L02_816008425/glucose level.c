#include <p18f452.h>
#include <delays.h>
#include "xlcd.h"
#include <stdio.h>
#include <stdlib.h>
#include <adc.h>
#include <timers.h>
#include <math.h>

#pragma config OSC = HS
#pragma config WDT = OFF //Disable watchdog timer
#pragma config LVP = OFF //Disable low voltage programming

#define _XTAL_FREQ 4000000UL

 
float result;
float adcresult;
char reading[20];
char output [20];
float  peak_val_1=0;
float  peak_val_2=0;
float optical_density;
int glucose;
int flag=0;


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
     
    PORTD = 0X00;
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
 
    OpenTimer0(TIMER_INT_ON & T0_16BIT & T0_SOURCE_INT & T0_PS_1_128);//5 seconds
}
 
void ADC_read (){
    
    OpenADC(ADC_FOSC_8 & ADC_RIGHT_JUST & ADC_8ANA_0REF,ADC_CH0 & ADC_INT_OFF );//Configure ADC
    init_Timer0(); 
    INTCONbits.TMR0IE = 1;  //enable Timer 0 Interrupts 
    INTCONbits.GIEL = 1;    //Enable Global low priority interrupts
    INTCONbits.GIE = 1;     //enable global interrupts
    WriteTimer0(26474);     //Timer interrupts at 5s
        
    
    while (1)   
    {
        adcresult = 0;
        result = 0;
        Delay10TCYx( 5 ); // Delay for 50TCY
        while(BusyADC());
        ConvertADC(); // Start conversion
        while( BusyADC() ); // Wait for completion
        adcresult = ReadADC(); // Read result         
        result = adcresult; 
        
                   
        if (flag == 0)
        {
            if (result > peak_val_1)
            {
                peak_val_1 = result;               
            } 
        }
        if (flag == 1)
        {
            if (result > peak_val_2)
            {
            peak_val_2 = result;            
            } 
        }     
    }  
    return;
}


void glucose_calc(void){
    
   
    //Glucose estimation
    optical_density = log10(1 + (peak_val_1/peak_val_2));            
    glucose = optical_density/(0.0032);
    
       
    itoa(glucose,reading);
    while(BusyXLCD());
            SetDDRamAddr(0x00);
        while(BusyXLCD());             
            putrsXLCD("Glucose:");        
        while(BusyXLCD());    
            Delay1KTCYx(10);             
        while(BusyXLCD());
            SetDDRamAddr(0x09);
        while(BusyXLCD());             
            putsXLCD(reading); 
        while(BusyXLCD());    
            Delay1KTCYx(10);             
        while(BusyXLCD());
            SetDDRamAddr(0x0B);
        while(BusyXLCD());             
            putrsXLCD("mg/dL"); 
        while(BusyXLCD());    
            Delay1KTCYx(10);  
   
    
    return;
}



/******************INTERRUPTS******************/


//---------------------------------------------------
//Low Priority Interrupts
//---------------------------------------------------

void low_ISR(void);
 
#pragma code low_vector = 0x18
 void low_interrupt_vector(void){
     _asm 
     GOTO low_ISR
     _endasm
 }
#pragma code 
 
#pragma interruptlow low_ISR
void low_ISR(void){
    
       
    INTCONbits.GIEL = 0;//Disable global low priority interrupts
    INTCONbits.TMR0IE = 0;//disable Timer 0 Interrupts  
    
    if((INTCONbits.TMR0IF == 1) && (flag == 0)){
    
        flag = 1;
        INTCONbits.TMR0IF = 0;//clear timer 0 interrupt flag
           
        ADC_read();       
    }
    if((INTCONbits.TMR0IF == 1) && (flag == 1)){
        
        while(BusyXLCD());
            WriteCmdXLCD(0b00000001); // display clear
        while(BusyXLCD());
            SetDDRamAddr(0x00);
        glucose_calc();        
     
        
    }
   
    INTCONbits.GIE = 1;
    return;
}
//---------------------------------------------------
//High Priority Interrupts
//---------------------------------------------------

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
         
        while(BusyXLCD());
            WriteCmdXLCD(0b00000001); // display clear
        while(BusyXLCD());
            SetDDRamAddr(0x00);
        while(BusyXLCD());             
            putrsXLCD("Measuring...");//press key to start reading         
        while(BusyXLCD());    
            Delay1KTCYx(10); 
        
    
        INTCONbits.INT0E = 0;
        INTCONbits.INT0F = 0;
            
         ADC_read();//start reading ADC
           
    }
    
        
    INTCONbits.INT0E = 1;
    INTCONbits.GIE = 1;
    return;
}

void main(){    
    
    flag = 0;    
    peak_val_1 = 0;
    peak_val_2 = 0;
    RCONbits.IPEN = 1;//Enable priority levels on interrupts 
    INTCON2bits.INTEDG0 = 0;
    INTCONbits.INT0E = 1;
    INTCON2bits.TMR0IP =0;//Timer 0 priority set to low
    INTCONbits.TMR0IE = 1;//enable timer 0 interrupt
    INTCONbits.TMR0IF = 0;
    INTCONbits.GIE = 1;//enable global interrupts
    
    init_LCD();//initialize LCD
    
    while(BusyXLCD());
    WriteCmdXLCD(0b00000001); // display clear
    while(BusyXLCD());
        SetDDRamAddr(0x00);        
    while(BusyXLCD());             
        putrsXLCD("Press key");//press key to start measurement        
    while(BusyXLCD());    
    Delay1KTCYx(10);  
    
   
       
    
    
    while (1){}
   
}
