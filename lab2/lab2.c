#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define NORTH 0
#define EAST 1
#define SOUTH 2
#define WEST 3
#define NONMOVE 4
#define DIMENSION 100


void save_room(char OutArray[DIMENSION][DIMENSION], int height, int width);
int get_room_height(FILE *room);
int get_room_width(FILE *room);
int get_iterations(FILE *room);
void load_room(FILE *room, char twoDArray[DIMENSION][DIMENSION], int row, int column);
void iterate_simulation(char twoDArray[DIMENSION][DIMENSION], int row, int column);
int get_zombie_direction(char twoDArray[DIMENSION][DIMENSION], int row, int column, int y, int x);
int get_puppy_direction(char twoDArray[DIMENSION][DIMENSION], int row, int column, int y, int x);
int rand(void);
int get_random_direction();
int get_distance(int x1,int y1,int x2,int y2);

/* NAME: Summer Xue */
/* DATE: Sep 17 */
/* Class: CSE 2421, T/TH 17:20 */

void save_room(char OutArray[DIMENSION][DIMENSION], int height, int width)
{
	FILE *outFile=fopen("results","w");//write the result file
	if(outFile==NULL){
		printf("ERROR");
	}
	int i,j;
	for (i = 0; i < height; i++) {
		//printf("TEST IN ARRAY?\n");
		for (j = 0; j < width; j++) {
			//printf("TEST IN COLUMN?\n");
			fputc(OutArray[i][j], outFile);
		}
		fputc('\n',outFile);
	}
	fclose(outFile);
}
int get_room_height(FILE *room) {//rows	
	int height = 0;
	fscanf(room, "DIMENSIONS: %d", &height);
	return height;
}
int get_room_width(FILE *room) {//columns
	int width = 0;
	fscanf(room, "%d\n", &width);
	return width;
}

