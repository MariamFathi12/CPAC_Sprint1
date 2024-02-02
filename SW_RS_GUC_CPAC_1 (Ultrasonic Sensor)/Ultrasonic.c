#include <stdint.h>
#include <stdbool.h>
#include "inc/tm4c123gh6pm.h"
#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/timer.h"
#include "driverlib/pin_map.h"

#include "Ultrasonic.h"

void Ultrasonic_void_TimerInit(void)
{
    /*Enable the Timer Peripheral*/
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_TIMER0))
    {
    }

    //Enable Port B Clock
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOB))
    {
    }

    //Enable Port A Clock
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOA))
    {
    }

    /*Set the Capture Pin of Timer0 to be Input, and configure it as Input capture pin for Timer0*/
    GPIOPinTypeGPIOInput(GPIO_PORTB_BASE, GPIO_PIN_6);//Echo Pin
    GPIODirModeSet(GPIO_PORTB_BASE, GPIO_PIN_6, GPIO_DIR_MODE_IN);
    GPIOPadConfigSet(GPIO_PORTB_BASE, GPIO_PIN_6, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD);
    GPIOPinTypeTimer(GPIO_PORTB_BASE, GPIO_PIN_6);
    GPIOPinConfigure(GPIO_PB6_T0CCP0);

    /*Set GPIO PA4 Output to Trig Pin*/
    GPIOPinTypeGPIOOutput(GPIO_PORTA_BASE, GPIO_PIN_4);

    /*Set the Trig Pin to LOW*/
    GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_4, 0);

    /*Configure the Timer0 base to use Timer A as 16-bits in input edge time capture mode, capture both events rising and falling*/
    TimerDisable(TIMER0_BASE, TIMER_A);
    TimerConfigure(TIMER0_BASE, TIMER_CFG_SPLIT_PAIR | TIMER_CFG_A_CAP_TIME_UP);
    TimerControlEvent(TIMER0_BASE, TIMER_A, TIMER_EVENT_BOTH_EDGES);
    TimerEnable(TIMER0_BASE, TIMER_A);

}

uint32_t Ultrasonic_u32_MeasureDistance(void)
{

    uint32_t lastEdge, thisEdge, Distance;
    /*Send a pulse for 10 microseconds on Trig Pin*/
    GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_4, GPIO_PIN_4);
    void_DelayMicroSecond(10);
    GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_4, 0);


    while(1){

        /*Clear the Capture event Pin*/
        TimerIntClear(TIMER0_BASE, TIMER_CAPA_EVENT);

        /*Polling on the input capture event flag to capture rising edge*/
        while(!(TimerIntStatus(TIMER0_BASE, false) & TIMER_CAPA_EVENT));

        /*Save the Timer Value*/
        lastEdge = TimerValueGet(TIMER0_BASE, TIMER_A);

        /*Clear the Capture event Pin*/
        TimerIntClear(TIMER0_BASE, TIMER_CAPA_EVENT);

        /*Polling on the input capture event flag to capture falling edge*/
        while(!(TimerIntStatus(TIMER0_BASE, false) & TIMER_CAPA_EVENT));
        /*Save the Timer Value*/
        thisEdge = TimerValueGet(TIMER0_BASE, TIMER_A);

        /*Distance is the difference between the 2 timer reading multiplied by the conversion factor from ticks to cm/s*/
        Distance = ((thisEdge - lastEdge) * 10625) / 10000000;

        if(!((Distance >= 2) && (Distance <= 350))){/*Check if Distance is out of bound*/
            Distance = 0u;
        }
        else{

        }

        return Distance;
    }
}


void void_DelayMicroSecond(int time)
{

    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER1);
    TimerConfigure(DELAY_TIMER_BASE, TIMER_CFG_PERIODIC);
    TimerLoadSet(DELAY_TIMER_BASE, TIMER_A, SysCtlClockGet() / 1000000 * time);

    TimerEnable(DELAY_TIMER_BASE, TIMER_A);

    while(TimerValueGet(DELAY_TIMER_BASE, TIMER_A) != 0);
}

void void_Delay(unsigned long counter)
{
    volatile unsigned long i = 0;
    for(i = 0; i < counter * 1000; i++);
}


