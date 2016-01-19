#include "button.h"
#include "Arduino.h"


//temporary data:
//only for items used just in the particular state

//call the short button press callback after release
//call the long button press callback when time is exceeded
//and again every poll period.  pass the number of poll periods
//the lp has existed, starting at one.  A zero indicates a
//button release


button::button(uint8_t io, uint8_t openval, uint8_t db_time_ms, uint16_t lp_time_ms)
: polled_event(DB_POLL_PERIOD)
{
	set_open_value(openval);
	gpio = io;
	
	spFunc = NULL;
	lpFunc = NULL;
	
	set_BUTTON_OPEN_STATE();
	
	//pinMode(gpio, INPUT);
	button_db_count_target = db_time_ms/DB_POLL_PERIOD;  //time in ms divided by poll period in ms
	button_lp_count_target = lp_time_ms/LP_POLL_PERIOD;  //time in ms divided by poll period in ms
}

void button::set_BUTTON_OPEN_STATE()
{
	lp_count = 0;
	debounce_count = 0;
	stateptr = &button::BUTTON_OPEN_STATE;
}

void button::set_open_value(uint8_t val)
{
	open_value = val;
}

void button::init()
{
	pinMode(gpio, INPUT);
}

void button::set_sp_callback(void(*func)())
{
	spFunc = func;
}

void button::set_lp_callback(void(*func)(uint16_t))
{
	lpFunc = func;
}

void button::BUTTON_OPEN_STATE()
{
	
	//only way to change states is a debounced button press
	if(digitalRead(gpio) != open_value)
	{
		//increment the count
		debounce_count++;
		if(debounce_count >= button_db_count_target)
		{
			//verified full on button press
			set_BUTTON_CLOSED_SHORT_STATE();
			return;
		}
		else
		{
			return;
		}
	}
	else
	{
		//make sure the debounce_count is zero
		//stay in this state
		debounce_count = 0;
		return;
	}
	
	return;
}

void button::set_BUTTON_CLOSED_SHORT_STATE()
{
	stateptr = &button::BUTTON_CLOSED_SHORT_STATE;
	debounce_count = 0;
}

void button::BUTTON_CLOSED_SHORT_STATE()
{
	
	//change state through button release or
	//lp press interval
	
	//first check for button release (should be higher priority than lp
	if(digitalRead(gpio) == open_value)
	{
		//increment the debounce count
		debounce_count++;
		if(debounce_count >= button_db_count_target)
		{
			//verified button release
			//have to the short released state
			
			//commit to the short press at release
			if(spFunc)
			{
				(*spFunc)();
			}
			set_BUTTON_OPEN_STATE();
			return;
		}
		else
		{
			//continue to check for long press anyway
		}
	}
	else
	{
		//zero out the count for next time
		debounce_count = 0;
	}
	
	//now we have to see if we transition to the lp state
	lp_count++;
	if(lp_count >= button_lp_count_target)
	{
		//passed the long press interval
		set_BUTTON_CLOSED_LONG_STATE();
		if(lpFunc)
		{
			(*lpFunc)(1);
		}
		return;
	}
	else
	{
		//continue in this state
	}
}

void button::set_BUTTON_CLOSED_LONG_STATE()
{
	stateptr = &button::BUTTON_CLOSED_LONG_STATE;
	debounce_count = 0;
	lp_count = 0;  //for next time through
	lp_pressed = 1;
}

void button::BUTTON_CLOSED_LONG_STATE()
{
	
	//only way to change states is a debounced button release 
	if(digitalRead(gpio) == open_value)
	{
		//increment the count
		debounce_count++;
		if(debounce_count >= button_db_count_target)
		{
			//verified full on button release
			set_BUTTON_OPEN_STATE();
			if(lpFunc)
			{
				(*lpFunc)(0);//0 indicates a release
			}
			return;
		}
		else
		{
			lp_pressed++;
			if(lpFunc)
			{
				(*lpFunc)(lp_pressed);
			}
			return;
		}
	}
	else
	{
		//make sure the debounce_count is zero
		debounce_count = 0;
		lp_pressed++;
		if(lpFunc)
		{
			(*lpFunc)(lp_pressed);
		}
		return;
	}
	
	return;
}

void button::execute()
{
	//start it all over again
	reset_counter();
	//finally execute the function
	(this->*stateptr)();
	
}