int get_iterations(FILE *room) {//iterate times
	int itTime=0;
	fscanf(room, "ITERATIONS: %d\n", &itTime);
	return itTime;
}
//recieves and initializes the room matrix.
void load_room(FILE *room, char twoDArray[DIMENSION][DIMENSION], int row, int column) {
	int i, j;
	for (i = 0; i < row; i++) {
		for (j = 0; j<column; j++){
			char l =fgetc(room);
			twoDArray[i][j] = l;
			
		}
		
		fgetc(room);
	}
	
}
//perform one simulation
void iterate_simulation(char twoDArray[DIMENSION][DIMENSION], int row, int column) {
	int i, j;//location of a puppy
	int move, zombieMove;
	char empty = ' ';
	for (i = 0; i < row; i++) {//puppy move first
		for (j = 0; j < column; j++) {
			if (twoDArray[i][j] == 'p') {//all tired puppy become rested puppy
				twoDArray[i][j] = 'P';
			}
		}
	}
	for (i = 0; i < row; i++) {
		for (j = 0; j < column; j++) {
			if (twoDArray[i][j] == 'P') {
				move = get_puppy_direction(twoDArray, row, column, i, j);
				if(move!=4){
					if (move == 0) {//if move to north
						if (twoDArray[i - 1][j] == empty) {//if the north cell is not occupied
							twoDArray[i - 1][j] = 'p';//the north cell has a tire puppy
							twoDArray[i][j] = ' ';//the old cell has nothing
						}
					}
					else if (move == 1) {//if move to east
						if (twoDArray[i][j + 1] == empty) {//if the east cell is not occupied
							twoDArray[i][j + 1] = 'p';//the east cell has a tire puppy
							twoDArray[i][j] = ' ';//the old cell has nothing
						}
					}
					else if (move == 2) {//if move to south
						if (twoDArray[i + 1][j] == empty) {//if the south cell is not occupied
							twoDArray[i + 1][j] = 'p';//the south cell has a tire puppy
							twoDArray[i][j] = ' ';//the old cell has nothing
						}
					}	
					else if (move == 3) {//if move to west
						if (twoDArray[i][j - 1] == empty) {//if the west cell is not occupied
							twoDArray[i][j - 1] = 'p';//the west cell has a tire puppy
							twoDArray[i][j] = ' ';//the old cell has nothing
						}
					}
				}
			}
		}
	}
	//zombie's turn to move
	for (i = 0; i < row; i++) {
		for (j = 0; j < column; j++) {
			//adjacent to 2 puppies, zombie destroied by puppies
			if (twoDArray[i][j] == 'z' || twoDArray[i][j] == 'Z') {
				int count = 0;
				if (twoDArray[i - 1][j] == 'p' || twoDArray[i - 1][j] == 'P')//north has puppy
					count++;
				if (twoDArray[i][j + 1] == 'p' || twoDArray[i][j + 1] == 'P')//east has puppy
					count++;
				if (twoDArray[i + 1][j] == 'p' || twoDArray[i + 1][j] == 'P')//south has puppy
					count++;
				if (twoDArray[i][j - 1] == 'p' || twoDArray[i][j - 1] == 'P')//west has puppy
					count++;
				if (count > 2 || count == 2)//2 or more puppies
					twoDArray[i][j] = ' ';//remove zombie
			}
		}
	}
	for (i = 0; i < row; i++) {
		for (j = 0; j < column; j++) {
			if (twoDArray[i][j] == 'z') {//all tired zombies become rested zombies
				twoDArray[i][j] = 'Z';
			}
		}
	}
	for (i = 0; i < row; i++) {
		for (j = 0; j < column; j++) {
			if (twoDArray[i][j] == 'Z') {//rested zombies infect adjacent puppy
				if (twoDArray[i - 1][j] == 'p' || twoDArray[i - 1][j] == 'P') {//north has puppy
					twoDArray[i][j] = 'z';
					twoDArray[i - 1][j] = 'z';
				}
				else if (twoDArray[i][j + 1] == 'p' || twoDArray[i][j + 1] == 'P') {//east has puppy
					twoDArray[i][j] = 'z';
					twoDArray[i][j + 1] = 'z';
				}
				else if (twoDArray[i + 1][j] == 'p' || twoDArray[i + 1][j] == 'P') {//south has puppy
					twoDArray[i][j] = 'z';
					twoDArray[i + 1][j] = 'z';
				}
				else if (twoDArray[i][j - 1] == 'p' || twoDArray[i][j - 1] == 'P') {//west has puppy
					twoDArray[i][j] = 'z';
					twoDArray[i][j - 1] = 'z';
				}
			}
		}
	}

	for (i = 0; i < row; i++) {
		for (j = 0; j < column; j++) {
			if (twoDArray[i][j] == 'Z') {
				zombieMove = get_zombie_direction(twoDArray, row, column, i, j);
				if(zombieMove!=4){
					if (zombieMove == 0) {//if move to north
						if (twoDArray[i - 1][j] == ' ') {//if the north cell is not occupied
							twoDArray[i - 1][j] = 'z';//the north cell has a tire puppy
							twoDArray[i][j] = ' ';//the old cell has nothing
						}
					}
					if (zombieMove == 1) {//if move to east
						if (twoDArray[i][j + 1] == ' ') {//if the east cell is not occupied
							twoDArray[i][j + 1] = 'z';//the east cell has a tire puppy
							twoDArray[i][j] = ' ';//the old cell has nothing
						}
					}
					if (zombieMove == 2) {//if move to int get_number_of_puppy_around_zombie(char a[DIMENSION][DIMENSION],int x , int y);south
						if (twoDArray[i + 1][j] == ' ') {//if the south cell is not occupied
							twoDArray[i + 1][j] = 'z';//the south cell has a tire puppy
							twoDArray[i][j] = ' ';//the old cell has nothing
						}
					}
					if (zombieMove == 3) {//if move to west
						if (twoDArray[i][j - 1] == ' ') {//if the west cell is not occupied
							twoDArray[i][j - 1] = 'z';//the west cell has a tire puppy
							twoDArray[i][j] = ' ';//the old cell has nothing
						}
					}
				}
			}
		}
	}
}


