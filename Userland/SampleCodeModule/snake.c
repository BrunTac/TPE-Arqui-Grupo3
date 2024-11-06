#include <color.h>
#include <snake.h>
#include <sys_calls.h>
#include <libc.h>
#include <stdlib.h>
#include <sounds.h>


#define BLOCKSIZE 32
#define STDIN 0
#define DEFAULT_SPEED 5
#define INCREASE_RATE 6

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

static int playing;
static int speed;
static int level;

static int ticksForState;

int snakeGame(){

    sys_scrHeight(&scrHeight);
    sys_scrWidth(&scrWidth);

    borderSizeX = scrWidth / 8;
    borderSizeY = scrHeight / 8;

    boardWidth = scrWidth - 2 * borderSizeX;
    boardHeight = scrHeight - 2 * borderSizeY;
 
    headerY = borderSizeY - BLOCKSIZE * 1.4;

    
    playing = 1;
    while(playing){
        int cantPlayers = menuSnake();
        speed = DEFAULT_SPEED;
        level = 1;
        printControlScreen(cantPlayers);
        

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
        int ticks1, ticks2;

        while (!lost1 && !lost2)
        {        
            sys_ticksElapsed(&ticks2);
            if ((ticks2 - ticks1) >= speed ){
                printPoints(cantPlayers, &player1, &player2);
                lost1 = movePlayer(&player1, &player2, cantPlayers);
                if(cantPlayers == 2){
                    lost2 = movePlayer(&player2, &player1, cantPlayers);
                }
                sys_ticksElapsed(&ticks1);
            }
            updateDirection(&player1, &player2, cantPlayers);
        }

        sys_clear();
        
        char c;
        if (cantPlayers == 1){
            c = defeatScreen1(&player1);
        }else if (lost2){
            c = defeatScreen2(&player1, &player2);
        }else {
            c = defeatScreen3(&player1, &player2);
        }
        if(c != ' '){
            playing = 0;
        }
    }
    sys_clear();
    return ticksForState;
}

void spawnPlayer(int x, int y, Player * player, Color color){
    for (int i = 0; i < player->size; i++)
    {
        sys_drawSquare(color, x + i*BLOCKSIZE, y);
    }

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
    int fontwidth;
    sys_getFontWidth(&fontwidth);

    // Print Level
    sys_writeInPos(LEVEL_STR, borderSizeX + BLOCKSIZE + boardWidth / 2 - strlen(LEVEL_STR) * fontwidth, headerY, PINE_GREEN, ICE_GREEN);
    printLevel();
    
    
    // Print players
    sys_writeInPos(POINTS_STR_1, borderSizeX + BLOCKSIZE * 0.5, headerY, PINE_GREEN, ICE_GREEN);

    if(cantPlayers == 2){
        sys_writeInPos(POINTS_STR_2, scrWidth - (borderSizeX + BLOCKSIZE) - strlen(POINTS_STR_2) * fontwidth, headerY, PINE_GREEN, ICE_GREEN);
    }
}



