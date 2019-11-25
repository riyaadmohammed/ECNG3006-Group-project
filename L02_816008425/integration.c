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
#include <ow.h>



#pragma config WDT = OFF //Disable watchdog timer
#pragma config LVP = OFF //Disable low voltage programming
#pragma config PWRT = OFF       // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOR = ON         // Brown-out Reset Enable bit (Brown-out Reset enabled)
#pragma config BORV = 20        // Brown-out Reset Voltage bits (VBOR set to 4.2V)

#define _XTAL_FREQ 4000000UL
#define bitmask 240

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
volatile int h=0;
volatile int b=0;
volatile int c=0;
volatile int d=0;
volatile int he=0;

// The system has to measure the ambient temperature (in degrees Centigrade to +/- 0.1 degree Centigrade) at which a reading is taken.

unsigned char temp_read_LSB;// = 10;
unsigned char temp_read_MSB;// = 65000;
unsigned char temp_degree = 0xDF;

unsigned int temp_LSB = 0;
unsigned int temp_MSB = 0;
unsigned int temp_integer = 0;
unsigned int temp_fraction = 0;
unsigned int temp_sign = 0;


float temp_float = 0.0;

char lcdVariable[20];                               //array that will contain the pulse count to display on the LCD


void configTemp (void);
void readTemp (void);
void printTemp (void);
void obtainInteger (void);
void obtainFraction (void);
void obtainSign (void); 
void resetTempConversion (void);
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
        pulse=1;
        pulse2= ReadTimer0();
        NN++;
        Interval= ((pulse2- pulse1)*256)/100;
        interval= Interval;
        if(interval>50){
            NN50++;
            
        }
        pulse1 = pulse2;
         while(BusyXLCD());
        SetDDRamAddr(0x40); //Set cursor to top line
        while(BusyXLCD());
        Delay1KTCYx(110); //Give time for LCD to refresh, writing to it too quickly causes flicker issues
        while(BusyXLCD());
        sprintf(out,"HR interval: %d ms",interval);
        putsXLCD(out); 
    }
}

