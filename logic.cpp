#include <iostream>
#include <fstream>
#include <string>
#include "logic.h"

using std::cout, std::endl, std::ifstream, std::string;

/**
 * TODO: Student implement this function
 * Load representation of the dungeon level from file into the 2D map.
 * Calls createMap to allocate the 2D array.
 * @param   fileName    File name of dungeon level.
 * @param   maxRow      Number of rows in the dungeon table (aka height).
 * @param   maxCol      Number of columns in the dungeon table (aka width).
 * @param   player      Player object by reference to set starting position.
 * @return  pointer to 2D dynamic array representation of dungeon map with player's location., or nullptr if loading fails for any reason
 * @updates  maxRow, maxCol, player
 */
char** loadLevel(const string& fileName, int& maxRow, int& maxCol, Player& player) { //fix 1c
    ifstream inFS;

    inFS.open(fileName);
    if (!inFS.is_open() || !inFS.good()) {
        inFS.close();
        return nullptr;
    }
    inFS >> maxRow;
    inFS >> maxCol;

    long area = maxRow * maxCol;
    if (maxRow > 999999 || maxRow <= 0 || maxCol > 999999 || maxCol <= 0 || area > INT32_MAX) {
        inFS.close();
        return nullptr;
    }

    inFS >> player.row;
    inFS >> player.col;
    if (inFS.fail()) {
        inFS.close();
        return nullptr;
    }
    if (player.row >= maxRow || player.row < 0 || player.col >= maxCol || player.col < 0) {
        inFS.close();
        return nullptr;
    }

    char** map = createMap(maxRow, maxCol);
    if (map == nullptr) {
        return nullptr;
    }


    for (int i = 0; i < maxRow; i++) {
        for (int j = 0; j < maxCol; j++) {
            if (!inFS.good()) {
                deleteMap(map, maxRow);
                inFS.close();
                return nullptr;
            } 
            if (inFS >> map[i][j]) {
                continue;
            }
            else {
                deleteMap(map, maxRow);
                inFS.close();
                return nullptr;
            }
        }
    }

    if (inFS.bad() || inFS.fail() || inFS.eof()) {
        deleteMap(map, maxRow);
        inFS.close();
        return nullptr;
    }
    string end;
    inFS >> end;
    if (!end.empty())
    {
        return nullptr;
    }
    
    map[player.row][player.col] = TILE_PLAYER;
    inFS.close();

    for (int i = 0; i < maxRow; i++) {
        for (int j = 0; j < maxCol; j++) { 
            if (map[i][j] == TILE_DOOR || map[i][j] == TILE_EXIT) {
                return map;
            }
        }
    } 
    return nullptr;
}

/**
 * TODO: Student implement this function
 * Translate the character direction input by the user into row or column change.
 * That is, updates the nextRow or nextCol according to the player's movement direction.
 * @param   input       Character input by the user which translates to a direction.
 * @param   nextRow     Player's next row on the dungeon map (up/down).
 * @param   nextCol     Player's next column on dungeon map (left/right).
 * @updates  nextRow, nextCol
 */
void getDirection(char input, int& nextRow, int& nextCol) {
    switch (input) {
        case(MOVE_DOWN):
            nextRow++;
            break;
        case(MOVE_UP):
            nextRow--;
            break;
        case(MOVE_LEFT):
            nextCol--;
            break;
        case(MOVE_RIGHT):
            nextCol++;
            break;
    }
}

/**
 * TODO: [suggested] Student implement this function
 * Allocate the 2D map array.
 * Initialize each cell to TILE_OPEN.
 * @param   maxRow      Number of rows in the dungeon table (aka height).
 * @param   maxCol      Number of columns in the dungeon table (aka width).
 * @return  2D map array for the dungeon level, holds char type.
 */
char** createMap(int maxRow, int maxCol) {
    char** map = new char*[maxRow];

    for (int i = 0; i < maxRow; i++) {
        map[i] = new char[maxCol];
    }

    for (int i = 0; i < maxRow; i++) {
        for (int j = 0; j < maxCol; j++) {
            map[i][j] = TILE_OPEN;  
        }
    }
    return map;
}

