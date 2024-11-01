#include <color.h>
#include <snake.h>
#include <sys_calls.h>
#include <libc.h>
#include <stdlib.h>


#define BLOCKSIZE 32
#define STDIN 0
#define LEVEL1_TICKS 6

#define POINTS_STR_1 "Player1: "
#define POINTS_STR_2 "Player2: "
#define LEVEL_STR "Level "

static Coordinates apple;

static int scrHeight;
static int scrWidth;

static int borderSizeX;
static int borderSizeY;

static int boardHeight;
static int boardWidth;

static int headerY;

void snake(){

    sys_scrHeight(&scrHeight);
    sys_scrWidth(&scrWidth);

    borderSizeX = scrWidth / 8;
    borderSizeY = scrHeight / 8;

    boardWidth = scrWidth - 2 * borderSizeX;
    boardHeight = scrHeight - 2 * borderSizeY;
 
    headerY = borderSizeY - BLOCKSIZE * 1.4;
    
    int cantPlayers = menuSnake();
    int speed = LEVEL1_TICKS / getLevel();

    drawMap(cantPlayers);

    Player player1; 
    Player player2;

    int posX = BLOCKSIZE + borderSizeX;
    int posY = BLOCKSIZE + borderSizeY;

    Coordinates aux1 = {posX, posY};
    player1.body[0] = aux1;
    Coordinates aux2 = {posX + BLOCKSIZE, posY};
    player1.body[1] = aux2;
    Coordinates aux3 = {posX+ 2*BLOCKSIZE, posY};
    player1.body[2] = aux3;
    player1.head = 2;
    player1.tail = 0;
    player1.size = 3;
    player1.currentDirection = RIGHT;
    player1.color = RED;
    player1.points = 0;

    spawnPlayer(posX, posY, &player1, player1.color);

    if (cantPlayers == 2)
    { 
        posY = scrHeight - BLOCKSIZE * 2 - borderSizeY;
        Coordinates aux4 = {posX, posY};
        player2.body[0] = aux4;
        Coordinates aux5 = {posX + BLOCKSIZE, posY};
        player2.body[1] = aux5;
        Coordinates aux6 = {posX + 2*BLOCKSIZE, posY};
        player2.body[2] = aux6;
        player2.head = 2;
        player2.tail = 0;
        player2.size = 3;
        player2.currentDirection = RIGHT;
        player2.color = BLUE;
        player2.points = 0;

        spawnPlayer(posX, posY, &player2, player2.color);
    }

    spawnApple(&player1, &player2, cantPlayers);

    int lost1 = 0;
    int lost2 = 0;
    int ticks;


    while (!lost1 && !lost2)
    {
        printPoints(cantPlayers, &player1, &player2);
        sys_sleep(speed);
        
        updateDirection(&player1, &player2, cantPlayers);
        lost1 = movePlayer(&player1, &player2, cantPlayers);
        if(cantPlayers == 2){
            lost2 = movePlayer(&player2, &player1, cantPlayers);
        }
        
        
    }
    sys_changeFont(1);
    sys_clear();
}

void spawnPlayer(int x, int y, Player * player, Color color){
    for (int i = 0; i < player->size; i++)
    {
        sys_drawSquare(color, x + i*BLOCKSIZE, y);
    }

}

int menuSnake(){
    clear();

    printf("________________________________________________________________________________________________________________________________%n");
    printf("|                                                                                                                              |%n");
    printf("|                                                                                                                              |%n");
    printf("|                                               sssss n    n  aaa  k   k eeeee                                                 |%n");
    printf("|                                              s      nn   n a   a k  k  e                                                     |%n");
    printf("|                                               ssss  n  n n aaaaa kkk   eee                                                   |%n");
    printf("|                                                   s n   nn a   a k  k  e                                                     |%n");
    printf("|                                              sssss  n    n a   a k   k eeeee                                                 |%n");
    printf("	                                                                                                                              |%n");
    printf("|                                                                                                                              |%n");
    printf("|                                                                                                                              |%n");
    printf("|                                               presione 1 para modo un jugador                                                |%n");
    printf("|                                                                                                                              |%n");
    printf("|                                             presione 2 para modo dos jugadores                                               |%n");
    printf("|                                                                                                                              |%n");
    printf("|                                                                                                                              |%n");
    printf("|                                                                                                                              |%n");
    printf("|______________________________________________________________________________________________________________________________|%n");


    char c = getChar();
    return c - '0';

}

