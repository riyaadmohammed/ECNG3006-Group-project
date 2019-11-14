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
#include    <semphr.h>
#include    <stdlib.h>
#include    <queue.h>
#include    <adc.h>
//#include    <string.h>

/*  Write the appropriate code to set configuration bits:
* - set HS oscillator
* - disable watchdog timer
* - disable low voltage programming
*/
#pragma config OSC = HS
#pragma config WDT = OFF
#pragma config LVP = OFF
volatile int a = 0;
volatile int Stimesend;
volatile int Stimereceive;
volatile int unreadspaces=0;
volatile int results;
volatile int results1;
volatile char Sig[10];
volatile char Sig1[10]="0";
volatile char Sig2[10]="0";
volatile char Sig3[10]="0";
volatile char Stime[10];
volatile char Spaces[10];
SemaphoreHandle_t xCountingSemaphore;
//xCountingSemaphore = xSemaphoreCreateCounting(1,0);
SemaphoreHandle_t xTimerSemaphore; 
SemaphoreHandle_t FDirSem;
SemaphoreHandle_t BDirSem;
QueueHandle_t xMQueue;

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
//appISR must be run by the interrupt service rouyine in port.c
#pragma interruptlow appISR save=PRODH, PRODL, TABLAT, section(".tmpdata")
void appISR(void){
//Check if timer0 overflowed
if(INTCONbits.TMR0IF == 1)
    {
    INTCONbits.TMR0IF = 0;
    WriteTimer0( 62411 );
    Delay10TCYx( 5 ); // Delay for 50TCY
    ConvertADC(); // Start conversion
    while( BusyADC() ); // Wait for completion
    results = ReadADC(); // Read result
    xQueueSendFromISR(xMQueue , &results ,NULL);
    }  
      //vTaskDelay(25);  
     xSemaphoreGiveFromISR(xCountingSemaphore, NULL);  
     //unreadspaces =   uxQueueMessagesWaiting( xMQueue );
    
   
//if timer0 overflowed, reset it and set value in timer to expire in 200ms
//release semaphore to signal to taskB
}
/* Write the appropriate code to do the following:
* Configure PORTB pin 1 as an output
* TaskA will loop until the global variable stopped is set.
* Within the loop -- print string "Task 1 rocks! \n" to LCD top row
*                 -- toggle PORTB pin 1
*                 -- delay for 1 second using vTaskDelay()
* TaskA will delete itself if the loop is exited.
*/
/*void TaskAnalog(void *pdata)
{
    
    while(a == 0)
    {
     if(INTCONbits.TMR0IF == 1)
    {
    INTCONbits.TMR0IF = 0;
    WriteTimer0( 62411 );
    Delay10TCYx( 5 ); // Delay for 50TCY
    ConvertADC(); // Start conversion
    while( BusyADC() ); // Wait for completion
    results = ReadADC(); // Read result
    xQueueSend(xMQueue , &results ,NULL);
    }  
      //vTaskDelay(25);  
     xSemaphoreTake(FDirSem,1000);
     xSemaphoreGive(xCountingSemaphore);
    }
    vTaskDelete(NULL);
}*/

