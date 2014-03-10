#include "../h/RobotProxy.h"
#include "../h/RobolabSimClient.h"

//implemetation of functions
int start_finding(int start_x, int start_y)
{
	int inter;
	int token = 0;
	int cur_x = start_x, cur_y = start_y;
	int dir;
	int ppath;
	int npop;	//how many points should be poped
	struct POINT *cur_p;
	struct POINT *tmp_p;
	int ret;
	int steps = 0;

	inter = Robot_GetIntersections();
	cur_p = mark_point(cur_x, cur_y, inter);
	printf("start point: ");
	print_point(cur_p);
	printf("\n");

	while(token < TOKEN_COUNT)
	{
		inter = Robot_GetIntersections();
		print_intersection(inter);

		push(cur_p);
		//print_stack();

		if(dir = get_direction(cur_p))
		{
			print_direction(cur_p, dir);

			ret = move(cur_p, dir);
			//update current point
			switch(dir)
			{
				case EAST:
					cur_y += 1;
					break;
				case SOUTH:
					cur_x += 1;
					break;
				case WEST:
					cur_y -= 1;
					break;
				case NORTH:
					cur_x -= 1;
					break;
			}
			inter = Robot_GetIntersections();
			cur_p = mark_point(cur_x, cur_y, inter);
			print_point(cur_p);

			if(ret == ROBOT_SUCCESS)
			{
			}
			else if(ret == ROBOT_TOKENFOUND)
			{
				tmp_p = &points[cur_x][cur_y];
				if(tmp_p->has_token == 0)
				{
					tmp_p->has_token = 1;
					token++;
					printf("[%d. token]\n", token);
				}
				else
				{
					printf("[not a new token]\n");
				}

				if(token == TOKEN_COUNT)
				{
					//all token were found, go back to start point
					printf("going back to start point......\n");
					push(cur_p);
					ppath = find_shortest_path(cur_p->x, cur_p->y, START_X, START_Y);
					if(ppath)
					{
						//going back to last open point
						ppath--;

						while(ppath >= 0)
						{
							tmp_p = shortest_path[ppath];
							print_point(tmp_p);
							printf("\n");
							Robot_Move(tmp_p->x, tmp_p->y);
							ppath--;
						}

						//delete the path in stack
						pop(npop + 1);
						cur_p = tmp_p;
						cur_x = cur_p->x;
						cur_y = cur_p->y;
					}
					printf("task finished!\n");

					break;
				}
			}
			else
			{
				printf("move failed!\n");
			}
		}
		else
		{
			//there is no ways forward, go back to nearest point
			tmp_p = get_last_open_point();
			npop = stack_pointer - get_stack_index(tmp_p->x, tmp_p->y);
			printf("going back to (%d, %d)\n", tmp_p->x, tmp_p->y);
			ppath = find_shortest_path(cur_p->x, cur_p->y, tmp_p->x, tmp_p->y);

			if(ppath)
			{
				//going back to last open point
				ppath--;

				while(ppath >= 0)
				{
					tmp_p = shortest_path[ppath];
					Robot_Move(tmp_p->x, tmp_p->y);
					ppath--;
				}

				//delete the path in stack
				pop(npop + 1);
				cur_p = tmp_p;
				cur_x = cur_p->x;
				cur_y = cur_p->y;
			}
			else
			{
				//was already at every point and back to start point
				//task should be ended
				//that means, not enough token can be found
				printf("task ended without enough token were found.\n");
				break;
			}
		}
		printf("\n");
	}

	return 0;
}

int move(struct POINT *cur_p, int dir)
{
	int ret;
	struct POINT *neighbor;

	switch(dir)
	{
		case EAST:
			neighbor = get_neighbor(cur_p, dir);
			if(neighbor)
			{
				GO_EAST(*cur_p);
				GO_WEST(*neighbor);
			}
			ret = Robot_Move(cur_p->x, cur_p->y + 1);
			break;
		case SOUTH:
			neighbor = get_neighbor(cur_p, dir);
			if(neighbor)
			{
				GO_SOUTH(*cur_p);
				GO_NORTH(*neighbor);
			}
			ret = Robot_Move(cur_p->x + 1, cur_p->y);
			break;
		case WEST:
			neighbor = get_neighbor(cur_p, dir);
			if(neighbor)
			{
				GO_WEST(*cur_p);
				GO_EAST(*neighbor);
			}
			ret = Robot_Move(cur_p->x, cur_p->y - 1);
			break;
		case NORTH:
			neighbor = get_neighbor(cur_p, dir);
			if(neighbor)
			{
				GO_NORTH(*cur_p);
				GO_SOUTH(*neighbor);
			}
			ret = Robot_Move(cur_p->x - 1, cur_p->y);
			break;
		default:
			ret = 0x00;
	}

	return ret;
}

