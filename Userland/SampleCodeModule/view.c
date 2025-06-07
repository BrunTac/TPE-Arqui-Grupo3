#include <view.h>
#include <libc.h>
#include <sys_calls.h>
#include <sounds.h>
#include <stdint.h>
#include <snake.h>
#include <structs.h>
#include <phylo.h>

extern void opcodeError();

char user[MAX_BUFFER];
char cmdline[MAX_BUFFER];
char cmdtokens[MAX_TOKENS][MAX_BUFFER];
int ticksInState;
int tokens = 0;
int exited = 0;
int zoomedIn = 0;
uint8_t defaultFds[FD_AMOUNT];

void initialize(){
    defaultFds[0] = STDIN;
    defaultFds[1] = STDOUT;
    defaultFds[2] = STDERR;
    
    sys_ticksElapsed(&ticksInState);
    sys_saveRegisters();
    sys_clear();
    printHeader();

    printf("%nEnter your user name: ");

    scanf("%s", &user);

    if(strcmp(user, "") == 0){
        strcpy(user, "user");
    }

    printf("%n%nHey %s! You can call the following commands:%n", user);
    printMenu();    
}

void terminal(){
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

void ps1() {
    uint8_t sem1 = sys_openSem("sem1", 1);
    uint8_t sem2 = sys_openSem("sem2", 0);
    for (int i = 0; i < 10; i++) {
        sys_waitSem(sem1);
        printf("1%n");
        sys_postSem(sem2);
    }
    sys_closeSem(sem1);
    sys_closeSem(sem2);
}

void ps2() {
    uint8_t sem1 = sys_openSem("sem1", 1);
    uint8_t sem2 = sys_openSem("sem2", 0);
    for (int i = 0; i < 10; i++) {
        sys_waitSem(sem2);
        printf("2%n");
        sys_postSem(sem1);
    }
    sys_closeSem(sem1);
    sys_closeSem(sem2);
}

void p1(uint64_t argc, char ** argv){
    for(int i = 0; i < 10; i++){
        printf("1%n");
        sys_sleep(20);
    }
}

void p2(uint64_t argc, char ** argv){
    for(int i = 0; i < 10; i++){
        printf("2%n");
        sys_sleep(20);
    }
}

void producer(uint64_t argc, char ** argv){
    for(int i = 0; i < 10; i++){
        printf("%d", i);
        sys_sleep(20);
    }
}

void consumer(uint64_t argc, char ** argv){
    char c[2] = {0};
    for(int i = 0; i < 10; i++){
        sys_read(&c[0]);
        printf("%s%n", &c);
        sys_sleep(20);
    }
}

void cat(){
    int charsInline = 0;
    char c;
    while((c = getChar()) != '\0'){
        if(c == '\b'){
            if(charsInline > 0){
                charsInline--;
                putChar(c);
            }
        }else{
            if(c == '\n'){
                charsInline = 0;
            }else{
                charsInline++;
            }
            putChar(c);
        }
    }
}

void wc(){
    char line[MAX_BUFFER];
    
    int lines = 0;
    int words = 0;
    int chars = 0;
    int charsInline = 0;
    char c;

    while((c = getChar()) != '\0'){
        if(c == '\b'){
            if(charsInline > 0){
                chars--;
                charsInline--;
                if(!isSpace(line[charsInline]) && (charsInline == 0 || isSpace(line[charsInline - 1]))){
                    words--;
                }
                putChar(c);
            }
        }else{
            if(c == '\n'){
                lines++;
                charsInline = 0;
            }else{
                if(!isSpace(c) && (charsInline == 0 || isSpace(line[charsInline - 1]))){
                    words++;
                }
                chars++;
                charsInline++;
            }
            putChar(c);
        }
    }
    if(chars > 0){
        lines++;
    }
    printf("%nlines: %d    words: %d    chars: %d%n", lines, words, chars);
}

void filter(){
    int charsInline = 0;
    char c;
    while((c = getChar()) != '\0'){
        if(c == '\b'){
            if(charsInline > 0){
                charsInline--;
                putChar(c);
            }
        }else{
            if(c == '\n'){
                charsInline = 0;
            }else{
                charsInline++;
            }
            if(!isVocal(c)){
                putChar(c);
            }
        }
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
    }else if(strcmp(cmd, "test1") == 0){
        sys_createProcess(p1, 0, 0, 1, "p1", defaultFds);
        sys_createProcess(p2, 0, 0, 5, "p2", defaultFds);
    }else if(strcmp(cmd, "test2") == 0){
        uint64_t pid = sys_createProcess(p1, 0, 0, 5, "p1", defaultFds);
        sys_waitpid(pid);
    }else if(strcmp(cmd, "test3") == 0){
        sys_createProcess(ps1, 0, 0, 1, "ps1", defaultFds);
        sys_createProcess(ps2, 0, 0, 3, "ps2", defaultFds);
    }else if(strcmp(cmd, "pipe") == 0){
        uint8_t pipeFd = sys_pipeOpen("mi primer pipe");
        uint8_t fds1[] = {STDIN, pipeFd, STDERR};
        uint8_t fds2[] = {pipeFd, STDOUT, STDERR};
        uint64_t pid1 = sys_createProcess(producer, 0, 0, 1, "p", fds1);
        uint64_t pid2 = sys_createProcess(consumer, 0, 0, 3, "c", fds2);
        sys_waitpid(pid1);
        sys_waitpid(pid2);
        sys_pipeClose(pipeFd);
    }else if(strcmp(cmd, "ps") == 0){
        uint64_t pid = sys_createProcess(ps, 0, 0, 5, "ps", defaultFds);
        sys_waitpid(pid);
    }else if (strcmp(cmd, "loop") == 0){
        sys_createProcess(loop, 0, 0, 1, "loop", defaultFds);
    }else if (strcmp(cmd, "cat") == 0){
        uint64_t pid = sys_createProcess(cat, 0, 0, 1, "cat", defaultFds);
        sys_waitpid(pid);
    }else if (strcmp(cmd, "wc") == 0){
        uint64_t pid = sys_createProcess(wc, 0, 0, 1, "wc", defaultFds);
        sys_waitpid(pid);
    }else if (strcmp(cmd, "filter") == 0){
        uint64_t pid = sys_createProcess(filter, 0, 0, 1, "filter", defaultFds);
        sys_waitpid(pid);
    }else if (strcmp(cmd, "nice") == 0){
        nice();
    }else if (strcmp(cmd, "kill") == 0){
        kill();
    }else if (strcmp(cmd, "phylo") == 0){
        char * argv[] = {"phylo", "5"};
        uint64_t pid = sys_createProcess(phylo, 2, argv, 1, argv[0], defaultFds);
        sys_waitpid(pid);
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

void ps() {
    ProcessInfo processes[MAX_PROCESSES];
    int count = sys_getProcessInfo(processes);

    for (int i = 0; i < count; i++) {
        printf("PID: %d | PPID: %d | PRI: %d | STATE: %d | NAME: %s | RSP: %d\n",
               processes[i].pid,
               processes[i].ppid,
               processes[i].priority,
               processes[i].status,
               processes[i].name,
               processes[i].rsp);
    }
}

void loop(){
    int pid = sys_getPid();
    for (size_t i = 0; i < 10; i++){
        printf("This is my pid:%d\n", pid);
        sys_sleep(20);
    }
}

void nice(){
    if (checkArguments(2)){
        if (sys_changePriority(atoi(cmdtokens[1]), atoi(cmdtokens[2])) == 0){
            printf("Priority changed successfully!\n");
        }else{
            printf("Invalid pid or priority\n");
        }   
    }
}

void kill(){
    if (checkArguments(1)){
        uint64_t pid = atoi(cmdtokens[1]);
        if (sys_killProcess(pid) == 0){
            printf("Process with pid %d killed successfully\n", atoi(cmdtokens[1]));
        }else{
            printf("Invalid pid\n");
        }   
    }
}