/**
 * TODO: Student implement this function
 * Deallocates the 2D map array.
 * @param   map         Dungeon map.
 * @param   maxRow      Number of rows in the dungeon table (aka height).
 * @return None
 * @update map, maxRow
 */
void deleteMap(char**& map, int& maxRow) {
    if (map != nullptr) {
        for (int i = 0; i < maxRow; i++) {
            delete[] map[i];
        }
    
    delete[] map;
    }
    map = nullptr;
    maxRow = 0;
}

/**
 * TODO: Student implement this function
 * Resize the 2D map by doubling both dimensions.
 * Copy the current map contents to the right, diagonal down, and below.
 * Do not duplicate the player, and remember to avoid memory leaks!
 * You can use the STATUS constants defined in logic.h to help!
 * @param   map         Dungeon map.
 * @param   maxRow      Number of rows in the dungeon table (aka height), to be doubled.
 * @param   maxCol      Number of columns in the dungeon table (aka width), to be doubled.
 * @return  pointer to a dynamically-allocated 2D array (map) that has twice as many columns and rows in size.
 * @update maxRow, maxCol
 */
char** resizeMap(char** map, int& maxRow, int& maxCol) { 
    if (map == nullptr) {
        return nullptr;
    }

    int oldRow = maxRow;
    int oldCol = maxCol;
    long newrow = oldRow * 2;
    long newcol = oldCol * 2;
    long area = maxRow * maxCol;
    if (newrow > 999999 || newrow <= 0 || newcol > 999999 || newcol <= 0 || area > INT32_MAX) {
        return nullptr;
    }
    maxRow *= 2;
    maxCol *= 2;

    // cout << maxRow << " " << maxCol << endl;
    // cout << oldRow << " " << oldCol << endl;
    char** newMap = createMap(maxRow, maxCol);

    for (int i = 0; i < maxRow; i++) {
        for (int j = 0; j < maxCol; j++) {
            if (i >= oldRow && j >= oldCol) {
                if (map[i - oldRow][j - oldCol] == TILE_PLAYER) {
                    newMap[i][j] = TILE_OPEN;
                }
                else {
                    newMap[i][j] = map[i - oldRow][j - oldCol];
                }
            }
            else if (i >= oldRow) {
                if (map[i - oldRow][j] == TILE_PLAYER) {
                    newMap[i][j] = TILE_OPEN;
                }
                else {
                    newMap[i][j] = map[i - oldRow][j];
                }
            }
            else if (j >= oldCol) {
                if (map[i][j - oldCol] == TILE_PLAYER) {
                    newMap[i][j] = TILE_OPEN;
                }
                else {
                    newMap[i][j] = map[i][j - oldCol];
                }
            }
            else {
                newMap[i][j] = map[i][j];
            }
            
        }
    }
    deleteMap(map, oldRow);
    return newMap;
}

/**
 * TODO: Student implement this function
 * Checks if the player can move in the specified direction and performs the move if so.
 * Cannot move out of bounds or onto TILE_PILLAR or TILE_MONSTER.
 * Cannot move onto TILE_EXIT without at least one treasure. 
 * If TILE_TREASURE, increment treasure by 1.
 * Remember to update the map tile that the player moves onto and return the appropriate status.
 * You can use the STATUS constants defined in logic.h to help!
 * @param   map         Dungeon map.
 * @param   maxRow      Number of rows in the dungeon table (aka height).
 * @param   maxCol      Number of columns in the dungeon table (aka width).
 * @param   player      Player object to by reference to see current location.
 * @param   nextRow     Player's next row on the dungeon map (up/down).
 * @param   nextCol     Player's next column on dungeon map (left/right).
 * @return  Player's movement status after updating player's position.
 * @update map contents, player
 */
