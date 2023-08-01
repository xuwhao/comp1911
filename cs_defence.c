// Program name
//
// This program was written by [your name] (z5555555)
// on [date]
//
// TODO: Description of program

#include <stdio.h>

#define MAP_ROWS 6
#define MAP_COLUMNS 12

////////////////////////////////////////////////////////////////////////////////
/////////////////////////// USER DEFINED TYPES  ////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
enum land_type {
    GRASS,
    WATER,
    PATH_START,
    PATH_END,
    PATH_UP,
    PATH_RIGHT,
    PATH_DOWN,
    PATH_LEFT,
    TELEPORTER
};

enum entity {
    EMPTY,
    ENEMY,
    BASIC_TOWER,
    POWER_TOWER,
    FORTIFIED_TOWER,
};

struct tile {
    enum land_type land;
    enum entity entity;

    int n_enemies;
};

////////////////////////////////////////////////////////////////////////////////
////////////////////////  YOUR FUNCTION PROTOTYPE  /////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// TODO: Put your function prototypes here
void read_basic_info(int* lives, int* money, int* start_x, int* start_y, int* end_x, int* end_y);
int valid_point(int row, int column);
void read_lake(struct tile map[MAP_ROWS][MAP_COLUMNS], int start_x, int start_y, int end_x, int end_y);
void read_path(struct tile map[MAP_ROWS][MAP_COLUMNS], int start_x, int start_y, int end_x, int end_y);
int valid_path(char ch);
void add_enemies(struct tile map[MAP_ROWS][MAP_COLUMNS], int start_x, int start_y);
void add_tower(struct tile map[MAP_ROWS][MAP_COLUMNS], int* money);

////////////////////////////////////////////////////////////////////////////////
////////////////////// PROVIDED FUNCTION PROTOTYPE  ////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void initialise_map(struct tile map[MAP_ROWS][MAP_COLUMNS]);
void print_map(struct tile map[MAP_ROWS][MAP_COLUMNS], int lives, int money);
void print_tile(struct tile tile, int entity_print);


int main(void) {
    // This `map` variable is a 2D array of `struct tile`s.
    // It is `MAP_ROWS` x `MAP_COLUMNS` in size (which is 6x12 for this
    // assignment!)
    struct tile map[MAP_ROWS][MAP_COLUMNS];

    // This will initialise all tiles in the map to have GRASS land and EMPTY
    // entity values.
    initialise_map(map);
    
    // TODO: Start writing code here!

    // TODO: Stage 1.1 - Scan in lives, money and start/ending points, then
    // print out the map!
    
    // set basic info
    int lives = -1, money = -1, start_x = -1, start_y = -1, end_x = -1, end_y = -1;
    read_basic_info(&lives, &money, &start_x, &start_y, &end_x, &end_y);
    // int is_basic_info_read = 1;
    if (lives == -1 || money == -1 || start_x == -1 || start_y == -1 || end_x == -1 || end_y == -1){
        // is_basic_info_read = 0;
    }else{
        map[start_x][start_y].land = PATH_START;
        map[end_x][end_y].land = PATH_END;
    }
    print_map(map, lives, money);

    // TODO: Stage 1.2 - Scan in the initial enemies. Make sure you change the
    // `entity` at the starting position to be ENEMY, and that you update the
    // `n_enemies` value at that position to be this scanned value!
    
    // set enemies
    
    printf("Initial Enemies: ");
    add_enemies(map, start_x, start_y);
    print_map(map, lives, money);

    // stage 1.3
    read_lake(map, start_x, start_y, end_x, end_y);
    print_map(map, lives, money);

    // stage 2.1
    read_path(map, start_x, start_y, end_x, end_y);
    print_map(map, lives, money);

    // command loop
    char cmd;
    while(1){
        printf("Enter Command: ");
        int res = scanf(" %c", &cmd);
        if (res == EOF){
            printf("\nGame Over!\n");
            break;
        }
        switch (cmd)
        {
        case 'e':
            add_enemies(map, start_x, start_y);
            break;
        case 't':
            add_tower(map, &money);
            break;
        // case '':
        //     /* code */
        //     break;
        default:
            break;
        }
        print_map(map, lives, money);
    }
}