int push(struct POINT *p)
{
	stack_pointer += 1;

	stack[stack_pointer].x = p->x;
	stack[stack_pointer].y = p->y;

	return 0;
}

struct POINT *pop(int count)
{
	if(count == 1)
	{
		if(stack_pointer >= 0)
		{
			stack_pointer--;
			return &points[stack[stack_pointer].x][stack[stack_pointer].y];
		}
	}
	else
	{
		if(count <= (stack_pointer + 1))
		{
			stack_pointer -= count;
		}
	}

	return NULL;
}

struct POINT *mark_point(int x, int y, int inter)
{
	points[x][y].detected = 1;
	points[x][y].x = x;
	points[x][y].y = y;

	if(inter & EAST)
	{
		MARK_EAST(points[x][y]);
		if(points[x][y + 1].detected)
		{
			GO_EAST(points[x][y]);
			GO_WEST(points[x][y + 1]);
		}
	}

	if(inter & SOUTH)
	{
		MARK_SOUTH(points[x][y]);
		if(points[x + 1][y].detected)
		{
			GO_SOUTH(points[x][y]);
			GO_NORTH(points[x + 1][y]);
		}
	}

	if(inter & WEST)
	{
		MARK_WEST(points[x][y]);
		if(points[x][y - 1].detected)
		{
			GO_WEST(points[x][y]);
			GO_EAST(points[x][y - 1]);
		}
	}

	if(inter & NORTH)
	{
		MARK_NORTH(points[x][y]);
		if(points[x - 1][y].detected)
		{
			GO_NORTH(points[x][y]);
			GO_SOUTH(points[x - 1][y]);
		}
	}

	return &points[x][y];
}

int get_direction(struct POINT *p)
{
	if(HAS_EAST(*p) && ((p->inter & (EAST>>4)) == 0))
		return EAST;
	else if(HAS_SOUTH(*p) && ((p->inter & (SOUTH>>4)) == 0))
		return SOUTH;
	else if(HAS_WEST(*p) && ((p->inter & (WEST>>4)) == 0))
		return WEST;
	else if(HAS_NORTH(*p) && ((p->inter & (NORTH>>4)) == 0))
		return NORTH;

	return 0;
}

struct POINT *get_neighbor(struct POINT *p, int dir)
{
	struct POINT *neighbor;

	switch(dir)
	{
		case EAST:
			if((p->y + 1) < MAX_WIDTH)
				neighbor = &points[p->x][p->y + 1];
			break;
		case SOUTH:
			if((p->x + 1) < MAX_LENGTH)
				neighbor = &points[p->x + 1][p->y];
			break;
		case WEST:
			if(p->y > 0)
				neighbor = &points[p->x][p->y - 1];
			break;
		case NORTH:
			if(p->x > 0)
				neighbor = &points[p->x - 1][p->y];
			break;
		default:
			neighbor = NULL;
	}

	return neighbor;
}

struct POINT *get_last_open_point()
{
	int sp = stack_pointer;
	struct POINT *p = NULL;
	struct STACK *s;

	while(sp >= 0)
	{
		s = &stack[sp];
		p = &points[s->x][s->y];
		if(IS_OPEN_POINT(*p))
			break;
		else
			sp--;
	}

	//if there is no any open point in stack, return the point in stack[0]
	return p;
}

