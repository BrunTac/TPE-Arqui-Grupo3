#include <view.h>
#include <libc.h>
#include <sys_calls.h>
#include <sounds.h>
#include <stdint.h>
#include <snake.h>

extern void opcodeError();

char user[MAX_BUFFER];
char cmdline[MAX_BUFFER];
char cmdtokens[MAX_TOKENS][MAX_BUFFER];
int ticksInState;
int tokens = 0;
int exited = 0;
int zoomedIn = 0;

void initialize(){
    sys_ticksElapsed(&ticksInState);
    sys_saveRegisters();
    sys_clear();
    printHeader();
    luffyTune();

    printf("%nEnter your user name: ");

    scanf("%s", &user);

    if(strcmp(user, "") == 0){
        strcpy(user, "user");
    }

    printf("%n%nHey %s! You can call the following commands:%n", user);
    printMenu();    
}

void terminal(uint64_t argc, char ** argv){
    while(!exited){
        printf("%s-$ > ", user);

        cmdtokens[0][0] = '\0';
        getCommandline();   

        // si se ingreso por lo menos un caracter     
        if(cmdtokens[0][0] != '\0'){
            commandline_handler();
        }
        newLine();
    }
}

void getCommandline(){
    int read = 0;
    char c;
    while((c = getChar()) != '\n' && read < MAX_BUFFER){
        if(c == '\b'){
            if(read > 0){
                read--;
                putChar(c);
            }
        }else if(c == '\t') {
            sys_saveRegisters();
            tabPressed(&ticksInState);
        }else{
            cmdline[read++] = c;
            putChar(c);
        }
    }
    while(read > 0 && (cmdline[read - 1] == ' ' || cmdline[read - 1] == '\t')){
        read--;
    }
    cmdline[read] = '\0';
    tokenize();
}

void tokenize(){
    tokens = 0;
    int j = 0;
    for(int i = 0; cmdline[i] != '\0'; i++){
        if(cmdline[i] == ' ' || cmdline[i] == '\t'){
            if(j > 0){
                cmdtokens[tokens++][j] = '\0';
                j = 0;
            }
        }else{
            cmdtokens[tokens][j++] = cmdline[i];
        }
    }
    cmdtokens[tokens++][j] = '\0';
}

void p1(uint64_t argc, char ** argv){
    for(int i = 0; i < 10; i++){
        printf("1%n");
        sys_sleep(100);
    }
}

void p2(uint64_t argc, char ** argv){
    for(int i = 0; i < 10; i++){
        printf("2%n");
        sys_sleep(100);
    }
}

void commandline_handler(){
    newLine();
    char * cmd = cmdtokens[0];
    if(strcmp(cmd, "menu") == 0){
        menu();
    }else if(strcmp(cmd, "clear") == 0){
        clear();
    }else if(strcmp(cmd, "time") == 0){
        time();
    }else if(strcmp(cmd, "showregisters") == 0){
        showregisters();
    }else if(strcmp(cmd, "exception") == 0){
        test_exception();
    }else if(strcmp(cmd, "snake") == 0){
        snake();
    }else if(strcmp(cmd, "zoomin") == 0){
        zoomin();
    }else if(strcmp(cmd, "zoomout") == 0){
        zoomout();
    }else if(strcmp(cmd, "exit") == 0) {
        exitShell();
    }else if(strcmp(cmd, "test") == 0){
        sys_createProcess(&p1, 0, NULL, 0);
        sys_createProcess(&p2, 0, NULL, 0);
    }else{
        invalid_command();
    }
}

void notEnoughArguments(int arguments){
    printError("Error: not enough arguments. '%s' needs %d argument%s.%n", cmdtokens[0], arguments, arguments == 1? "" : "s");
}

