
// flood types
#define FLOOD_EXIT 0
#define FLOOD_ENTRANCE 1
#define FLOOD_PLAYER 2
#define FLOOD_TMP1 3
#define FLOOD_TYPE_MAX 4
#define FLOOD_VALUE_MAX 999999

#define TILE_EMPTY 0

#define TILE_FLAG_EXIT (1<<0)
#define TILE_FLAG_ENTRANCE (1<<1)
#define TILE_FLAG_ENEMYSPAWN (1<<2) // for internal use only, don't spawn stuff in this case
#define TILE_FLAG_TRAP_SPIKES (1<<3)
#define TILE_FLAG_TRAP_HOLE (1<<4)
#define TILE_FLAG_TRAP_TURRET (1<<5)
#define TILE_FLAG_TRAP_BAT (1<<6)

struct _TILE
{
	int value; // separate from flags for easier levelgen coding - @Felix: char type not compatible with code ~~
	int flags;
	int tmp; // used for various levelgen algorithms
	int prev[2];
	int flood[4]; // 4 == FLOOD_TYPE_MAX
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
	int numWalkableTiles;
	TILE* tiles;
	LEVELGENSTACK* workingStack;
	int numEnemies;
	VECTOR *enemyPositions;
	int* enemyData;
	int floodExitMax;
};

typedef struct _STAGE STAGE;

// used by stageFill to create differently looking levels
struct _TEMPLATE
{
	char cfile[32];
	int size[2];
	int *values;
};

typedef struct _TEMPLATE TEMPLATE;

