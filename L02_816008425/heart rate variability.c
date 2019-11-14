#include<p18f452.h>
#include<delays.h>
#include"xlcd.h"
#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include<timers.h>
#include<capture.h>
#include<string.h>
#include<math.h>
#include<float.h>

#pragma config OSC = HS
#pragma config WDT = OFF
#pragma config LVP = OFF

#define _XTAL_FREQ 4000000UL

void init_timer(void);
void high_isr(void);
void init_lcd(void);

unsigned int first_read=0,
 next_read=0,
 risingEdges=0,
 interval=0,
 overFlow=0,
 t=0,
 x=0,
 final=0,
 nn=0,
 nn_50=0;
float temp=0;
float current_hrv=0;

void DelayFor18TCY(void)
{
     Nop(); Nop(); Nop(); Nop(); 
     Nop(); Nop(); Nop(); Nop(); 
     Nop(); Nop(); Nop(); Nop(); 
     Nop(); Nop();
     return;
}

void DelayXLCD(void)
{
    Delay1KTCYx(5);	
    return;
}

void DelayPORXLCD(void)
{
    Delay1KTCYx(15);
    return;
}

void init_timer()
{
    OpenTimer1 (TIMER_INT_ON & T1_16BIT_RW & T1_PS_1_1 & T1_OSC1EN_OFF & T1_SYNC_EXT_OFF & T1_SOURCE_INT & T1_SOURCE_CCP );  
    OpenCapture2 (CAPTURE_INT_ON & C2_EVERY_RISE_EDGE);    
}

void disp_hrv(void)
{
    int temp_var = 0;
    char hrvv[20];
    temp_var = current_hrv;
    sprintf(hrvv,"HRV: %d%",temp_var);
    
    while( BusyXLCD() );
    SetDDRamAddr( 0x40 );
    putsXLCD(hrvv);
    Delay10KTCYx(0x64);
}

void init_lcd()
{
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

void HRV_interrupts(void)
{
    RCONbits.IPEN =   1;             //Enable Priority Levels
    PIR2bits.CCP2IF = 0;             //CCP Interrupt Flag
    IPR2bits.CCP2IP = 1;             //CCPIP Interrupt High Priority 
    IPR1bits.TMR1IP = 1;             //Timer1 Interrupt ; High Priority 
    PIE1bits.TMR1IE = 1;             //Timer1 Interrupt Enable
    PIR1bits.TMR1IF=0;               //Timer1 Interrupt Flag initially cleared.
    INTCONbits.GIEH= 1;              //Enable Interrupt
    TRISDbits.RD3 = 0;               //Set RB7 as Output
    PORTDbits.RD3 = 0;               //Set RB7 LOW     
    TRISCbits.RC1 = 1;               //Set RC1 as Input        
}
void hrvMain(void)
{
    if(risingEdges == 0)
    {
        if (x==0)
        { 
            while(BusyXLCD());
            SetDDRamAddr(0x00);
            while(BusyXLCD());
            putrsXLCD("Calculating..."); 
            while(BusyXLCD());
            
            first_read = ReadCapture2();  
        }
        else
        {
            next_read = ReadCapture2();   
        }
    } 
    else if ( (risingEdges == 1) && (x ==0 ))
    {
        next_read = ReadCapture2();   
    }
    
    risingEdges++;
    
    if( (risingEdges > 1) || (x == 1))
    {
        x=1;
        interval = 65535*overFlow + next_read-first_read;
        nn++;
        t = next_read;                                      // prev was here, if there are errors, switch back t to prev
        first_read = t;                                     // prev was here, if there are errors, switch back t to prev
        temp = (float)interval/(float)1000;
        
        if((float)temp > (float)50)
        {  
            nn_50++;
            PORTDbits.RD3 = !PORTDbits.RD3;//Heartbeat LED on RD3 
        }
        
        risingEdges = 0;
    
        if(nn>10)
        {
            current_hrv = (float)nn_50/(float)10;
            current_hrv = current_hrv*100;
            final = 1;       
            CloseCapture2();
            CloseTimer1();
        }
    
        if (final == 1)
        {
            disp_hrv();
            PORTDbits.RD3 = 0;//HeartBeat LED On RD3
        }
    }
}


/*****************High priority interrupt vector **************************/
#pragma code high_vector=0x08
void interrupt_at_high_vector(void)
{
  _asm GOTO high_isr _endasm
}
 
#pragma code
/*****************High priority ISR **************************/

#pragma interrupt high_isr
void high_isr (void)
{
    INTCONbits.GIE = 0;
    //float temp = 0;
    if(PIR1bits.TMR1IF==1)
    {
        PIR1bits.TMR1IF=0;
        overFlow++;
    } 
    
    if(PIR2bits.CCP2IF==1)
    {  
        PIR2bits.CCP2IF = 0;  
        hrvMain();
    }   
    INTCONbits.GIE = 1;
}      


void main()
{   
    x=0;
    HRV_interrupts();
    init_timer();                    //Initialize Timer3 and Capture mode
    init_lcd();                      //Initialize LCD
    
    while(1)
    {
        
    }   
}