#include <color.h>
#include <snake.h>
#include <sys_calls.h>
#include <libc.h>
#include <stdlib.h>


#define BLOCKSIZE 32
#define STDIN 0
#define LEVEL1_TICKS 12
#define MAX_LENGTH_SNAKE 100

static Coordinates apple;

static int scrHeight;
static int scrWidth;

void snake(){

    sys_scrHeight(&scrHeight);
    sys_scrWidth(&scrWidth);
    
    int cantPlayers = menuSnake();
    int speed = LEVEL1_TICKS / getLevel();

    drawMap();

    Coordinates body1[MAX_LENGTH_SNAKE];
    Player * player1; 
    spawnPlayer(BLOCKSIZE, BLOCKSIZE, body1, player1);

    if (cantPlayers == 2)
    {
        Coordinates body2[MAX_LENGTH_SNAKE];
        Player * player2;
        spawnPlayer(BLOCKSIZE, scrHeight - BLOCKSIZE, body2, player2);
    }

    spawnApple();

    int lost = 0;

    while (!lost)
    {
        sys_sleep(speed);

        updateDirection(player1);
        lost = movePlayer(player1);
    }
    sys_clear();
}

void spawnPlayer(int x, int y, Coordinates body[100], Player * player){
    Coordinates aux1 = {x, y};
    player->body[0] = aux1;
    Coordinates aux2 = {x + BLOCKSIZE, y};
    player->body[1] = aux2;
    Coordinates aux3 = {x + 2*BLOCKSIZE, y};
    player->body[2] = aux3;
    player->head = 2;
    player->tail = 0;
    player->size = 3;
    player->currentDirection = RIGHT;

    for (int i = 0; i < player->size; i++)
    {
        sys_drawSquare(RED, x + i*BLOCKSIZE, y);
    }

}

int menuSnake(){
    return 1;
}

void drawMap(){

	for (int i = 0; i < scrWidth; i += BLOCKSIZE * 2)
	{
		for (int j = 0; j < scrHeight; j += BLOCKSIZE)
		{
			if (j%(2 * BLOCKSIZE) == 0)
			{
				sys_drawSquare(DARK_GREEN, i, j);
				sys_drawSquare(GREEN, i+BLOCKSIZE, j);
			}else{
				sys_drawSquare(GREEN, i, j);
				sys_drawSquare(DARK_GREEN, i+BLOCKSIZE, j);
			}	
		}	
	}
}

void updateDirection(Player * player){

    char c;
    sys_readLastPressed(STDIN, &c);

    switch (c)
    {
    case 'w':
        if (player->currentDirection != DOWN)
            player->currentDirection = UP;
        break;
    case 'a':
        if (player->currentDirection != RIGHT)    
            player->currentDirection = LEFT;
        break;
    case 'd':
        if (player->currentDirection != LEFT)
            player->currentDirection = RIGHT;
        break;
    case 's':
        if (player->currentDirection != UP)    
            player->currentDirection = DOWN;
        break;
    }
}

int movePlayer(Player * player){
    
    Coordinates prev = player->body[player->head];

    if(player->head == MAX_LENGTH_SNAKE - 1){
        player->head = 0;
    }else{
        player->head += 1;
    }

    switch (player->currentDirection)
    {
    case UP:
        Coordinates aux1 = {prev.x, prev.y - BLOCKSIZE};
        player->body[player->head] = aux1;
        break;
    case DOWN:
        Coordinates aux2 = {prev.x, prev.y + BLOCKSIZE};
        player->body[player->head] = aux2;
        break;
    case LEFT:
        Coordinates aux3 = {prev.x - BLOCKSIZE, prev.y};
        player->body[player->head] = aux3;
        break;
    case RIGHT:
        Coordinates aux4 = {prev.x + BLOCKSIZE, prev.y};
        player->body[player->head] = aux4;
        break;
    }
    
    int Collision = checkCollision(player);

    if (Collision == 0){
        int tailX = player->body[player->tail].x;
        int tailY = player->body[player->tail].y;
        if (((tailX + tailY) % 64 == 0))
        {
            sys_drawSquare(DARK_GREEN, tailX, tailY);
        }else{
            sys_drawSquare(GREEN, tailX, tailY);
        }
    
        if(player->tail == MAX_LENGTH_SNAKE - 1){
           player->tail = 0;
        }else{
            player->tail += 1;
        }
    }else if (Collision == -1){
        return 1;
    }else if (Collision == 1)
    {
        if (((apple.x + apple.y) % (2 * BLOCKSIZE) == 0))
        {
            sys_drawSquare(DARK_GREEN, apple.x, apple.y);
        }else{
            sys_drawSquare(GREEN, apple.x, apple.y);
        }
        spawnApple();
    }
    
    sys_drawSquare(RED, player->body[player->head].x, player->body[player->head].y);
    return 0;
}


int checkCollision(Player * player){

    if (player->body[player->head].x == apple.x &&
            player->body[player->head].y == apple.y)
    {
        return 1;
    }
    

    if (player->body[player->head].x >= scrWidth || player->body[player->head].y >= scrHeight
        || player->body[player->head].x < 0 || player->body[player->head].y < 0){
        return -1;
    }

    int i = player->tail;
    while (i != player->head)
    {
        if (player->body[player->head].x == player->body[i].x &&
            player->body[player->head].y == player->body[i].y){
            return -1;
        }

        if (i == MAX_LENGTH_SNAKE - 1){
            i = 0;
        }else{
            i++;
        }
    }
    return 0;
}

void spawnApple(){
    int rand = getRandomNumber();
    apple.x = (rand % (scrWidth / BLOCKSIZE)) * BLOCKSIZE;
    apple.y = (rand % (scrHeight / BLOCKSIZE))  * BLOCKSIZE;
    sys_drawSquare(BLUE, apple.x, apple.y);
}

char getCharSnake(){
    char c;
    sys_read(STDIN, &c);
    return c;
}

int getLevel(){
    return 2;
}

int getRandomNumber(){
    int rand;
    sys_ticksElapsed(&rand);
    return rand;
}
