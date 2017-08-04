#include "state_machine.h"


const state_variable states[]= {
		{STATE_1,STATE_2,&function_state1},
		{STATE_2,STATE_3,&function_state2},
		{STATE_3,STATE_1,&function_state3}
};

void function_state1(){};
void function_state2(){};
void function_state3(){};


void state_machine() {};
