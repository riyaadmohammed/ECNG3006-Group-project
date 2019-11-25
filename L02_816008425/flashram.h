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
    DQ0_PIN_TRIS = 0;       
    DQ1_PIN_TRIS = 0;       
    DQ2_PIN_TRIS = 0;       
    DQ3_PIN_TRIS = 0;      
    DQ4_PIN_TRIS = 0;      
    DQ5_PIN_TRIS = 0;
    DQ6_PIN_TRIS = 0;       
    DQ7_PIN_TRIS = 0;
    
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

unsigned int shift_register_read_byte(void){
   
    char i=0;
    int  bitVal=0;
    DQ0_PIN_TRIS = 1;       
    DQ1_PIN_TRIS = 1;       
    DQ2_PIN_TRIS = 1;       
    DQ3_PIN_TRIS = 1;      
    DQ4_PIN_TRIS = 1;      
    DQ5_PIN_TRIS = 1;
    DQ6_PIN_TRIS = 1;       
    DQ7_PIN_TRIS = 1;
    
    
    //Toggle bits to enable read operation on flash memory
    WE_PIN=1;
    OE_PIN = 0;
    CE_PIN=0;
    
    //Give signals time to settle
    for(i=0;i<5;i++){Delay10TCYx(3);}
    
    //Start serial read
   
    if(i==0){
        bitVal= DQ7_PIN; 
        shiftReg1 |= ((bitVal) << (7-i));
        i++;
    }
    if(i==1){
        bitVal= DQ6_PIN; 
        shiftReg1 |= ((bitVal) << (7-i));
        i++;
    }
    if(i==2){
        bitVal= DQ5_PIN; 
        shiftReg1 |= ((bitVal) << (7-i));
        i++;
    }
    if(i==3){
        bitVal= DQ4_PIN; 
        shiftReg1 |= ((bitVal) << (7-i));
        i++;
    }
    if(i==4){
        bitVal= DQ3_PIN; 
        shiftReg1 |= ((bitVal) << (7-i));
        i++;
    }
    if(i==5){
        bitVal= DQ2_PIN; 
        shiftReg1 |= ((bitVal) << (7-i));
        i++;
    }
    if(i==6){
        bitVal= DQ1_PIN; 
        shiftReg1 |= ((bitVal) << (7-i));
        i++;
    }
    if(i==7){
        bitVal= DQ0_PIN; 
        shiftReg1 |= ((bitVal) << (7-i));
        i=0;
    }
    
    
   //Toggle bits to disable read operation on flash memory
    OE_PIN = 1;
    CE_PIN=1;
    return shiftReg1;
}

void checkFlashMemoryPresent(void){
    
    unsigned int manufacturerID =0;
    unsigned int deviceID =0;
    SSTPresent=0;
    setCheckFlashPins();
    
    //Write data block first, 1st Bus Write Cycle
    //Software ID Entry
    flashWriteAddrAndByte(0x5555,0xAA);
    //Write data block first, 2nd Bus Write Cycle
    flashWriteAddrAndByte(0x2AAA,0x55);
    //Write data block first, 3rd Bus Write Cycle
    flashWriteAddrAndByte(0x5555,0x90);
    

    //Write address lines to read manufacturer ID
    flashWriteAddrAndByte(0x0000,0x00);
    manufacturerID = shift_register_read_byte();
    while(BusyXLCD());
    SetDDRamAddr(0x00); //Set cursor to top line
    while(BusyXLCD());
    Delay1KTCYx(110); //Give time for LCD to refresh, writing to it too quickly causes flicker issues
    while(BusyXLCD());
    sprintf(y,"Maker ID:%X",manufacturerID);
    putsXLCD(y); 


    //Write address lines to read device ID
    flashWriteAddrAndByte(0x0001,0x00);
    deviceID = shift_register_read_byte();
    while(BusyXLCD());
    SetDDRamAddr(0x40); //Set cursor to third line
    while(BusyXLCD());
    Delay1KTCYx(110); //Give time for LCD to refresh, writing to it too quickly causes flicker issues
    while(BusyXLCD());
    sprintf(y,"Device ID:%X",deviceID);
    putsXLCD(y); 
    
    if ((manufacturerID == SST_ID) && (deviceID ==SST_39SF040)){
        SSTPresent=0xFF;
        while(BusyXLCD());
        SetDDRamAddr(0x10); //Set cursor to third line
        while(BusyXLCD());
        Delay1KTCYx(110); //Give time for LCD to refresh, writing to it too quickly causes flicker issues
        while(BusyXLCD());
        sprintf(y,"39SF040 detected!");
        putsXLCD(y); 
    }
    else{
        SSTPresent=0xAA;
        while(BusyXLCD());
        SetDDRamAddr(0x10); //Set cursor to third line
        while(BusyXLCD());
        Delay1KTCYx(110); //Give time for LCD to refresh, writing to it too quickly causes flicker issues
        while(BusyXLCD());
        sprintf(y,"39SF040 !present");
        putsXLCD(y); 
    }
    
        
    //Software ID Exit
      //Only need to call once to turn on the data shift reg
                       //Since no reads are being done after this operation
    //Write data block first, 1st Bus Write Cycle
    flashWriteAddrAndByte(0x5555,0xAA);
    //Write data block first, 2nd Bus Write Cycle
    flashWriteAddrAndByte(0x2AAA,0x55);
    //Write data block first, 3rd Bus Write Cycle
    flashWriteAddrAndByte(0x5555,0xF0);
    
    while(BusyXLCD());
    SetDDRamAddr(0x50); //Set cursor to third line
    while(BusyXLCD());
    Delay1KTCYx(110); //Give time for LCD to refresh, writing to it too quickly causes flicker issues
    while(BusyXLCD());
    sprintf(y,"Exiting SDP...");  //SDP - software data protection
    putsXLCD(y);
    clearLCD();
    Delay1KTCYx(100);
    return;
}

