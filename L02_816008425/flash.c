#include <p18f452.h>
#include <delays.h>
#include "xlcd.h"
#include "SST39SF040v2.h"
#include <stdio.h>
#include <stdlib.h>
#include <timers.h>



#pragma config WDT = OFF //Disable watchdog timer
#pragma config LVP = OFF //Disable low voltage programming

#define _XTAL_FREQ 4000000UL

void main(void){
    
    init_LCD();
    init_ShiftRegisterPins();
    init_FlashMemoryPins();
    checkFlashMemoryPresent();
    sectorWriteByte(0x0003,0xEE);
    sectorWriteByte(0x0004,0xE0);
    sectorWriteByte(0x0005,0xE1);
    
  
    while(1){
    SSTPresent=0xFF;
    sectorReadByte(0x0003);
    Delay1KTCYx(255);
    sectorReadByte(0x0004);
    Delay1KTCYx(255);
    sectorReadByte(0x0005);
    Delay1KTCYx(255);
    }
}