void tooManyArguments(int arguments){
    if(arguments == 0){
        printError("Error: '%s' doesn't expect arguments.%n", cmdtokens[0]);
    }else{
        printError("Error: '%s' expects %d argument%s.%n", cmdtokens[0], arguments, arguments == 1? "" : "s");
    }
}

int checkArguments(int arguments){
    if(tokens < arguments + 1){
        notEnoughArguments(arguments);
        return 0;
    }
    if(tokens > arguments + 1){
        tooManyArguments(arguments);
        return 0;
    }
    return 1;
}

void menu(){
    if(checkArguments(0)){
        printMenu();
    }
}

void clear(){
    if (checkArguments(0)){
        // vacia la pantalla y ubica los punteros de posicion en el principio
        sys_clear();
    }
}

void time(){
    if(checkArguments(0)){
        char time[3][3];
        sys_time(time);
        printf("[Time]: %s:%s:%s", time[0], time[1], time[2]);
    }
}

char * registerNames[] = {"Instruction pointer: ", "CS: ", "RAX: ", "RBX: ", "RCX: ", "RDX: ", "RSP: ", 
			"RBP: ", "RDI: ", "RSI: ", "R8: ", "R9: ", "R10: ", "R11: ", "R12: ", 
			"R13: ", "R14: ", "R15: ", "RFLAGS: " };

void showregisters(){
    if(checkArguments(0)){
        printf("Registers at tick number %d:%n", ticksInState);
        sys_showRegisters();
    }
}

void test_exception(){
    if(checkArguments(1)){
        char * arg = cmdtokens[1];
        if(strcmp(arg, "opcode") == 0){
            test_opcode_exep();
        }else if(strcmp(arg, "divzero") == 0){
            test_divzero_exep();
        }else{
            printf("%nInvalid Argument. Please enter one of the following options:%n");
            printf("1. opcode%n2. divzero%n");
        }
    }
}

void zoomin(){
    if(checkArguments(0)){
        if(!zoomedIn){
            zoomedIn = 1;
            sys_changeFont(2);
            sys_clear();
        }else{
            printError("Error: max zoom in%n");
        }
    }
}

void zoomout(){
    if(checkArguments(0)){
        if(zoomedIn){
            zoomedIn = 0;
            sys_changeFont(1);
            sys_clear();
        }else{
            printError("Error: max zoom out%n");
        }
    }
}

void snake() {
    if(checkArguments(0)) {
        ticksInState = snakeGame();
    }
}

void exitShell(){
    printf("%nSee you soon %s!", user);
    exited = 1;
}

void invalid_command(){
    printError("Error. '%s' is an invalid command.%n", cmdtokens[0]);
    printf("%nToo see all available commands enter: 'menu'");
}


void test_opcode_exep(){
    opcodeError();
}
void test_divzero_exep(){
    int a = 5 / 0;
}

void printHeader(){
    printDashLine();
    printDashLine();
    printf("%nWelcome to ");
    printfColor("Cuervazos", RED, BLUE); printf(" "); printfColor("Millonarios", RED, WHITE); printf(" OS%n%n");
    printDashLine();
    printDashLine();
}

void printDashLine(){
    printf("--------------------------------------------------------------------------------------------------------------------------------%n");
}

void printMenu(){
    newLine();
    printDashLine();
    printf("COMMAND MENU%n");
    printDashLine();
    printf("- menu............................prints this command menu%n");
    printf("- time............................prints the current time%n");
    printf("- showregisters...................prints the saved register values. To save the register values, press the TAB key at any time%n");
    printf("- clear...........................clears the screen%n");
    printf("- exception.......................tests exceptions. Use one of the following arguments:%n");
    printf("    -opcode.......................throws the 'invalid opcode' exception%n");
    printf("    -divzero......................throws the 'zero division' exception%n");
    printf("- zoomin..........................increases the character font%n");
    printf("- zoomout.........................decreases the character font%n");
    printf("- snake...........................play Snake game%n");
    printf("- exit............................exits the terminal%n%n");
}
