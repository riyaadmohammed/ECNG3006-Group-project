/* 
 * File:   flashram.h
 * Author: 816008425
 *
 * Created on November 18, 2019, 5:46 PM
 */

/* 
 * File:   SST39SF040.h
 * Author: Simeon Ramjit & Ezekiel Cudjoe
 *
 * Created on November 12, 2017, 9:20 AM
 */

#ifndef SST39SF040_H
#define	SST39SF040_H

#include <p18f452.h>
#include <delays.h>
#include "xlcd.h"
#include <stdio.h>
#include <stdlib.h>

#define SER_PIN_TRIS          TRISAbits.TRISA1  //Serial or Data out pin
#define SHCP_PIN_TRIS         TRISAbits.TRISA2  //Pin to give clock pulse for STORAGE REGISTER to store data also known as RCLOCK
#define STCP_PIN_TRIS         TRISAbits.TRISA3  //Pin to give clock pulse for SHIFT REGISTER also known as SRCLOCK



#define WE_PIN_TRIS           TRISAbits.TRISA5  //Pin to give clock pulse for SHIFT REGISTER also known as SRCLOCK
#define OE_PIN_TRIS           TRISEbits.TRISE0  //Serial or Data out pin
#define CE_PIN_TRIS           TRISEbits.TRISE1  //Pin to give clock pulse for STORAGE REGISTER to store data also known as RCLOCK

#define SH_LD_PIN_TRIS        TRISEbits.TRISE2  //Parallel Load Pin, pull low to latch data
#define CLK_INH_PIN_TRIS      TRISAbits.TRISA6
#define CLK_PIN_TRIS          TRISCbits.TRISC0
#define SER_IN_PIN_TRIS       TRISCbits.TRISC1  //Remember to make a digital input

#define SHIFT_REG_SWITCH_TRIS TRISCbits.TRISC3

#define SER_PIN               LATAbits.LATA1
#define SHCP_PIN              LATAbits.LATA2
#define STCP_PIN              LATAbits.LATA3

#define WE_PIN                LATAbits.LATA5
#define OE_PIN                LATEbits.LATE0
#define CE_PIN                LATEbits.LATE1


#define SH_LD_PIN             LATEbits.LATE2
#define CLK_INH_PIN           LATAbits.LATA6
#define CLK_PIN               LATCbits.LATC0
#define SER_IN_PIN            PORTCbits.RC1

#define SHIFT_REG_SWITCH      LATCbits.LATC3


#define SST_ID            0xBF    /* SST Manufacturer's ID code   */
#define SST_39SF040       0xB7    /* SST 39SF040 device code      */


/*Variable Declarations*/

unsigned int shiftReg1;
unsigned int mainReg1;
char y[20];
int SSTPresent =0;

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

void init_ShiftRegisterPins(void){                         
    ADCON1 = 0x07;//PORTA is multiplexed with the analog inputs and configured as such on boot   
                   //This ensures that all analog pins can be configured as digital I/O
    SH_LD_PIN_TRIS = 0;  
    CLK_PIN_TRIS  =0;
    CLK_INH_PIN_TRIS =0;
    SER_IN_PIN_TRIS  =1;
    SHCP_PIN_TRIS = 0;
    STCP_PIN_TRIS = 0;
    SER_PIN_TRIS = 0;
    return;
}

void init_FlashMemoryPins(void){
                            
    WE_PIN_TRIS = 0;
    CE_PIN_TRIS = 0;
    OE_PIN_TRIS = 0;
    SHIFT_REG_SWITCH_TRIS = 0;
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

void dataShiftRegOff(void){
    SHIFT_REG_SWITCH = 1; //using a pnp transistor, because that's all i had
}

void dataShiftRegOn(void){
    SHIFT_REG_SWITCH = 0; //using a pnp transistor, because that's all i had
}

unsigned int shift_register_read_byte(void){
   
    char i=0;
    int  bitVal=0;
    shiftReg1 = 0;
    //Turn off data shift register to enable readings from Flash Memory only
    dataShiftRegOff();
    
    //Enable Parallel Load on Parallel to Serial Converter (SN74LS165AN)
    CLK_INH_PIN=1;
    SH_LD_PIN=0;
    
    //Toggle bits to enable read operation on flash memory
    WE_PIN=1;
    OE_PIN = 0;
    CE_PIN=0;
    
    //Give signals time to settle
    for(i=0;i<5;i++){Delay10TCYx(3);}
    
    //Disable parallel load and latch data 
    CLK_INH_PIN=0;
    SH_LD_PIN=1;
    
    //Start serial read
    for(i=0;i<8;i++){
        //Read Qh from SN74LS165AN
        bitVal= SER_IN_PIN;
        shiftReg1 |= ((bitVal) << (7-i));
        //Pulse clock to shift up bits on SN74LS165AN
        CLK_PIN=1;
        CLK_PIN=0;
    }
   //Toggle bits to disable read operation on flash memory
    OE_PIN = 1;
    CE_PIN=1;
    return shiftReg1;
}

void shift_register_write_byte(unsigned int data){
    unsigned char dataTemp=0;
    char i;
    dataTemp=data;
    
    //Disable all outputs
    SHCP_PIN = 0;
    for (i=0;i<8;i++){
        
        STCP_PIN = 0; //Pull clock low to inhibit write
        SER_PIN = (dataTemp >> (7-i));  //Determine what the MSb is and set serial output as high/low
        STCP_PIN = 1; //Clock in data
    }
    //Latch all the outputs at the same time 
    SHCP_PIN = 1;
    Delay10TCYx(1);
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
    addressHigh = (address >>16);
    addressMiddle =(address >> 8);
    addressLow =((address << 7)>>7);
    shift_register_write_byte(data);
    shift_register_write_byte(addressHigh);
    shift_register_write_byte(addressMiddle);
    shift_register_write_byte(addressLow);
    latchDataWriteCommand();
    return;
}

void setCheckFlashPins(void){
    SHIFT_REG_SWITCH = 0;
    OE_PIN=1;
    WE_PIN = 1;
    CE_PIN =1;
    return;
}

void setSectorEraseFlashPins(void){
    SHIFT_REG_SWITCH = 0;
    OE_PIN=1;
    WE_PIN = 0;
    CE_PIN =0;
    return;
}

void setReadFlashPins(void){
    SHIFT_REG_SWITCH = 1;
    WE_PIN=1;
    CE_PIN=1;
    OE_PIN=1;
    Delay1TCY();
    CE_PIN=0;
    OE_PIN=0;
    return;
}

void revertCheckFlashPins(void){
    SHIFT_REG_SWITCH = 1;
    WE_PIN=0;
    CE_PIN=1;
    OE_PIN=1;
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
#endif	/* SST39SF040_H */