void checkSectorWrite(unsigned int data){
    int dataPollStatus=0;
    dataShiftRegOff();
    Delay10TCYx(1);   //delay Tbp for byte program 
    while(dataPollStatus != 1){
        dataPollStatus = dataPoll("BP",data);
        while(BusyXLCD());
        SetDDRamAddr(0x40); //Set cursor to second line
        while(BusyXLCD());
        Delay1KTCYx(110); //Give time for LCD to refresh, writing to it too quickly causes flicker issues
        while(BusyXLCD());
        sprintf(y,"Data !good");
        putsXLCD(y); 
    }
        while(BusyXLCD());
        SetDDRamAddr(0x40); //Set cursor to second line
        while(BusyXLCD());
        Delay1KTCYx(110); //Give time for LCD to refresh, writing to it too quickly causes flicker issues
        while(BusyXLCD());
        sprintf(y,"Data good ");
        putsXLCD(y);
        return;
}

void sectorWriteByte(unsigned int address, unsigned int data){
    if(SSTPresent==0xFF){
        sectorErase(address,data);
        setCheckFlashPins();
        //Write data block first, 1st Bus Write Cycle
        flashWriteAddrAndByte(0x5555,0xAA);
        //Write data block first, 2nd Bus Write Cycle
        flashWriteAddrAndByte(0x2AAA,0x55);
        //Write data block first, 3rd Bus Write Cycle
        flashWriteAddrAndByte(0x5555,0xA0);
        //Write data block first, 4th Bus Write Cycle
        flashWriteAddrAndByte(address,data);
        checkSectorWrite(data);
        revertCheckFlashPins();
        clearLCD();
    }
    else{
        clearLCD();
        while(BusyXLCD());
        SetDDRamAddr(0x00); //Set cursor to second line
        while(BusyXLCD());
        Delay1KTCYx(110); //Give time for LCD to refresh, writing to it too quickly causes flicker issues
        while(BusyXLCD());
        putrsXLCD("Chip not present");
        while(BusyXLCD());
        SetDDRamAddr(0x40); //Set cursor to second line
        while(BusyXLCD());
        Delay1KTCYx(110); //Give time for LCD to refresh, writing to it too quickly causes flicker issues
        while(BusyXLCD());
        sprintf(y,"No write to %X",address);
        putsXLCD(y);
    }
    return;
}

void sectorReadByte(unsigned int address){
    unsigned int readByte=0;
    setReadFlashPins();
    if(SSTPresent==0xFF){
        flashWriteAddrAndByte(address,0x00);
        readByte=shift_register_read_byte();

        while(BusyXLCD());
        SetDDRamAddr(0x00); //Set cursor to top line
        while(BusyXLCD());
        Delay1KTCYx(110); //Give time for LCD to refresh, writing to it too quickly causes flicker issues
        while(BusyXLCD());
        sprintf(y,"Sector:%X",address);
        putsXLCD(y); 
        while(BusyXLCD());
        SetDDRamAddr(0x40); //Set cursor to top line
        while(BusyXLCD());
        Delay1KTCYx(110); //Give time for LCD to refresh, writing to it too quickly causes flicker issues
        while(BusyXLCD());
        sprintf(y,"Byte:%X",readByte);
        putsXLCD(y); 
    }
    else{
        clearLCD();
        while(BusyXLCD());
        SetDDRamAddr(0x00); //Set cursor to second line
        while(BusyXLCD());
        Delay1KTCYx(110); //Give time for LCD to refresh, writing to it too quickly causes flicker issues
        while(BusyXLCD());
        putrsXLCD("Chip not present");
        while(BusyXLCD());
        SetDDRamAddr(0x40); //Set cursor to second line
        while(BusyXLCD());
        Delay1KTCYx(110); //Give time for LCD to refresh, writing to it too quickly causes flicker issues
        while(BusyXLCD());
        sprintf(y,"No read from %X",address);
        putsXLCD(y);
    }
    return;
}

