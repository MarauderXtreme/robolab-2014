#include "../h/RobotProxy.h"	//always delete this line for robot!
#ifndef DEBUG
#include "../h/hardware.h"
#endif
#include "../h/RobolabSimClient.h"

//implemetation of functions
int start_finding(int start_x, int start_y)
{
	int inter = 0;
	int token = 0;
	int cur_x = start_x, cur_y = start_y;
	int dir = SOUTH;
	int ppath = -1;
	int npop = 0;	//how many points should be poped
	struct POINT *cur_p = NULL;
	struct POINT *tmp_p = NULL;
	int ret = 0;

	#ifdef DEBUG
	inter = Robot_GetIntersections();
	#else
	inter = get_intersection(get_reverse_dir(dir));
	#endif

	cur_p = mark_point(cur_x, cur_y, inter);
	dir = get_direction(cur_p);

	#ifdef DEBUG
	printf("start point: ");
	print_point(cur_p);
	printf("\n");
	#endif

	while(token < TOKEN_COUNT)
	{
		#ifdef DEBUG
		inter = Robot_GetIntersections();
		print_intersection(inter);
		#endif

		cur_p = mark_point(cur_x, cur_y, inter);
		push(cur_p);
		//print_stack();

		if(dir = get_direction(cur_p))
		{
			//update current point
			switch(dir)
			{
				case EAST:
					cur_x += 1;
					break;
				case SOUTH:
					cur_y -= 1;
					break;
				case WEST:
					cur_x -= 1;
					break;
				case NORTH:
					cur_y += 1;
					break;
			}

			#ifdef DEBUG
			print_direction(cur_p, dir);
			ret = aud_move(cur_p, dir);
			#else
			//move one step
			display_clear(0);
			display_goto_xy(0, 0);
			display_int(cur_p->x, 2);
			display_goto_xy(3, 0);
			display_int(cur_p->y, 2);
			display_goto_xy(7, 0);
			display_int(cur_x, 2);
			display_goto_xy(10, 0);
			display_int(cur_y, 2);
			display_goto_xy(0, 1);
			display_int(g_dir, 3);
			display_goto_xy(5, 1);
			display_int(dir, 3);
			display_goto_xy(0, 2);
			display_int(cur_p->inter&0xF0, 3);
			display_update();

			ret = move(cur_x, cur_y, get_reverse_dir(dir));
			#endif

			#ifdef DEBUG
			inter = Robot_GetIntersections();
			#else
			inter = get_intersection(get_reverse_dir(dir));
			#endif

			cur_p = mark_point(cur_x, cur_y, inter);
			#ifdef DEBUG
			print_point(cur_p);
			#endif

			if(ret == ROBOT_SUCCESS)
			{
				#ifndef DEBUG
				beep2();
				#endif
			}
			else if(ret == ROBOT_TOKENFOUND)
			{
				tmp_p = &points[cur_x][cur_y];
				if(tmp_p->has_token == 0)
				{
					tmp_p->has_token = 1;
					token++;
					#ifdef DEBUG
					printf("[%d. token]\n", token);
					#endif
				}
				else
				{
					#ifdef DEBUG
					printf("[not a new token]\n");
					#endif
				}

				if(token == TOKEN_COUNT)
				{
					//all token were found, go back to start point
					#ifdef DEBUG
					printf("going back to start point......\n");
					#endif
					push(cur_p);
					ppath = find_shortest_path(cur_p->x, cur_p->y, START_X, START_Y);
					if(ppath)
					{
						//going back to last open point
						ppath--;

						while(ppath >= 0)
						{
							tmp_p = shortest_path[ppath];
							dir = calc_direction(cur_p->x, cur_p->y, tmp_p->x, tmp_p->y);
							#ifdef DEBUG
							print_point(tmp_p);
							printf("\n");
							ROBOT_MOVE(tmp_p->x, tmp_p->y);
							#else
							display_clear(0);
							display_goto_xy(0, 0);
							display_int(cur_p->x, 2);
							display_goto_xy(3, 0);
							display_int(cur_p->y, 2);
							display_goto_xy(7, 0);
							display_int(tmp_p->x, 2);
							display_goto_xy(10, 0);
							display_int(tmp_p->y, 2);
							display_goto_xy(0, 1);
							display_int(g_dir, 3);
							display_goto_xy(5, 1);
							display_int(dir, 3);
							display_goto_xy(0, 2);
							display_int(cur_p->inter&0xF0, 3);
							display_update();

							move(tmp_p->x, tmp_p->y, get_reverse_dir(dir));
							#endif
							cur_p = tmp_p;
							ppath--;
						}

						//delete the path in stack
						pop(npop + 1);
						cur_p = tmp_p;
						cur_x = cur_p->x;
						cur_y = cur_p->y;
					}
					#ifdef DEBUG
					printf("task finished!\n");
					#endif

					break;
				}
			}
			else
			{
				#ifdef DEBUG
				printf("move failed!\n");
				#endif
			}
		}
		else
		{
			//there is no ways forward, go back to nearest point
			tmp_p = get_last_open_point();
			npop = stack_pointer - get_stack_index(tmp_p->x, tmp_p->y);
			#ifdef DEBUG
			printf("going back to (%d, %d)\n", tmp_p->x, tmp_p->y);
			#endif
			ppath = find_shortest_path(cur_p->x, cur_p->y, tmp_p->x, tmp_p->y);

			if(ppath)
			{
				//going back to last open point
				ppath--;

				while(ppath >= 0)
				{
					tmp_p = shortest_path[ppath];
					dir = calc_direction(cur_p->x, cur_p->y, tmp_p->x, tmp_p->y);
					#ifdef DEBUG
					ROBOT_MOVE(tmp_p->x, tmp_p->y);
					#else
					display_clear(0);
					display_goto_xy(0, 0);
					display_int(cur_p->x, 2);
					display_goto_xy(3, 0);
					display_int(cur_p->y, 2);
					display_goto_xy(7, 0);
					display_int(tmp_p->x, 2);
					display_goto_xy(10, 0);
					display_int(tmp_p->y, 2);
					display_goto_xy(0, 1);
					display_int(g_dir, 3);
					display_goto_xy(5, 1);
					display_int(dir, 3);
					display_goto_xy(0, 2);
					display_int(cur_p->inter&0xF0, 3);
					display_update();

					move(tmp_p->x, tmp_p->y, get_reverse_dir(dir));
					#endif
					cur_p = tmp_p;
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
				#ifdef DEBUG
				printf("task ended without enough token were found.\n");
				#endif
				break;
			}
		}
		#ifdef DEBUG
		printf("\n");
		#endif
	}

	return 0;
}

int aud_move(struct POINT *cur_p, int dir)
{
	int ret = 0;
	struct POINT *neighbor = NULL;

	switch(dir)
	{
		case EAST:
			neighbor = get_neighbor(cur_p, dir);
			if(neighbor)
			{
				GO_EAST(*cur_p);
				GO_WEST(*neighbor);
			}
			#ifdef DEBUG
			ret = ROBOT_MOVE(cur_p->x + 1, cur_p->y);
			#else
			move(cur_p->x + 1, cur_p->y, dir);
			#endif
			break;
		case SOUTH:
			neighbor = get_neighbor(cur_p, dir);
			if(neighbor)
			{
				GO_SOUTH(*cur_p);
				GO_NORTH(*neighbor);
			}
			#ifdef DEBUG
			ret = ROBOT_MOVE(cur_p->x, cur_p->y - 1);
			#else
			move(cur_p->x, cur_p->y - 1, dir);
			#endif
			break;
		case WEST:
			neighbor = get_neighbor(cur_p, dir);
			if(neighbor)
			{
				GO_WEST(*cur_p);
				GO_EAST(*neighbor);
			}
			#ifdef DEBUG
			ret = ROBOT_MOVE(cur_p->x - 1, cur_p->y);
			#else
			move(cur_p->x - 1, cur_p->y, dir);
			#endif
			break;
		case NORTH:
			neighbor = get_neighbor(cur_p, dir);
			if(neighbor)
			{
				GO_NORTH(*cur_p);
				GO_SOUTH(*neighbor);
			}
			#ifdef DEBUG
			ret = ROBOT_MOVE(cur_p->x, cur_p->y + 1);
			#else
			move(cur_p->x, cur_p->y + 1, dir);
			#endif
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
	points[x][y].exist = 1;
	points[x][y].detected = 1;
	points[x][y].x = x;
	points[x][y].y = y;

	if(inter & EAST)
	{
		MARK_EAST(points[x][y]);
		MARK_WEST(points[x + 1][y]);
		points[x + 1][y].x = x + 1;
		points[x + 1][y].y = y;
		points[x + 1][y].exist = 1;
		if(points[x + 1][y].detected)
		{
			GO_EAST(points[x][y]);
			GO_WEST(points[x + 1][y]);
		}
	}

	if(inter & SOUTH)
	{
		MARK_SOUTH(points[x][y]);
		MARK_NORTH(points[x][y - 1]);
		points[x][y - 1].x = x;
		points[x][y - 1].y = y - 1;
		points[x][y - 1].exist = 1;
		if(points[x][y - 1].detected)
		{
			GO_SOUTH(points[x][y]);
			GO_NORTH(points[x][y - 1]);
		}
	}

	if(inter & WEST)
	{
		MARK_WEST(points[x][y]);
		MARK_EAST(points[x - 1][y]);
		points[x - 1][y].x = x - 1;
		points[x - 1][y].y = y;
		points[x - 1][y].exist = 1;
		if(points[x - 1][y].detected)
		{
			GO_WEST(points[x][y]);
			GO_EAST(points[x - 1][y]);
		}
	}

	if(inter & NORTH)
	{
		MARK_NORTH(points[x][y]);
		MARK_SOUTH(points[x][y + 1]);
		points[x][y + 1].x = x;
		points[x][y + 1].y = y + 1;
		points[x][y + 1].exist = 1;
		if(points[x][y + 1].detected)
		{
			GO_NORTH(points[x][y]);
			GO_SOUTH(points[x][y + 1]);
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
	struct POINT *neighbor = NULL;

	switch(dir)
	{
		case EAST:
			if((p->x + 1) < MAX_LENGTH)
				neighbor = &points[p->x + 1][p->y];
			break;
		case SOUTH:
			if(p->y > 0)
				neighbor = &points[p->x][p->y - 1];
			break;
		case WEST:
			if(p->x > 0)
				neighbor = &points[p->x - 1][p->y];
			break;
		case NORTH:
			if((p->y + 1) < MAX_WIDTH)
				neighbor = &points[p->x][p->y + 1];
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
	struct STACK *s = NULL;

	while(sp >= 0)
	{
		s = &stack[sp];
		p = &points[s->x][s->y];
		if(HAS_EAST(*p))
		{
			if(points[p->x + 1][p->y].detected == 0)
				break;
		}
		if(HAS_SOUTH(*p))
		{
			if(points[p->x][p->y - 1].detected == 0)
				break;
		}
		if(HAS_WEST(*p))
		{
			if(points[p->x - 1][p->y].detected == 0)
				break;
		}
		if(HAS_NORTH(*p))
		{
			if(points[p->x][p->y + 1].detected == 0)
				break;
		}
		sp--;
	}

	//if there is no any open point in stack, return the point in stack[0]
	return p;
}

//return the count of points in shortest path stack
int find_shortest_path(int start_x, int start_y, int end_x, int end_y)
{
	int x = INVALID, y = INVALID;
	int start_index = STACK_INVALID, end_index = STACK_INVALID;
	int sp = STACK_INVALID;	//pointer of global stack
	int ppath = STACK_INVALID;	//pointer of shortest path stack
	struct POINT *p = NULL;
	struct POINT *target_p = NULL;
	struct STACK *s = NULL;

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

	//empty global queue
	queue_start = QUEUE_INVALID;
	queue_end = QUEUE_INVALID;
	//start from top of points' stack
	p = &points[start_x][start_y];
	queue_append(p);

	//end_index = get_stack_index(end_x, end_y);
	//target_p = &points[end_x][end_y];
	do
	{
		//p = extract_min(sp, end_index);
		p = queue_fetch();
		if(p)
		{
			if(HAS_EAST(*p))
			{
				x = p->x + 1;
				y = p->y;
				if(points[x][y].exist && (dist[x][y] == DIST_INFINITY))
					queue_append(&points[x][y]);
				if(dist[x][y] > (dist[p->x][p->y] + 1))
				{
					dist[x][y] = dist[p->x][p->y] + 1;
					prev[x][y] = &points[p->x][p->y];
				}
			}
			if(HAS_SOUTH(*p))
			{
				x = p->x;
				y = p->y - 1;
				if(points[x][y].exist && (dist[x][y] == DIST_INFINITY))
					queue_append(&points[x][y]);
				if(dist[x][y] > (dist[p->x][p->y] + 1))
				{
					dist[x][y] = dist[p->x][p->y] + 1;
					prev[x][y] = &points[p->x][p->y];
				}
			}
			if(HAS_WEST(*p))
			{
				x = p->x - 1;
				y = p->y;
				if(points[x][y].exist && (dist[x][y] == DIST_INFINITY))
					queue_append(&points[x][y]);
				if(dist[x][y] > (dist[p->x][p->y] + 1))
				{
					dist[x][y] = dist[p->x][p->y] + 1;
					prev[x][y] = &points[p->x][p->y];
				}
			}
			if(HAS_NORTH(*p))
			{
				x = p->x;
				y = p->y + 1;
				if(points[x][y].exist && (dist[x][y] == DIST_INFINITY))
					queue_append(&points[x][y]);
				if(dist[x][y] > (dist[p->x][p->y] + 1))
				{
					dist[x][y] = dist[p->x][p->y] + 1;
					prev[x][y] = &points[p->x][p->y];
				}
			}

			//sp--;
			//s = &stack[sp];
		}
	}while(queue_length());

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
	int sp = INVALID;

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
	int i = INVALID;
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

int queue_append(struct POINT *p)
{
	if(p)
	{
		queue_end++;
		queue[queue_end] = p;

		if(queue_start == QUEUE_INVALID)
			queue_start++;
	}

	return 0;
}

struct POINT *queue_fetch()
{
	struct POINT *p = NULL;

	if(queue_start != QUEUE_INVALID)
	{
		p = queue[queue_start];
		queue_start++;

		if(queue_end < queue_start)
		{
			queue_start = QUEUE_INVALID;
			queue_end = QUEUE_INVALID;
		}
	}

	return p;
}

int queue_length()
{
	return (queue_start == -1)?0:(queue_end - queue_start + 1);
}

int get_reverse_dir(int dir)
{
	if(dir == EAST)
		return WEST;
	else if(dir == WEST)
		return EAST;
	else if(dir == SOUTH)
		return NORTH;
	else if(dir == NORTH)
		return SOUTH;

	return 0;
}

int calc_direction(int start_x, int end_x, int start_y, int end_y)
{
	if((start_x - end_x) == -1)
		return EAST;
	else if((start_y - end_y) == 1)
		return SOUTH;
	else if((start_x - end_x) == 1)
		return WEST;
	else if((start_y - end_y) == -1)
		return NORTH;

	return 0;
}

#ifdef DEBUG
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

int print_queue()
{
	int qp;

	printf("Queue: ");
	for(qp = queue_start;qp <= queue_end;qp++)
		printf("(%d, %d) ", queue[qp]->x, queue[qp]->y);
	printf("\n");

	return 0;

}

int print_path()
{
}

int main(void) {
	start_finding(START_X, START_Y);

	return 0;
}
#endif
