#include <p18f452.h>
#include <stdio.h>
#include <stdlib.h>
#include <delays.h>
#include "ow.h"
#include "xlcd.h"

/* Set configuration bits for use with PICKit3 */
#pragma config OSC = XT
#pragma config WDT = OFF
#pragma config LVP = OFF

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
void obtainInteger (void);
void obtainFraction (void);
void obtainSign (void); 
void resetTempConversion (void);

void DelayFor18TCY(void){
    Nop(); Nop(); Nop(); Nop(); Nop(); Nop();
    Nop(); Nop(); Nop(); Nop(); Nop(); Nop();
    Nop(); Nop(); Nop(); Nop(); Nop(); Nop();         
}
 
void DelayXLCD(void){                               //1000us = 1ms
    Delay1KTCYx(5);  
 }
 
void DelayPORXLCD(void){
    Delay1KTCYx(15);
 }
 
void init_lcd(void){ 
     OpenXLCD(FOUR_BIT & LINES_5X7);
     while(BusyXLCD());
     WriteCmdXLCD(SHIFT_DISP_LEFT);
     while(BusyXLCD());
}

void printTemp (void){
    SetDDRamAddr(0x00);
    while(BusyXLCD());
    putsXLCD(lcdVariable);
    while(BusyXLCD());
}

void main(void)
{
    init_lcd();
    
    configTemp();
    readTemp();
    obtainInteger();
    obtainFraction();
    obtainSign();
    resetTempConversion();
    printTemp();
    
    while(1){        
        readTemp();
        obtainInteger();
        obtainFraction();
        obtainSign();
        resetTempConversion();
        printTemp();
    }
    
    Sleep();
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

