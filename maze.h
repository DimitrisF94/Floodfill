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
#define MAX_DIST 255 // max distance for flood search

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

//Floodfill functions -- floodfill.c
//Returns a direction to go in
unsigned char floodfill(MAZE * m, COORD goal, MOUSE mouse);
int atCenter(MAZE * maze, MOUSE mouse);
unsigned char getMin(MAZE * maze, COORD coord, unsigned char * direction);

void initMaze(MAZE * maze);
void initDist(MAZE * maze, COORD goal);
void printMaze(MAZE * maze);
void readMaze(MAZE * m);

//Stack functions -- stack.c
int push(STACK * s, const COORD c);
int pop(STACK * s);
int empty(STACK * s);
int full(STACK * s);
COORD top(STACK * s);

//Graphic functions -- graphics.c
unsigned char hasNorth(unsigned char c);
unsigned char hasEast(unsigned char c);
unsigned char hasSouth(unsigned char c);
unsigned char hasWest(unsigned char c);
unsigned char hasTrace(unsigned char c);
unsigned char isDeadEnd(unsigned char c);
void printGrid(MAZE * maze);
void visualizeGrid(MAZE * maze, MOUSE mouse);