////////////////////////////////////////////////////////////////////////////////
/////////////////////////////  YOUR FUNCTIONS //////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// TODO: Put your functions here
void read_basic_info(int* lives, int* money, int* start_x, int* start_y, int* end_x, int* end_y){
    int i, j, k ,t;

    printf("Starting Lives: ");
    scanf(" %d", &i);

    printf("Starting Money($): ");
    scanf(" %d", &j);

    if (i < 0 || j < 0){
        return;
    }
    *lives = i;
    *money = j;

    printf("Start Point: ");
    scanf(" %d %d", &i, &j);

    printf("End Point: ");
    scanf(" %d %d", &k, &t);

    if ((i==k && j==t) || !valid_point(i, j) || !valid_point(k, t)){
        return;
    }

    *start_x = i;
    *start_y = j;
    *end_x = k;
    *end_y = t;
}

int valid_point(int row, int column) {
    return row >= 0 && column >= 0 && row < MAP_ROWS && column < MAP_COLUMNS;
}

void read_lake(struct tile map[MAP_ROWS][MAP_COLUMNS], int start_x, int start_y, int end_x, int end_y){
    int lake_row, lake_col, height, width;
    printf("Enter Lake: ");
    scanf(" %d %d %d %d", &lake_row, &lake_col, &height, &width);

    if (!valid_point(lake_row, lake_col) || !valid_point(lake_row + height - 1, lake_col + width - 1) 
    || (height == 0 && width == 0)
    || (lake_row <= start_x && start_x <= lake_row + height && lake_col <= start_y && start_y < lake_col + width)
    || (lake_row <= end_x && end_x <= lake_row + height && lake_col <= end_y && end_y < lake_col + width)){
        printf("Error: Lake out of bounds, ignoring...\n");
        return;
    }

    for(int i = lake_row; i < lake_row + height; i++){
        for(int j = lake_col; j < lake_col + width; j++){
            map[i][j].land = WATER;
        }
    }
}

void read_path(struct tile map[MAP_ROWS][MAP_COLUMNS], int start_x, int start_y, int end_x, int end_y){
    printf("Enter Path: ");

    char ch;
    char path[MAP_ROWS * MAP_COLUMNS + 10]; 

    int len = 0;
    int x = start_x, y = start_y;
    char visited[MAP_ROWS][MAP_COLUMNS];
    for(int i = 0; i< MAP_ROWS; i++){
        for(int j = 0; j < MAP_COLUMNS; j++){
            visited[i][j] = 'e';
        }
    }

    while(scanf(" %c", &ch)){
        if (!valid_path(ch) || visited[x][y] != 'e'){
            return;
        }

        visited[x][y] = ch;
        switch (ch)
        {
            case 'u':
                x = x - 1;
                break;
            case 'd':
                x = x + 1;
                break;
            case 'l':
                y = y - 1;
                break;
            case 'r':
                y = y + 1;
                break;
            default:
                break;
        }

        path[len++] = ch;
        
        if (x == end_x && y == end_y){
            break;
        }
    }

    x = start_x;
    y = start_y;
    for(int i = 0; i< len; i++){
        switch (path[i])
        {
            case 'u':
                map[x][y].land = PATH_UP;
                x = x - 1;
                break;
            case 'd':
                map[x][y].land = PATH_DOWN;
                x = x + 1;
                break;
            case 'l':
                map[x][y].land = PATH_LEFT;
                y = y - 1;
                break;
            case 'r':
                map[x][y].land = PATH_RIGHT;
                y = y + 1;
                break;
            default:
                break;
        }
    }
}

int valid_path(char ch){
    return ch == 'r' || ch == 'd' || ch == 'l' || ch == 'u';
}

