/* Graphics */
#define HIDESOUTH 1   // Hide south cell wall when printing
#define HIDEEAST 1    // Hide east cell wall when printing

//Wall info 
#define NWALL 1
#define EWALL 2
#define SWALL 4
#define WWALL 8
#define TRACE 16
#define DE 32
#define VISITED 64

//  0 0 0 0       0 0 0 0
//    D V T       W S E N
// [row] [col]
// [ y ] [ x ]

#define SIZE 16
#define STACKSIZE 512
#define MAX_BUF 8192  // print buffer
#define MAX_DIST 128 // max distance for flood search

struct Maze {
	//Input maze (all walls known)
	unsigned char input [SIZE][SIZE];
	//Holds all walls currently known to mouse 
	unsigned char walls [SIZE][SIZE];
	//Distance of each tile to goal
	unsigned char dist [SIZE][SIZE];
};

struct Coord {
	unsigned char x;
	unsigned char y;
};

struct Stack {
	struct Coord stack[STACKSIZE];
	int top; //index of last item pushed on stack (if -1, stack is empty)
};

struct Mouse {
	unsigned char orientation;
	struct Coord location;
};

typedef struct Maze MAZE;
typedef struct Coord COORD;
typedef struct Stack STACK; 
typedef struct Mouse MOUSE;

void floodfill(MAZE * m, COORD goal, MOUSE * mouse);

void initMaze(MAZE * maze);
void initDist(MAZE * maze, COORD goal);
void printMaze(MAZE * maze);
void readMaze(MAZE * m);

//Stack functions -- stack.c
int push(STACK * s, COORD c);
int pop(STACK * s, COORD c);
int empty(STACK * s);
int full(STACK * s);
COORD top(STACK * s);

//Graphic functions -- graphics.c
int hasNorth(unsigned char c);
int hasEast(unsigned char c);
int hasSouth(unsigned char c);
int hasWest(unsigned char c);
int hasTrace(unsigned char c);
int isDeadEnd(unsigned char c);
int atCenter(MAZE * maze, MOUSE mouse);
int getMin(MAZE * maze, COORD coord);
void printGrid(MAZE * maze);
void visualizeGrid(MAZE * maze, MOUSE mouse);