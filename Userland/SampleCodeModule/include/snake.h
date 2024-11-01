#include <stdint.h>
#include <stdlib.h>
#include <color.h>

#define MAX_LENGTH_SNAKE 100

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
    Coordinates body [MAX_LENGTH_SNAKE];
    tDirection currentDirection;
    Color color;
    int points;
} Player;


void snake();
int menuSnake();
void drawMap(int cantPlayers);
void spawnPlayer(int x, int y, Player * player, Color color);
void updateDirection(Player * player1, Player * player2, int cantPlayers);
int movePlayer(Player * player, Player * otherPlayer, int cantPlayers);
int checkCollision(Player * player, Player * otherPlayer, int cantPlayers);
int checkCollisionWithBody(Player * player, Coordinates point);
void spawnApple(Player * player1, Player * player2, int cantPlayers);
int getRandomNumber();
int getLevel();
void printPoints(int cantPlayers, Player * player1, Player * player2);
void defeatScreen1(Player * player);
void defeatScreen2(Player * player1, Player * player2);
void defeatScreen3(Player * player1, Player * player2);



