/************* login.c file ****************/
#ifndef LOGIN
#define LOGIN

#include "ucode.c"
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
    int file = open("/etc/passwd", 0);
    char buffer[1024];
    if(file == null){
        printf("Password file doesnt exist!\n");
        return 0;
    }
    if(read(file,buffer,1024) < 0){
        printf("Unable to read passwd file\n");
        return 0;
    }
    char *line = strtok(buffer,":\n");
    while(line!=null){
        if (strcmp(line,user)==0){ //found user
            line = strtok(null, ":\n"); //grabbing the passwd

            if(strcmp(line, passwd)==0){
                uid = atoi(strtok(null, ":\n"));
                gid = atoi(strtok(null, ":\n"));
                strcpy(uname, strtok(null, ":\n"));
                strcpy(name,user);
                strcpy(home, strtok(null, ":\n"));
                strcpy(program, strtok(null,":\n"));

                close(file);
                return 1;
            }
        }
        line = strtok(null,":\n"); //next line
    }
    close(file);
    return 0;
}


#endif