/* Graphics */
#define HIDESOUTH 1   // Hide south cell wall when printing
#define HIDEEAST 1    // Hide east cell wall when printing

//Wall info 
#define NWALL 1
#define EWALL 2
#define SWALL 4
#define WWALL 8
#define VISITED 16
#define TRACE 32
#define DE 64

//  0 0 0 0       0 0 0 0
//    D T V       W S E N
// [row] [col]
// [ y ] [ x ]

#define SIZE 16
#define STACKSIZE 512
#define MAX_BUF 8192  // print buffer
#define MAX_DIST 500  // max distance for flood search

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

struct Mouse {
	unsigned char orientation;
	struct Coord;
};

struct Stack {
	struct Coord stack[STACKSIZE];
	int top; //index of last item pushed on stack (if -1, stack is empty)
};

typedef struct Stack STACK; 

void initGrid(struct Maze * maze);
void printGrid(struct Maze * maze);
void readMaze(struct Maze * m);

//Stack functions
//void push(struct Coord c);

//void floodfill(struct Maze * maze);