void sectorErase(unsigned int address, unsigned int data){
    
    
    setSectorEraseFlashPins();
    //Sector-Erase - 6 byte sequence
    //Write data block first, 1st Bus Write Cycle
    flashWriteAddrAndByte(0x5555,0xAA);
    
    //Write data block first, 2nd Bus Write Cycle
    flashWriteAddrAndByte(0x2AAA,0x55);
    
    //Write data block first, 3rd Bus Write Cycle
    flashWriteAddrAndByte(0x5555,0x80);
    
    //Write data block first, 4th Bus Write Cycle
    flashWriteAddrAndByte(0x5555,0xAA);
    
    //Write data block first, 5th Bus Write Cycle
    flashWriteAddrAndByte(0x2AAA,0x55);
    
    //Write data block first, 6thBus Write Cycle
    flashWriteAddrAndByte(data,0x30);
    checkSectorErase(address,data);
    return;
}

void checkSectorErase(unsigned int address, unsigned int data){
    int dataPollStatus=0;
    dataShiftRegOff();
    Delay1KTCYx(20);   //delay Tsce for sector chip erase 
    while(dataPollStatus != 1){
        dataPollStatus = dataPoll("E",data);
        while(BusyXLCD());
        SetDDRamAddr(0x00); //Set cursor to first line
        while(BusyXLCD());
        Delay1KTCYx(110); //Give time for LCD to refresh, writing to it too quickly causes flicker issues
        while(BusyXLCD());
        sprintf(y,"Sector %X !erased",address);
        putsXLCD(y); 
    }
        while(BusyXLCD());
        SetDDRamAddr(0x00); //Set cursor to first line
        while(BusyXLCD());
        Delay1KTCYx(110); //Give time for LCD to refresh, writing to it too quickly causes flicker issues
        while(BusyXLCD());
        sprintf(y,"Sector %X ready  ",address);
        putsXLCD(y);
    return;
}

int dataPoll(rom char *operation,unsigned int data){
    
    int status=0;
    char pollType;
    unsigned int flashDataDQ7 =0;
    unsigned int complementFlashDataDQ7=0;
    
    
    CE_PIN = !CE_PIN; //Toggling of these lines are necessary for polling
    OE_PIN = !OE_PIN; //Consult Figure 8 in spec sheet 
    WE_PIN= 1;
    
    //When writing data, the flash chip gives the MSbcomplement of the data 
    //being written. When the write is complete is gives the true (written) data.
    //If we store the data to be written and constantly compare the MSb and the value
    //on DQ7, we can determine when the chip has finished the write. There are two different
    //types of polling, BP or byte program polling and E or erase polling. For E polling, the 
    //sector being erased has a value of FFH written to it. So we simply need to compare
    //the data lines to determine when it is done.
    flashDataDQ7 = shift_register_read_byte();
    complementFlashDataDQ7 = !data;
    
    pollType = *operation;
    if(pollType=='E'){
        if(flashDataDQ7==0xFF){
            status = 1;
        }
        else{
            status = 0;
        }
    }
    if(pollType=='BP'){
        if(((flashDataDQ7>>7)^(complementFlashDataDQ7>>7)) == 0){
            status = 0;
        }
        else{
            status =1;
        }
    }
return status;    
}

