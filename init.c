/********** init.c file ***********/
#include "ucode.c"
int console;
main(){
    int in, out;  // these are file descriptors for terminal I/O
    in = open("/dev/tty0", O_RDONLY);
    out = open("/dev/tty0", O_WRONLY); //displays to console
    printf("INIT: fork a login proc on console\n");
    console = fork();
    if(console){
        parent();
    }
    else{ //child exec login on tty0
        exec("login /dev/tty0");
    }
}

int parent(){
    int pid, status;
    while(1){
        printf("INIT: wait for ZOMBIE child\n");
        pid = wait(&status);
        if(pid==console){
            printf("INIT: forks a new console login\n");
            console = fork();
            if (console){
                continue;
            }
            else{
                exec("login /dev/tty0");
            }
        }
        printf("INIT: buried an orphan child proc %d\n", pid);
    }
}