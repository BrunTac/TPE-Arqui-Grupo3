#include <color.h>
#include <snake.h>
#include <sys_calls.h>
#include <libc.h>
#include <stdlib.h>


#define BLOCKSIZE 32
#define STDIN 0
#define LEVEL1_TICKS 6
#define MAX_LENGTH_SNAKE 100

static Coordinates apple;

static int scrHeight;
static int scrWidth;

static int borderSizeX;
static int borderSizeY;

static int boardHeight;
static int boardWidth;

int points;

void snake(){

    sys_scrHeight(&scrHeight);
    sys_scrWidth(&scrWidth);

    borderSizeX = scrWidth / 8;
    borderSizeY = scrHeight / 8;

    boardWidth = scrWidth - 2 * borderSizeX;
    boardHeight = scrHeight - 2 * borderSizeY;
    
    int cantPlayers = menuSnake();
    int speed = LEVEL1_TICKS / getLevel();

    points = 0;

    drawMap();

    Coordinates body1[MAX_LENGTH_SNAKE];
    Player * player1; 
    spawnPlayer(BLOCKSIZE + borderSizeX, BLOCKSIZE + borderSizeY, body1, player1);

    if (cantPlayers == 2)
    {
        Coordinates body2[MAX_LENGTH_SNAKE];    
        Player * player2;
        spawnPlayer(BLOCKSIZE + borderSizeX, boardHeight - BLOCKSIZE - borderSizeY, body2, player2);
    }

    spawnApple(player1);

    int lost = 0;

    while (!lost)
    {
        printPoints();
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
			if(i >= borderSizeX && i < (boardWidth + borderSizeX) && j >=  borderSizeY - 2 * BLOCKSIZE && j <= borderSizeY - BLOCKSIZE){
                sys_drawSquare(ICE_GREEN, i, j);
                sys_drawSquare(ICE_GREEN, i + BLOCKSIZE, j);
            }else if((i < borderSizeX || i >= (boardWidth + borderSizeX)) || (j < borderSizeY || j >= (boardHeight + borderSizeY))){
                sys_drawSquare(PINE_GREEN, i, j);
                sys_drawSquare(PINE_GREEN, i + BLOCKSIZE, j);
            }
            else if (j%(2 * BLOCKSIZE) == 0){
				sys_drawSquare(DARK_GREEN, i, j);
				sys_drawSquare(GREEN, i+BLOCKSIZE, j);
			}else{
				sys_drawSquare(GREEN, i, j);
				sys_drawSquare(DARK_GREEN, i+BLOCKSIZE, j);
			}	
		}	
	}
    sys_writeInPos("Puntos: ", borderSizeX + BLOCKSIZE, borderSizeY - BLOCKSIZE * 1.2, DARK_GRAY, ICE_GREEN);
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
    
    int collision = checkCollision(player);

    if (collision == 0){
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
    }else if (collision == -1){
        return 1;
    }else if (collision == 1)
    {
        points++;
        if (((apple.x + apple.y) % (2 * BLOCKSIZE) == 0))
        {
            sys_drawSquare(DARK_GREEN, apple.x, apple.y);
        }else{
            sys_drawSquare(GREEN, apple.x, apple.y);
        }
        spawnApple(player);
    }
    
    sys_drawSquare(RED, player->body[player->head].x, player->body[player->head].y);
    return 0;
}


int checkCollision(Player * player){

    if (player->body[player->head].x == apple.x && player->body[player->head].y == apple.y){
        return 1;
    }
    if (player->body[player->head].x >= boardWidth + borderSizeX || player->body[player->head].y >= boardHeight + borderSizeY
        || player->body[player->head].x < borderSizeX || player->body[player->head].y < borderSizeY){
        return -1;
    }

    return checkCollisionWithBody(player, player->body[player->head]);
}

void spawnApple(Player * player){
    int availableSpot = 0;
    int rand;
    Coordinates aux;
    while(!availableSpot){
        rand = getRandomNumber();
        aux.x = (rand % ((boardWidth - 1)/ BLOCKSIZE)) * BLOCKSIZE + borderSizeX;
        aux.y = (rand % ((boardHeight - 1) / BLOCKSIZE)) * BLOCKSIZE + borderSizeY;
        if(!checkCollisionWithBody(player, aux)){
            availableSpot = 1;
        }
    }
    apple = aux;

    sys_drawSquare(BLUE, apple.x, apple.y);
}

int getLevel(){
    return 2;
}

int getRandomNumber(){
    int rand;
    sys_ticksElapsed(&rand);
    return rand;
}

int checkCollisionWithBody(Player * player, Coordinates point){
    int i = player->tail;
    while (i != player->head)
    {
        if (point.x == player->body[i].x &&
            point.y == player->body[i].y){
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

void printPoints(){
    char aux[MAX_BUFFER];
    numToStr(points, aux);
    sys_writeInPos(aux, borderSizeX + BLOCKSIZE + 8 * 8, borderSizeY - BLOCKSIZE * 1.2, DARK_GRAY, ICE_GREEN);
}
