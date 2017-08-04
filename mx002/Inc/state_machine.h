#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H

typedef enum {
	STATE_ZERO=0,
	STATE_1,
	STATE_2,
	STATE_3,
	STATE_4,
	STATE_INIT,
	STATE_LAST
} state_name;


typedef struct  {
	state_name current_state;
	state_name next_state;
	void* state_function;
} state_variable;


void function_state1(void);
void function_state2(void);
void function_state3(void);

void state_machine(void);







#endif  // STATE_MACHINE_H
