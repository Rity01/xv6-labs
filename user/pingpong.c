#include "kernel/types.h"
#include "user/user.h"

int
main()
{
  int pid;
  int status;
  int process;
  char buf[100];
  int fds[2];
  int fds2[2];
  pipe(fds);
  pipe(fds2);
  pid = fork();
  if(pid == 0){
    read(fds[1],buf,sizeof(buf));
    process = getpid();
    fprintf(2, "%d: received pong\n",process);
    write(fds2[0],"p",1);
 
  } else {
    process = getpid();
    write(fds[0],"p",1);
    read(fds2[1],buf,sizeof(buf));
    // 为什么这里要等待
    wait(&status);
    fprintf(2, "%d: received ping\n",process);
    
  }

  exit(0);
}