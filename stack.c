#include <stdio.h>
#include "maze.h"

int push(STACK * s, const COORD c)
{
	if(full(s))
		return 0;

	s->stack[s->top] = c;
  printf("coord row: %d coord col: %d \n", c.row, c.col);
  printf("stack row: %d stack col: %d \n", s->stack[s->top].row, s->stack[s->top].col);
	s->top++;
	return 1;
}

int pop(STACK * s)
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
	return s->stack[s->top-1];
}
