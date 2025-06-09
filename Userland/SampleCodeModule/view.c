#include <view.h>
#include <libc.h>
#include <sys_calls.h>
#include <sounds.h>
#include <stdint.h>
#include <stddef.h>
#include <snake.h>
#include <structs.h>
#include <phylo.h>
<<<<<<< HEAD
#include "processes.h"
#include "tests.h"
=======
#include <tests.h>

extern void opcodeError();
>>>>>>> memoryManager

char user[MAX_BUFFER];
char cmdline[MAX_BUFFER];
char cmdtokens[MAX_TOKENS][MAX_BUFFER];
int ticksInState;
int zoomedIn = 0;
int exited = 0;
int tokens = 0;
uint8_t defaultFds[] = {STDIN, STDOUT, STDERR};

uint8_t pipes[MAX_PIPES];
uint8_t pipeCounter;
uint8_t isBackground;

char * registerNames[] = {"Instruction pointer: ", "CS: ", "RAX: ", "RBX: ", "RCX: ", "RDX: ", "RSP: ", "RBP: ", 
    "RDI: ", "RSI: ", "R8: ", "R9: ", "R10: ", "R11: ", "R12: ", "R13: ", "R14: ", "R15: ", "RFLAGS: " };

Command commands[] = {
    {"menu", "prints this command menu", menu, 1, 0, 1}, {"time", "prints the current time", time, 1, 1, 0}, 
    {"showregisters", "prints the saved register values. To save the register values, press the TAB key at any time", showregisters, 1, 1, 0},
    {"clear", "clears the screen", clear, 1, 1, 0}, {"exception", "tests exceptions. Use one of the following arguments:", test_exception, 2, 1, 0},
    {"zoomin", "increases the character font", zoomin, 1, 1, 0}, {"zoomout", "decreases the character font", zoomout, 1, 1, 0},
    {"snake", "play Snake game", runSnake, 1, 0, 0}, {"exit", "exits the terminal", exitShell, 1, 1, 0}, {"ps", "list running processes", ps, 1, 0, 1},
    {"loop", "run a looped program that prints its pid", runLoop, 1, 0, 1}, {"cat", "concatenate and display received content", runCat, 1, 0, 0},
    {"wc", "counts lines, words and chars", runWc, 1, 0, 0}, {"filter", "filters vocals", runFilter, 1, 0, 0}, {"block", "blocks/unblocks process given a pid", block, 2, 1, 0},
    {"kill", "terminates a process given a pid", kill, 2, 1, 0}, {"nice", "updates priority, given a pid and a prority", nice, 3, 1, 0},
    {"phylo", "displays the philosophers-comensals problem given an initial amount", runPhylo, 2, 0, 0}, {NULL, NULL, NULL, 0, 0, 0}
};

void initialize(){
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
    menu();    
}

