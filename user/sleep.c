#include "kernel/types.h"
#include "user/user.h"


int
main(int argc, char *argv[]){

    if(argc <= 1 ){
        fprintf(2, "sleep needs an argument\n");
        exit(1);
    }
    if(argc > 2 ){
        fprintf(2, "argument excceed 1 for sleep\n");
        exit(1);
    }
    // 此处应该如何处理sleep系统调用的返回
    int time = atoi(argv[1]);
    int status = sleep(time);
    exit(status);
}