#include <stdio.h>
#include "maze.h"

void mouser(MAZE * maze, const COORD goal, MOUSE * mouse)
{
	char temp;
	//  0 0 0 0       0 0 0 0
	//    D V T       W S E N
	// [row] [col]
	//  N S   W E
  int moveCount = 0;
	while((mouse->location.row != goal.row) || (mouse->location.col != goal.col))	
	{
		mouse->orientation = floodfill(maze, goal, mouse);

		switch(mouse->orientation)
		{
			case 'N':
				mouse->location.row++;
				break;
			case 'E':
				mouse->location.col++;
				break;
			case 'W':
				mouse->location.col--;
				break;
			case 'S':
				mouse->location.row--;
				break;
			case 'D':
				printf("FOUND GOAL\n");
				break;
			default:
				printf("IDK \n");
				break;
		}
    moveCount++;
		visualizeGrid(maze, *mouse);    // Print after
		printf("CURRENT MOUSE LOCATION: row: %d col: %d\n", (int) mouse->location.row, (int) mouse->location.col );
		printf("CURR DIST TO CENTER: %d\n", (int) maze->dist[mouse->location.row][mouse->location.col]);
		printf("Total Moves: %d\n", moveCount);
		scanf("%c", &temp); // Wait for keypress
	}

  //For return trip
  int i, j;
  for(i = 0; i < SIZE; i++)
    for(j = 0; j < SIZE; j++)
      maze->dist[i][j] = MAX_DIST;

  COORD retGoal = { .row = 0, .col = 0 };
  COORD prev = { .row = 1, .col = 0 };
  maze->dist[retGoal.row][retGoal.col] = 0;
  updateDist(maze, prev, 0);
  visualizeGrid(maze, *mouse);
  moveCount = 0;

  while((mouse->location.row != 0) || (mouse->location.col != 0))
  {
    mouse->orientation = floodfill(maze, retGoal, mouse);

		switch(mouse->orientation)
		{
			case 'N':
				mouse->location.row++;
				break;
			case 'E':
				mouse->location.col++;
				break;
			case 'W':
				mouse->location.col--;
				break;
			case 'S':
				mouse->location.row--;
				break;
			case 'D':
				printf("FOUND GOAL\n");
				break;
			default:
				printf("IDK \n");
				break;
		}
    moveCount++;
		printf("CURRENT MOUSE LOCATION: row: %d col: %d\n", (int) mouse->location.row, (int) mouse->location.col );
		printf("CURR DIST TO CENTER: %d\n", (int) maze->dist[mouse->location.row][mouse->location.col]);
		printf("Total Moves: %d\n", moveCount);
    visualizeGrid(maze, *mouse);
		scanf("%c", &temp); // Wait for keypress
  }
	printf("done\n");
}

