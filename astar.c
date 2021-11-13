/*
 * Example Input: 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 0
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define N 4
#define DN 0
#define RT 1
#define UP 2
#define LT 3

struct node {
    int direction;
    int board[N+1][N];
    struct node *next;
};

struct node *sort_nodes(struct node *a, struct node *b);
void merge_sort(struct node **list);
void split_list(struct node *list, struct node **a, struct node **b);

void print_a_node(struct node *np);
void print_nodes(struct node *np);
void find_path(struct node *cp, struct node *open, struct node *closed);
int solvable(struct node *pt);
int count_distance(int x, int y, int num);
int count_linear_conflict(struct node *curr_pt);
int check_board_equal(struct node *a, struct node *b);

struct node *start, *goal;
struct node *initialize(int argc, char **argv);
struct node *expand(struct node *curr_pt);
struct node *move(struct node *curr_pt, int a, int b, int x, int y, int dir);
struct node *append(struct node *temp_pt, struct node *succ_pt);
struct node *filter(struct node *succ, struct node *hp);
struct node *merge(struct node *succ, struct node *open);
struct node *goal_found(struct node *curr_pt, struct node *goal_pt);

int main( int argc, char **argv ) {
    int perm;
    struct node *curr_pt, *temp_pt, *open, *closed, *succ;
        
    open = closed = succ = NULL;
    start = initialize(argc, argv);
    perm = solvable(start);
    if(perm & 1) {
        printf("Error: Not Solvable\n");
        return 1;
    }

    int count = 0;

    open = start;
    while(open) {
        curr_pt = open;
        open = open->next;
        curr_pt->next = NULL;

        succ = expand(curr_pt);
        succ = filter(succ, open);
        succ = filter(succ, closed);

        if(succ) {
            open = merge(succ, open);
        }
        closed = append(curr_pt, closed);
        if(curr_pt = goal_found(succ, goal)) {
            printf("Goal Found:\n");
            break;
        }
    }

    find_path(curr_pt, open, closed);    
    
    return 0;
}

void print_a_node(struct node *np) {
    int i, j, x, y;

    for(i = 0; i < N + 1; i++) {
        for(j = 0; j < N; j++) {
            printf("%2d ", np->board[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

void find_path(struct node *cp, struct node *open, struct node *closed) {
    int i, j, temp, index;
    char *solution[500];
    struct node *hp;

    index = 0;
    hp = closed;
    
    for(i = 0; i < N; i++) {
        for(j = 0; j < N; j++) {
            if(cp->board[i][j] == 0) {
                break;
            }
        }

        if(j < N) {
            break;
        }
    }
    
    while(!check_board_equal(cp, start)) {
        if(cp->direction == DN) {
            temp = cp->board[i][j];
            cp->board[i][j] = cp->board[i-1][j];
            cp->board[i-1][j] = temp;
            i--;
            solution[index++] = "DN ";
        }
        else if(cp->direction == RT) {
            temp = cp->board[i][j];
            cp->board[i][j] = cp->board[i][j-1];
            cp->board[i][j-1] = temp;
            j--;
            solution[index++] = "RT ";
        }
        else if(cp->direction == UP) {
            temp = cp->board[i][j];
            cp->board[i][j] = cp->board[i+1][j];
            cp->board[i+1][j] = temp;
            i++;
            solution[index++] = "UP ";
        }
        else if(cp->direction == LT) {
            temp = cp->board[i][j];
            cp->board[i][j] = cp->board[i][j+1];
            cp->board[i][j+1] = temp;
            j++;
            solution[index++] = "LT ";
        }    
    
        while(hp && !check_board_equal(cp, hp)) {
            hp = hp->next;
        }

        cp->direction = hp->direction;
        hp = closed;
    }

    solution[index++] = "\0";

    printf("Solution:\n");
    for(index; index >= 0; index--) {
        printf(solution[index]);
    }
    printf("\n");
}

void print_nodes(struct node *np) {
    while(np) {
        print_a_node(np);
        np = np->next;
    }
}

int solvable(struct node *pt) {
    int i, j, lst[N * N], total;

    total = 0;
    for(i = 0; i < N; i++) {
        for(j = 0; j < N; j++) {
            total += pt->board[i][j];
        }
    }
    return total;
}

int count_distance(int x, int y, int num) {
    int i, j;

    for(i = 0; i < N; i++) {
        for(j = 0; j < N; j++) {
            if(goal->board[i][j] == num) {
                return abs(x - i) + abs(y - j);
            }
        }
    }
}

int count_linear_conflict(struct node *curr_pt) {
	int i, j, k, zflag;
	int conflict = 0;

	//Check horizontal conflict
	for(i = 0; i < N; i++) {
		for(j = 0; j < N; j++) {
			zflag = 0;
			for(k = j + 1; k < N; k++) {
				if(curr_pt->board[i][j] == 0 || curr_pt->board[i][k] == 0) {
					zflag++;
					continue;
				}
				if(curr_pt->board[i][j] == goal->board[i][k]) {
					conflict += k - j - zflag;
				}
			}
		}
	}

	//Check vertical conflict
	for(j = 0; j < N; j++) {
		for(i = 0; i < N; i++) {
			zflag = 0;
			for(k = i + 1; k < N; k++) {
				if(curr_pt->board[i][j] == 0 || curr_pt->board[k][j] == 0) {
					zflag++;
					continue;
				}
				if(curr_pt->board[i][j] == goal->board[k][j]) {
					conflict += k - i - zflag;
				}
			}
		}
	}

	return conflict * 2;
}

int check_board_equal(struct node *a, struct node *b) {
    int i, j;

    for(i = 0; i < N; i++) {
        for(j = 0; j < N; j++) {
            if(a->board[i][j] != b->board[i][j]) {
                return 0; 
            }
        }
    }
    return 1;
}

struct node *initialize(int argc, char **argv) {
    int i, j, sidx, gidx;
    struct node *temp_pt;
    
    temp_pt = (struct node *)malloc(sizeof(struct node));
    sidx = 1;

    for(i = 0; i < N; i++) {
        for(j = 0; j < N; j++) {
            temp_pt->board[i][j] = atoi(argv[sidx++]);
        }
    }

    for(i = 0; i < N; i++) {
        temp_pt->board[N][i] = 0;
    }

    temp_pt->next = NULL;
    start = temp_pt;

    printf("Initial State: \n");
    print_a_node(start);

    temp_pt = (struct node *)malloc(sizeof(struct node));
    gidx = 1;
    
    for(i = 0; i < N; i++) {
        for(j = 0; j < N; j++) {
            temp_pt->board[i][j] = gidx++;
        }
    }
    temp_pt->board[N-1][N-1] = 0;
    for(i = 0; i < N; i++) {
        temp_pt->board[N][i] = 0;
    }
    temp_pt->next = NULL;
    goal = temp_pt;

    printf("Goal State: \n");
    print_a_node(goal);

    return start;  
}

struct node *expand(struct node *curr_pt) {
    int i, j;
    struct node *succ, *temp;
    
    succ = temp = NULL;

    for(i = 0; i < N; i++) {
        for(j = 0; j < N; j++) {
            if(curr_pt->board[i][j] == 0) {
                break;
            }
        }

        if(j < N) {
            break;
        }
    }

    if(i + 1 < N) {
	//printf("down\n");
        temp = move(curr_pt, i, j, i + 1, j, DN);
        temp->next = NULL;
        succ = append(temp, succ);
    }
    if(j + 1 < N) {
        //printf("right\n");
        temp = move(curr_pt, i, j, i, j + 1, RT);
        temp->next = NULL;
        succ = append(temp, succ);
    }
    if(i - 1 < N && i - 1 >= 0) {
        //printf("up\n");
        temp = move(curr_pt, i, j, i - 1, j, UP);
        temp->next = NULL;
        succ = append(temp, succ);
    }
    if(j - 1 < N && j - 1 >= 0) {
        //printf("left\n");
        temp = move(curr_pt, i, j, i, j - 1, LT);
        temp->next = NULL;
        succ = append(temp, succ);
    }
    //printf("expand %d, %d:\n", i, j);
    //print_nodes(succ);
    return succ;
}

struct node *move(struct node *curr_pt, int a, int b, int x, int y, int dir) {
    struct node *new_pt;
    int i, j, temp, f, g, h;

    new_pt = (struct node *)malloc(sizeof(struct node));
    for(i = 0; i < N + 1; i++) {
        for(j = 0; j < N; j++) {
	    	new_pt->board[i][j] = curr_pt->board[i][j];
        }
    }
    
    temp = new_pt->board[a][b];
    new_pt->board[a][b] = new_pt->board[x][y];
    new_pt->board[x][y] = temp;

    g = new_pt->board[N][1] + 1;
    h = 0;
    for(i = 0; i < N; i++) {
        for(j = 0; j < N; j++) {
		    if(new_pt->board[i][j] - goal->board[i][j] != 0) {
                h += count_distance(i, j, new_pt->board[i][j]);
            }
        }
    }
    h += count_linear_conflict(new_pt);
    f = g + h;
    
    new_pt->board[N][0] = f;
    new_pt->board[N][1] = g;
    new_pt->board[N][2] = h;
    new_pt->direction = dir;    
    new_pt->next = NULL;

    return new_pt;
}

struct node *append(struct node *temp_pt, struct node *succ_pt) {
    struct node *curr_pt;

    if(!succ_pt) {
        return temp_pt;
    }
    
    curr_pt = succ_pt;
    
    while(curr_pt->next) {
        curr_pt = curr_pt->next;
    }

    curr_pt->next = temp_pt;
    return succ_pt;
}

struct node *filter(struct node *succ, struct node *hp) {   
    int i, j;
    struct node *lsp, *rsp, *tp, *fp, *temp;
    
    if(!hp) {
        return succ;
    }
    
    fp = (struct node *)malloc(sizeof(struct node));
    fp = NULL;
    lsp = succ;
    tp = hp;

    while(lsp) {   
        tp = hp;
        while(tp && !check_board_equal(lsp, tp)) {
            tp = tp->next;
        }
        if(!tp) {
            temp = (struct node *)malloc(sizeof(struct node));
            for(i = 0; i < N + 1; i++) {
                for(j = 0; j < N; j++) {
                    temp->board[i][j] = lsp->board[i][j];
                }
            }
            temp->next = NULL;
            temp->direction = lsp->direction;
            fp = append(temp, fp);
        }
        lsp = lsp->next;
    }
    return fp;
}

struct node *sort_nodes(struct node *a, struct node *b) {
	struct node *result = NULL;

	if(a == NULL) {
		return b;
	}
	else if(b == NULL) {
		return a;
	}

	if(a->board[N][0] <= b->board[N][0]) {
		result = a;
		result->next = sort_nodes(a->next, b);
	}
	else {
		result = b;
		result->next = sort_nodes(b->next, a);
	}

	return result;
}

void merge_sort(struct node **list) {
	struct node *head = *list;
	struct node *a, *b;

	if(head == NULL || head->next == NULL) {
		return;
	}

	split_list(head, &a, &b);

	merge_sort(&a);
	merge_sort(&b);

	*list = sort_nodes(a, b);
}

void split_list(struct node *list, struct node **a, struct node **b) {
	struct node *slow, *fast;

	slow = list;
	fast = list->next;

	while(fast != NULL) {
		fast = fast->next;
		if(fast != NULL) {
			slow = slow->next;
			fast = fast->next;
		}
	}

	*a = list;
	*b = slow->next;
	slow->next = NULL;
}

struct node *merge(struct node *succ, struct node *open) {
    int i, j;
    struct node *msucc, *mopen, *s_list, *temp;

    s_list = (struct node *)malloc(sizeof(struct node));    
    s_list = NULL;
    msucc = succ;
    merge_sort(&msucc);
    mopen = open;
    merge_sort(&mopen);
    
    while(msucc && mopen) {
        if(msucc->board[N][0] < mopen->board[N][0]) {
            temp = (struct node *)malloc(sizeof(struct node));
            for(i = 0; i < N + 1; i++) {
                for(j = 0; j < N; j++) {
                    temp->board[i][j] = msucc->board[i][j];
                }
            }
            temp->next = NULL;
            temp->direction = msucc->direction;
            s_list = append(temp, s_list);
            msucc = msucc->next;
    	}
        else {
            temp = (struct node *)malloc(sizeof(struct node));
            for(i = 0; i < N + 1; i++) {
                for(j = 0; j < N; j++) {
                    temp->board[i][j] = mopen->board[i][j];
                }
            }
            temp->next = NULL;
            temp->direction = mopen->direction;
            s_list = append(temp, s_list);
            mopen = mopen->next;
        }
    }
    
    if(msucc) {
        if(s_list) {
            s_list = append(msucc, s_list);
        }
        else {
            s_list = msucc;
        }
    }
    else {
        if(s_list) {
            s_list = append(mopen, s_list);
        }
        else {
            s_list = mopen;
        }
    }

    return s_list;
}

struct node *goal_found(struct node *curr_pt, struct node *goal_pt) {
    while(curr_pt) {
        if(check_board_equal(curr_pt, goal_pt)) {
		    return curr_pt;
        }

		curr_pt = curr_pt->next;
    }
    return NULL;
}
