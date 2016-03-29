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
unsigned char getMin(MAZE * maze, COORD coord) 
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

  min = distN;
  min = (distE < min) ? distE : min;
  min = (distS < min) ? distS : min;
  min = (distW < min) ? distW : min;

  return min;
}

unsigned char getDir(MAZE * maze, COORD coord, MOUSE * mouse)
{
  unsigned char min = MAX_DIST;
  unsigned char distN;
  unsigned char distE;
  unsigned char distS;
  unsigned char distW;

  unsigned char orientation = mouse->orientation;

  distN = hasNorth(maze->walls[coord.row][coord.col]) ? MAX_DIST : maze->dist[coord.row + 1][coord.col];
  distE = hasEast(maze->walls[coord.row][coord.col]) ? MAX_DIST : maze->dist[coord.row][coord.col + 1];
  distS = hasSouth(maze->walls[coord.row][coord.col]) ? MAX_DIST : maze->dist[coord.row - 1][coord.col];
  distW = hasWest(maze->walls[coord.row][coord.col]) ? MAX_DIST : maze->dist[coord.row][coord.col - 1];

  // 1. Pick the shortest route
  if ( (distN < distE) && (distN < distS) && (distN < distW) )
    mouse->orientation = 'N';
  else if ( (distE < distN) && (distE < distS) && (distE < distW) )
    mouse->orientation = 'E';
  else if ( (distS < distE) && (distS < distN) && (distS < distW) )
    mouse->orientation = 'S';
  else if ( (distW < distE) && (distW < distS) && (distW < distN) )
    mouse->orientation = 'W';

  // 2. If multiple equally short routes, go straight if possible
  else if ( orientation == 'N' && !hasNorth(maze->walls[mouse->location.row][mouse->location.col]) )
    mouse->orientation = 'N';
  else if ( orientation == 'E' && !hasEast(maze->walls[mouse->location.row][mouse->location.col]) )
  {
    mouse->orientation = 'E';
    printf("PRIORITIZED EAST \n\n");
  }
  else if ( orientation == 'S' && !hasSouth(maze->walls[mouse->location.row][mouse->location.col]) )
    mouse->orientation = 'S';
  else if ( orientation == 'W' && !hasWest(maze->walls[mouse->location.row][mouse->location.col]) )
    mouse->orientation = 'W';

  // 3. Otherwise prioritize N > E > S > W
  else if (!hasNorth(maze->walls[mouse->location.row][mouse->location.col]))
    mouse->orientation = 'N';
  else if (!hasEast(maze->walls[mouse->location.row][mouse->location.col]))
    mouse->orientation = 'E';
  else if (!hasSouth(maze->walls[mouse->location.row][mouse->location.col]))
    mouse->orientation = 'S';
  else if (!hasWest(maze->walls[mouse->location.row][mouse->location.col]))
    mouse->orientation = 'W';

  else
    printf("Stuck... Can't find center.\n");
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
 * Description: Flood search to starting position
 *              Priority is given as followed:
 *              1. Pick the shortest route
 *              2. If multiple equally short routes, 
 *                    choose untraced route N > E > S > W
 *              3. Else, go straight if possible
 *              4. Otherwise prioritize N > E > S > W
 *              Once starting position is found, isolate known dead ends
 */
unsigned char floodfill(MAZE * maze, MOUSE * mouse)
{
  //Return if we're at the center (direction is set to D for done)
  if(maze->dist[mouse->location.row][mouse->location.col] == 0)
    return 'D';

  //Initialize stack
  STACK s = { .stack = {{0}}, .top = 0 }; 

  //Add traces to see route 
  if(!hasTrace(maze->walls[mouse->location.row][mouse->location.col]))
  {
    maze->walls[mouse->location.row][mouse->location.col] |= TRACE;
    mouse->traceCount++;
  }

  int maxTop = 0;

  //If new walls discovered --> cush current cell and adj to new walls on stack
  detectWalls(maze, *mouse);

  //Push current cell onto stack 
  push(&s, mouse->location);

  printf("MOUSE COORD: row: %d col: %d\n", (int) mouse->location.row, (int) mouse->location.col );

  //While stack isn't empty
  while(!empty(&s))
  {
    if(maxTop < s.top)
      maxTop = s.top;
    //pop a cell from the stack
    COORD current = top(&s);
    pop(&s);

    //Check if cell's value = 1 + min. value of (accessible) neighbors

    printf("BEFORE: row: %d col: %d\n", (int) current.row, (int) current.col );

    //If yes, keep popping and checking cells 
    if(maze->dist[current.row][current.col] == (getMin(maze, current) + 1))
      continue;

    //If not, change cell's value to 1 + min of (accessible) neighbors,
    else
    {
      maze->dist[current.row][current.col] = getMin(maze, current) + 1;
      //and push all of cell's (accessible) neighbors onto the stack.
      if(!hasNorth(maze->walls[current.row][current.col]))
      {
        COORD north = current;
        north.row++;
        push(&s, north);
      }
      if(!hasEast(maze->walls[current.row][current.col]))
      {
        COORD east = current;
        east.col++;
        push(&s, east);
      }
      if(!hasSouth(maze->walls[current.row][current.col]))
      {
        COORD south = current;
        south.row--;
        push(&s, south);
      }
      if(!hasWest(maze->walls[current.row][current.col]))
      {
        COORD west = current;
        west.col--;
        push(&s, west);
      }
    }
  }
  getDir(maze, mouse->location, mouse);
  printf("MAX UPDATEDIST SIZE: %d\n",maxTop);
  printf("NEXT MOVE: GO [%c]\n", mouse->orientation);	
  return mouse->orientation;
}

void detectWalls(MAZE * maze, const MOUSE mouse) 
{
  unsigned char wallDetected = 0;
  // Update walls and adjacent walls
  if (hasNorth(maze->input[mouse.location.row][mouse.location.col])) 
  {
    maze->walls[mouse.location.row][mouse.location.col] |= NWALL;
    if (mouse.location.row < SIZE - 1)  // Update adjacent wall
      maze->walls[mouse.location.row + 1][mouse.location.col] |= SWALL;
    wallDetected |= NWALL;
  }
  if (hasEast(maze->input[mouse.location.row][mouse.location.col])) 
  {
    maze->walls[mouse.location.row][mouse.location.col] |= EWALL;
    if (mouse.location.col < SIZE - 1)  // Update adjacent wall
      maze->walls[mouse.location.row][mouse.location.col + 1] |= WWALL;
    wallDetected |= EWALL;
  }
  if (hasSouth(maze->input[mouse.location.row][mouse.location.col])) 
  {
    maze->walls[mouse.location.row][mouse.location.col] |= SWALL;
    if (mouse.location.row > 0)  // Update adjacent wall
      maze->walls[mouse.location.row - 1][mouse.location.col]  |= NWALL;
    wallDetected |= SWALL;
  }
  if (hasWest(maze->input[mouse.location.row][mouse.location.col])) 
  {
    maze->walls[mouse.location.row][mouse.location.col] |= WWALL;
    if (mouse.location.col > 0)  // Update adjacent wall
      maze->walls[mouse.location.row][mouse.location.col - 1] |=  EWALL;
    wallDetected |= WWALL;
  }
  if(wallDetected)
    updateDist(maze, mouse.location, wallDetected);
}

unsigned char updateDist(MAZE * maze, COORD coord, unsigned char detectedWalls)
{
  STACK s = { .stack = {{0}}, .top = 0 }; 
  int maxTop = 0;

  //Push current cell & cells adj. to NEW walls (i.e. detectedWalls) onto stack
  push(&s, coord);
  if((detectedWalls & NWALL) == NWALL && coord.row != SIZE - 1)
  {
    COORD north = coord;
    north.row++;
    push(&s, north); 
  }
  if((detectedWalls & EWALL) == EWALL && coord.col != SIZE - 1)
  {
    COORD east = coord;
    east.col++;
    push(&s, east); 
  }
  if((detectedWalls & SWALL) == SWALL && coord.row != 0)
  {
    COORD south = coord;
    south.row--;
    push(&s, south); 
  }
  if((detectedWalls & WWALL) == WWALL && coord.col != 0)
  {
    COORD west = coord;
    west.col--;
    push(&s, west); 
  }

  while(!empty(&s))
  {
    if(maxTop < s.top)
      maxTop = s.top;
    COORD current = top(&s);
    pop(&s);

    //If yes, keep popping and checking cells 
    if(maze->dist[current.row][current.col] == (getMin(maze, current) + 1))
      continue;

    //If not, change cell's value to 1 + min of (accessible) neighbors, if 
    //it's not a goal cell
    if(maze->dist[current.row][current.col] != 0)
      maze->dist[current.row][current.col] = getMin(maze, current) + 1;

    //and push all of cell's (accessible) neighbors onto the stack.
    if(!hasNorth(maze->walls[current.row][current.col]))
    {
      COORD north = current;
      north.row++;
      if(maze->dist[north.row][north.col] != 0)
        push(&s, north);
    }
    if(!hasEast(maze->walls[current.row][current.col]))
    {
      COORD east = current;
      east.col++;
      if(maze->dist[east.row][east.col] != 0)
        push(&s, east);
    }
    if(!hasSouth(maze->walls[current.row][current.col]))
    {
      COORD south = current;
      south.row--;
      if(maze->dist[south.row][south.col] != 0)
        push(&s, south);
    }
    if(!hasWest(maze->walls[current.row][current.col]))
    {
      COORD west = current;
      west.col--;
      if(maze->dist[west.row][west.col] != 0)
        push(&s, west);
    }
  }
  printf("MAX UPDATEDIST SIZE: %d\n",maxTop);
}
