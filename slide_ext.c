// Slide
// slide.c
//
// This program was written by Zihang Xu (z586348)
// on 13-03-2021
//
// Version 1.0.0 (2021-03-08): Initial Assignment Release

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define SIZE 15
#define EMPTY 0
#define STONE 1
#define NIGHT 2
// TODO: Add any extra #defines here.

void print_map(int map[NIGHT][SIZE][SIZE], int laser_y, int n_realm);
void place_block(int row, int col, int block, int block_number, int map[NIGHT][SIZE][SIZE]);
void shift_map(int map[NIGHT][SIZE][SIZE], int n_realm);
void rotate_map(int map[NIGHT][SIZE][SIZE], int rotation_direction, int n_realm);
void tnt_explode(int map[NIGHT][SIZE][SIZE], int laser_y, int tnt_col, int n_realm);
void move_laser(int *laser_y, int *direction);
void fire_laser(int map[NIGHT][SIZE][SIZE], int laser_y, int n_realm);
void split_laser(int map[NIGHT][SIZE][SIZE], int power, int row, int col, int n_realm);
void realm_change(int map[NIGHT][SIZE][SIZE], int *n_realm);
int check_col(int map[NIGHT][SIZE][SIZE], int col_check, int n_realm);
int check_map(int map[NIGHT][SIZE][SIZE]);
int input_check(int row, int col);
int check_night(int map[NIGHT][SIZE][SIZE]);

// TODO: Add any extra function prototypes here.

int main (void) {
    // This line creates our 2D array called "map" and sets all
    // of the blocks in the map to EMPTY.
    int map[NIGHT][SIZE][SIZE] = {EMPTY};

    // This line creates our laser_y variable. The laser starts in the
    // middle of the map, at position 7.
    int laser_y = SIZE / 2;
    int block_number;
    printf("How many blocks? ");
    // TODO: Scan in the number of blocks.
    scanf("%d", &block_number);

    printf("Enter blocks:\n");
    // TODO: Scan in the blocks.
    int row, col, block;
    scanf("%d %d %d", &row, &col, &block);
    int n_realm = 0;
    place_block(row, col, block, block_number, map);
    print_map(map, laser_y, n_realm);

    // TODO: Scan in commands until EOF.
    // After each command is processed, you should call print_map.
    int instruction;
    int keeplooping = 1;
    int condition = 1; // determine whether the map has been rotated
    while (keeplooping && scanf("%d", &instruction) != EOF) {
         
        if (instruction == 1) {
            int direction;
            scanf("%d", &direction);
            if (laser_y > 0 && laser_y < SIZE - 1) {
                move_laser(&laser_y, &direction);
            } else if (laser_y == 0 && direction == 1) {
                move_laser(&laser_y, &direction);
            } else if (laser_y == SIZE - 1 && direction == -1) {
                move_laser(&laser_y, &direction);
            }
            print_map(map, laser_y, n_realm);
        } else if (instruction == 2) {
            fire_laser(map, laser_y, n_realm); 
            print_map(map, laser_y, n_realm);
            if (check_map(map) == 0) {  
                keeplooping = 0;
                printf("Game Won!\n");
            }
        } else if (instruction == 3) {
            realm_change(map, &n_realm);
            if (check_col(map, 0, n_realm) == 1) {
                keeplooping = 0;
                print_map(map, laser_y, n_realm);
                printf("Game Lost!\n");
            } else {
                shift_map(map, n_realm);
                print_map(map, laser_y, n_realm);
            }
        } else {
            int rotation_direction;
            scanf("%d", &rotation_direction);
            if (instruction == 4 && condition == 1) {                           
                rotate_map(map, rotation_direction, n_realm);
                condition = 0;            
            } else {            
                condition = 0;
            }
            print_map(map, laser_y, n_realm);
        } 
          
        
    }

    return 0;
}

// Print out the contents of the map array. 
// Also print out a > symbol to denote the current laser position.
void print_map(int map[NIGHT][SIZE][SIZE], int laser_y, int n_realm) {
    int i = 0;
    while (i < SIZE) {
        if (i == laser_y) {
            printf("> ");
        } else {
            printf("  ");
        }
        int j = 0;
        while (j < SIZE) {
            printf("%d ", map[n_realm][i][j]);
            j++;
        }
        printf("\n");
        i++;
    }
}