void speakeroff(void){
          h = 0;
          b=0;
          c=0;
          CloseTimer1();
          ClosePWM1();

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
    if(HRv<=90)
    {
          b = 1;
          OpenTimer2(  TIMER_INT_OFF &    T2_PS_1_16 & T2_POST_1_1  );
          OpenPWM1(2000);
          SetDCPWM1(511);
    }
     if(HRv>90)
    {
          b = 0;
          CloseTimer1();
          ClosePWM1();
    }
    sprintf(out,"HRV:%d %% G: 0mg/L",HRv);
    putsXLCD(out);
    if(allowCount==1){
    while(BusyXLCD());
    SetDDRamAddr(0x14); //Set cursor to top line
    while(BusyXLCD());
    Delay1KTCYx(110); //Give time for LCD to refresh, writing to it too quickly causes flicker issues
    while(BusyXLCD());
    heartBeats = (6*heartBeats);
    heartBeatz = heartBeats;
    sprintf(out,"Heartrate:%d BPM",heartBeats);
    putsXLCD(out);
    while(BusyXLCD());
    if(he==0)
        {
        sum = sum + heartBeats;
        avg = sum/countsamples;
        Avg=avg;
        
        while(BusyXLCD());
        SetDDRamAddr(0x54); //Set cursor to top line
        while(BusyXLCD());
        Delay1KTCYx(110); //Give time for LCD to refresh, writing to it too quickly causes flicker issues
        while(BusyXLCD());
        sprintf(out,"Avg HR: %d BPM",Avg);
        putsXLCD(out);
        }
     if(heartBeatz>100)
         {
          h = 1;
          OpenTimer2(  TIMER_INT_OFF &    T2_PS_1_16 & T2_POST_1_1  );
          OpenPWM1(2000);
          SetDCPWM1(511);
          
         
         }
    if(heartBeatz<=100)
         {
          h = 0;
          CloseTimer1();
          ClosePWM1();
                   
         }
    if(he==1){
    readTemp();
    obtainInteger();
    if(temp_integer>28)
    {
          c = 1;
          OpenTimer2(  TIMER_INT_OFF &    T2_PS_1_16 & T2_POST_1_1  );
          OpenPWM1(2000);
          SetDCPWM1(511);
    }
     if(temp_integer<=28)
    {
          c = 0;
          CloseTimer1();
          ClosePWM1();
    }
    obtainFraction();
    obtainSign();
    resetTempConversion();
    printTemp();
    he=0;
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
   

void readAndDisplayKey(void){

    int output;    
    char y[20];
    
   
	output =(PORTC&bitmask);  
    switch (output){
        case 0:
          sprintf(y,"1");
          resetcount();
          INTCON3bits.INT2IE = 1;
          break;
        case 16:
          sprintf(y,"4");
          he=1;
          resetcount();
          INTCON3bits.INT2IE = 1;
          
          break;
        case 32:
          sprintf(y,"7");
          if(d==0){d=1;}
          else{d=0;}
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
          speakeroff();
          break;
          
    }
        
       /* while(BusyXLCD());
        SetDDRamAddr(0x00); //Set cursor to top line
        while(BusyXLCD());
        Delay1KTCYx(110); //Give time for LCD to refresh, writing to it too quickly causes flicker issues
        while(BusyXLCD());
        sprintf(out,"Key : %s",y);
        //sprintf(out,"%04x",output);
        putsXLCD(out);  */    
 } 

void configTemp (void){
    ow_reset();                                             //resets the D1822 thermometer
    
    ow_write_byte(0xCC);                                    //Skip ROM check command
    ow_write_byte(0x4E);
    ow_write_byte(0x00);
    ow_write_byte(0x00);
    ow_write_byte(0x5F);
}

void readTemp (void){    
    ow_reset();
    ow_write_byte(0xCC);
    ow_write_byte(0x44);                                    //Begin temperature read and conversion
    Delay10KTCYx(40);                                     //The required time needed for the temp conversion process is 750ms. 800ms was implemented to give
                                                            //a sizeable error window
    ow_reset();
    ow_write_byte(0xCC);
    ow_write_byte(0xBE);
    temp_read_LSB = ow_read_byte();
    temp_read_MSB = ow_read_byte();   
}

void obtainInteger (void){
    temp_LSB = temp_read_LSB >> 4;
    temp_MSB = temp_read_MSB << 4;
    temp_integer = temp_MSB | temp_LSB;  
}

void obtainFraction (void){
    /*Assortment of checks to determine the fraction*/
    if(temp_read_LSB & 0x01){
        temp_float += 0.0625;
    }
    if(temp_read_LSB & 0x02){
        temp_float += 0.125;
    }
    if(temp_read_LSB & 0x04){
        temp_float += 0.25;
    }
    if(temp_read_LSB & 0x08){
        temp_float += 0.5;
    }
    
    temp_fraction = temp_float*1000;
}

void obtainSign (void){
    temp_sign = (temp_read_MSB & 0xF8);
    
    if(temp_sign == 0){
        sprintf(lcdVariable, "Temp: +%d.%03d%cC", temp_integer, temp_fraction, temp_degree);
    }
    else{
        sprintf(lcdVariable, "Temp: -%d.%03d%cC", temp_integer, temp_fraction, temp_degree);
    }
}

void resetTempConversion (void){
    temp_float = 0.0;
    temp_fraction = 0;
    temp_sign = 0;
}

void printTemp (void){
    //while(BusyXLCD);
    SetDDRamAddr(0x54);
    while(BusyXLCD());
    putsXLCD(lcdVariable);
    while(BusyXLCD());
    
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
    /*if(INTCON3bits.INT1IF==1){
        INTCON3bits.INT1IE = 0;
        resetcount();
        INTCON3bits.INT1IF = 0;
        INTCON3bits.INT1IE = 1;
        INTCON3bits.INT2IE = 1;
    }*/
    if(INTCONbits.INT0F==1){
     
     readAndDisplayKey();
     
     
     INTCONbits.INT0E = 0;
     INTCONbits.INT0F = 0;
     INTCONbits.INT0E = 1;
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
     configTemp();
     //readTemp();
     //obtainInteger();
     //obtainFraction();
     //obtainSign();
     //resetTempConversion();
    // printTemp();
     //init_Timer0();
     TRISCbits.RC7=1;
     TRISCbits.RC6=1;
     TRISCbits.RC5=1;
     TRISCbits.RC4=1;
     RCONbits.IPEN = 1;              //Enable Priority Levels
     
     //INTCON3bits.INT1IP = 1;
     //INTCON2bits.INTEDG1 = 1;
     //INTCON3bits.INT1IE = 1;
     
      
     
     
     INTCONbits.GIE = 1;
     INTCONbits.INT0IE = 1;
     INTCON2bits.INTEDG0 = 0;
     INTCONbits.TMR0IE = 1;          //Enable Timer0 Interrupt
     INTCONbits.TMR0IF = 0;          //Clear Timer0 Interrupt Flag
     INTCON2bits.TMR0IP = 1;          //Timer0 High Priority
     
     //startHeartBeatSample();
     while(1){
      //software unit testing
     /* resetcount();
      INTCON3bits.INT2IE = 1;
      while(INTCON3bits.INT2IE == 1){
      INTCON3bits.INT2IF=1;
      Delay10KTCYx(100); 
      }
      */
         while(h==1||b==1||c==1){
          Delay10KTCYx(100); 
          CloseTimer1();
          ClosePWM1();
          Delay10KTCYx(100); 
          OpenTimer2(  TIMER_INT_OFF &    T2_PS_1_16 & T2_POST_1_1  );
          OpenPWM1(2000);
          SetDCPWM1(511);
          if(d==1){
          readTemp();
          obtainInteger();
          if(temp_integer>28)
            {
                  c = 1;
                  OpenTimer2(  TIMER_INT_OFF &    T2_PS_1_16 & T2_POST_1_1  );
                  OpenPWM1(2000);
                  SetDCPWM1(511);
            }
             if(temp_integer<=28)
            {
                  c = 0;
                  CloseTimer1();
                  ClosePWM1();
            }
          obtainFraction();
          obtainSign();
          resetTempConversion();
          printTemp();
          }
         }
         if(h==0||b==0||c==0){
          CloseTimer1();
          ClosePWM1();
         }
         if(d==1){
          readTemp();
          obtainInteger();
          if(temp_integer>28)
            {
                  c = 1;
                  OpenTimer2(  TIMER_INT_OFF &    T2_PS_1_16 & T2_POST_1_1  );
                  OpenPWM1(2000);
                  SetDCPWM1(511);
            }
             if(temp_integer<=28)
            {
                  c = 0;
                  CloseTimer1();
                  ClosePWM1();
            }
          obtainFraction();
          obtainSign();
          resetTempConversion();
          printTemp();
          }
     } 
    
    
     
 }

