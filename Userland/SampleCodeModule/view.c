#include <view.h>
#include <libc.h>
#include <sys_calls.h>

//FALTA: printMenu (facil pero paja), todas las sys_ (kernel), los test_ y snake obvioooooooooooo
//OBS: le puse sys_algo a todas las funciones que van a estar en kernel. pero no se si decirles sys_calls. capaz les cambiamos el nombre
// y la de time no estoy segura si se hace en kernel o aca. hice que el kernel da los valores y esto imprime. creo que es lo mejor pero not sure.
// en la de los registros se lo dejo todo al kernel, asi que no se que criterio usar!! y si cambiar criterio

//chequeo que no se manden argumentos extras al hacer los comandos; si se mandan de mas, no se hace (y avisa). capaz es mejor ignorar esto y que se haga igual

//podriamos cambiar algunos de estos printf para que impriman a color!! no se si rojo pero algun color para error

//no se bien que va en los testeos de las excepciones 


char user[MAX_BUFFER];
char cmdtoken[MAX_BUFFER];
char cmdline[MAX_TOKENS][MAX_BUFFER];
int tokens = 0;
int exited = 0;

void initialize(){
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

        cmdline[0][0] = '\0';
        getCommandline();   

        // si se ingreso por lo menos un caracter     
        if(cmdline[0][0] != '\0'){
            commandline_handler();
        }
        newLine();
    }
}

void getCommandline(){
    char c;
    int read = 0;
    int prevRead = 0;
    tokens = 0;
    while((c = getChar()) != '\n' && tokens < MAX_TOKENS && read < MAX_BUFFER){
        if(c == '\b'){
            if(read > 0){
                read--;
            }else{
                tokens--;
                read = prevRead;
            }
            putChar(c);
        }else if(c == ' ' || c == '\t'){
            cmdtoken[read] = '\0';
            prevRead = read;
            read = 0;
            strcpy(cmdline[tokens++], cmdtoken);
            putChar(c);
            while((c = getChar()) == ' ' || c == '\t'){
                putChar(c);
            }
        }else{
            cmdtoken[read++] = c;
            putChar(c);
        }
        
    }
    // guardo la ultima, solo si se termino por un \n
    if(c == '\n'){
        cmdtoken[read] = '\0';
        strcpy(cmdline[tokens++], cmdtoken);
    }
}

void commandline_handler(){
    newLine();
    char * cmd = cmdline[0];
    if(strcmp(cmd, "help") == 0){
        help();
    }else if(strcmp(cmd, "clear") == 0){
        clear();
    }else if(strcmp(cmd, "changeusername") == 0){
        changeusername();
    }else if(strcmp(cmd, "whoami") == 0){
        whoami();
    }else if(strcmp(cmd, "time") == 0){
        time();
    }else if(strcmp(cmd, "showregisters") == 0){
        showregisters();
    }else if(strcmp(cmd, "test_exception") == 0){
        test_exception();
    }else if(strcmp(cmd, "snake") == 0){
        snake();
    }else if(strcmp(cmd, "exit") == 0){
        exit();
    }else{
        invalid_command();
    }
}

void notEnoughArguments(int arguments){
    printf("%nError: faltan argumentos. El comando '%s' necesita %d argumento%s.%n", cmdline[0], arguments, arguments == 1? "" : "s");
}

void tooManyArguments(int arguments){
    if(arguments == 0){
        printf("%nError: el comando '%s' no acepta argumentos.%n", cmdline[0]);
    }else{
        printf("%nError: el comando '%s' solo acepta %d argumento%s.%n", cmdline[0], arguments, arguments == 1? "" : "s");
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

void help(){
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

void changeusername(){
    if(checkArguments(1)){
        strcpy(user, cmdline[1]);    
        printf("%nListo %s! Su nombre de usuario ha sido actualizado correctamente%n", user);
    }
}

void whoami(){
    if(checkArguments(0)){
        printf("%s", user);
    }
}

void time(){
    if(checkArguments(0)){
        //printf("%d:%d:%d", sys_hours(), sys_minutes(), sys_seconds());
        printf("Time");
    }
}

void showregisters(){
    if(checkArguments(0)){
        sys_show_registers();
    }
}

void test_exception(){
    if(checkArguments(1)){
        char * arg = cmdline[1];
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

void snake(){

}

void exit(){
    printf("%nHasta pronto %s!", user);
    exited = 1;
}

void invalid_command(){
    printf("Error. El comando '%s' es invalido.%n", cmdline[0]);
    printf("%nPara ver el menu de opciones utilice el comando: 'help'");
}


void test_opcode_exep(){
    //NO SE QUE LA TIRA :P
}
void test_divzero_exep(){
    int a = 5 / 0;
}

void printHeader(){
    printDashLine();
    printDashLine();
    printf("%nBienvenido a la terminal de Cuervazos SO%n%n");
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
    printf("- help............................imprime el menu de comandos%n");
    printf("- whoami..........................imprime el nombre de usuario%n");
    printf("- changeusername..................recibe como argumento el nuevo nombre de usuario%n");
    printf("- time............................imprime la hora actual%n");
    printf("- showregisters...................imprime los valores actuales de todos los registros%n");
    printf("- clear...........................vacia la pantalla%n");
    printf("- test_exception..................prueba las exceptiones. Se llama con los siguientes argumentos:%n");
    printf("    -opcode.......................prueba la excepcion 'invalid opcode'%n");
    printf("    -divzero......................prueba la excepcion generada al dividir por cero%n");
    printf("- snake...........................llama al juego snake%n");
    printf("- exit............................sale de la terminal%n%n");
}


