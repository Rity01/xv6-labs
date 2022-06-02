#include "kernel/types.h"
#include "user/user.h"
#include "kernel/param.h"
#define MAX_LEN 100

int
main(int argc, char *argv[])
{
    // 假设buf不溢出
    char buf[512];
    int i = 0;
    char* args[MAXARG];
    for(int j = 1; j<argc; j++){
        args[j-1] = argv[j];
    }
    while(read(0,&buf[i],sizeof(char)) != 0){
        if(buf[i] == '\n'){
            buf[i] = '\0';
            args[argc-1] = buf;
            args[argc] = 0;
            int pid = fork();
            if(pid < 0){
                fprintf(2,"xargs:fork failed\n");
                exit(1);
            }else if(pid > 0){
                wait(0);
                i = 0;
            }else{
                exec(args[0],args);
                exit(0);
            }
            
        }
        i++;
        
    }
    exit(0);
}