void terminal(){
    while(!exited){
        printf("%s-$ > ", user);

        cmdtokens[0][0] = '\0';
        getCommandline();   
   
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

int8_t getCommand(char * name){
    for(uint8_t i = 0; commands[i].name != NULL; i++){
        if(strcmp(commands[i].name, name) == 0){
            return i;
        }
    }
    return -1;
}

void pipe_handler(){
    int64_t pids[MAX_PIPES + 1];
    uint8_t i;
    for(i = 0; i < MAX_PIPES + 1; i++){
        pids[i] = -1;
    }
    for(i = 0; i < pipeCounter; i++){
        char name[] = {'p', 'i', 'p', 'e', ' ', i + '0', '\0'};
        pipes[i] = sys_pipeOpen(name);
    }
    uint8_t fds[] = {-1, -1, STDERR};
    for(uint8_t processes = 0; processes < pipeCounter + 1; processes++){
        int8_t cmdIdx = getCommand(cmdtokens[processes * 2]);
        if(commands[cmdIdx].argc > 1){
            printError("Can not pipe %s%n", commands[cmdIdx].name);
            for(i = 0; i < processes; i++){
                sys_killProcess(pids[i]);
            }
            break ;
        }
        if(processes == 0){
            fds[0] = STDIN;
            fds[1] = pipes[0];
        }else if(processes == pipeCounter){
            fds[0] = pipes[processes - 1];
            fds[1] = STDOUT;
        }else{
            fds[0] = pipes[processes - 1];
            fds[1] = pipes[processes];
        }
        char * argv[] = {commands[cmdIdx].name};
        pids[processes] = sys_createProcess(commands[cmdIdx].fn, 1, argv, DEFAULT_PRIORITY, argv[0], fds);
    }
    for(i = 0; i < pipeCounter; i++){
        sys_waitpid(pids[i]);
        sys_pipeClose(pipes[i]);
    }
    sys_waitpid(pids[i]);
}

void runTestmm() {
    testmm();
}

void runTestprio() {
    testprio();
}

void runTestprocesses() {
    testprocesses();
}

void commandline_handler(){
    newLine();
    char * cmd = cmdtokens[0];
<<<<<<< HEAD
    isBackground = 0;
    pipeCounter = 0;
    int8_t i;
    for(i = 0; i < tokens; i++){
        if(strcmp(cmdtokens[i], "|") == 0){
            if(i % 2 == 0){
                printError("Invalid use of pipes. Try 'processA | processB | process C | ... '%n");
                return ;
            }
            if(++pipeCounter == MAX_PIPES){
                printError("You can only use up to %d pipes%n", MAX_PIPES);
                return ;
            }
        }else if(strcmp(cmdtokens[i], "&") == 0){
            if(i == 0 || i != tokens - 1){
                printError("Invalid way to run background. Try 'process [args] &'%n");
                return ;
            }
            isBackground = 1;
        }
    }
    if(isBackground && pipeCounter > 0){
        printError("Can not pipe and run in background at the same time%n");
        return ;
    }
    if(pipeCounter > 0){
        if(tokens % 2 == 0 || pipeCounter < (tokens - 1) / 2){
            printError("Invalid use of pipes. Try 'processA | processB | process C | ... '%n");
        }else{
            pipe_handler();
        }
        return ;
    }
    if((i = getCommand(cmd)) < 0){
=======
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
        char * argv[] = {"phylo"};
        uint64_t pid = sys_createProcess(runPhylo, 1, argv, 1, argv[0], defaultFds);
        sys_waitpid(pid);
    } else if (strcmp(cmd, "testmm") == 0) {
        uint64_t pid = sys_createProcess(runTestmm, 0, 0, 1, "testmm", defaultFds);
        sys_waitpid(pid);
    } else if (strcmp(cmd, "viewmem") == 0) {
        sys_viewmem();
    } else if (strcmp(cmd, "block") == 0){
        block();
    } else if (strcmp(cmd, "testprio") == 0) {
        uint64_t pid = sys_createProcess(runTestprio, 0, 0, 1, "testprio", defaultFds);
        sys_waitpid(pid);
    } else if (strcmp(cmd, "testprocesses") == 0) {
        uint64_t pid = sys_createProcess(runTestprocesses, 0, 0, 1, "testprocesses", defaultFds);
        sys_waitpid(pid);
    }else{
>>>>>>> memoryManager
        invalid_command();
        return ;
    }
    if(isBackground && !commands[i].canBeBackground){
        printError("Can not run command '%s' on background%n", cmd);
    }else if(checkArguments(commands[i].argc - 1 + isBackground)){
        char * argv[MAX_TOKENS];
        for(uint8_t j = 0; j < commands[i].argc; j++){
            argv[j] = cmdtokens[j];
        }
        if(commands[i].isBuiltIn){
            commands[i].fn(commands[i].argc, argv);
        }else{
            uint64_t pid = sys_createProcess(commands[i].fn, commands[i].argc, argv, DEFAULT_PRIORITY, argv[0], defaultFds);
            if(!isBackground){
                sys_waitpid(pid);
            }
        }
    }
}

void menu(){
    newLine();
    printDashLine();
    printf("COMMAND MENU%n");
    printDashLine();

    for(int i = 0; commands[i].name != NULL; i++){
        int dots = 32 - strlen(commands[i].name);
        printf("- %s", commands[i].name);
        for(int j = 0; j < dots; j++){
            putChar('.');
        }
        printf("%s%n", commands[i].description);
        if(strcmp("exception", commands[i].name) == 0){
            printf("    -opcode.......................throws the 'invalid opcode' exception%n");
            printf("    -divzero......................throws the 'zero division' exception%n");
        }
    }
    newLine();
    printf("PROCESS MODIFIERS%n");
    printf("- Use '|' between processes commands to pipe them%n");
    printf("- Use '&' after a process to run it in background%n");
    newLine();
}

void zoomin(){
    if(!zoomedIn){
        zoomedIn = 1;
        sys_changeFont(2);
        sys_clear();
    }else{
        printError("Error: max zoom in%n");
    }
}

void zoomout(){
    if(zoomedIn){
        zoomedIn = 0;
        sys_changeFont(1);
        sys_clear();
    }else{
        printError("Error: max zoom out%n");
    }
}

void clear(){
    sys_clear();
}

void time(){
    char time[3][3];
    sys_time(time);
    printf("[Time]: %s:%s:%s", time[0], time[1], time[2]);
}

void showregisters(){
    printf("Registers at tick number %d:%n", ticksInState);
    sys_showRegisters();
}

void exitShell(){
    printf("%nSee you soon %s!", user);
    exited = 1;
}

void nice(uint64_t argc, char * argv[]){
    if (sys_changePriority(atoi(argv[1]), atoi(argv[2])) == 0){
        printf("Priority changed successfully!\n");
    }else{
        printf("Invalid pid or priority\n");
    }   
}

void kill(uint64_t argc, char * argv[]){
    uint64_t pid = atoi(argv[1]);
    if (sys_killProcess(pid) == 0){
        printf("Process with pid %d killed successfully\n", atoi(argv[1]));
    }else{
        printf("Invalid pid\n");
    }   
}

void block(uint64_t argc, char * argv[]){
    uint64_t pid = atoi(argv[1]);
    if (sys_getProcessStatus(pid) == 0){
        sys_blockProcess(pid);
        printf("Process with pid %d blocked successfully\n", atoi(argv[1]));
    }else{
        sys_unblockProcess(pid);
        printf("Process with pid %d unblocked successfully\n", atoi(argv[1]));
    }
}

void runSnake() {
    ticksInState = snakeGame();
}

void runPhylo(uint64_t argc, char * argv[]){
    int8_t philos = atoi(argv[1]);
    if(philos < MIN_PHILOSOPHERS || philos > MAX_PHILOSOPHERS){
        printError("Invalid philosopher amount. Must be between %d and %d.%n", MIN_PHILOSOPHERS, MAX_PHILOSOPHERS);
    }else{
        phylo(philos);
    }
}

void runLoop(){
    loop();
}

void runPs(){
    ps();
}

void runCat(){
    cat();
}

void runWc(){
    wc();
}

void runFilter(){
    filter();
}

void invalid_command(){
    printError("Error. '%s' is an invalid command.%n", cmdtokens[0]);
    printf("%nToo see all available commands enter: 'menu'");
}

void printHeader(){
    printDashLine();
    printDashLine();
    printf("%nWelcome to ");
    printfColor("Cuervazos", RED, BLUE); printf(" "); printfColor("Millonarios", RED, WHITE); printf(" OS%n%n");
    printDashLine();
    printDashLine();
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
    if(tokens > arguments + 1 && pipeCounter == 0){
        tooManyArguments(arguments);
        return 0;
    }
    return 1;
}