void updateDirection(Player * player1, Player * player2, int cantPlayers){
    
    char c;

    if(cantPlayers == 2){
        int read1 = 0;
        int read2 = 0;
        for(int i = 0; i < 2; i++){
        
            sys_readLastPressed(STDIN, &c);

            switch (c){
                case 'w':
                    if (player1->currentDirection != DOWN && !read1){
                        player1->currentDirection = UP;
                        read1 = 1;
                    }
                    break;
                case 'a':
                    if (player1->currentDirection != RIGHT && !read1){
                        player1->currentDirection = LEFT;
                        read1 = 1;
                    }
                    break;
                case 'd':
                    if (player1->currentDirection != LEFT && !read1){
                        player1->currentDirection = RIGHT;
                        read1 = 1;
                    }
                    break;
                case 's':
                    if (player1->currentDirection != UP && !read1){
                        player1->currentDirection = DOWN;
                        read1 = 1;
                    }
                    break;
                case 'i':
                    if (player2->currentDirection != DOWN && !read2){
                        player2->currentDirection = UP;
                        read2 = 1;
                    }
                    break;
                case 'j':
                    if (player2->currentDirection != RIGHT && !read2){
                        player2->currentDirection = LEFT;
                        read2 = 1;
                    }
                    break;
                case 'l':
                    if (player2->currentDirection != LEFT && !read2){
                        player2->currentDirection = RIGHT;
                        read2 = 1;
                    }
                    break;
                case 'k':
                    if (player2->currentDirection != UP && !read2){
                        player2->currentDirection = DOWN;
                        read2 = 1;
                    }
                    break;
                case '\t':
                    sys_saveRegisters();
                    tabPressed(&ticksForState);
                    sys_clearLastPressed();
                    break;
                }   
            }
    }else{
        sys_readLastPressed(STDIN, &c);
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
            case '\t':
                sys_saveRegisters();
                tabPressed(&ticksForState);
                sys_clearLastPressed();
                break;
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
        lost();
        return 1;
    }else if (collision == 1)
    {
        player->points++;
        if (player->points % INCREASE_RATE == 0 && speed > 1)
        {
            levelUp();
            speed--;
            level++;
            printLevel();
        }else{
            appleEaten();
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
    sys_writeInPos(aux, borderSizeX + BLOCKSIZE * 0.3 + strlen(POINTS_STR_1) * fontwidth, headerY, PINE_GREEN, ICE_GREEN);

    if(cantPlayers == 2){
        numToStr(player2->points, aux);
        sys_writeInPos(aux, scrWidth - (borderSizeX + BLOCKSIZE * 1.2), headerY, PINE_GREEN, ICE_GREEN);
    }
}

void printLevel(){
    char aux[MAX_BUFFER];
    numToStr(level, aux);

    sys_writeInPos(aux, borderSizeX + BLOCKSIZE + boardWidth / 2, headerY, PINE_GREEN, ICE_GREEN);
}



char defeatScreen1(Player * player){
    printCanvas(GRAY);

    printf("%n%n%n%n%n%n%n%n%n%n%n%n%n%n%n%n%n");
    printfColor("                                   y   y   ooo   u   u        l      oooo   ssss  ttttt   !!                                   %n", ICE_GREEN, GRAY);
    printfColor("                                   y   y  o   o  u   u        l     o    o s        t     !!                                   %n", ICE_GREEN, GRAY);
    printfColor("                                    yyy   o   o  u   u        l     o    o  ssss    t     !!                                   %n", ICE_GREEN, GRAY);
    printfColor("                                     y    o   o  u   u        l     o    o      s   t                                          %n", ICE_GREEN, GRAY);
    printfColor("                                     y     ooo    uuu         llll   oooo   ssss    t     !!                                   %n", ICE_GREEN, GRAY);
    printf("%n%n%n");
    printfColor("                                                          Score : %d                                                            %n", ICE_GREEN, GRAY, player->points);
    printf("%n%n");
    printfColor("                                                    press SPACE to play again                                                  %n", ICE_GREEN, GRAY);
    printf("%n");
    printfColor("                                                   press anything else to exit                                                 %n", ICE_GREEN, GRAY);

    printBorder(MUSTARD, LIGHT_MUSTARD, 1.5);
    char c = getChar();
    while(c == '\t') {
        sys_saveRegisters();
        tabPressed(&ticksForState);
        c = getChar();
    }
    return c;
}

char defeatScreen2(Player * player1, Player * player2){
    printCanvas(GRAY);

    printf("%n%n%n%n%n%n%n%n%n%n%n%n%n%n%n%n%n");
    printfColor("                         ppp    l      aaaa   y   y  eeee  rrrr      11      w     w  i  n   n   ssss   !!                    %n", ICE_GREEN, GRAY);
    printfColor("                         p  p   l      a  a    y y   e     r   r   1111      w     w  i  nn  n  s       !!                    %n", ICE_GREEN, GRAY);
    printfColor("                         ppp    l      aaaa     y    eee   rrrr      11      w  w  w  i  n n n   sss    !!                    %n", ICE_GREEN, GRAY);
    printfColor("                         p      l      a  a     y    e     r r       11       w w w   i  n  nn      s                         %n", ICE_GREEN, GRAY);
    printfColor("                         p      llll   a  a     y    eeee  r  rr   111111      w w    i  n   n  ssss    !!                    %n", ICE_GREEN, GRAY);
    printf("%n%n%n%n");
    printfColor("                                                      Player 1 Score : %d                                                         %n", ICE_GREEN, GRAY, player1->points);
    printf("%n");
    printfColor("                                                      Player 2 Score : %d                                                         %n", ICE_GREEN, GRAY, player2->points);
    printf("%n%n%n");
    printfColor("                                                    press SPACE to play again                                                  %n", ICE_GREEN, GRAY);
    printf("%n");
    printfColor("                                                   press anything else to exit                                                 %n", ICE_GREEN, GRAY);

    printBorder(BURGUNDY, SOFT_RED, 1);
    char c = getChar();
    while(c == '\t') {
        sys_saveRegisters();
        tabPressed(&ticksForState);
        c = getChar();
    }
    return c;
}

char defeatScreen3(Player * player1, Player * player2){
    printCanvas(GRAY);
    
    printf("%n%n%n%n%n%n%n%n%n%n%n%n%n%n%n");
    printf("%n");
    printf("%n");
    printfColor("                        ppp    l      aaaa   y   y  eeee  rrrr     2222       w     w  i  n   n   ssss   !!                  %n", ICE_GREEN, GRAY);
    printfColor("                        p  p   l      a  a    y y   e     r   r        2      w     w  i  nn  n  s       !!                  %n", ICE_GREEN, GRAY);
    printfColor("                        ppp    l      aaaa     y    eee   rrrr      2222      w  w  w  i  n n n   sss    !!                  %n", ICE_GREEN, GRAY);
    printfColor("                        p      l      a  a     y    e     r r      2           w w w   i  n  nn      s                       %n", ICE_GREEN, GRAY);
    printfColor("                        p      llll   a  a     y    eeee  r  rr    22222        w w    i  n   n  ssss    !!                  %n", ICE_GREEN, GRAY);
    printf("%n");
    printf("%n");
    printf("%n");
    printfColor("                                                     Player 1 Score : %d                                                          %n", ICE_GREEN, GRAY, player1->points);
    printf("%n");
    printfColor("                                                     Player 2 Score : %d                                                          %n", ICE_GREEN, GRAY, player2->points);
    printf("%n");
    printf("%n");
    printf("%n");
    printfColor("                                                    press SPACE to play again                                                  %n", ICE_GREEN, GRAY);
    printf("%n");
    printfColor("                                                   press anything else to exit                                                 %n", ICE_GREEN, GRAY);

    
    printBorder(GRAY_BLUE, LIGHT_GRAY_BLUE, 1);
    char c = getChar();
    while(c == '\t') {
        sys_saveRegisters();
        tabPressed(&ticksForState);
        c = getChar();
    }
    return c;
}

int menuSnake(){
    printCanvas(PINE_GREEN);

    printf("%n%n%n%n%n%n%n%n%n%n%n%n%n%n%n");
    printfColor("                                                                                                                                %n", ICE_GREEN, PINE_GREEN);
    printfColor("                                                                                                                                %n", ICE_GREEN, PINE_GREEN);
    printfColor("                                                sssss n    n  aaa  k   k eeeee                                                  %n", ICE_GREEN, PINE_GREEN);
    printfColor("                                               s      nn   n a   a k  k  e                                                      %n", ICE_GREEN, PINE_GREEN);
    printfColor("                                                ssss  n  n n aaaaa kkk   eee                                                    %n", ICE_GREEN, PINE_GREEN);
    printfColor("                                                    s n   nn a   a k  k  e                                                      %n", ICE_GREEN, PINE_GREEN);
    printfColor("                                               sssss  n    n a   a k   k eeeee                                                  %n", ICE_GREEN, PINE_GREEN);
    printfColor("                                                                                                                                %n", ICE_GREEN, PINE_GREEN);
    printfColor("                                                                                                                                %n", ICE_GREEN, PINE_GREEN);
    printfColor("                                                                                                                                %n", ICE_GREEN, PINE_GREEN);
    printfColor("                                                   press 1 for single player                                                    %n", ICE_GREEN, PINE_GREEN);
    printfColor("                                                                                                                                %n", ICE_GREEN, PINE_GREEN);
    printfColor("                                                  press 2 for two player mode                                                   %n", ICE_GREEN, PINE_GREEN);
    printfColor("                                                                                                                                %n", ICE_GREEN, PINE_GREEN);
    printfColor("                                                                                                                                %n", ICE_GREEN, PINE_GREEN);
    printfColor("                                                                                                                                %n", ICE_GREEN, PINE_GREEN);

    
    printBorder(DARK_GREEN, GREEN, 1.5);
    char c;
    int correctKey = 0;
    while(!correctKey) {
        c = getChar();
        if(c == '\t') {
            sys_saveRegisters();
            tabPressed(&ticksForState);
        }
        if(c == '1' || c == '2')
            correctKey = 1;
    }
    return c - '0';

}

void printCanvas(Color color){
    sys_clear();
    for(int i = 0; i < scrWidth; i += BLOCKSIZE){
        for(int j = 0; j < scrHeight; j += BLOCKSIZE){
            sys_drawSquare(color, i, j);
        }
    }
}

void printBorder(Color dark, Color light, double borderFactor){
    for (int i = 0; i < scrWidth; i += BLOCKSIZE * 2)
	{
		for (int j = 0; j < scrHeight; j += BLOCKSIZE)
		{
            if((i < borderSizeX * borderFactor || i >= (scrWidth - borderSizeX * borderFactor)) || (j < borderSizeY * borderFactor || j >= scrHeight - borderSizeY * borderFactor * 4 /3)){
                if (j%(2 * BLOCKSIZE) == 0){
                    sys_drawSquare(dark, i, j);
                    sys_drawSquare(light, i+BLOCKSIZE, j);
                }else{
                    sys_drawSquare(light, i, j);
                    sys_drawSquare(dark, i+BLOCKSIZE, j);
                }
            }	
		}	
	}
}

void printControlScreen(int cantPlayers){

    printCanvas(PINE_GREEN);

    printf("%n%n%n%n%n%n%n%n%n%n");

    if(cantPlayers == 2){
        printfColor("                                                                                                                                %n", ICE_GREEN, PINE_GREEN);
        printfColor("                                                                                                                                %n", ICE_GREEN, PINE_GREEN);
        printfColor("                                 rrrr  eeeee dddd                      bbbb  ll     u   u eeeee                   %n", ICE_GREEN, PINE_GREEN);
        printfColor("                                 r   r e     d   d                     b   b ll     u   u e                       %n", ICE_GREEN, PINE_GREEN);
        printfColor("                                 rrrr  eee   d   d                     bbb   ll     u   u eee                     %n", ICE_GREEN, PINE_GREEN);
        printfColor("                                 r  r  e     d   d                     b   b ll     u   u e                       %n", ICE_GREEN, PINE_GREEN);
        printfColor("                                 r   r eeeee dddd                      bbbb  llllll  uuu  eeeee                   %n", ICE_GREEN, PINE_GREEN);
        printfColor("                                                                                                        %n", ICE_GREEN, PINE_GREEN);
        printfColor("                                                                                                        %n", ICE_GREEN, PINE_GREEN);
        printfColor("                                                                                                        %n", ICE_GREEN, PINE_GREEN);
        printfColor("                                      w     w                                   ii                              %n", ICE_GREEN, PINE_GREEN);
        printfColor("                                      w     w                                                                   %n", ICE_GREEN, PINE_GREEN);
        printfColor("                                      w  w  w                                   ii                              %n", ICE_GREEN, PINE_GREEN);
        printfColor("                                       w w w                                    ii                              %n", ICE_GREEN, PINE_GREEN);
        printfColor("                                        w w                                     ii                              %n", ICE_GREEN, PINE_GREEN);  
        printfColor("                                                                                                     %n", ICE_GREEN, PINE_GREEN);
        printfColor("                                  aaa    sss   dddd                     jjjjj   k  k   ll                        %n", ICE_GREEN, PINE_GREEN);
        printfColor("                                 a   a  s      d   d                       j    k k    ll                        %n", ICE_GREEN, PINE_GREEN);
        printfColor("                                 aaaaa   sss   d   d                       j    kk     ll                        %n", ICE_GREEN, PINE_GREEN);
        printfColor("                                 a   a      s  d   d                   j   j    k k    ll                        %n", ICE_GREEN, PINE_GREEN);
        printfColor("                                 a   a   sss   dddd                     jjj     k  k   llllll                    %n", ICE_GREEN, PINE_GREEN);
        printfColor("                                                                                                     %n", ICE_GREEN, PINE_GREEN);
        printfColor("                                                                                                     %n%n%n", ICE_GREEN, PINE_GREEN);
    printfColor("                                                     press SPACE to continue                                                      %n", ICE_GREEN, PINE_GREEN);
        printBorder(DARK_GREEN, GREEN, 1);
    }else{
        printf("%n%n%n");
        printfColor("                                                    MOVEMENT OF THE SNAKE                                 %n%n%n", ICE_GREEN, PINE_GREEN);
        printfColor("                                                           w     w                                 %n", ICE_GREEN, PINE_GREEN);
        printfColor("                                                           w     w                                %n", ICE_GREEN, PINE_GREEN);
        printfColor("                                                           w  w  w                                 %n", ICE_GREEN, PINE_GREEN);
        printfColor("                                                            w w w                                   %n", ICE_GREEN, PINE_GREEN);
        printfColor("                                                             w w                                                                  %n", ICE_GREEN, PINE_GREEN);  
        printfColor("                                                                                                     %n", ICE_GREEN, PINE_GREEN);
        printfColor("                                                       aaa    sss   dddd                                            %n", ICE_GREEN, PINE_GREEN);
        printfColor("                                                      a   a  s      d   d                                            %n", ICE_GREEN, PINE_GREEN);
        printfColor("                                                      aaaaa   sss   d   d                                         %n", ICE_GREEN, PINE_GREEN);
        printfColor("                                                      a   a      s  d   d                                         %n", ICE_GREEN, PINE_GREEN);
        printfColor("                                                      a   a   sss   dddd                                        %n", ICE_GREEN, PINE_GREEN);
        printfColor("                                                                                                     %n", ICE_GREEN, PINE_GREEN);
        printfColor("                                                                                                     %n%n", ICE_GREEN, PINE_GREEN);
        printfColor("                                                    press SPACE to continue                                                      %n", ICE_GREEN, PINE_GREEN);
        printBorder(DARK_GREEN, GREEN, 1.5);
    }

    char c;
    int correctKey = 0;
    while(!correctKey) {
        c = getChar();
        if(c == '\t') {
            sys_saveRegisters();
            tabPressed(&ticksForState);
        }
        if(c == ' ')
            correctKey = 1;
    }

}

