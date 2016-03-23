/* Headers */
#include <stdio.h>
#include "maze.h"

/*
 * Use * for visited 
 * Use --- for n/s walls
 * Use | for w/e walls 
 * Use + for wall posts 
 */

/* Driver for the micromouse program
 * Takes in 2 arguments: 
 * 1st - simulation or actual mouse run 
 * 2nd - if simulation, reads maze data from this argument input file  
 */ 
int main( int argc, char * argv[] ) 
{
	char temp;

	struct Maze m = { .input = {0}, .walls = {0}, .dist = {0} };

	struct Coord goal = { .x = 3, .y = 3 };

	initGrid(&m);

	initDist(&m, goal);

	//Reads in an input file with maze wall data (for simulation only) 
	readMaze(&m);

	//initDist();
	printf("done");
}

/*
 * Function name: initGrid()
 * Description: Initializes a 16 by 16 square grid. 
 *              Lower left square is (0, 0).
 *              Initializes distances.
 */
void initGrid(struct Maze * m) 
{	
  //  0 0 0 0       0 0 0 0
  //     DE TRACE   W S E N

  int i, j, k;
  
  // Map known walls
  m->input[0][0] |= SWALL;  // Starting walls
  m->input[0][0] |= WWALL;
  m->input[0][0] |= EWALL;

  m->input[0][1] |= SWALL;
  m->input[0][1] |= WWALL;

  // Map north wall
  for (int j = 0; j < SIZE; j++) 
    m->input[SIZE-1][j] |= NWALL;

  // Map east wall
  for (int i = 0; i < SIZE; i++)   
    m->input[i][SIZE-1] |= EWALL;

  // Map south wall
  for (int j = 0; j < SIZE; j++) 
    m->input[0][j] |= SWALL;

  // Map west wall
  for (int i = 0; i < SIZE; i++) 
    m->input[i][0] |= WWALL;
}

void initDist(struct Maze * m, struct Coord goal)
{
	/*
	int i, j, k;
	// Initialize distances for flood fill
	k = SIZE - 2;
	for ( i = 0; i < SIZE; i++ ) 
	{
	  for ( j = 0; j < SIZE; j++) 
	  {
	    distance[i][j] = k;
	    if (j < (SIZE/2 - 1) )
	      k--;
	    else if ( (j > SIZE/2 - 1) && (j < SIZE-1) )
	      k++;
	  }
	  if (i < SIZE/2 - 1)
	    k--;
	  else if ( (i > SIZE/2 - 1) && (i < SIZE-1) )
	    k++;
	}
	*/
}

void readMaze(struct Maze * m) {
	printf("Enter custom maze: \n");
	for(int i = SIZE-1; i >= 0; i--) {
		for(int j = 0; j < SIZE; j++) {
			scanf("%d", &m->input[i][j]);
		}
	}
}