void drawMap(int cantPlayers){

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
    //sys_changeFont(2);
    int fontwidth;
    sys_getFontWidth(&fontwidth);

    // Print Level
    sys_writeInPos(LEVEL_STR, borderSizeX + BLOCKSIZE + boardWidth / 2 - strlen(LEVEL_STR) * fontwidth, headerY, DARK_GRAY, ICE_GREEN);

    char aux[MAX_BUFFER];
    numToStr(getLevel(), aux);

    sys_writeInPos(aux, borderSizeX + BLOCKSIZE + boardWidth / 2, headerY, DARK_GRAY, ICE_GREEN);
    
    
    // Print players
    sys_writeInPos(POINTS_STR_1, borderSizeX + BLOCKSIZE * 0.5, headerY, DARK_GRAY, ICE_GREEN);

    if(cantPlayers == 2){
        sys_writeInPos(POINTS_STR_2, scrWidth - (borderSizeX + BLOCKSIZE) - strlen(POINTS_STR_2) * fontwidth, headerY, DARK_GRAY, ICE_GREEN);
    }
}



void updateDirection(Player * player1, Player * player2, int cantPlayers){
    
    char c;

    for(int i = 0; i < 2; i++){
        
        sys_readLastPressed(STDIN, &c);

        if(cantPlayers == 2){
            switch (c){
            case 'w':
                if (player1->currentDirection != DOWN)
                    player1->currentDirection = UP;
                break;
            case 'a':
                if (player1->currentDirection != RIGHT)    
                    player1->currentDirection = LEFT;
                break;
            case 'd':
                if (player1->currentDirection != LEFT)
                    player1->currentDirection = RIGHT;
                break;
            case 's':
                if (player1->currentDirection != UP)    
                    player1->currentDirection = DOWN;
                break;
            case 'i':
                if (player2->currentDirection != DOWN)
                    player2->currentDirection = UP;
                break;
            case 'j':
                if (player2->currentDirection != RIGHT)    
                    player2->currentDirection = LEFT;
                break;
            case 'l':
                if (player2->currentDirection != LEFT)
                    player2->currentDirection = RIGHT;
                break;
            case 'k':
                if (player2->currentDirection != UP)    
                    player2->currentDirection = DOWN;
                break;
            }
        }else{
            switch (c){
            case 'w':
                if (player1->currentDirection != DOWN)
                    player1->currentDirection = UP;
                break;
            case 'a':
                if (player1->currentDirection != RIGHT)    
                    player1->currentDirection = LEFT;
                break;
            case 'd':
                if (player1->currentDirection != LEFT)
                    player1->currentDirection = RIGHT;
                break;
            case 's':
                if (player1->currentDirection != UP)    
                    player1->currentDirection = DOWN;
                break;
            }
        }
    }
    
}

int movePlayer(Player * player, Player * otherPlayer, int cantPlayers){
    
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
    
    int collision = checkCollision(player, otherPlayer, cantPlayers);

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
        player->points++;
        if (((apple.x + apple.y) % (2 * BLOCKSIZE) == 0))
        {
            sys_drawSquare(DARK_GREEN, apple.x, apple.y);
        }else{
            sys_drawSquare(GREEN, apple.x, apple.y);
        }
        spawnApple(player, otherPlayer, cantPlayers);
    }
    
    sys_drawSquare(player->color, player->body[player->head].x, player->body[player->head].y);
    return 0;
}


int checkCollision(Player * player, Player * otherPlayer, int cantPlayers){

    if (player->body[player->head].x == apple.x && player->body[player->head].y == apple.y){
        return 1;
    }
    if (player->body[player->head].x >= boardWidth + borderSizeX || player->body[player->head].y >= boardHeight + borderSizeY
        || player->body[player->head].x < borderSizeX || player->body[player->head].y < borderSizeY){
        return -1;
    }
    int ans = checkCollisionWithBody(player, player->body[player->head]);
    if(ans == 0 && cantPlayers == 2){
        ans = checkCollisionWithBody(otherPlayer, player->body[player->head]);
    }
    return  ans;
}


void spawnApple(Player * player1, Player * player2, int cantPlayers){
    int availableSpot = 0;
    int rand;
    Coordinates aux;
    while(!availableSpot){
        rand = getRandomNumber();
        aux.x = (rand % ((boardWidth - 1)/ BLOCKSIZE)) * BLOCKSIZE + borderSizeX;
        aux.y = (rand % ((boardHeight - 1) / BLOCKSIZE)) * BLOCKSIZE + borderSizeY;
        if(!checkCollisionWithBody(player1, aux) && (cantPlayers == 1 || !checkCollisionWithBody(player2, aux))){
            availableSpot = 1;
        }
    }
    apple = aux;

    sys_drawSquare(YELLOW, apple.x, apple.y);
}

int getLevel(){
    return 1;
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

void printPoints(int cantPlayers, Player * player1, Player * player2){
    int fontwidth;
    sys_getFontWidth(&fontwidth);
    char aux[MAX_BUFFER];
    numToStr(player1->points, aux);
    sys_writeInPos(aux, borderSizeX + BLOCKSIZE * 0.3 + strlen(POINTS_STR_1) * fontwidth, headerY, DARK_GRAY, ICE_GREEN);

    if(cantPlayers == 2){
        numToStr(player2->points, aux);
        sys_writeInPos(aux, scrWidth - (borderSizeX + BLOCKSIZE * 1.2), headerY, DARK_GRAY, ICE_GREEN);
    }
}
