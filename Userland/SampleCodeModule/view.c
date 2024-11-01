#include <view.h>
#include <libc.h>
#include <sys_calls.h>

extern void opcodeError();

char user[MAX_BUFFER];
char cmdline[MAX_BUFFER];
char cmdtokens[MAX_TOKENS][MAX_BUFFER];
int tokens = 0;
int exited = 0;

void initialize(){
    sys_clear();
    printHeader();
    printf("%nIngrese su nombre de usuario: ");
    scanf("%s", &user);

    if(strcmp(user, "") == 0){
        strcpy(user, "user");
    }

    printf("%n%nHola %s! Estos son los comandos que puedes llamar:%n", user);
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
    }else if(strcmp(cmd, "fontsize") == 0){
        fontsize();
    }else if(strcmp(cmd, "exit") == 0) {
        exit();
    }else{
        invalid_command();
    }
}

void notEnoughArguments(int arguments){
    printError("Error: faltan argumentos. El comando '%s' necesita %d argumento%s.%n", cmdtokens[0], arguments, arguments == 1? "" : "s");
}

void tooManyArguments(int arguments){
    if(arguments == 0){
        printError("Error: el comando '%s' no acepta argumentos.%n", cmdtokens[0]);
    }else{
        printError("Error: el comando '%s' solo acepta %d argumento%s.%n", cmdtokens[0], arguments, arguments == 1? "" : "s");
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

void showregisters(){
    if(checkArguments(0)){
        sys_show_registers();
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
            printf("%nArgumento invalido. Por favor ingresar una se las siguientes opciones:%n");
            printf("1. opcode%n2. divzero%n");
        }
    }
}

void fontsize() {
    if(checkArguments(1)) {
        char * arg = cmdtokens[1];
        if(strcmp(arg, "1") == 0) {
            sys_changeFont(1);
            sys_clear();
        }else if(strcmp(arg, "2") == 0) {
            sys_changeFont(2);
            sys_clear();
        }else if(strcmp(arg, "3") == 0) { 
            sys_changeFont(3);
            sys_clear();
        }else if(strcmp(arg, "4") == 0) {
            sys_changeFont(4);
            sys_clear();
        }else if(strcmp(arg, "5") == 0) {
            sys_changeFont(5);
            sys_clear();
        }else printf("%nArgumento invalido. Por favor ingresar un numero del 1 al 5");
    }

}

void exit(){
    printf("%nHasta pronto %s!", user);
    exited = 1;
}

void invalid_command(){
    printError("Error. El comando '%s' es invalido.%n", cmdtokens[0]);
    printf("%nPara ver el menu de opciones utilice el comando: 'menu'");
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
    printf("%nBienvenido a la terminal de ");
    printfColor("Cuervazos", RED, BLUE); printf(" "); printfColor("Millonarios", RED, WHITE); printf(" SO%n%n");
    printDashLine();
    printDashLine();
}

void printDashLine(){
    printf("--------------------------------------------------------------------------------------------------------------------------------%n");
}

void printMenu(){
    newLine();
    printDashLine();
    printf("MENU DE COMANDOS%n");
    printDashLine();
    printf("- menu............................imprime el menu de comandos%n");
    printf("- time............................imprime la hora actual%n");
    printf("- showregisters...................imprime los valores actuales de todos los registros%n");
    printf("- clear...........................vacia la pantalla%n");
    printf("- exception.......................prueba las exceptiones. Se llama con los siguientes argumentos:%n");
    printf("    -opcode.......................prueba la excepcion 'invalid opcode'%n");
    printf("    -divzero......................prueba la excepcion generada al dividir por cero%n");
    printf("- fontsize........................cambia el tamaño de letra. Se pueden ingresar numeros del 1 al 5%n");
    printf("- snake...........................llama al juego snake%n");
    printf("- exit............................sale de la terminal%n%n");
}


