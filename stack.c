#include "maze.h"

int push(STACK * s, COORD c)
{
	if(full(s))
		return 0;

	s->stack[s->top] = c;
	s->top++;
	return 1;
}

int pop(STACK * s, COORD c)
{
	if(empty(s))
		return 0;

	s->top--;
	return 1;
}

int empty(STACK * s)
{
	return (s->top == 0) ? 1 : 0;
}

int full(STACK * s) 
{
	return (s->top == (STACKSIZE - 1)) ? 1 : 0;
}

COORD top(STACK * s)
{
	return s->stack[s->top];
}