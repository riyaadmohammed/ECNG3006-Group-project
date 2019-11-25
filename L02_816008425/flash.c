#include <p18f452.h>
#include <delays.h>
#include "xlcd.h"
//#include "SST39SF040v2.h"
#include "flashram.h"
#include <stdio.h>
#include <stdlib.h>
#include <timers.h>



#pragma config WDT = OFF //Disable watchdog timer
#pragma config LVP = OFF //Disable low voltage programming

#define _XTAL_FREQ 4000000UL

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

void main(void){
    
    DQ0_PIN = 0;            
    DQ1_PIN =0;            
    DQ2_PIN =0;           
    DQ3_PIN =0;           
    DQ4_PIN =0;           
    DQ5_PIN =0;           
    DQ6_PIN =0;            
    DQ7_PIN =0;            
    
    init_LCD();
    initial_shiftregister();
    initial_FlashMemoryPins();
    //checkFlashMemoryPresent();
    //sectorWriteByte(0x0003,0xEE);
    //sectorWriteByte(0x0004,0xE0);
    //sectorWriteByte(0x0005,0xE1);
    //flashWriteAddr(9);
            
  
    while(1){
    flashWriteAddr(1);
    Delay10KTCYx(200);
    flashWriteAddr(2);
    Delay10KTCYx(200);
    flashWriteAddr(4);
    Delay10KTCYx(200);
    flashWriteAddr(8);
    Delay10KTCYx(200);
    flashWriteAddr(11);
    Delay10KTCYx(200);
    flashWriteAddr(32768);//binary value 1000000000000000
    Delay10KTCYx(200);
    flashWriteAddr(262144);//binary value 1000000000000000000
    Delay10KTCYx(200);
    //flashWriteAddr(0x0004);
   /* SSTPresent=0xFF;
    sectorReadByte(0x0003);
    Delay1KTCYx(255);
    sectorReadByte(0x0004);
    Delay1KTCYx(255);
    sectorReadByte(0x0005);
    Delay1KTCYx(255);*/
    }
}