//return the count of points in shortest path stack
int find_shortest_path(int start_x, int start_y, int end_x, int end_y)
{
	int x, y;
	int start_index, end_index;
	int sp;	//pointer of global stack
	int ppath;	//pointer of shortest path stack
	struct POINT *p;
	struct POINT *target_p;
	struct STACK *s;

	//init distances to every point
	for(x = 0;x < MAX_LENGTH;x++)
	{
		for(y = 0;y < MAX_WIDTH;y++)
		{
			dist[x][y] = DIST_INFINITY;
			prev[x][y] = NULL;
		}
	}
	dist[start_x][start_y] = 0;

	//start from top of points' stack
	sp = stack_pointer;
	s = &stack[sp];

	end_index = get_stack_index(end_x, end_y);
	target_p = &points[end_x][end_y];
	while(sp >= end_index)
	{
		p = extract_min(sp, end_index);
		if(p)
		{
			if(HAS_EAST(*p) && VISITED_EAST(*p))
			{
				x = p->x;
				y = p->y + 1;
				if(dist[x][y] > (dist[p->x][p->y] + 1))
				{
					dist[x][y] = dist[p->x][p->y] + 1;
					prev[x][y] = &points[p->x][p->y];
				}
			}
			if(HAS_SOUTH(*p) && VISITED_SOUTH(*p))
			{
				x = p->x + 1;
				y = p->y;
				if(dist[x][y] > (dist[p->x][p->y] + 1))
				{
					dist[x][y] = dist[p->x][p->y] + 1;
					prev[x][y] = &points[p->x][p->y];
				}
			}
			if(HAS_WEST(*p) && VISITED_WEST(*p))
			{
				x = p->x;
				y = p->y - 1;
				if(dist[x][y] > (dist[p->x][p->y] + 1))
				{
					dist[x][y] = dist[p->x][p->y] + 1;
					prev[x][y] = &points[p->x][p->y];
				}
			}
			if(HAS_NORTH(*p) && VISITED_NORTH(*p))
			{
				x = p->x - 1;
				y = p->y;
				if(dist[x][y] > (dist[p->x][p->y] + 1))
				{
					dist[x][y] = dist[p->x][p->y] + 1;
					prev[x][y] = &points[p->x][p->y];
				}
			}

			sp--;
			s = &stack[sp];
		}
	}

	//the shortest path to target point was found
	//empty shortest path stack
	//the way is simply set the first element invalid
	ppath = 0;
	shortest_path[ppath] = NULL;

	//make the shortest path
	p = &points[end_x][end_y];
	while((p->x != start_x) || (p->y != start_y))
	{
		shortest_path[ppath] = p;
		ppath++;
		shortest_path[ppath] = NULL;

		//go to previous point
		x = p->x;
		y = p->y;
		p = prev[x][y];
	}

	return ppath;
}

int get_stack_index(int x, int y)
{
	int sp;

	for(sp = stack_pointer;sp >= 0;sp--)
	{
		if((stack[sp].x == x) && (stack[sp].y == y))
			return sp;
	}

	return STACK_INVALID;
}

struct POINT *extract_min(int start, int end)
{
	int min = DIST_INFINITY;
	int i;
	struct POINT *p = NULL;

	for(i = start;i >= end;i--)
	{
		if(min > dist[stack[i].x][stack[i].y])
		{
			min = dist[stack[i].x][stack[i].y];
			p = &points[stack[i].x][stack[i].y];
		}
	}

	return p;
}

////////////////////////////////////////////////////////////////////
//////		debugging functions
////////////////////////////////////////////////////////////////////

int print_direction(struct POINT *cur_p, int dir)
{
	if(dir == EAST)
		printf("go EAST -> ", cur_p->x, cur_p->y);
	else if(dir == SOUTH)
		printf("go SOUTH -> ");
	else if(dir == WEST)
		printf("go WEST -> ");
	else if(dir == NORTH)
		printf("go NORTH -> ");

	return 0;
}

int print_intersection(int inter)
{
	if(inter & EAST)
		printf("EAST ");
	if(inter & SOUTH)
		printf("SOUTH ");
	if(inter & WEST)
		printf("WEST ");
	if(inter & NORTH)
		printf("NORTH ");

	return 0;
}

int print_point(struct POINT *p)
{
	printf("(%d, %d)[%02X, %02X, %02X, %02X]{%d} ", p->x, p->y,
			p->inter & (EAST | (EAST>>4)),
			p->inter & (SOUTH | (SOUTH>>4)),
			p->inter & (WEST | (WEST>>4)),
			p->inter & (NORTH | (NORTH>>4)),
			p->has_token);

	return 0;
}

int print_stack()
{
	int sp;

	printf("Stack: ");
	for(sp = 0;sp <= stack_pointer;sp++)
		printf("(%d, %d) ", stack[sp].x, stack[sp].y);
	printf("\n");

	return 0;
}

int main(void) {
	start_finding(START_X, START_Y);

	return EXIT_SUCCESS;
}