int get_zombie_direction(char room[DIMENSION][DIMENSION], int height, int width, int x, int y) {//x, y are zombie location
    
    int i, j;
    int min = 0,count = 0,miniX = 0, miniY = 0;
    
    for (i = 0; i < height; i++) {
        for (j = 0; j < width; j++) {
            if (room[i][j] == 'p' || room[i][j] == 'P') {
                int distance = get_distance(x, y, i, j);//calculate the distance of every pupppy to this zombie
                if (min == 0 || distance < min) {
                    min= distance;//find the nearst zombie
                    count = 1;
                    miniX = i, miniY = j;
                } else if (distance == min) {//two or more nearst zombie
                    count++;
                }
            }
        }
    }
    
    int move;
    if (count > 1) {//no move when there are two nearst puppies
        move = NONMOVE;
    } else if (abs(y - miniY) == abs(x - miniX)) {//no move when the vertical and horizontal distance are the same
        move = NONMOVE;
    } else {//move toward the nearest puppy
        if (abs(y - miniY) < abs(x - miniX)) {//vertical distance is larger, move vertically
            if (x > miniX) {//zombie below puppy,move up
                move = NORTH;
            } else {
                move = SOUTH;
            }
        } else {//horizontal distance is large, move horizontally
            if (y > miniY) {//zombie on the right of puppy, move left
                move = WEST;
            } else {
                move = EAST;
            }
        }
    }
    
    if (rand() % 5) {//4 over 5 random chance
        return get_random_direction();
    } else {
        return move;//didn't pass the 4 over 5 chance, move toward puppy
    }
}

int get_puppy_direction(char a[DIMENSION][DIMENSION], int height, int width, int x, int y) {//x , y are pupy location
    int i, j;
    
    int min = 0, count = 0,miniX = 0, miniY = 0;
    
    for (i = 0; i < height; i++) {
        for (j = 0; j < width; j++) {
            if (a[i][j] == 'z' || a[i][j] == 'Z') {
                int distance = get_distance(x, y, i, j);
                
                if (min == 0|| distance < min) {
                    min = distance;
                    count = 1;
                    
                    miniX= i, miniY = j;
                } else if (distance == min) {
                    count++;
                }
            }
        }
    }
    
    int move;
    if (count > 1) {
        move = NONMOVE;
    } else if (abs(y - miniY) == abs(x - miniX)) {
        move= NONMOVE;
    } else {
        if (abs(y - miniY) < abs(x - miniX)) {
            if (x > miniX) {
                move= SOUTH;
            } else {
                move= NORTH;
            }
        } else {
            if (y > miniY) {
                move= EAST;
            } else {
                move= WEST;
            }
        }
    }
    
    if (rand() % 5 == 0) {//1 over 5 chance random
        return get_random_direction();
    } else {//didn't pass random, puppy move away from zombie
        return move;
    }
}
//calculate zombie puppy distance
int get_distance(int x1,int y1,int x2,int y2){
    int dist=0;
    dist = (x1-x2)*(x1-x2)+(y1-y2)*(y1-y2);
    return dist;
}

int get_random_direction() {
	int num = 0;
	num = (rand() % 4);
	if (num == 0)
		return NORTH;//define macros
	if (num == 1)
		return EAST;
	if (num == 2)
		return SOUTH;
	else
		return WEST;
	
}
//the random function gived to make sure everyone get the same results
int rand(void) {
	static unsigned long int next = 0;
	next = next * 1103515245 + 12345;
	return (unsigned int)(next / 65536) % 32768;
}


int main(int argc, char *argv[]) {
	FILE *inf;
	int width=0, height=0, iteration=1, i;
	char roomArray[DIMENSION][DIMENSION]={{}};
	inf = fopen("sample_in_3", "r");//open room file
	if (inf == NULL) {
		perror("Can't open the file room\n");
		exit(1);
	}
	height = get_room_height(inf);	
	width = get_room_width(inf);	
	iteration = get_iterations(inf);	
	load_room(inf, roomArray, height, width);
	fclose(inf);
	for (i = 0; i < iteration; i++) {//iterat the gave time
		iterate_simulation(roomArray, height, width);
	}
	save_room(roomArray, height, width);
	return 0;
}
