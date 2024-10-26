#include <view.h>
#include <libc.h>

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
int exit = 0;

void initialize(){
    
    printf("Ingrese su nombre de usuario: ");
    scanf("%s", &user);

    if(strcmp(user, "") == 0){
        strcpy(user, "user");
    }

    printf("%nHola %s! Bienvenido a la terminal de Cuervazos SO%n", user);
    printMenu();    
}

void terminal(){
    while(!exit){
        printf("%s$> ", user);

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
    tokens = 0;
    while((c = getchar()) != '\n' && tokens < MAX_TOKENS && read < MAX_BUFFER){
        if(c == BACK_SPACE){
            if(read > 0){
                read--;
                putchar(c);
            }
        }else if(c == ' ' || c == '\t'){
            cmdtoken[read] = '\0';
            read = 0;
            strcpy(cmdline[tokens++], cmdtoken);
            putchar(c);
            while((c = getchar()) == ' ' || c == '\t'){
                putchar(c);
            }
        }else{
            cmdtoken[read++] = c;
            putchar(c);
        }
        
    }
    // guardo la ultima, solo si se termino por un \n
    if(c == '\n'){
        cmdtoken[read] = '\0';
        strcpy(cmdline[tokens], cmdtoken);
    }
}

void commandline_handler(){
    switch(cmdline[0]){
        case help:
            help();
            break;
        case clear:
            clear();
            break;
        case changeusername:
            changeusername();
            break;
        case whoami:
            whoami();
            break;
        case time:
            time();
            break;
        case showregisters:
            showregisters();
            break;
        case test_exception:
            test_exception();
            break;
        case snake:
            snake();
            break;
        case exit:
            break;
        default:
            invalid_command();
            break;
    }
}

void notEnoughArguments(int arguments){
    printf("Error: faltan argumentos. El comando '%s' necesita %d argumentos.", cmdline[0], argumentos);
}

void tooManyArguments(int arguments){
    if(arguments == 0){
        printf("Error: el comando '%s' no acepta argumentos.", cmdline[0]);
    }else{
        printf("Error: el comando '%s' solo acepta %d argumentos.", cmdline[0], argumentos);
    }
}

int checkArguments(int arguments){
    if(tokens < arguments + 1){
        notEnoughArguments(arguments);
        return 0;
    }
    if(tokens > arguments + 1){
        tooManyArguments(argumentos);
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
    if checkArguments(0){
        // vacia la pantalla y ubica los punteros de posicion en el principio
        sys_clear();
    }
}

void changeusername(){
    if(checkArguments(1)){
        strcpy(user, cmdline[1]);    
        printf("%nListo %s! Su nombre de usuario ha sido actualizado correctamente", user);
    }
}

void whoami(){
    if(checkArguments(0)){
        printf("%s", user);
    }
}

void time(){
    if(checkArguments(0)){
        printf("%d:%d:%d", sys_hours(), sys_minutes(), sys_seconds());
    }
}

void showregisters(){
    if(checkArguments(0)){
        sys_showregisters();
    }
}

void test_exception(){
    if(checkArguments(1)){
        switch(cmdline[1]){
            case "opcode":
                test_opcode_exep();
                break;
            case "divzero":
                test_divzero_exep();
                break;
            default:
                printf("Argumento invalido. Por favor ingresar una se las siguientes opciones:%n");
                printf("1. opcode%n2. divzero%n");
                break;
        }
    }
}

void snake(){

}

void exit(){
    exit = 0;
}

void invalid_command(){
    printf("Error. El comando '%s' es invalido.%n", cmdline[0]);
    printf("Para ver el menu de opciones utilice el comando: 'help'");
}


void test_opcode_exep(){
    //NO SE QUE LA TIRA :P
}
void test_divzero_exep(){
    // esto seria asi??
    int a = 5 / 0;
}


void printMenu(){
    
}