int doPlayerMove(char** map, int maxRow, int maxCol, Player& player, int nextRow, int nextCol) {
    if (nextRow < 0 || nextRow >= maxRow || nextCol < 0 || nextCol >= maxCol) { //out of bounds
        return STATUS_STAY;
    }
    else if (map[nextRow][nextCol] == TILE_PILLAR || map[nextRow][nextCol] == TILE_MONSTER) { //into monster or into pillar
        return STATUS_STAY;
    }
    else if (map[nextRow][nextCol] == TILE_TREASURE) { // treasure tile
        map[nextRow][nextCol] = TILE_PLAYER;
        map[player.row][player.col] = TILE_OPEN;
        player.col = nextCol;
        player.row = nextRow;
        player.treasure ++;
        return STATUS_TREASURE;
    }
    else if (map[nextRow][nextCol] == TILE_AMULET) { //amulet tile
        map[nextRow][nextCol] = TILE_PLAYER;
        map[player.row][player.col] = TILE_OPEN;
        player.col = nextCol;
        player.row = nextRow;
        return STATUS_AMULET;
    }
    else if (map[nextRow][nextCol] == TILE_DOOR) { //door tile
        map[nextRow][nextCol] = TILE_PLAYER;
        map[player.row][player.col] = TILE_OPEN;
        player.col = nextCol;
        player.row = nextRow;
        return STATUS_LEAVE;
    }
    else if (map[nextRow][nextCol] == TILE_EXIT) { //exit tile
        if (player.treasure < 1) { //not enough treasure
            return STATUS_STAY;
        }
        else {
            map[nextRow][nextCol] = TILE_PLAYER;
            map[player.row][player.col] = TILE_OPEN;
            player.col = nextCol;
            player.row = nextRow;
            return STATUS_ESCAPE; //successful exit
        }
    }
    else if (map[nextRow][nextCol] == TILE_OPEN) { //open tile
        map[nextRow][nextCol] = TILE_PLAYER;
        map[player.row][player.col] = TILE_OPEN;
        player.col = nextCol;
        player.row = nextRow;
        return STATUS_MOVE;
    }
    return 0;
}

/**
 * TODO: Student implement this function
 * Update monster locations:
 * We check up, down, left, right from the current player position.
 * If we see an obstacle, there is no line of sight in that direction, and the monster does not move.
 * If we see a monster before an obstacle, the monster moves one tile toward the player.
 * We should update the map as the monster moves.
 * At the end, we check if a monster has moved onto the player's tile.
 * @param   map         Dungeon map.
 * @param   maxRow      Number of rows in the dungeon table (aka height).
 * @param   maxCol      Number of columns in the dungeon table (aka width).
 * @param   player      Player object by reference for current location.
 * @return  Boolean value indicating player status: true if monster reaches the player, false if not.
 * @update map contents
 */
bool doMonsterAttack(char** map, int maxRow, int maxCol, const Player& player) {
    //check up
    for (int i = player.row - 1; i >= 0; i--) {
        if (map[i][player.col] == TILE_PILLAR) {
            break;
        }
        else if (map[i][player.col] == TILE_MONSTER) {
            map[i+1][player.col] = TILE_MONSTER;
            map[i][player.col] = TILE_OPEN;
        }
    }
    //check down
    for (int i = player.row + 1; i < maxRow; i++) {
        if (map[i][player.col] == TILE_PILLAR) {
            break;
        }
        else if (map[i][player.col] == TILE_MONSTER) {
            map[i-1][player.col] = TILE_MONSTER;
            map[i][player.col] = TILE_OPEN;
        }
    }
    //check left
    for (int i = player.col - 1; i >= 0; i--) {
        if (map[player.row][i] == TILE_PILLAR) {
            break;
        }
        else if (map[player.row][i]  == TILE_MONSTER) {
            map[player.row][i+1] = TILE_MONSTER;
            map[player.row][i] = TILE_OPEN;
        }
    }
    //check right
    for (int i = player.col + 1; i < maxCol; i++) {
        if (map[player.row][i] == TILE_PILLAR) {
            break;
        }
        else if (map[player.row][i]  == TILE_MONSTER) {
            map[player.row][i-1] = TILE_MONSTER;
            map[player.row][i] = TILE_OPEN;
        }
    }
    if (map[player.row][player.col] == TILE_MONSTER) {
        return true;
    }
    return false;
}
