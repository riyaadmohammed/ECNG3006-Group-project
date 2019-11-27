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


unsigned int dataPollStatus;

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
 
 void checkFlashMemoryPresent(void){
    
    unsigned int manufacturerID =0;
    unsigned int deviceID =0;
    SSTPresent=0;
    setCheckFlashPin();
    
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
    unsigned int dataPollStatus=0;
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
        setCheckFlashPin();
        //Write data block first, 1st Bus Write Cycle
        flashWriteAddrAndByte(0x5555,0xAA);
        //Write data block first, 2nd Bus Write Cycle
        flashWriteAddrAndByte(0x2AAA,0x55);
        //Write data block first, 3rd Bus Write Cycle
        flashWriteAddrAndByte(0x5555,0xA0);
        //Write data block first, 4th Bus Write Cycle
        flashWriteAddrAndByte(address,data);
        checkSectorWrite(data);
        revertCheckFlashPin();
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
    setReadFlashPin();
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
    
    
    setSectorEraseFlashPin();
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
    
}
 
 void chckFlashMemoryPresent(void){
    
    unsigned int manufacturerID =0;
    unsigned int deviceID =0;
    SSTPresent=0;
    setCheckFlashPin();
    
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
    
}
 
 void setCheckFlashPin(void){
   
    OE_PIN=1;
    WE_PIN = 1;
    CE_PIN =1;
   
}

void setSectorEraseFlashPin(void){
   
    OE_PIN=1;
    WE_PIN = 0;
    CE_PIN =0;
   
}

void setReadFlashPin(void){
    
    WE_PIN=1;
    CE_PIN=1;
    OE_PIN=1;
    Delay1TCY();
    CE_PIN=0;
    OE_PIN=0;
  
}

void revertCheckFlashPin(void){
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

void clearLCD(void){
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
    checkFlashMemoryPresent();
    sectorWriteByte(0x0003,0xEE);
    sectorWriteByte(0x0004,0xE0);
    sectorWriteByte(0x0005,0xE1);
    //flashWriteAddr(9);
            
  
    while(1){
   /* flashWriteAddr(1);
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
    //flashWriteAddr(0x0004);*/
    SSTPresent=0xFF;
    sectorReadByte(0x0003);
    Delay1KTCYx(255);
    sectorReadByte(0x0004);
    Delay1KTCYx(255);
    sectorReadByte(0x0005);
    Delay1KTCYx(255);
    }
}
