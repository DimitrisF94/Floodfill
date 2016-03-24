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
 * that are not separated .row a wall
 */
 unsigned char getMin(MAZE * maze, COORD coord, unsigned char * direction) 
 {
   unsigned char min = MAX_DIST;
   unsigned char distN;
   unsigned char distE;
   unsigned char distS;
   unsigned char distW;
   
   distN = hasNorth(maze->walls[coord.row][coord.col]) ? MAX_DIST : maze->dist[coord.row + 1][coord.col];
   distE = hasEast(maze->walls[coord.row][coord.col]) ? MAX_DIST : maze->dist[coord.row][coord.col + 1];
   distS = hasSouth(maze->walls[coord.row][coord.col]) ? MAX_DIST : maze->dist[coord.row - 1][coord.col];
   distW = hasWest(maze->walls[coord.row][coord.col]) ? MAX_DIST : maze->dist[coord.row][coord.col - 1];
   
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
 * Assumptions: Maze distances have been preset .row initDist 
 * Result: Sets the distance of each cell from the goal cell. 
 * Parameters: 
 * 		m - a struct holding the maze arr.row, 
 *		goal - the coordinates of the goal in the maze
 * 		init - bool to determine whether or not this call is initializing the maze
 *		mouse - representation of the "mouse"
 *		
 */
unsigned char floodfill(MAZE * maze, COORD goal, MOUSE mouse)
{
	// Walls
	//  0 0 0 0       0 0 0 0
	//    D V T       W S E N
	// [row] [col]
	//  N S   W E 
	unsigned char direction;

	detectWalls(maze, mouse);

	//Return if we're at the center (direction is set to D for done)
	if((mouse.location.row == goal.row && mouse.location.col == goal.col))
		return 'D';

	//Initialize stack
	STACK s = { .stack = {{0}}, .top = 0 }; 

	//Update visited? 

	//Add traces to see route 
	if(!hasTrace(maze->walls[mouse.location.row][mouse.location.col]))
		maze->walls[mouse.location.row][mouse.location.col] |= TRACE;

	//Push current cell onto stack 
	push(&s, mouse.location);

	//While stack isn't empty
	while(!empty(&s))
	{
		//pop a cell from the stack
		COORD current = top(&s);
		pop(&s);

		//Check if cell's value = 1 + min. value of (accessible) neighbors

		printf("BEFORE: row: %d col: %d\n", (int) current.row, (int) current.col );
		//If yes, keep popping and checking cells 
		if(maze->dist[current.row][current.col] == (getMin(maze, current, &direction) + 1))
		{
			printf("WHY AM I IN HERE\n");
			continue;
		}
		//If not, change cell's value to 1 + min of (accessible) neighbors,
		else
		{
			maze->dist[current.row][current.col] = getMin(maze, current, &direction) + 1;
			//and push all of cell's (accessible) neighbors onto the stack.
		    if(!hasNorth(maze->walls[current.row][current.col]))
		    {
		    	COORD north = current;
		    	north.row++;
				printf("CURR NORTH: row: %d col: %d\n", (int) north.row, (int) north.col );
		    	push(&s, north);
		    }
		    if(!hasEast(maze->walls[current.row][current.col]))
		    {
		    	COORD east = current;
		    	east.col++;
				printf("CURR EAST: row: %d col: %d\n", (int) east.row, (int) east.col );
		    	push(&s, east);
		    }
		    if(!hasSouth(maze->walls[current.row][current.col]))
		    {
		    	COORD south = current;
		    	south.row--;
				printf("CURR SOUTH: row: %d col: %d\n", (int) south.row, (int) south.col );
		    	push(&s, south);
		    }
		    if(!hasWest(maze->walls[current.row][current.col]))
		    {
		    	COORD west = current;
		    	west.col--;
				printf("CURR WEST: row: %d col: %d\n", (int) west.row, (int) west.col );
		    	push(&s, west);
		    }
		}
	}
	getMin(maze, mouse.location, &direction);
	printf("NEXT MOVE: GO [%c]\n", direction);	
	return direction;
}

void detectWalls(MAZE * maze, const MOUSE mouse) 
{
  // Update walls and adjacent walls
  if (hasNorth(maze->input[mouse.location.row][mouse.location.col])) 
  {
    maze->walls[mouse.location.row][mouse.location.col] |= NWALL;
    if (mouse.location.row < SIZE - 1)  // Update adjacent wall
      maze->walls[mouse.location.row + 1][mouse.location.col] |= SWALL;
  }
  if (hasEast(maze->input[mouse.location.row][mouse.location.col])) 
  {
    maze->walls[mouse.location.row][mouse.location.col] |= EWALL;
    if (mouse.location.col < SIZE - 1)  // Update adjacent wall
      maze->walls[mouse.location.row][mouse.location.col + 1] |= WWALL;
  }
  if (hasSouth(maze->input[mouse.location.row][mouse.location.col])) 
  {
    maze->walls[mouse.location.row][mouse.location.col] |= SWALL;
    if (mouse.location.row > 0)  // Update adjacent wall
      maze->walls[mouse.location.row - 1][mouse.location.col]  |= NWALL;
  }
  if (hasWest(maze->input[mouse.location.row][mouse.location.col])) 
  {
    maze->walls[mouse.location.row][mouse.location.col] |= WWALL;
    if (mouse.location.col > 0)  // Update adjacent wall
      maze->walls[mouse.location.row][mouse.location.col - 1] |=  EWALL;
  }
}