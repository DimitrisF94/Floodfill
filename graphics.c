#include <string.h> //strcat
#include <stdio.h> //sprintf
#include "maze.h"

/* Check for north wall */
int hasNorth(const unsigned char currentBlock) {
  return ((currentBlock & NWALL) == NWALL)? 1 : 0;
}

/* Check for east wall */
int hasEast(const unsigned char currentBlock) {
  return ((currentBlock & EWALL) == EWALL)? 1 : 0;
}

/* Check for south wall */
int hasSouth(const unsigned char currentBlock) {
  return ((currentBlock & SWALL) == SWALL)? 1 : 0;
}

/* Check for west wall */
int hasWest(const unsigned char currentBlock) {
  return ((currentBlock & WWALL) == WWALL)? 1 : 0;
}

/* Check for Trace */
int hasTrace(const unsigned char currentBlock) {
  return ((currentBlock & TRACE) == TRACE)? 1 : 0;
}

/* Check for dead end */
int isDeadEnd(const unsigned char currentBlock) {
  return ((currentBlock & DE) == DE)? 1: 0;
}

/* Check if object at center, and place pseudo walls accordinglcoord.y */
int atCenter(MAZE * maze, MOUSE mouse) {
  if ( (((SIZE - 1)/2 == mouse.location.x) || (SIZE/2 == mouse.location.y)) &&
   (((SIZE - 1)/2 == mouse.location.y) || (SIZE/2 == mouse.location.y)) ) {

    // not at southwest
    if ( !((mouse.location.y == (SIZE-1)/2) && (mouse.location.x == (SIZE-1)/2)) ) {
      maze->walls[(SIZE-1)/2][(SIZE-1)/2] |= 12;  // Place pseudo walls on all sides
      // Update adjacent walls
      maze->walls[(SIZE - 1) / 2 - 1][(SIZE - 1) / 2] |= 1;
      maze->walls[(SIZE - 1) / 2][(SIZE - 1) / 2 - 1] |= 2;
    }
    
    // not at northwest
    if ( !((mouse.location.y == SIZE/2) && (mouse.location.x == (SIZE-1)/2)) ) {
      maze->walls[SIZE/2][(SIZE-1)/2] |= 9;     // Place pseudo walls on all sides     
      // Update adjacent walls
      maze->walls[SIZE / 2 + 1][(SIZE - 1) / 2] |= 4;
      maze->walls[SIZE / 2][(SIZE - 1) / 2 - 1] |= 2;
    }
    
    // not at northeast
    if ( !((mouse.location.y == SIZE/2) && (mouse.location.x == SIZE/2)) ) {
      maze->walls[SIZE/2][SIZE/2] |= 3;        // Place pseudo walls on all sides
      // Update adjacent walls
      maze->walls[SIZE / 2 + 1][SIZE / 2] |= 4;
      maze->walls[SIZE / 2][SIZE / 2 + 1] |= 8; 
    }
    
    // not at southeast
    if ( !((mouse.location.y == (SIZE-1)/2) && (mouse.location.x == SIZE/2)) ) {
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

/* 
 * Return the smallest maze->dist from the surrounding maze->wallss
 * that are not separated bcoord.y a wall
 */
 int getMin(MAZE * maze, COORD coord) 
 {
   int min;
   int distN;
   int distE;
   int distS;
   int distW;
   
   distN = hasNorth(maze->walls[coord.y][coord.x])? MAX_DIST : maze->dist[coord.y + 1][coord.x];
   distE = hasEast(maze->walls[coord.y][coord.x])? MAX_DIST : maze->dist[coord.y][coord.x + 1];
   distS = hasSouth(maze->walls[coord.y][coord.x])? MAX_DIST : maze->dist[coord.y - 1][coord.x];
   distW = hasWest(maze->walls[coord.y][coord.x])? MAX_DIST : maze->dist[coord.y][coord.x - 1];
   
   min = distN;
   min = (distE < min)? distE : min;
   min = (distS < min)? distS : min;
   min = (distW < min)? distW : min;
   
   return min;
 }

/* 
 * Function name: printGrid()
 * Description: Prints the encoded wall information starting with 
 *              the topmost row and working its way down.
 */
 void printGrid(MAZE * maze) 
 {
  char buffer[MAX_BUF] = {0};
  char bufBlock[6] = {0};
  for (int i = SIZE - 1; i >= 0; i--) {
    for (int j = 0; j < SIZE; j++) {
      sprintf(bufBlock, "%3d", maze->walls[i][j]);
      strcat(buffer, bufBlock);
    }
    strcat(buffer, "\n");
  }
  printf("%s", buffer);
}

/* 
 * Function name: visualizeGrid()
 * Description: Prints grid by decoding the value of
 *              each block into a specific wall configuration.
 *              Then prints the mouse's known wall layout.
 */
 void visualizeGrid(MAZE * maze, MOUSE mouse) 
 {
  char buffer[MAX_BUF] = {0};
  char bufBlock[6] = {0};
  
  //Loop thru all rows  
  for (int i = SIZE - 1; i >= 0; i--) 
  {
    // Print north cell wall 
    for (int j = 0; j < SIZE; j++) 
    {
      if ( hasNorth(maze->walls[i][j]) )
      {
        if (HIDEEAST)
          strcat(buffer, "+---");
        else 
          strcat(buffer, "+---+");
      }
      else
      {
        if (HIDEEAST)
          strcat(buffer, "+   ");
        else
          strcat(buffer, "+   +");
      }
    }

    if (HIDEEAST)
      strcat(buffer, "+");

    strcat(buffer, "\n");

    /*
     * Print west and east wall, object, and traces
     */
     for (int j = 0; j < SIZE; j++) 
     {
      // Print west wall
      if ( hasWest(maze->walls[i][j]) )
        strcat(buffer, "|");
      else
        strcat(buffer, " ");
      
      // Print if object present
      if ( i == mouse.location.y && j == mouse.location.x ) {
        if (mouse.orientation == 'N')
          strcat(buffer, " ^ ");
        else if (mouse.orientation == 'E')
          strcat(buffer, " > ");
        else if (mouse.orientation == 'S')
          strcat(buffer, " v ");
        else if (mouse.orientation == 'W')
          strcat(buffer, " < ");
      }
      
      // Print markers
      else if ( isDeadEnd(maze->walls[i][j]) )
        strcat(buffer, " x ");
      else if ( hasTrace(maze->walls[i][j]) ) {
        sprintf(bufBlock, " . ", maze->dist[i][j]);
        strcat(buffer, bufBlock);
      }
      else {
        sprintf(bufBlock, "%3d", maze->dist[i][j]);
        strcat(buffer, bufBlock);
      }
      
      // Opt to print east wall
      if (!HIDEEAST) {
        if ( hasEast(maze->walls[i][j]) )
          strcat(buffer, "|");
        else
          strcat(buffer, " ");
      }
    }
    
    // Print east boundarcoord.y if necessarcoord.y
    if (HIDEEAST) {
      strcat(buffer, "|");
    }
    
    strcat(buffer, "\n");
    
    // Opt to print south wall
    if (!HIDESOUTH) 
    {
      for (int j = 0; j < SIZE; j++) 
      {
        if ( hasSouth(maze->walls[i][j]) ) 
        {
          if (HIDEEAST) 
            strcat(buffer, "+---");
          else
            strcat(buffer, "+---+");
        }
        else
        {
          if (HIDEEAST)
            strcat(buffer, "+   ");
          else 
            strcat(buffer, "+   +");
        }
      }
      
      if (HIDEEAST)
        strcat(buffer, "+");
      
      strcat(buffer, "\n");
    }
  }
  
  // Print south boundary if necessary 
  if(HIDESOUTH) 
  {
    for(int i = 0; i < SIZE; i++) 
    {
      if(HIDEEAST)
        strcat(buffer, "+---");
      else 
        strcat(buffer, "+---+");     
    }

    if (HIDEEAST)
      strcat(buffer, "+");

    strcat(buffer, "\n");
  }
  
  printf("The maze: \n%s", buffer);
}