// Place block in the map if the row and col given are in map
void place_block(int row, int col, int block, int block_number, int map[NIGHT][SIZE][SIZE]) {
    int counter = 0;
    if (input_check(row, col) == 1) {
        if (block > 0) {
            map[0][row][col] = block;
        } else { 
            block = abs(block);
            map[1][row][col] = block;
        }
    }
    while (counter < block_number - 1) {
        scanf("%d %d %d", &row, &col, &block); 
        if (input_check(row, col) == 1) {
            if (block > 0) {
                map[0][row][col] = block;
            } else { 
                block = abs(block);
                map[1][row][col] = block;
            }
        }
        counter++;
    }
}

// This function was ment to check if the 0 column was empty
// Later it was used to check if every column was empty
int check_col(int map[NIGHT][SIZE][SIZE], int col_check, int n_realm) {
    int count = 0;
    while (count < SIZE) {
        if (map[n_realm][count][col_check] != EMPTY) {
            return 1;
        }
        count++;
    }
    return 0;
}

// Putting the map into a temp_map except the 0 column
// And then put temp_map back into map
void shift_map(int map[NIGHT][SIZE][SIZE], int n_realm) {
    int temp_map[NIGHT][SIZE][SIZE] = {EMPTY};
    int row_counter = 0;
    int col_counter = 1;
    // Copy the remaining map into temp_map
    while (row_counter < SIZE) {
        col_counter = 1;
        while (col_counter < SIZE) {
            temp_map[n_realm][row_counter][col_counter - 1] 
            = map[n_realm][row_counter][col_counter];
            col_counter++;
        }
        row_counter++;
    }
    // Paste the temp_map back into map
    row_counter = 0;
    col_counter = 0;
    while (row_counter < SIZE) {
        col_counter = 0;
        while (col_counter < SIZE) {
            map[n_realm][row_counter][col_counter] 
            = temp_map[n_realm][row_counter][col_counter];
            col_counter++;
        }
        row_counter++;
    }
}

// Previous check_col function was called several times here to check 
// if the whole map was empty
int check_map(int map[NIGHT][SIZE][SIZE]) {
    int count = 0;
    int realm_count = 0;
    while (realm_count < NIGHT) {
        count = 0;
        while (count < SIZE) {
            if (check_col(map, count, realm_count) == 1) {
                return 1;
            }
            count++;
        }
        realm_count++;
    }
    return 0;
}


// This funtion is using Linear Transformation 
// to rotate every element in the map.
// Take the center of the map, which is map[n_realm][SIZE/2][SIZE/2], as the origin.
// Then rotate, and finally put every element back into the map. 
void rotate_map(int map[NIGHT][SIZE][SIZE], int rotation_direction, int n_realm) {
    int temp[NIGHT][SIZE][SIZE] = {EMPTY};
    int row_count = 0, col_count = 0, x_cdt = 0, y_cdt = 0;
    int center = SIZE / 2;
    if (rotation_direction == 1 || rotation_direction == 2) {
        while (row_count < SIZE) {
            col_count = 0;
            while (col_count < SIZE) {
                x_cdt = col_count - center;
                y_cdt = row_count - center;
                if (rotation_direction == 2) {
                    temp[n_realm][-x_cdt + center][y_cdt + center] 
                    = map[n_realm][row_count][col_count];             
                } else if (rotation_direction == 1) {
                    temp[n_realm][x_cdt + center][-y_cdt + center] 
                    = map[n_realm][row_count][col_count];
                }
                col_count++;
            }
            row_count++;
        }
        row_count = 0;
        col_count = 0;
        while (row_count < SIZE) {
            col_count = 0;
            while (col_count < SIZE) {
                map[n_realm][row_count][col_count] 
                = temp[n_realm][row_count][col_count];
                col_count++;
            }
            row_count++;
        }
    }
}

// Calculate the distance from the tnt block first, 
// if the block was in the blast range empty it.  
void tnt_explode(int map[NIGHT][SIZE][SIZE], int laser_y, int tnt_col, int n_realm) {
    int row_counter = 0, col_counter = 0;
    double distance;
    int radius = map[n_realm][laser_y][tnt_col];
    while (row_counter < SIZE) {
        col_counter = 0;
        while (col_counter < SIZE) {
            distance = (double)(sqrt(pow(row_counter - laser_y, 2) + 
                pow(col_counter - tnt_col, 2)));
            if (distance < radius) {
                map[n_realm][row_counter][col_counter] = EMPTY;
            }
            col_counter++;
        }
        row_counter++;
    }
}

// A help function if the input row and col was inside the map.
int input_check(int row, int col) {
    if (row < 0 || row > SIZE - 1) {
        return 0;
    } else if (col < 0 || col > SIZE - 1) {
        return 0;
    } else {
        return 1;
    }
    return -1;
}

