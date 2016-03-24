/* Headers */
#include <stdio.h>
#include "maze.h"

/* Driver for the micromouse program
 * Takes in 2 arguments: 
 * 1st - simulation or actual mouse run 
 * 2nd - if simulation, reads maze data from this argument input file  
 */ 
int main( int argc, char * argv[] ) 
{
	char temp;

	MAZE m = { .input = {0}, .walls = {0}, .dist = {0} };

	COORD goal = { .row = 7, .col = 7 };

	MOUSE mouse = { 
					  .orientation = 'N', 
				    .location = { .row = 0, .col = 0 }, 
				    .traceCount = 0
				  };

	initMaze(&m);

	initDist(&m);

	//floodfill(&m, goal, 1, &mouse);

	//Reads in an input file with maze wall data (for simulation only) 
	readMaze(&m);

	//Prints wall info for each cell as integers 
	printGrid(&m);

	//Prints the grid with mouse location and distances to center cells
	visualizeGrid(&m, mouse);

	mouser(&m, goal, &mouse);
	
	/*
	printf("Grid visualization after floodfill \n");

	visualizeGrid(&m, mouse);

	printf("done");
	*/
}

/*
 * Function name: initMaze()
 * Description: Initializes a 16 by 16 square maze. 
 *              Lower left square is (0, 0).
 *              Initializes known walls.
 */
void initMaze(MAZE * m) 
{	
  //  0 0 0 0       0 0 0 0
  //     DE TRACE   W S E N
  int i, j, k;

  // Map known walls
  m->walls[0][0] = SWALL | WWALL | EWALL;  // Starting walls
  m->walls[0][1] = SWALL | WWALL;

  // Map north wall
  for (j = 0; j < SIZE; j++) 
    m->walls[SIZE-1][j] |= NWALL;

  // Map east wall
  for (i = 0; i < SIZE; i++)   
    m->walls[i][SIZE-1] |= EWALL;

  // Map south wall
  for (j = 0; j < SIZE; j++) 
    m->walls[0][j] |= SWALL;

  // Map west wall
  for (i = 0; i < SIZE; i++) 
    m->walls[i][0] |= WWALL;
}

/*
 * Function name: initDist()
 * Description: Initializes floodfill distances for 16 by 16 square maze. 
 *              Lower left square is (0, 0).
 *				Initializes distances starting from goal instead of mouse.
 */
void initDist(MAZE * maze)
{
	int i, j, k;
	if (SIZE%2 == 1) 
	{    // Odd size
	    k = SIZE - 1;
	    for (i = 0; i < SIZE; i++) {      
	      for (j = 0; j < SIZE; j++) {
	      maze->dist[i][j] = k;
	      if (j < SIZE/2)
	        k--;
	      else
	        k++;
	      }
	    }
	    if (i < SIZE/2)
	      k--;
	    else 
	      k++;
	}
	else if (SIZE%2 == 0) 
	{   // Even size
		k = SIZE - 2;
		for ( i = 0; i < SIZE; i++ ) 
		{
		  for ( j = 0; j < SIZE; j++) {
		    maze->dist[i][j] = k;
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
	}	
}

void readMaze(MAZE * m) 
{
	printf("Enter custom maze: \n");
  int i;
	for(i = SIZE-1; i >= 0; i--)
	{
    int j;
		for(j = 0; j < SIZE; j++)
		{
			int temp = 0;
			scanf("%d", &temp);
			m->input[i][j] = (unsigned char) temp;
		}
	}
}
