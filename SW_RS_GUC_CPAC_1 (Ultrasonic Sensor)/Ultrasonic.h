#ifndef ULTRASONIC_H
#define ULTRASONIC_H

/*Configure The Capture Timer Base*/
#define DELAY_TIMER_BASE    TIMER1_BASE

uint32_t Ultrasonic_u32_MeasureDistance(void);

void Ultrasonic_void_TimerInit(void);

void void_DelayMicroSecond(int time);

void void_Delay(unsigned long counter);


#endif
