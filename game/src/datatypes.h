
struct _TILE
{
	char value; // separate from flags for easier levelgen coding
	int flags;
	int tmp; // used for various levelgen algorithms
	int prev[2];
	int floodFromPlayer;
	int floodFromExit;
	int floodFromEntrance;
};

typedef struct _TILE TILE;

struct _LEVELGENSTACK
{
	int stackSize;
	int stackCurrent;
	int stackMax;
	int* values;
};

typedef struct _LEVELGENSTACK LEVELGENSTACK;

// this is the main game level stage thing
struct _STAGE
{
	var seed;
	int level;
	int size[2];
	int exitPos[2];
	int entrancePos[2];
	TILE* tiles;
	LEVELGENSTACK* workingStack;
};

typedef struct _STAGE STAGE;


