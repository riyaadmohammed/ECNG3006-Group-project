#include <p18f452.h>
#include <delays.h>
#include "xlcd.h"
#include <string.h>
#include <stdio.h>
#include <adc.h>
#include <stdlib.h>
#include <float.h>
#include <pwm.h>
#include <timers.h> 

/*  Write the appropriate code to set configuration bits:
* - set HS oscillator
* - disable watchdog timer
* - disable low voltage programming
*/
#pragma config OSC = HS
#pragma config WDT = OFF
#pragma config LVP = OFF

void main(){
    OpenTimer2(  TIMER_INT_OFF &    T2_PS_1_16 & T2_POST_1_1  );
    OpenPWM2(2000);
    SetDCPWM2(511);

    while(1){}
}