// Using pointer to change the value stored in laser_y.
void move_laser(int *laser_y, int *direction) {
    if (*direction == 1) {
        *laser_y = *laser_y + 1;
    } else if (*direction == -1) {
        *laser_y = *laser_y - 1;
    }
}

// If the laser touches a non tnt block, empty it and add one to counter.
// If it meets a tnt block, an explosion happened and laser_y is not going further.
void fire_laser(int map[NIGHT][SIZE][SIZE], int laser_y, int n_realm) {
    int counter = 0;
    int sign_col = 0;
    int power = 4;
    while (counter < 4 && sign_col < SIZE) {
        if (map[n_realm][laser_y][sign_col] != EMPTY) {
            if (map[n_realm][laser_y][sign_col] > 3 
            && map[n_realm][laser_y][sign_col] < 10
            && map[n_realm][laser_y][sign_col] != 2) {
                int tnt_col = sign_col;
                tnt_explode(map, laser_y, tnt_col, n_realm);
                counter = 4;
            }
            if (map[n_realm][laser_y][sign_col] == 2) {
                map[n_realm][laser_y][sign_col] = EMPTY;
                power = power / 2;
                int row = laser_y, col = sign_col;
                split_laser(map, power, row, col, n_realm);
                counter = 4;
            }
            map[n_realm][laser_y][sign_col] = EMPTY;
            counter = counter + 1;
            power = power - 1;
        }
        sign_col++;
    }
}

int check_night(int map[NIGHT][SIZE][SIZE]) {
    int row = 0;
    int col = 0;
    while (row < SIZE) {
        col = 0;
        while (col < SIZE) {
            if (map[1][row][col] != EMPTY) {
                return 0;
            }
            col++;
        }
        row++;
    }
    return 1;
}

void realm_change(int map[NIGHT][SIZE][SIZE], int *n_realm) {
    if (check_night(map) == 0 || *n_realm == 1) {
        if (*n_realm == 0) {
            *n_realm = 1;
        } else {
            *n_realm = 0;
        }
    }
}

void split_laser(int map[NIGHT][SIZE][SIZE], int power, int laser_y, int sign_col, int n_realm) {
    int strength = power;
    int row_indicator = 1, col_indicator = 0;
    int row = laser_y, col = sign_col;
    // split direction 1
    while (strength >= 1 && input_check(row + row_indicator, col + col_indicator) == 1) {
        if(map[n_realm][row + row_indicator][col + col_indicator] >= 4 
        && map[n_realm][row + row_indicator][col + col_indicator] <= 9) {
            map[n_realm][row + row_indicator][col + col_indicator] = EMPTY;
            strength = 0;
        } else if (map[n_realm][row + row_indicator][col + col_indicator] == 2 && strength > 1) {
            map[n_realm][row + row_indicator][col + col_indicator] = EMPTY;
            row = row + row_indicator, col = col + col_indicator;
            row_indicator = abs(row_indicator - 1);
            col_indicator = abs(col_indicator - 1);
            if (strength > 1) {
                map[n_realm][row + row_indicator][col + col_indicator] = EMPTY;
                map[n_realm][row - row_indicator][col - col_indicator] = EMPTY;
            }
            strength = 0;
        } else {
            map[n_realm][row + row_indicator][col + col_indicator] = EMPTY;
            row = row + row_indicator;
            col = col + col_indicator;
            strength--;
        }
    }
    // split direction 2
    strength = power, row_indicator = 1, col_indicator = 0;
    row = laser_y, col = sign_col;
    while (strength >= 1 && input_check(row - row_indicator, col - col_indicator) == 1) {
        if(map[n_realm][row - row_indicator][col - col_indicator] >= 4 
        && map[n_realm][row - row_indicator][col - col_indicator] <= 9) {
            map[n_realm][row - row_indicator][col - col_indicator] = EMPTY;
            strength = 0;
        } else if (map[n_realm][row - row_indicator][col - col_indicator] == 2) {
            map[n_realm][row - row_indicator][col - col_indicator] = EMPTY;
            row = row - row_indicator, col = col - col_indicator;
            row_indicator = abs(row_indicator - 1);
            col_indicator = abs(col_indicator - 1);
            if (strength > 1) {
                map[n_realm][row + row_indicator][col + col_indicator] = EMPTY;
                map[n_realm][row - row_indicator][col - col_indicator] = EMPTY;
            }
            strength = 0;
        } else {
            map[n_realm][row - row_indicator][col - col_indicator] = EMPTY;
            row = row - row_indicator;
            col = col - col_indicator;
            strength--;
        }
    
    }

}








