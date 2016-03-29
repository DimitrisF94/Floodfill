#include <string.h> //strcat
#include <stdio.h> //sprintf
#include "maze.h"


/* Check if object at center, and place pseudo walls accordingly */
/*
int atCenter(MAZE * maze, MOUSE mouse) {
  if ( (((SIZE - 1)/2 == mouse.location.col) || (SIZE/2 == mouse.location.row)) &&
   (((SIZE - 1)/2 == mouse.location.row) || (SIZE/2 == mouse.location.row)) ) {

    // not at southwest
    if ( !((mouse.location.row == (SIZE-1)/2) && (mouse.location.col == (SIZE-1)/2)) ) {
      maze->walls[(SIZE-1)/2][(SIZE-1)/2] |= 12;  // Place pseudo walls on all sides
      // Update adjacent walls
      maze->walls[(SIZE - 1) / 2 - 1][(SIZE - 1) / 2] |= 1;
      maze->walls[(SIZE - 1) / 2][(SIZE - 1) / 2 - 1] |= 2;
    }
    
    // not at northwest
    if ( !((mouse.location.row == SIZE/2) && (mouse.location.col == (SIZE-1)/2)) ) {
      maze->walls[SIZE/2][(SIZE-1)/2] |= 9;     // Place pseudo walls on all sides     
      // Update adjacent walls
      maze->walls[SIZE / 2 + 1][(SIZE - 1) / 2] |= 4;
      maze->walls[SIZE / 2][(SIZE - 1) / 2 - 1] |= 2;
    }
    
    // not at northeast
    if ( !((mouse.location.row == SIZE/2) && (mouse.location.col == SIZE/2)) ) {
      maze->walls[SIZE/2][SIZE/2] |= 3;        // Place pseudo walls on all sides
      // Update adjacent walls
      maze->walls[SIZE / 2 + 1][SIZE / 2] |= 4;
      maze->walls[SIZE / 2][SIZE / 2 + 1] |= 8; 
    }
    
    // not at southeast
    if ( !((mouse.location.row == (SIZE-1)/2) && (mouse.location.col == SIZE/2)) ) {
      maze->walls[(SIZE-1)/2][SIZE/2] |= 4;    // Place pseudo walls on all sides     
      // Update adjacent walls
      maze->walls[(SIZE - 1) / 2 - 1][SIZE / 2] |= 1;
      maze->walls[(SIZE - 1) / 2][SIZE / 2 + 1] |= 8;
    }
    
    return 1;
    
  }
  // not at center
  else     
    return 0;
}
*/

/* 
 * Function name: printGrid()
 * Description: Prints the encoded wall information starting with 
 *              the topmost row and working its w.row down.
 */
 void printGrid(const MAZE * maze) 
 {
  int i;
   for (i = SIZE - 1; i >= 0; i--)
   {
    int j;
     for (j = 0; j < SIZE; j++)
       printf("%3d", (int) maze->walls[i][j]);

    printf("\n\r");
   }
 }

/* 
 * Function name: visualizeGrid()
 * Description: Prints grid .row decoding the value of
 *              each block into a specific wall configuration.
 *              Then prints the mouse's known wall l.rowout.
 */
 void visualizeGrid(const MAZE * maze, const MOUSE mouse) 
 {
  int i;
  //Loop thru all rows  
  for (i = SIZE - 1; i >= 0; i--) 
  {
    int j;
    // Print north cell wall 
    for (j = 0; j < SIZE; j++) 
    {
      if ( hasNorth(maze->walls[i][j]) )
      {
        if (HIDEEAST)
          printf("+---");
        else 
          printf("+---+");
      }
      else
      {
        if (HIDEEAST)
          printf("+   ");
        else
          printf("+   +");
      }
    }

    if (HIDEEAST)
      printf("+");

    printf("\n\r");

    /*
     * Print west and east wall, object, and traces
     */
     for (j = 0; j < SIZE; j++) 
     {
      // Print west wall
      if ( hasWest(maze->walls[i][j]) )
        printf("|");
      else
        printf(" ");
      
      // Print if object present
      if ( i == mouse.location.row && j == mouse.location.col ) {
        if (mouse.orientation == 'N')
          printf("^%02d", (int) maze->dist[i][j]);
        else if (mouse.orientation == 'E')
          printf(">%02d", (int) maze->dist[i][j]);
        else if (mouse.orientation == 'S')
          printf("v%02d", (int) maze->dist[i][j]);
        else if (mouse.orientation == 'W')
          printf("<%02d", (int) maze->dist[i][j]);
      }
      
      // Print markers
      else if ( isDeadEnd(maze->walls[i][j]) )
        printf(" x ");
      else if ( hasTrace(maze->walls[i][j]) )
        printf("*%02d", (int) maze->dist[i][j]);
      else
        printf("%3d", maze->dist[i][j]);
      
      // Opt to print east wall
      if (!HIDEEAST) 
      {
        if ( hasEast(maze->walls[i][j]) )
          printf("|");
        else
          printf(" ");
      }
    }
    
    // Print east boundary if necessary
    if (HIDEEAST)
      printf("|");
    
    printf("\n\r");
    
    // Opt to print south wall
    if (!HIDESOUTH) 
    {
      int j;
      for (j = 0; j < SIZE; j++) 
      {
        if ( hasSouth(maze->walls[i][j]) ) 
        {
          if (HIDEEAST) 
            printf("+---");
          else
            printf("+---+");
        }
        else
        {
          if (HIDEEAST)
            printf("+   ");
          else 
            printf("+   +");
        }
      }
      
      if (HIDEEAST)
        printf("+");
      
      printf("\n\r");
    }
  }
  
  // Print south boundary if necessary
  if(HIDESOUTH) 
  {
    int i;
    for(i = 0; i < SIZE; i++) 
    {
      if(HIDEEAST)
        printf("+---");
      else 
        printf("+---+");     
    }

    if (HIDEEAST)
      printf("+");

    printf("\n\r");
  }
}
