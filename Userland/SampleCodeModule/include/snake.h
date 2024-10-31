#include <stdint.h>


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
Player spawnPlayer(int x, int y, Coordinates body[100]);
void updateDirection(Player * player);
void movePlayer(Player * player);
int canMove(Player player);



