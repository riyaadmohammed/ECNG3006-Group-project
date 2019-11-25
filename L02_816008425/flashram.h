/* 
 * File:   flashram.h
 * Author: 816008425
 *
 * Created on November 21, 2019, 11:22 AM
 */

#ifndef FLASHRAM_H
#define	FLASHRAM_H

#include <p18f452.h>
#include <delays.h>
#include "xlcd.h"
#include <stdio.h>
#include <stdlib.h>

#define DQ0_PIN_TRIS        TRISAbits.TRISA0
#define DQ1_PIN_TRIS        TRISAbits.TRISA1
#define DQ2_PIN_TRIS        TRISAbits.TRISA2
#define DQ3_PIN_TRIS        TRISAbits.TRISA3
#define DQ4_PIN_TRIS        TRISAbits.TRISA4
#define DQ5_PIN_TRIS        TRISAbits.TRISA5
#define DQ6_PIN_TRIS        TRISAbits.TRISA6
#define DQ7_PIN_TRIS        TRISEbits.TRISE0

#define WE_PIN_TRIS        TRISEbits.TRISE1
#define CE_PIN_TRIS        TRISEbits.TRISE2
#define OE_PIN_TRIS        TRISCbits.TRISC0

#define DATAIN_PIN_TRIS     TRISCbits.TRISC1
#define LAT_CLK_PIN_TRIS    TRISCbits.TRISC3 //as latch AND clk SIGNALS ARE SYNCHRONIZED THE OUTPUT WILL ALWAYS LAG INPUT BY ONE TICK

#define DQ0_PIN             PORTAbits.RA0
#define DQ1_PIN             PORTAbits.RA1
#define DQ2_PIN             PORTAbits.RA2
#define DQ3_PIN             PORTAbits.RA3
#define DQ4_PIN             PORTAbits.RA4
#define DQ5_PIN             PORTAbits.RA5
#define DQ6_PIN             PORTAbits.RA6
#define DQ7_PIN             PORTEbits.RE0

#define WE_PIN        LATEbits.LATE1
#define CE_PIN        LATEbits.LATE2
#define OE_PIN        LATCbits.LATC0

#define ADDRESS_PIN    LATCbits.LATC1
#define LAT_CLK_PIN    LATCbits.LATC3

#define SST_ID            0xBF    /* SST Manufacturer's ID code   */
#define SST_39SF040       0xB7    /* SST 39SF040 device code      */

void initial_shiftregister(void)
{
DATAIN_PIN_TRIS = 0;
LAT_CLK_PIN_TRIS = 0;
}

void initial_FlashMemoryPins(void){
                            
    WE_PIN_TRIS = 0;
    CE_PIN_TRIS = 0;
    OE_PIN_TRIS = 0;
    
}

void shift_register_write_byte(unsigned int data){
    unsigned char dataTemp=0;
    char i;
    dataTemp=data;
    
    //Disable all outputs
    for (i=0;i<8;i++){
        
        LAT_CLK_PIN = 1; //Pull clock low to inhibit write
        Delay1KTCYx(1);
        ADDRESS_PIN = (dataTemp >> (7-i));  //Determine what the MSb is and set serial output as high/low
        Delay1KTCYx(1);
        LAT_CLK_PIN = 0; //Clock in data
        Delay1KTCYx(1);
    }
    //Latch all the outputs at the same time 
    Delay10TCYx(1);
    return;
}


void flashWriteAddr(unsigned long short int address/*,unsigned int data*/){

    unsigned int addressHigh =0;
    unsigned int addressMiddle =0;
    unsigned int addressLow =0;
     addressHigh = (address >>16);
    addressMiddle =(address >> 8);
    addressLow =((address << 7)>>7);
    
    
    
    
    //shift_register_write_byte(data);
    shift_register_write_byte(addressHigh);
    shift_register_write_byte(addressMiddle);
    shift_register_write_byte(addressLow);
    LAT_CLK_PIN = 1;
    LAT_CLK_PIN = 0;
    LAT_CLK_PIN = 1;
    LAT_CLK_PIN = 0;
    //latchDataWriteCommand();
    return;
}

void latchDataWriteCommand(void){
    
    
    //Latch address
    WE_PIN = 0;
    CE_PIN= 0;
    
    //Latch data
    WE_PIN = 1;
    CE_PIN= 1;
    

    //Write Commmand
    WE_PIN=1;
    CE_PIN=0;
    
    //Restore Pins
    WE_PIN = 1;
    CE_PIN =1;

}

void flashWriteAddrAndByte(unsigned long short int address,unsigned int data){

    unsigned int addressHigh =0;
    unsigned int addressMiddle =0;
    unsigned int addressLow =0;
    unsigned char dataTemp=0;
    char i =0;
    dataTemp=data;
    DQ0_PIN_TRIS = 0;       
    DQ1_PIN_TRIS = 0;       
    DQ2_PIN_TRIS = 0;       
    DQ3_PIN_TRIS = 0;      
    DQ4_PIN_TRIS = 0;      
    DQ5_PIN_TRIS = 0;
    DQ6_PIN_TRIS = 0;       
    DQ7_PIN_TRIS = 0;
   
    addressHigh = (address >>16);
    addressMiddle =(address >> 8);
    addressLow =((address << 7)>>7);
    shift_register_write_byte(addressHigh);
    shift_register_write_byte(addressMiddle);
    shift_register_write_byte(addressLow);
    if(i==0){
        DQ0_PIN = (dataTemp >> (7-i));  //Determine what the MSb is and set serial output as high/low
        i++;
    }
    else if(i==1){
        DQ1_PIN = (dataTemp >> (7-i));  //Determine what the MSb is and set serial output as high/low
        i++;
    }
    else if(i==2){
        DQ2_PIN = (dataTemp >> (7-i));  //Determine what the MSb is and set serial output as high/low
        i++;
    }
    else if(i==3){
        DQ3_PIN = (dataTemp >> (7-i));  //Determine what the MSb is and set serial output as high/low
        i++;
    }
    else if(i==4){
        DQ4_PIN = (dataTemp >> (7-i));  //Determine what the MSb is and set serial output as high/low
        i++;
    }
    else if(i==5){
        DQ5_PIN = (dataTemp >> (7-i));  //Determine what the MSb is and set serial output as high/low
        i++;
    }
    else if(i==6){
        DQ6_PIN = (dataTemp >> (7-i));  //Determine what the MSb is and set serial output as high/low
        i++;
    }
    else if(i==7){
        DQ7_PIN = (dataTemp >> (7-i));  //Determine what the MSb is and set serial output as high/low
        i=0;
    }
    PORTA = data;
    return;
}



#endif	/* FLASHRAM_H */

