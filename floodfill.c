#include <stdio.h>
#include "maze.h"

/* Check for north wall */
unsigned char hasNorth(const unsigned char currentBlock) {
  return ((currentBlock & NWALL) == NWALL)? 1 : 0;
}

/* Check for east wall */
unsigned char hasEast(const unsigned char currentBlock) {
  return ((currentBlock & EWALL) == EWALL)? 1 : 0;
}

/* Check for south wall */
unsigned char hasSouth(const unsigned char currentBlock) {
  return ((currentBlock & SWALL) == SWALL)? 1 : 0;
}

/* Check for west wall */
unsigned char hasWest(const unsigned char currentBlock) {
  return ((currentBlock & WWALL) == WWALL)? 1 : 0;
}

/* Check for Trace */
unsigned char hasTrace(const unsigned char currentBlock) {
  return ((currentBlock & TRACE) == TRACE)? 1 : 0;
}

/* Check for dead end */
unsigned char isDeadEnd(const unsigned char currentBlock) {
  return ((currentBlock & DE) == DE)? 1 : 0;
}

/* 
 * Return the smallest maze->dist from the surrounding maze->walls
 * that are not separated by a wall
 */
 unsigned char getMin(MAZE * maze, COORD coord, unsigned char * direction) 
 {
   unsigned char min = MAX_DIST;
   unsigned char distN;
   unsigned char distE;
   unsigned char distS;
   unsigned char distW;
   
   distN = hasNorth(maze->walls[coord.y][coord.x]) ? MAX_DIST : maze->dist[coord.y + 1][coord.x];
   distE = hasEast(maze->walls[coord.y][coord.x]) ? MAX_DIST : maze->dist[coord.y][coord.x + 1];
   distS = hasSouth(maze->walls[coord.y][coord.x]) ? MAX_DIST : maze->dist[coord.y - 1][coord.x];
   distW = hasWest(maze->walls[coord.y][coord.x]) ? MAX_DIST : maze->dist[coord.y][coord.x - 1];
   
   if(distW < min) 
   {
   	 min = distW;
   	 *direction = 'W';
   }
   if(distS < min) 
   {
   	 min = distS;
   	 *direction = 'S';
   }
   if(distE < min) 
   {
   	 min = distE;
   	 *direction = 'E';	
   }
   if(distN < min)
   {
     min = distN;
	 *direction = 'N';
   }
   
   return min;
 }



/* Function name: floodfill()
 * Description: Runs the floodfill algorithm
 * Assumptions: Maze distances have been preset by initDist 
 * Result: Sets the distance of each cell from the goal cell. 
 * Parameters: 
 * 		m - a struct holding the maze array, 
 *		goal - the coordinates of the goal in the maze
 * 		init - bool to determine whether or not this call is initializing the maze
 *		mouse - representation of the "mouse"
 *		
 */
unsigned char floodfill(MAZE * maze, COORD goal, MOUSE mouse)
{
	//  0 0 0 0       0 0 0 0
	//    D V T       W S E N
	// [row] [col]
	// [ y ] [ x ]
	unsigned char direction;

	//Return if we're at the center (direction is set to D for done)
	if(mouse.location.y == goal.y && mouse.location.x == goal.x)
		return 'D';

	//Initialize stack
	STACK s = { .stack = {{0}}, .top = 0 }; 

	//Update visited? 

	//Push current cell onto stack 
	push(&s, mouse.location);

	//While stack isn't empty
	while(!empty(&s))
	{
		//pop a cell from the stack
		COORD current = top(&s);
		pop(&s);

		//Check if cell's value = 1 + min. value of (accessible) neighbors

		//If yes, keep popping and checking cells 
		if(maze->dist[current.y][current.x] == (getMin(maze, current, &direction) + 1))
			continue;
		//If not, change cell's value to 1 + min of (accessible) neighbors,
		else
		{
			maze->dist[current.y][current.x] = getMin(maze, current, &direction);
			//and push all of cell's (accessible) neighbors onto the stack.
		    if(!hasNorth(maze->walls[current.y][current.x]))
		    {
		    	COORD north = current;
		    	north.y++;
		    	push(&s, north);
		    }
		    if(!hasEast(maze->walls[current.y][current.x]))
		    {
		    	COORD east = current;
		    	east.x++;
		    	push(&s, east);
		    }
		    if(!hasSouth(maze->walls[current.y][current.x]))
		    {
		    	COORD south = current;
		    	south.y--;
		    	push(&s, south);
		    }
		    if(!hasWest(maze->walls[current.y][current.x]))
		    {
		    	COORD west = current;
		    	west.x--;
		    	push(&s, west);
		    }
		}
	}

	getMin(maze, mouse.location, &direction);
	printf("NEXT MOVE: GO [%c]\n", direction);	
	return direction;
}