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
enum land_type
{
    GRASS,
    WATER,
    PATH_END,
    PATH_START,
    PATH_UP,
    PATH_RIGHT,
    PATH_DOWN,
    PATH_LEFT,
    TELEPORTER
};

enum entity
{
    EMPTY,
    ENEMY,
    BASIC_TOWER,
    POWER_TOWER,
    FORTIFIED_TOWER,
};

struct tile
{
    enum land_type land;
    enum entity entity;

    int n_enemies;

    enum land_type path;
    int pair_x;
    int pair_y; 
    int n_enemies_next;
};

////////////////////////////////////////////////////////////////////////////////
////////////////////////  YOUR FUNCTION PROTOTYPE  /////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// TODO: Put your function prototypes here
void read_basic_info(int *lives, int *money, int *start_x, int *start_y, int *end_x, int *end_y);
int valid_point(int row, int column);
void read_lake(struct tile map[MAP_ROWS][MAP_COLUMNS], int start_x, int start_y, int end_x, int end_y);
void read_path(struct tile map[MAP_ROWS][MAP_COLUMNS], int start_x, int start_y, int end_x, int end_y);
int valid_path(char ch);
void add_enemies(struct tile map[MAP_ROWS][MAP_COLUMNS], int start_x, int start_y);
void add_tower(struct tile map[MAP_ROWS][MAP_COLUMNS], int *money);
void move_enemies(struct tile map[MAP_ROWS][MAP_COLUMNS], int start_x, int start_y, int *lives);
void upgrade_tower(struct tile map[MAP_ROWS][MAP_COLUMNS], int *money);
int attack(struct tile map[MAP_ROWS][MAP_COLUMNS], int x, int y, enum entity entity);
void attack_enemies(struct tile map[MAP_ROWS][MAP_COLUMNS], int *money);
void rain(struct tile map[MAP_ROWS][MAP_COLUMNS]);
void flood(struct tile map[MAP_ROWS][MAP_COLUMNS]);
void teleporters(struct tile map[MAP_ROWS][MAP_COLUMNS], int start_x, int start_y);
// void next_teleporter(struct tile map[MAP_ROWS][MAP_COLUMNS], int x, int y, int *next_x, int *next_y);

////////////////////////////////////////////////////////////////////////////////
////////////////////// PROVIDED FUNCTION PROTOTYPE  ////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void initialise_map(struct tile map[MAP_ROWS][MAP_COLUMNS]);
void print_map(struct tile map[MAP_ROWS][MAP_COLUMNS], int lives, int money);
void print_tile(struct tile tile, int entity_print);

