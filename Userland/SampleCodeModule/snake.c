#include <color.h>
#include <snake.h>
#include <sys_calls.h>
#include <libc.h>

#define BLOCKSIZE 32
    


void snake(){

    int cantPlayers = menuSnake();


    drawMap();

    Coordinates body1[100];
    Player player1 = spawnPlayer(BLOCKSIZE, BLOCKSIZE, body1);

    if (cantPlayers == 2)
    {
        Coordinates body2[100];
        Player player2 = spawnPlayer(BLOCKSIZE, sys_scrHeight() - BLOCKSIZE, body2);
    }

    int lost = 0;

    while (!lost)
    {
        for(int i = 0; i < 100; i++){}
        //sys_sleep();
        if (canMove(player1))
        {
            movePlayer(&player1);
            updateDirection(&player1);

        }else{
            lost = 1;
        }
        
    }
}

Player spawnPlayer(int x, int y, Coordinates body[100]){
    Coordinates aux1 = {x, y};
    body[0] = aux1;
    Coordinates aux2 = {x + BLOCKSIZE, y};
    body[1] = aux2;
    Coordinates aux3 = {x + 2*BLOCKSIZE, y};
    body[2] = aux3;
    Player player = {2, 0, 3, body, RIGHT};

    for (int i = 0; i < player.size; i++)
    {
        sys_drawSquare(RED, x + i*BLOCKSIZE, y);
    }

    return player;
}

int menuSnake(){
    return 1;
}



void drawMap(){

	int width = 1024; // sys_scrWidth();
    int height = 1024; // sys_scrHeight();

	for (int i = 0; i < width; i += BLOCKSIZE * 2)
	{
		for (int j = 0; j < height; j += BLOCKSIZE)
		{
			if (j%64 == 0)
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

    char c = getChar();

    switch (c)
    {
    case 'w':
        player->currentDirection = UP;
        break;
    case 'a':
        player->currentDirection = LEFT;
        break;
    case 'd':
        player->currentDirection = RIGHT;
        break;
    case 's':
        player->currentDirection = DOWN;
        break;
    }
}

void movePlayer(Player * player){
    
    int tailX = player->body[player->tail].x;
    int tailY = player->body[player->tail].y;
    if (((tailX + tailY) % 2 == 0))
    {
        sys_drawSquare(DARK_GREEN, tailX, tailY);
    }else{
        sys_drawSquare(GREEN, tailX, tailY);
    }
    
    if(player->tail == 100 - 1){
        player->tail = 0;
    }else{
        player->tail += 1;
    }

    if(player->head == 100 - 1){
        player->head = 0;
    }else{
        player->head += 1;
    }

    Coordinates prev = player->body[player->head];

    switch (player->currentDirection)
    {
    case UP:
        Coordinates aux = {prev.x, prev.y - BLOCKSIZE};
        player->body[player->head] = aux;
        break;
    case DOWN:
        Coordinates aux1 = {prev.x, prev.y + BLOCKSIZE};
        player->body[player->head] = aux1;
        break;
    case LEFT:
        Coordinates aux2 = {prev.x - BLOCKSIZE, prev.y};
        player->body[player->head] = aux2;
        break;
    case RIGHT:
        Coordinates aux3 = {prev.x + BLOCKSIZE, prev.y};
        player->body[player->head] = aux3;
        break;
    }
    sys_drawSquare(RED, player->body[player->head].x, player->body[player->head].y);
}

int canMove(Player player){
    return 1;
}