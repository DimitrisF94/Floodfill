#include <stdio.h>
#include "maze.h"

void mouser(MAZE * maze, const COORD goal, MOUSE * mouse)
{
	char temp;
	//  0 0 0 0       0 0 0 0
	//    D V T       W S E N
	// [row] [col]
	//  N S   W E
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
		visualizeGrid(maze, *mouse);    // Print after
		printf("CURRENT MOUSE LOCATION: row: %d col: %d\n", (int) mouse->location.row, (int) mouse->location.col );
		printf("CURR DIST TO CENTER: %d\n", (int) maze->dist[mouse->location.row][mouse->location.col]);
		scanf("%c", &temp); // Wait for keypress
	}

	printf("done\n");
}

