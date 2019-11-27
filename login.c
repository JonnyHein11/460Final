/************* login.c file ****************/
#ifndef LOGIN
#define LOGIN

#include "ucode.c"
#include "stddef.h"
int in, out, err;
char *tty;

/* User info for login once authenticated */
char uname[64], home[64], program[64];
int uid, gid;
/******************************************/
main(int argc, char *argv[]){
    char user[64], passwd[64]; //currently assuming max length is 64
    tty = argv[1];
    // 1. close file descriptors 0,1 inherited from INIT.
    close(0); close(1);
    // 2. open argv[1] 3 times as in(0), out(1), err(2)
    in = open(tty, 0);
    out = open(tty, 1);
    err = open(tty, 2);
    // 3. settty(argv[1]); // set tty name string in PROC.tty
    settty(tty);
    

    
    while(1){
        printf("login: ");
        gets(user);
        printf("\npassword: ");
        gets(passwd);
        printf("hrllo\n");
        
        if(authenticate(user, passwd)){
            printf("Login Successful!\nHello %s!\n",user);
            chuid(uid,gid);
            chdir(home);
            exec(program);
        }
        printf("Login failed...!\nTry again!\n");
    }
    exit(0);
}

int authenticate(char user[], char passwd[]){
    // 4. open /etc/passwd file for READ;
    printf("hrllo\n");
    int file = open("/etc/passwd", 0);
    printf("%d\n",file);
    char buffer[1024];
    if(file == NULL){
        printf("Password file doesnt exist!\n");
        return 0;
    }
    if(read(file,buffer,1024) < 0){
        printf("Unable to read passwd file\n");
        return 0;
    }
    char *line = strtok(buffer,":\n");
    while(line!=NULL){
        if (strcmp(line,user)==0){ //found user
            line = strtok(NULL, ":\n"); //grabbing the passwd

            if(strcmp(line, passwd)==0){
                uid = atoi(strtok(NULL, ":\n"));
                gid = atoi(strtok(NULL, ":\n"));
                strcpy(uname, strtok(NULL, ":\n"));
                strcpy(name,user);
                strcpy(home, strtok(NULL, ":\n"));
                strcpy(program, strtok(NULL,":\n"));

                close(file);
                return 1;
            }
        }
        line = strtok(NULL,":\n"); //next line
    }
    close(file);
    return 0;
}


#endif