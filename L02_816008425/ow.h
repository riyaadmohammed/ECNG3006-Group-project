/* Copyright � 2011 Demetris Stavrou
 * version 1.0
 * ---------------------------------
 * OneWire Library for the C18 Compiler
 * Only the basic functions are included
 * ow_reset() for OneWire Reset
 * ow_write_byte(char) for writting byte
 * char ow_read_byte() for reading byte
 *
 * Just include the ow.h file in your project and define three required
 * names - OW_LAT, OW_PIN, OW_TRIS
 *
 * You can use this library for interfacing the Maxim thermometers like DS18S20
 * NOTE: This library works for 40MHz Frequency i.e. TCY = 0.1us
 * You can adjust the timings accordingly, at least for now!
 */
#ifndef __OW_H
#define __OW_H

#define OW_LAT LATBbits.LATB5
#define OW_PIN PORTBbits.RB5
#define OW_TRIS TRISBbits.TRISB5

#define OUTPUT 0
#define INPUT 1

unsigned char ow_reset(void)
{
    short device_found=0;
    // DQ High
    OW_TRIS=INPUT;
    // DQ Low
    OW_LAT=0;
    OW_TRIS=OUTPUT;
    // delay 480us
    Delay10TCYx(48);
    // DQ High
    OW_TRIS=INPUT;
    // Wait for the sensors to respond
    Delay10TCYx(24);
    // Determine is a device has responded
    device_found = !OW_PIN;
    return device_found;
}

void ow_write_byte(unsigned char data)
{
    char i;
    for (i=0;i<8;i++)
    {
        // DQ Low
        OW_LAT=0;
        OW_TRIS=OUTPUT;
        // Keep it low for 10us to start the WRITE
        Delay10TCYx(1);
        // Keep low i.e. keep output mode and low if WRITE0
        // or release line i.e. make input to take it high if WRITE1
        OW_TRIS = data & 0x01;
        Delay10TCYx(5);
        // Release the line. Total of 60us
        OW_TRIS=INPUT;
        // Some recovery time between the bits 2us
        Delay10TCYx(2);
        data =data >>1;
    }
}



char ow_read_byte(void)
{
    char data=0;
    char i;
    for (i=0;i<8;i++)
    {
        // DQ Low. Initiate READ
        OW_LAT=0;
        OW_TRIS=OUTPUT;
        Nop();
        Nop();
        // DQ High. Release
        OW_TRIS=INPUT;
        // Wait for total of 10 us since READ initiation
        Nop();
        Nop();
        Nop();
        Nop();
        Nop();
        Nop();
        Nop();
        Nop();
        // Read the bit and place it in the data at the right position
        data = (OW_PIN << i) | data;
        // Additional time to complete the minimum 60 us READ cycle
        Delay10TCYx(5);
    }
    return data;
}
#endif