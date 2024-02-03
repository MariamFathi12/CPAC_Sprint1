#include <stdint.h>
#include <stdbool.h>
#include "inc/tm4c123gh6pm.h"
#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/timer.h"
#include "driverlib/pin_map.h"

#include "Ultrasonic.h"


int main(void)
{
    uint32_t Distance = 0; /* stores measured distance value */

    //Enable Port B Clock
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOB))
    {
    }

    //Enable Port E Clock
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOE))
    {
    }

    //Enable Port A Clock
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOA))
    {
    }

    //Set PB0, PB1, PB4 as Digital Output pins
    GPIOPinTypeGPIOOutput(GPIO_PORTB_BASE, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_4);

    //Set PE1(RS PIN),PE2(E Pin) PE4, PE5 as Digital Output pins
    GPIOPinTypeGPIOOutput(GPIO_PORTE_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_4|GPIO_PIN_5);

    //Set PA4(TRIG),PA5, PA6, PA7 as Digital Output pins
    GPIOPinTypeGPIOOutput(GPIO_PORTA_BASE, GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7);


    /*Initialize Timer to Capture Echo Pin Event*/
    Ultrasonic_void_TimerInit();

    while(1)
    {
        /*Get the Distance Measurment from the Ultrasonic */
        Distance = Ultrasonic_u32_MeasureDistance();

        void_Delay(1000);
    }
}


