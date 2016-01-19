#ifndef BUTTON_H
#define BUTTON_H

#include "scheduler.h"
#include <stdint.h>

const uint8_t DB_POLL_PERIOD = 10;
const uint8_t LP_POLL_PERIOD = 10;

class button : public polled_event
{
public:
	button(uint8_t io, uint8_t openval, uint8_t db_time, uint16_t lp_time);
	virtual void init();
	virtual void execute();
	void set_sp_callback(void(*func)());
	void set_lp_callback(void(*func)(uint16_t));
private:
	void set_open_value(uint8_t);
	uint8_t open_value;
	uint8_t gpio;
	uint8_t button_db_count_target; //how long to debounce for 
	uint16_t button_lp_count_target; //how long to hold button until triggers long press
	
	void (*spFunc)();
	void (*lpFunc)(uint16_t);
	
	uint8_t debounce_count;
	uint16_t lp_count;
	uint16_t lp_pressed;
	
	void set_BUTTON_OPEN_STATE();
	void BUTTON_OPEN_STATE();
	void set_BUTTON_CLOSED_SHORT_STATE();
	void BUTTON_CLOSED_SHORT_STATE();
	void set_BUTTON_CLOSED_LONG_STATE();
	void BUTTON_CLOSED_LONG_STATE();
	void (button::*stateptr)(void);
};
#endif