#ifndef __ROBOLIBSIM_H__
#define __ROBOLIBSIM_H__

//robot proxy
#define NORTH 0x10
#define SOUTH 0x20
#define WEST 0x40
#define EAST 0x80

#define D_N     0x10    // North
#define D_S     0x20    // South
#define D_NS    0x30    // North and South
#define D_W     0x40    // West
#define D_NW    0x50    // North and West
#define D_SW    0x60    // South and West
#define D_NSW   0x70    // North, South and West
#define D_E     0x80    // East
#define D_NE    0x90    // North and East
#define D_SE    0xA0    // South and East
#define D_NSE   0xB0    // North, South and East
#define D_WE    0xC0    // West and East
#define D_NWE   0xD0    // North, West and East
#define D_SWE   0xE0    // South, West and East
#define D_NSWE  0xF0    // North, South, West and East
#define ROBOT_FAIL        0x00
#define ROBOT_SUCCESS     0x01
#define ROBOT_TOKENFOUND  0x02
//end of robot proxy definitions

#define STACK_LENGTH 128
#define QUEUE_LENGTH 128
#define MAX_LENGTH 13
#define MAX_WIDTH 7
#define TOKEN_COUNT 3
#define DIST_INFINITY 128
#define STACK_INVALID -1
#define QUEUE_INVALID -1
#define START_X 7
#define START_Y 6
#define NULL 0x00

#define MARK_EAST(POINT) ((POINT).inter |= EAST)
#define MARK_SOUTH(POINT) ((POINT).inter |= SOUTH)
#define MARK_WEST(POINT) ((POINT).inter |= WEST)
#define MARK_NORTH(POINT) ((POINT).inter |= NORTH)

#define HAS_EAST(POINT) ((POINT).inter & EAST)
#define HAS_SOUTH(POINT) ((POINT).inter & SOUTH)
#define HAS_WEST(POINT) ((POINT).inter & WEST)
#define HAS_NORTH(POINT) ((POINT).inter & NORTH)

#define VISITED_EAST(POINT) ((POINT).inter & (EAST>>4))
#define VISITED_SOUTH(POINT) ((POINT).inter & (SOUTH>>4))
#define VISITED_WEST(POINT) ((POINT).inter & (WEST>>4))
#define VISITED_NORTH(POINT) ((POINT).inter & (NORTH>>4))

#define GO_EAST(POINT) ((POINT).inter |= (EAST>>4))
#define GO_SOUTH(POINT) ((POINT).inter |= (SOUTH>>4))
#define GO_WEST(POINT) ((POINT).inter |= (WEST>>4))
#define GO_NORTH(POINT) ((POINT).inter |= (NORTH>>4))

#define IS_OPEN_POINT(POINT) \
			((((POINT).inter & EAST)^(((POINT).inter & (EAST>>4))<<4)) || \
			 (((POINT).inter & SOUTH)^(((POINT).inter & (SOUTH>>4))<<4)) || \
			 (((POINT).inter & WEST)^(((POINT).inter & (WEST>>4))<<4)) || \
			 (((POINT).inter & NORTH)^(((POINT).inter & (NORTH>>4))<<4)))

#define COPY_POINT(SRC, DST) ((DST).x = (SRC).x; \
								(DST).y = (SRC).y; \
								(DST).detected = (SRC).detected; \
								(DST).inter = (SRC).inter; \
								(DST).has_token = (SRC).has_token;)

struct POINT
{
	int x;
	int y;
	int exist;
	int detected;
	int inter;
	int has_token;
};

struct STACK
{
	int x;
	int y;
};

//definition of structs
struct POINT points[MAX_LENGTH][MAX_WIDTH];
struct STACK stack[STACK_LENGTH];
int dist[MAX_LENGTH][MAX_WIDTH];
struct POINT *shortest_path[DIST_INFINITY];
struct POINT *queue[QUEUE_LENGTH];
struct POINT *prev[MAX_LENGTH][MAX_WIDTH];

//definition of functions
int start_finding(int, int);
int aud_move(struct POINT *cur_p, int);
struct POINT *mark_point(int, int, int);
int get_direction(struct POINT *);
int push(struct POINT*);
struct POINT *pop(int);
struct POINT *get_neighbor(struct POINT *, int);
struct POINT *get_last_open_point();
int find_shortest_path(int start_x, int start_y, int end_x, int end_y);
struct POINT *extract_min(int, int);
int get_stack_index(int, int);
int queue_append(struct POINT *);
struct POINT *queue_fetch();
int queue_length();
int get_reverse_dir(int);
int calc_direction(int, int, int, int);

//global variables
int stack_pointer = STACK_INVALID;
int queue_start = QUEUE_INVALID;
int queue_end = QUEUE_INVALID;

#ifdef DEBUG
//for debugging
#define ROBOT_MOVE(x, y) (Robot_Move(START_Y - (y), x - START_X))
int print_direction(struct POINT *cur_p, int);
int print_intersection(int);
int print_point(struct POINT*);
int print_stack();
int print_queue();
int print_path();
#endif

#endif