int main(void)
{
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
    if (lives == -1 || money == -1 || start_x == -1 || start_y == -1 || end_x == -1 || end_y == -1)
    {
        // is_basic_info_read = 0;
    }
    else
    {
        map[start_x][start_y].land = PATH_START;
        map[start_x][start_y].path = PATH_START;
        map[end_x][end_y].land = PATH_END;
        map[end_x][end_y].path = PATH_END;
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
    while (1)
    {
        printf("Enter Command: ");
        int res = scanf(" %c", &cmd);
        if (res == EOF)
        {
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
        case 'm':
            move_enemies(map, start_x, start_y, &lives);
            break;
        case 'u':
            upgrade_tower(map, &money);
            break;
        case 'a':
            attack_enemies(map, &money);
            break;
        case 'r':
            rain(map);
            break;
        case 'f':
            flood(map);
            break;
        case 'c':
            teleporters(map, start_x, start_y);
            break;
        default:
            break;
        }
        print_map(map, lives, money);

        if (lives <= 0)
        {
            printf("Oh no, you ran out of lives!\nGame Over!\n");
            break;
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
/////////////////////////////  YOUR FUNCTIONS //////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// TODO: Put your functions here
void read_basic_info(int *lives, int *money, int *start_x, int *start_y, int *end_x, int *end_y)
{
    int i, j, k, t;

    printf("Starting Lives: ");
    scanf(" %d", &i);

    printf("Starting Money($): ");
    scanf(" %d", &j);

    if (i < 0 || j < 0)
    {
        return;
    }
    *lives = i;
    *money = j;

    printf("Start Point: ");
    scanf(" %d %d", &i, &j);

    printf("End Point: ");
    scanf(" %d %d", &k, &t);

    if ((i == k && j == t) || !valid_point(i, j) || !valid_point(k, t))
    {
        return;
    }

    *start_x = i;
    *start_y = j;
    *end_x = k;
    *end_y = t;
}

int valid_point(int row, int column)
{
    return row >= 0 && column >= 0 && row < MAP_ROWS && column < MAP_COLUMNS;
}

void read_lake(struct tile map[MAP_ROWS][MAP_COLUMNS], int start_x, int start_y, int end_x, int end_y)
{
    int lake_row, lake_col, height, width;
    printf("Enter Lake: ");
    scanf(" %d %d %d %d", &lake_row, &lake_col, &height, &width);

    if (!valid_point(lake_row, lake_col) || !valid_point(lake_row + height - 1, lake_col + width - 1) || (height == 0 && width == 0) || (lake_row <= start_x && start_x < lake_row + height && lake_col <= start_y && start_y < lake_col + width) || (lake_row <= end_x && end_x < lake_row + height && lake_col <= end_y && end_y < lake_col + width))
    {
        printf("Error: Lake out of bounds, ignoring...\n");
        return;
    }

    for (int i = lake_row; i < lake_row + height; i++)
    {
        for (int j = lake_col; j < lake_col + width; j++)
        {
            map[i][j].land = WATER;
        }
    }
}

void read_path(struct tile map[MAP_ROWS][MAP_COLUMNS], int start_x, int start_y, int end_x, int end_y)
{
    printf("Enter Path: ");

    char ch;
    char path[MAP_ROWS * MAP_COLUMNS + 10];

    int len = 0;
    int x = start_x, y = start_y;
    char visited[MAP_ROWS][MAP_COLUMNS];
    for (int i = 0; i < MAP_ROWS; i++)
    {
        for (int j = 0; j < MAP_COLUMNS; j++)
        {
            visited[i][j] = 'e';
        }
    }

    while (scanf(" %c", &ch))
    {
        if (!valid_path(ch) || visited[x][y] != 'e')
        {
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

        if (x == end_x && y == end_y)
        {
            break;
        }
    }

    x = start_x;
    y = start_y;
    for (int i = 0; i < len; i++)
    {
        switch (path[i])
        {
        case 'u':
            map[x][y].land = PATH_UP;
            map[x][y].path = PATH_UP;
            x = x - 1;
            break;
        case 'd':
            map[x][y].land = PATH_DOWN;
            map[x][y].path = PATH_DOWN;
            x = x + 1;
            break;
        case 'l':
            map[x][y].land = PATH_LEFT;
            map[x][y].path = PATH_LEFT;
            y = y - 1;
            break;
        case 'r':
            map[x][y].land = PATH_RIGHT;
            map[x][y].path = PATH_RIGHT;
            y = y + 1;
            break;
        default:
            break;
        }
    }
}

int valid_path(char ch)
{
    return ch == 'r' || ch == 'd' || ch == 'l' || ch == 'u';
}

void add_enemies(struct tile map[MAP_ROWS][MAP_COLUMNS], int start_x, int start_y)
{
    int enemies = 0;
    scanf(" %d", &enemies);
    if (enemies > 0)
    {
        if (map[start_x][start_y].entity != ENEMY)
        {
            map[start_x][start_y].entity = ENEMY;
            map[start_x][start_y].n_enemies = enemies;
        }
        else
        {
            map[start_x][start_y].n_enemies += enemies;
        }
    }

    if (map[start_x][start_y].n_enemies > 999)
    {
        map[start_x][start_y].n_enemies = 999;
    }
}

void add_tower(struct tile map[MAP_ROWS][MAP_COLUMNS], int *money)
{
    int tr, tc;
    scanf(" %d %d", &tr, &tc);

    if (*money >= 200 && valid_point(tr, tc) && map[tr][tc].land == GRASS && map[tr][tc].entity == EMPTY)
    {
        *money -= 200;
        map[tr][tc].entity = BASIC_TOWER;
        printf("Tower successfully created!\n");
    }
    else
    {
        printf("Error: Tower creation unsuccessful. Make sure you have at least $200 and that the tower is placed on a grass block with no entity.\n");
    }
}

void move_enemies(struct tile map[MAP_ROWS][MAP_COLUMNS], int start_x, int start_y, int *lives)
{
    int step;
    scanf(" %d", &step);

    int cnt = 0;
    for (int k = 0; k < step; k++)
    {

        struct tile prev;
        int x = start_x, y = start_y, next_x = -1, next_y = -1;

        prev.entity = map[x][y].entity;
        prev.n_enemies = map[x][y].n_enemies;

        map[x][y].entity = EMPTY;
        map[x][y].n_enemies = 0;

        while (map[x][y].path != PATH_END)
        {
            // get next position of current enemies
            switch (map[x][y].path)
            {
            case PATH_UP:
                next_x = x - 1;
                next_y = y;
                break;
            case PATH_DOWN:
                next_x = x + 1;
                next_y = y;
                break;
            case PATH_LEFT:
                next_x = x;
                next_y = y - 1;
                break;
            case PATH_RIGHT:
                next_x = x;
                next_y = y + 1;
                break;
            default:
                break;
            }

            if (map[x][y].land == TELEPORTER && map[x][y].pair_x != -1 && map[x][y].pair_y != -1){
                next_x = map[x][y].pair_x;
                next_y = map[x][y].pair_y;
            }

            // move
            struct tile temp;

            temp.entity = map[next_x][next_y].entity;
            temp.n_enemies = map[next_x][next_y].n_enemies;

            map[next_x][next_y].entity = prev.entity;
            map[next_x][next_y].n_enemies = prev.n_enemies;

            prev.entity = temp.entity;
            prev.n_enemies = temp.n_enemies;

            // go to next pos
            x = next_x;
            y = next_y;
        }

        if (map[x][y].entity != ENEMY)
        {
            map[x][y].n_enemies = 0;
        }

        cnt += map[x][y].n_enemies;

        map[x][y].entity = EMPTY;
        map[x][y].n_enemies = 0;
    }

    *lives -= cnt;
    printf("%d enemies reached the end!\n", cnt);
}

void upgrade_tower(struct tile map[MAP_ROWS][MAP_COLUMNS], int *money)
{
    int tr, tc;
    scanf(" %d %d", &tr, &tc);

    if (!valid_point(tr, tc))
    {
        printf("Error: Upgrade target is out-of-bounds.\n");
        return;
    }

    if (!(map[tr][tc].entity == BASIC_TOWER || map[tr][tc].entity == POWER_TOWER || map[tr][tc].entity == FORTIFIED_TOWER))
    {
        printf("Error: Upgrade target contains no tower entity.\n");
        return;
    }

    switch (map[tr][tc].entity)
    {
    case BASIC_TOWER:
        if (*money < 300)
        {
            printf("Error: Insufficient Funds.\n");
            return;
        }
        map[tr][tc].entity = POWER_TOWER;
        *money -= 300;
        break;
    case POWER_TOWER:
        if (*money < 500)
        {
            printf("Error: Insufficient Funds.\n");
            return;
        }
        map[tr][tc].entity = FORTIFIED_TOWER;
        *money -= 500;
        break;
    case FORTIFIED_TOWER:
        printf("Error: Tower cannot be upgraded further.\n");
        return;
    default:
        break;
    }

    printf("Upgrade Successful!\n");
}

void attack_enemies(struct tile map[MAP_ROWS][MAP_COLUMNS], int *money)
{
    int num, cnt = 0;
    scanf(" %d", &num);
    while (num--)
    {
        for (int i = 0; i < MAP_ROWS; i++)
        {
            for (int j = 0; j < MAP_COLUMNS; j++)
            {
                if (map[i][j].entity == BASIC_TOWER || map[i][j].entity == POWER_TOWER || map[i][j].entity == FORTIFIED_TOWER)
                {
                    cnt += attack(map, i, j, map[i][j].entity);
                }
            }
        }
    }
    printf("%d enemies destroyed!\n", cnt);
    *money += (cnt * 5);
}

int attack(struct tile map[MAP_ROWS][MAP_COLUMNS], int x, int y, enum entity entity)
{
    int range, power, cnt = 0;
    if (entity == BASIC_TOWER)
    {
        range = 1;
        power = 1;
    }

    if (entity == POWER_TOWER)
    {
        range = 1;
        power = 2;
    }

    if (entity == FORTIFIED_TOWER)
    {
        range = 2;
        power = 3;
    }

    int start_x = x - range, start_y = y - range, end_x = x + range, end_y = y + range;

    if (start_x < 0)
    {
        start_x = 0;
    }
    if (start_y < 0)
    {
        start_y = 0;
    }
    if (end_x >= MAP_ROWS)
    {
        end_x = MAP_ROWS - 1;
    }
    if (end_y >= MAP_COLUMNS)
    {
        end_y = MAP_COLUMNS - 1;
    }

    for (int i = start_x; i <= end_x; i++)
    {
        for (int j = start_y; j <= end_y; j++)
        {
            if (i == x && j == y)
            {
                continue;
            }
            if (map[i][j].entity == ENEMY)
            {
                if (map[i][j].n_enemies >= power)
                {
                    cnt += power;
                    map[i][j].n_enemies -= power;
                }
                else
                {
                    cnt += map[i][j].n_enemies;
                    map[i][j].n_enemies = 0;
                    map[i][j].entity = EMPTY;
                }
            }
        }
    }

    return cnt;
}

void rain(struct tile map[MAP_ROWS][MAP_COLUMNS])
{
    int row_spacing, col_spacing, row_offset, col_offset;
    scanf(" %d %d %d %d", &row_spacing, &col_spacing, &row_offset, &col_offset);

    // calculate start position
    while (row_offset < 0)
    {
        row_offset += row_spacing;
    }

    while (col_offset < 0)
    {
        col_offset += col_spacing;
    }

    while (row_offset >= MAP_ROWS)
    {
        row_offset -= row_spacing;
    }

    while (col_offset >= MAP_COLUMNS)
    {
        col_offset -= col_spacing;
    }

    for (int i = row_offset; i < MAP_ROWS; i += row_spacing)
    {
        for (int j = col_offset; j < MAP_COLUMNS; j += col_spacing)
        {
            if (map[i][j].land == GRASS)
            {
                map[i][j].land = WATER;
                if (map[i][j].entity == BASIC_TOWER || map[i][j].entity == POWER_TOWER)
                {
                    map[i][j].entity = EMPTY;
                }
            }
        }
    }
}

void flood(struct tile map[MAP_ROWS][MAP_COLUMNS])
{
    int num;
    scanf(" %d", &num);

    int flag[MAP_ROWS][MAP_COLUMNS];

    while (num--)
    {
        for (int i = 0; i < MAP_ROWS; i++)
        {
            for (int j = 0; j < MAP_COLUMNS; j++)
            {
                flag[i][j] = 0;
            }
        }

        for (int i = 0; i < MAP_ROWS; i++)
        {
            for (int j = 0; j < MAP_COLUMNS; j++)
            {
                if (map[i][j].land == WATER && flag[i][j] == 0)
                {
                    if (valid_point(i - 1, j) && map[i - 1][j].land == GRASS)
                    { // up
                        flag[i - 1][j] = 1;
                        map[i - 1][j].land = WATER;
                        if (map[i - 1][j].entity == BASIC_TOWER || map[i - 1][j].entity == POWER_TOWER)
                        {
                            map[i - 1][j].entity = EMPTY;
                        }
                    }

                    if (valid_point(i + 1, j) && map[i + 1][j].land == GRASS)
                    { // down
                        flag[i + 1][j] = 1;
                        map[i + 1][j].land = WATER;
                        if (map[i + 1][j].entity == BASIC_TOWER || map[i + 1][j].entity == POWER_TOWER)
                        {
                            map[i + 1][j].entity = EMPTY;
                        }
                    }

                    if (valid_point(i, j - 1) && map[i][j - 1].land == GRASS)
                    { // left
                        flag[i][j - 1] = 1;
                        map[i][j - 1].land = WATER;
                        if (map[i][j - 1].entity == BASIC_TOWER || map[i][j - 1].entity == POWER_TOWER)
                        {
                            map[i][j - 1].entity = EMPTY;
                        }
                    }

                    if (valid_point(i, j + 1) && map[i][j + 1].land == GRASS)
                    { // right
                        flag[i][j + 1] = 1;
                        map[i][j + 1].land = WATER;
                        if (map[i][j + 1].entity == BASIC_TOWER || map[i][j + 1].entity == POWER_TOWER)
                        {
                            map[i][j + 1].entity = EMPTY;
                        }
                    }
                }
            }
        }
    }
}

// void next_teleporter(struct tile map[MAP_ROWS][MAP_COLUMNS], int x, int y, int *next_x, int *next_y)
// {
// }

void teleporters(struct tile map[MAP_ROWS][MAP_COLUMNS], int start_x, int start_y)
{
    int row1, col1, row2, col2;
    scanf(" %d %d %d %d", &row1, &col1, &row2, &col2);

    if (map[row1][col1].land < 3 || map[row1][col1].land > 7 || map[row2][col2].land < 3 || map[row2][col2].land > 7)
    {
        printf("Error: Teleporters can only be created on path tiles.\n");
        return;
    }

    int x1 = -1, y1 = -1, x2 = -1, y2 = -1, x = start_x, y = start_y;

    // get begin and end position
    while (map[x][y].path != PATH_END)
    {

        if (x == row1 && y == col1)
        {
            x1 = row1;
            y1 = col1;
            x2 = row2;
            y2 = col2;
            break;
        }

        if (x == row2 && y == col2)
        {
            x1 = row2;
            y1 = col2;
            x2 = row1;
            y2 = col1;
            break;
        }

        switch (map[x][y].path)
        {
        case PATH_UP:
            x = x - 1;
            break;
        case PATH_DOWN:
            x = x + 1;
            break;
        case PATH_LEFT:
            y = y - 1;
            break;
        case PATH_RIGHT:
            y = y + 1;
            break;
        default:
            break;
        }
    }

    x = start_x;
    y = start_y;

    int flag = 0;

    while (map[x][y].path != PATH_END)
    {
        if (x == x2 && y == y2)
        {
            break;
        }

        if (flag == 1 && map[x][y].path >= 3 && map[x][y].path <= 7)
        {
            map[x][y].land = GRASS;
            map[x][y].entity = EMPTY;
            map[x][y].n_enemies = 0;
        }

        if (x == x1 && y == y1)
        {
            flag = 1;
        }

        switch (map[x][y].path)
        {
        case PATH_UP:
            x = x - 1;
            break;
        case PATH_DOWN:
            x = x + 1;
            break;
        case PATH_LEFT:
            y = y - 1;
            break;
        case PATH_RIGHT:
            y = y + 1;
            break;
        default:
            break;
        }
    }

    map[row1][col1].land = TELEPORTER;
    map[row1][col1].pair_x = row2;
    map[row1][col1].pair_y = col2;

    map[row2][col2].land = TELEPORTER;
    map[row2][col2].pair_x = -1;
    map[row2][col2].pair_y = -1;
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
void initialise_map(struct tile map[MAP_ROWS][MAP_COLUMNS])
{
    for (int row = 0; row < MAP_ROWS; ++row)
    {
        for (int col = 0; col < MAP_COLUMNS; ++col)
        {
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
void print_map(struct tile map[MAP_ROWS][MAP_COLUMNS], int lives, int money)
{
    printf("\nLives: %d Money: $%d\n", lives, money);
    for (int row = 0; row < MAP_ROWS * 2; ++row)
    {
        for (int col = 0; col < MAP_COLUMNS; ++col)
        {
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
void print_tile(struct tile tile, int land_print)
{
    if (land_print)
    {
        if (tile.land == GRASS)
        {
            printf(" . ");
        }
        else if (tile.land == WATER)
        {
            printf(" ~ ");
        }
        else if (tile.land == PATH_START)
        {
            printf(" S ");
        }
        else if (tile.land == PATH_END)
        {
            printf(" E ");
        }
        else if (tile.land == PATH_UP)
        {
            printf(" ^ ");
        }
        else if (tile.land == PATH_RIGHT)
        {
            printf(" > ");
        }
        else if (tile.land == PATH_DOWN)
        {
            printf(" v ");
        }
        else if (tile.land == PATH_LEFT)
        {
            printf(" < ");
        }
        else if (tile.land == TELEPORTER)
        {
            printf("( )");
        }
        else
        {
            printf(" ? ");
        }
    }
    else
    {
        if (tile.entity == EMPTY)
        {
            printf("   ");
        }
        else if (tile.entity == ENEMY)
        {
            printf("%03d", tile.n_enemies);
        }
        else if (tile.entity == BASIC_TOWER)
        {
            printf("[B]");
        }
        else if (tile.entity == POWER_TOWER)
        {
            printf("[P]");
        }
        else if (tile.entity == FORTIFIED_TOWER)
        {
            printf("[F]");
        }
        else
        {
            printf(" ? ");
        }
    }
}
