#include <stdint.h>
#include <stdlib.h>


typedef enum{
    RIGHT = 0,
    LEFT = 1,
    UP = 2,
    DOWN = 3
} tDirection;

typedef struct Coord{
    int x;
    int y;
}Coordinates;

typedef struct Player 
{
    int head;
    int tail;
    int size;
    Coordinates body [100];
    tDirection currentDirection;
} Player;


void snake();
int menuSnake();
void drawMap();
void spawnPlayer(int x, int y, Coordinates body[100], Player * player);
void updateDirection(Player * player);
int movePlayer(Player * player);
int checkCollision(Player * player);
void spawnApple();

char getCharSnake();