void checkFlashMemoryPresent(void){
    
    unsigned int manufacturerID =0;
    unsigned int deviceID =0;
    SSTPresent=0;
    setCheckFlashPins();
    
    //Write data block first, 1st Bus Write Cycle
    //Software ID Entry
    flashWriteAddrAndByte(0x5555,0xAA);
    //Write data block first, 2nd Bus Write Cycle
    flashWriteAddrAndByte(0x2AAA,0x55);
    //Write data block first, 3rd Bus Write Cycle
    flashWriteAddrAndByte(0x5555,0x90);
    dataShiftRegOff();

    //Write address lines to read manufacturer ID
    flashWriteAddrAndByte(0x0000,0x00);
    manufacturerID = shift_register_read_byte();
    while(BusyXLCD());
    SetDDRamAddr(0x00); //Set cursor to top line
    while(BusyXLCD());
    Delay1KTCYx(110); //Give time for LCD to refresh, writing to it too quickly causes flicker issues
    while(BusyXLCD());
    sprintf(y,"Maker ID:%X",manufacturerID);
    putsXLCD(y); 


    //Write address lines to read device ID
    flashWriteAddrAndByte(0x0001,0x00);
    deviceID = shift_register_read_byte();
    while(BusyXLCD());
    SetDDRamAddr(0x40); //Set cursor to third line
    while(BusyXLCD());
    Delay1KTCYx(110); //Give time for LCD to refresh, writing to it too quickly causes flicker issues
    while(BusyXLCD());
    sprintf(y,"Device ID:%X",deviceID);
    putsXLCD(y); 
    
    if ((manufacturerID == SST_ID) && (deviceID ==SST_39SF040)){
        SSTPresent=0xFF;
        while(BusyXLCD());
        SetDDRamAddr(0x10); //Set cursor to third line
        while(BusyXLCD());
        Delay1KTCYx(110); //Give time for LCD to refresh, writing to it too quickly causes flicker issues
        while(BusyXLCD());
        sprintf(y,"39SF040 detected!");
        putsXLCD(y); 
    }
    else{
        SSTPresent=0xAA;
        while(BusyXLCD());
        SetDDRamAddr(0x10); //Set cursor to third line
        while(BusyXLCD());
        Delay1KTCYx(110); //Give time for LCD to refresh, writing to it too quickly causes flicker issues
        while(BusyXLCD());
        sprintf(y,"39SF040 !present");
        putsXLCD(y); 
    }
    
        
    //Software ID Exit
    dataShiftRegOn();  //Only need to call once to turn on the data shift reg
                       //Since no reads are being done after this operation
    //Write data block first, 1st Bus Write Cycle
    flashWriteAddrAndByte(0x5555,0xAA);
    //Write data block first, 2nd Bus Write Cycle
    flashWriteAddrAndByte(0x2AAA,0x55);
    //Write data block first, 3rd Bus Write Cycle
    flashWriteAddrAndByte(0x5555,0xF0);
    
    while(BusyXLCD());
    SetDDRamAddr(0x50); //Set cursor to third line
    while(BusyXLCD());
    Delay1KTCYx(110); //Give time for LCD to refresh, writing to it too quickly causes flicker issues
    while(BusyXLCD());
    sprintf(y,"Exiting SDP...");  //SDP - software data protection
    putsXLCD(y);
    clearLCD();
    Delay1KTCYx(100);
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

void setCheckFlashPins(void){
   
    OE_PIN=1;
    WE_PIN = 1;
    CE_PIN =1;
   
}

void setSectorEraseFlashPins(void){
   
    OE_PIN=1;
    WE_PIN = 0;
    CE_PIN =0;
   
}

void setReadFlashPins(void){
    
    WE_PIN=1;
    CE_PIN=1;
    OE_PIN=1;
    Delay1TCY();
    CE_PIN=0;
    OE_PIN=0;
  
}

void revertCheckFlashPins(void){
    WE_PIN=0;
    CE_PIN=1;
    OE_PIN=1;
   
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
    //PORTA = data;
    latchDataWriteCommand();
    return;
}

void clearLCD(){
    int i=0;
    for(i=0;i<15;i++){
    Delay1KTCYx(200);
    }
    
    while(BusyXLCD());
    SetDDRamAddr(0x00); //Set cursor to first line
    while(BusyXLCD());
    Delay1KTCYx(1); //Give time for LCD to refresh, writing to it too quickly causes flicker issues
    while(BusyXLCD());
    putrsXLCD("                ");
    while(BusyXLCD());
    SetDDRamAddr(0x40); //Set cursor to second line
    while(BusyXLCD());
    Delay1KTCYx(1); //Give time for LCD to refresh, writing to it too quickly causes flicker issues
    while(BusyXLCD());
    putrsXLCD("                ");
    while(BusyXLCD());
    SetDDRamAddr(0x10); //Set cursor to third line
    while(BusyXLCD());
    Delay1KTCYx(1); //Give time for LCD to refresh, writing to it too quickly causes flicker issues
    while(BusyXLCD());
    putrsXLCD("                ");
    while(BusyXLCD());
    SetDDRamAddr(0x50); //Set cursor to fourth line
    while(BusyXLCD());
    Delay1KTCYx(1); //Give time for LCD to refresh, writing to it too quickly causes flicker issues
    while(BusyXLCD());
    putrsXLCD("                ");
    return;
    
}



#endif	/* FLASHRAM_H */

