#include "kernel/types.h"
#include "user/user.h"

void primes(int* fd){
    close(fd[1]);
    int n;
    int res = read(fd[0],&n,sizeof(int));
    if(res != 0){
        printf("prime %d\n",n);
        int pid,fd_new[2];
        pipe(fd_new);
        if((pid = fork()) < 0){
            fprintf(2,"primes:fork failed\n");
            close(fd[0]);
            close(fd_new[0]);
            close(fd_new[1]);
            exit(1);
        }else if(pid > 0){ //parent 
            close(fd_new[0]);
            int num;
            while(read(fd[0],&num,sizeof(int)) != 0){
                if(num%n != 0){
                    write(fd_new[1],&num,sizeof(int));
                }
            }

            close(fd[0]);
            // 如果不关闭写的一端，程序无法退出，可能是由于要是不关闭写的一端，读取时会阻塞？
            close(fd_new[1]);

            wait(0);
        }else{
            primes(fd_new);
            exit(0);
        }
    }
    close(fd[0]);    
}

int 
main(int argc, char *argv[]){
    int pid,fd[2];
    pipe(fd);
    if((pid = fork()) < 0){
        fprintf(2,"primes:fork failed\n");
        exit(1);
    }else if(pid > 0){ //parent 
        close(fd[0]);
        for(int i = 2;i <= 35;i++){
            write(fd[1],&i,sizeof(int));
        }
        close(fd[1]);
        wait(0);
        exit(0);
    }else{
        primes(fd);
        exit(0);
    }

}
    