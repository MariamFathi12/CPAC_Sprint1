#include <stdbool.h>
#include <stdint.h>
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "inc/tm4c123gh6pm.h"
#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "InfraredSensor.h"



void Delay_ms(uint32_t MS){
    volatile uint16_t i,j;

    for(i=0;i<MS;i++){
        for(j=0;j<3180;j++){

        }
    }
  
}


void IR_void_Init(void)
{

    Delay_ms(20);

    //Enable Port B Clock
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
    while (!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOB))
    {
    }
    
    /*Initialize the GPIO Lock Register to unlock the commit register*/
    GPIOUnlockPin(GPIO_PORTB_BASE, GPIO_PIN_0);
    
    //Sets PB0 internal pull-up
    GPIOPadConfigSet(GPIO_PORTB_BASE, GPIO_PIN_0, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);

    GPIOPinTypeGPIOInput(GPIO_PORTB_BASE, GPIO_PIN_0);
    
    

}



bool IR_bool_Read(void)
{
    if (!(GPIO_PORTB_DATA_R & (1<<0))) // Active Low
    {
        return (true);
    }
    else
    {
        return (false);
       
    }
}
