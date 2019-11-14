///////////////////////////////////////////////////////////////////
//Student ID No.:
//Date:
///////////////////////////////////////////////////////////////////

#include    "FreeRTOS.h"
#include 	"task.h"
#include    <timers.h>
#include    <p18f452.h>
#include    <p18cxxx.h>
#include    <xlcd.h>

/*  Write the appropriate code to set configuration bits:
* - set HS oscillator
* - disable watchdog timer
* - disable low voltage programming
*/
#pragma config OSC = HS
#pragma config WDT = OFF
#pragma config LVP = OFF
int a = 0;

void vSerialTxISR(void){

}
void vSerialRxISR(void){

}

void lcdconfig (void){
    OpenXLCD (FOUR_BIT & LINES_5X7);
    //while(BusyXLCD());
    //SetDDRamAddr(0x23);
    //while(BusyXLCD());
    //WriteCmdXLCD(SHIFT_DISP_LEFT);
     
}

/* Write LCD delay functions */
void DelayFor18TCY( void )
{
  /* Create a delay of 18 cycles  */
    Nop();
    Nop();
    Nop();
    Nop();
    Nop();
    Nop();
    Nop();
    Nop();
    Nop();
    Nop();
    Nop();
    Nop();
    Nop();
    Nop();
    Nop();
    Nop();
    Nop();
    Nop();  
}

void DelayPORXLCD (void)
{
  /* Create a delay of 15 ms  */ 
    Delay1KTCYx(15);
}

void DelayXLCD (void)
{
  /* Create a delay of 5 ms  */  
    Delay1KTCYx(5);
}

/* Write the appropriate code to do the following:
* Configure PORTB pin 1 as an output
* TaskA will loop until the global variable stopped is set.
* Within the loop -- print string "Task 1 rocks! \n" to LCD top row
*                 -- toggle PORTB pin 1
*                 -- delay for 1 second using vTaskDelay()
* TaskA will delete itself if the loop is exited.
*/
void TaskA(void *pdata)
{
    TRISBbits.RB1 = 0;
    while(a == 0)
    {
        while(BusyXLCD()) ; 
        WriteCmdXLCD(SHIFT_DISP_LEFT);
        while(BusyXLCD());
        SetDDRamAddr(0x00);
        while(BusyXLCD());
        putrsXLCD("Task 1 rocks!");//Task 1 rocks!
        while(BusyXLCD());
        if(PORTBbits.RB1 == 0){
        PORTBbits.RB1 = 1;
        }
        else{
        PORTBbits.RB1 = 0;
        } 
        vTaskDelay(250);
    }
    vTaskDelete(NULL);
}

/* Write the appropriate code to do the following:
* Configuee PORTB pin 2 as an output
* TaskB will loop until the global variable stopped is set.
* Within the loop -- print string "Task 2 rules?\n" to LCD bottom row
*                 -- toggle PORTB pin 2
*                 -- delay of 200 ms using vTaskDelay(), specifying the time in ms instead of ticks
* TaskB will delete itself if the loop is exited.
*/
void TaskB(void *pdata)
{
    TRISBbits.RB2 = 0;
    while(a == 0)
    {
        while(BusyXLCD()) ; 
        WriteCmdXLCD(SHIFT_DISP_LEFT);
        while(BusyXLCD());
        SetDDRamAddr(0x55);
        while(BusyXLCD());
        putrsXLCD("Task 2 rules?");//"Task 2 rules"
         while(BusyXLCD());
        if(PORTBbits.RB2 == 0){
        PORTBbits.RB2 = 1;
        }
        else{
        PORTBbits.RB2 = 0;
        }
        vTaskDelay(50);
    }
    vTaskDelete(NULL);
}

void main (void)
{
    //TRISB=0;
  // Write the appropriate code to disable all interrupts 
	INTCONbits.GIE = 0;
  // Write code to create tasks A and B
	xTaskCreate(TaskA, "TaskA", configMINIMAL_STACK_SIZE, NULL, 1, NULL);
    xTaskCreate(TaskB, "TaskB", configMINIMAL_STACK_SIZE, NULL, 1, NULL);
    
  /* Write the appropriate code to define the priorities for taskA
  *   and taskB in "FreeRTOSConfig.h". Use these defines to assign priorities 
  *   when creating taskA and taskB with xTaskCreate()
  */
  //Creating a counting semaphore
    
  // Initialise the LCD display
    lcdconfig();

  // Write the appropriate code to start FreeRTOS kernel
    vTaskStartScheduler();
}