void add_enemies(struct tile map[MAP_ROWS][MAP_COLUMNS], int start_x, int start_y){
    int enemies = 0;
    scanf(" %d", &enemies);
    if (enemies > 0){
        if (map[start_x][start_y].entity != ENEMY){
            map[start_x][start_y].entity = ENEMY;
            map[start_x][start_y].n_enemies = enemies;
        }else{
            map[start_x][start_y].n_enemies += enemies;
        }
    }

    if (map[start_x][start_y].n_enemies > 999){
        map[start_x][start_y].n_enemies = 999;
    }
}

void add_tower(struct tile map[MAP_ROWS][MAP_COLUMNS], int* money){
    int tr, tc;
    scanf(" %d %d", &tr, &tc);

    if (*money >= 200 && valid_point(tr, tc) && map[tr][tc].land == GRASS && map[tr][tc].entity == EMPTY){
        *money -= 200;
        map[tr][tc].entity = BASIC_TOWER;
        printf("Tower successfully created!\n");
    }else{
        printf("Error: Tower creation unsuccessful. Make sure you have at least $200 and that the tower is placed on a grass block with no entity.");
    }
}

////////////////////////////////////////////////////////////////////////////////
/////////////////////////// PROVIDED FUNCTIONS  ///////////////////////////////
////////////////////////////////////////////////////////////////////////////////

/**
 * Initialises map tiles to contain GRASS land and EMPTY entity.
 * 
 * Parameters:
 *     map - The map to initialise.
 * Returns:
 *     Nothing.
 */
void initialise_map(struct tile map[MAP_ROWS][MAP_COLUMNS]) {
    for (int row = 0; row < MAP_ROWS; ++row) {
        for (int col = 0; col < MAP_COLUMNS; ++col) {
            map[row][col].land = GRASS;
            map[row][col].entity = EMPTY;
            map[row][col].n_enemies = 0;
        }
    }
}

/**
 * Prints all map tiles based on their value, with a header displaying lives
 * and money.
 * 
 * Parameters:
 *     map   - The map to print tiles from.
 *     lives - The number of lives to print with the map.
 *     money - The amount of money to print with the map.
 * Returns:
 *     Nothing.
 */
void print_map(struct tile map[MAP_ROWS][MAP_COLUMNS], int lives, int money) {
    printf("\nLives: %d Money: $%d\n", lives, money);
    for (int row = 0; row < MAP_ROWS * 2; ++row) {
        for (int col = 0; col < MAP_COLUMNS; ++col) {
            print_tile(map[row / 2][col], row % 2);
        }
        printf("\n");
    }
}

/**
 * Prints either the land or entity component of a single tile, based on
 * the `land_print` parameter;
 * 
 * Parameters:
 *     tile         - The tile to print the land/entity from
 *     land_print - Whether to print the land part of the tile or the entity
 *         part of the tile. If this value is 0, it prints the land, otherwise
 *         it prints the entity.
 * Returns:
 *     Nothing.
 */
void print_tile(struct tile tile, int land_print) {
    if (land_print) {
        if (tile.land == GRASS) {
            printf(" . ");
        } else if (tile.land == WATER) {
            printf(" ~ ");
        } else if (tile.land == PATH_START) {
            printf(" S ");
        } else if (tile.land == PATH_END) {
            printf(" E ");
        } else if (tile.land == PATH_UP) {
            printf(" ^ ");
        } else if (tile.land == PATH_RIGHT) {
            printf(" > ");
        } else if (tile.land == PATH_DOWN) {
            printf(" v ");
        } else if (tile.land == PATH_LEFT) {
            printf(" < ");
        } else if (tile.land == TELEPORTER) {
            printf("( )");
        } else {
            printf(" ? ");
        }
    } else {
        if (tile.entity == EMPTY) {
            printf("   ");
        } else if (tile.entity == ENEMY) {
            printf("%03d", tile.n_enemies);
        } else if (tile.entity == BASIC_TOWER) {
            printf("[B]");
        } else if (tile.entity == POWER_TOWER) {
            printf("[P]");
        } else if (tile.entity == FORTIFIED_TOWER) {
            printf("[F]");
        } else {
            printf(" ? ");
        }
    }
}