/* Write the appropriate code to do the following:
* Configuee PORTB pin 2 as an output
* TaskB will loop until the global variable stopped is set.
* Within the loop -- print string "Task 2 rules?\n" to LCD bottom row
*                 -- toggle PORTB pin 2
*                 -- delay of 200 ms using vTaskDelay(), specifying the time in ms instead of ticks
* TaskB will delete itself if the loop is exited.
*/
void TaskLCD(void *pdata)
{
    
    while(a == 0)
    {
        unreadspaces++;
        if(unreadspaces==1){
        xQueueReceive(xMQueue, &results1, 1 );
        itoa(results1 , Sig);
        itoa(results1 , Sig1);
        //xQueueReset(xMQueue);
        while(BusyXLCD()) ; 
        WriteCmdXLCD(SHIFT_DISP_LEFT);
        while(BusyXLCD());
        SetDDRamAddr(0x00);
        while(BusyXLCD());
        putsXLCD(Sig/*"Task 1 rocks!"*/);//Task 1 rocks!
        while(BusyXLCD());
        SetDDRamAddr(0x40);
        while(BusyXLCD());
        putsXLCD(Sig3/*"Task 1 rocks!"*/);//Task 1 rocks!
        while(BusyXLCD());
        SetDDRamAddr(0x14);
        while(BusyXLCD());
        putsXLCD(Sig2/*"Task 1 rocks!"*/);//Task 1 rocks!
        while(BusyXLCD());
        SetDDRamAddr(0x54);
        while(BusyXLCD());
        putsXLCD(Sig1/*"Task 1 rocks!"*/);//Task 1 rocks!
        while(BusyXLCD());
        //SetDDRamAddr(0x54);
        //while(BusyXLCD());
        //putsXLCD(Stime/*"Task 2 rules?"*/);//"Task 2 rules"-
        //while(BusyXLCD());  
        }
        if(unreadspaces==2){
        xQueueReceive(xMQueue, &results1, 10 );
        itoa(results1 , Sig);
        itoa(results1 , Sig2);
        //xQueueReset(xMQueue);
        while(BusyXLCD()) ; 
        WriteCmdXLCD(SHIFT_DISP_LEFT);
        while(BusyXLCD());
        SetDDRamAddr(0x00);
        while(BusyXLCD());
        putsXLCD(Sig/*"Task 1 rocks!"*/);//Task 1 rocks!
        while(BusyXLCD());
        SetDDRamAddr(0x40);
        while(BusyXLCD());
        putsXLCD(Sig3/*"Task 1 rocks!"*/);//Task 1 rocks!
        while(BusyXLCD());
        SetDDRamAddr(0x14);
        while(BusyXLCD());
        putsXLCD(Sig2/*"Task 1 rocks!"*/);//Task 1 rocks!
        while(BusyXLCD());
        SetDDRamAddr(0x54);
        while(BusyXLCD());
        putsXLCD(Sig1/*"Task 1 rocks!"*/);//Task 1 rocks!
        while(BusyXLCD());
          
        }
        if(unreadspaces==3){
        xQueueReceive(xMQueue, &results1, 10 );
        itoa(results1 , Sig);
        itoa(results1 , Sig3);
        //xQueueReset(xMQueue);
        while(BusyXLCD()) ; 
        WriteCmdXLCD(SHIFT_DISP_LEFT);
        while(BusyXLCD());
       SetDDRamAddr(0x00);
        while(BusyXLCD());
        putsXLCD(Sig/*"Task 1 rocks!"*/);//Task 1 rocks!
        while(BusyXLCD());
        SetDDRamAddr(0x40);
        while(BusyXLCD());
        putsXLCD(Sig3/*"Task 1 rocks!"*/);//Task 1 rocks!
        while(BusyXLCD());
        SetDDRamAddr(0x14);
        while(BusyXLCD());
        putsXLCD(Sig2/*"Task 1 rocks!"*/);//Task 1 rocks!
        while(BusyXLCD());
        SetDDRamAddr(0x54);
        while(BusyXLCD());
        putsXLCD(Sig1/*"Task 1 rocks!"*/);//Task 1 rocks!
        while(BusyXLCD());
    
        }
        if(unreadspaces==4){
        unreadspaces = 0;
        xQueueReceive(xMQueue, &results1, 10 );
        itoa(results1 , Sig);
        //xQueueReset(xMQueue);
        while(BusyXLCD()) ; 
        WriteCmdXLCD(SHIFT_DISP_LEFT);
        while(BusyXLCD());
        SetDDRamAddr(0x00);
        while(BusyXLCD());
        putsXLCD(Sig/*"Task 1 rocks!"*/);//Task 1 rocks!
        while(BusyXLCD());
        SetDDRamAddr(0x40);
        while(BusyXLCD());
        putsXLCD(Sig3/*"Task 1 rocks!"*/);//Task 1 rocks!
        while(BusyXLCD());
        SetDDRamAddr(0x14);
        while(BusyXLCD());
        putsXLCD(Sig2/*"Task 1 rocks!"*/);//Task 1 rocks!
        while(BusyXLCD());
        SetDDRamAddr(0x54);
        while(BusyXLCD());
        putsXLCD(Sig1/*"Task 1 rocks!"*/);//Task 1 rocks!
        while(BusyXLCD());
      
        }
        //vTaskDelay(10);  
        xSemaphoreTake(xCountingSemaphore,100);
        xSemaphoreGive(BDirSem);
    }
    vTaskDelete(NULL);
}

void TaskFilter(void *pdata)
{
   
    while(a == 0)
    {
     xQueueReceive(xMQueue, &results1, 10 );
     itoa(results1 , Sig);   
     //vTaskDelay(5);
     xSemaphoreTake(BDirSem,1000);
     xSemaphoreGive(FDirSem);
    }
    vTaskDelete(NULL);
}

void main (void)
{
    //TRISB=0;
  // Write the appropriate code to disable all interrupts 
	INTCONbits.GIE = 0;
    //Creating Semaphores
    xCountingSemaphore = xSemaphoreCreateCounting(1,1);
    xTimerSemaphore = xSemaphoreCreateCounting(1,0);
    FDirSem = xSemaphoreCreateCounting(1,1);
    BDirSem = xSemaphoreCreateCounting(1,1);
    xMQueue = xQueueCreate( 4, sizeof(TickType_t )); 
  // Write code to create tasks A and B
	//xTaskCreate(TaskAnalog, "TaskAnalog", configMINIMAL_STACK_SIZE, NULL, 2, NULL);
    xTaskCreate(TaskLCD, "TaskLCD", configMINIMAL_STACK_SIZE, NULL, 1, NULL);
    xTaskCreate(TaskFilter, "TaskFilter", configMINIMAL_STACK_SIZE, NULL, 1, NULL);
  /* Write the appropriate code to define the priorities for taskA
  *   and taskB in "FreeRTOSConfig.h". Use these defines to assign priorities 
  *   when creating taskA and taskB with xTaskCreate()
  */
    INTCONbits.GIE = 1;
  //Creating a counting semaphore
  //Configure Timer0
    OpenTimer0( TIMER_INT_ON & T0_16BIT & T0_SOURCE_INT & T0_PS_1_32 );
    WriteTimer0( 62411 );
    OpenADC( ADC_FOSC_32 & ADC_RIGHT_JUST & ADC_1ANA_0REF, ADC_CH0 & ADC_INT_OFF );
  // Initialise the LCD display
    lcdconfig();
    results = 0;
    xQueueSend(xMQueue , &results ,NULL);
  // Write the appropriate code to start FreeRTOS kernelo
    vTaskStartScheduler();
}

