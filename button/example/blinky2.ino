#include "scheduler.h"
#include <Arduino.h>
#include "blinky_led.h"
#include "button.h"
#include "FlexiTimer2.h"

#define LED1_DEFAULT	1000
#define LED2_DEFAULT	1500
#define LED3_DEFAULT	1750	

blinky_led led1(2, 1000, 1); 
blinky_led led2(3, 1500, 1);
blinky_led led3(4, 1750, 1);

button button1(10, 1, 70, 3000);
button button2(11, 1, 70, 3000);
button button3(12, 1, 70, 3000);

void button1_sp_callback();
void button2_sp_callback();
void button3_sp_callback();

void button1_lp_callback(uint16_t call_count);
void button2_lp_callback(uint16_t call_count);
void button3_lp_callback(uint16_t call_count);

void schedGpioFunc(uint8_t lvl);

void setup() 
{
	pinMode(7, OUTPUT);
	
	led1.init();
	led1.enable();
	led2.init();
	led2.enable();
	led3.init();
	led3.enable();
	
	button1.init();
	button1.set_sp_callback(button1_sp_callback);
	button1.set_lp_callback(button1_lp_callback);
	button1.enable();
	
	button2.init();
	button2.set_sp_callback(button2_sp_callback);
	button2.set_lp_callback(button2_lp_callback);
	button2.enable();
	
	button3.init();
	button3.set_sp_callback(button3_sp_callback);
	button3.set_lp_callback(button3_lp_callback);
	button3.enable();
	
	scheduler_o.add_event(&led1);
	scheduler_o.add_event(&led2);
	scheduler_o.add_event(&led3);
	scheduler_o.add_event(&button1);
	scheduler_o.add_event(&button2);
	scheduler_o.add_event(&button3);
	scheduler_o.init(schedGpioFunc);
	scheduler_o.timer_start();
}

void loop()
{
	scheduler_o.execute_events();
}


void button1_sp_callback()
{
	uint16_t period;
	
	period = led1.get_on_off_time();
	
	if(period > 15)
	{
		period = period - 15;
	}
	led1.set_on_off_time(period);
}
	
void button2_sp_callback()
{
	uint16_t period;
	
	period = led2.get_on_off_time();
	
	if(period > 15)
	{
		period = period - 15;
	}
	led2.set_on_off_time(period);
}

void button3_sp_callback()
{
	uint16_t period;
	
	period = led3.get_on_off_time();
	
	if(period > 15)
	{
		period = period - 15;
	}
	led3.set_on_off_time(period);
}

void button1_lp_callback(uint16_t call_count)
{
	//for this example, only do something the first time calling
	//this function for a given long press event_callback
	
	if(call_count == 1)
	{
		//set the led timer to the default value_comp
		led1.set_on_off_time(LED1_DEFAULT);
	}
}

void button2_lp_callback(uint16_t call_count)
{
	//for this example, only do something the first time calling
	//this function for a given long press event_callback
	
	if(call_count == 1)
	{
		//set the led timer to the default value_comp
		led2.set_on_off_time(LED2_DEFAULT);
	}
}

void button3_lp_callback(uint16_t call_count)
{
	//for this example, only do something the first time calling
	//this function for a given long press event_callback
	
	if(call_count == 1)
	{
		//set the led timer to the default value_comp
		led3.set_on_off_time(LED3_DEFAULT);
	}
}

void schedGpioFunc(uint8_t lvl)
{
	digitalWrite(7, lvl);
}