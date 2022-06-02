#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"


// 这里需要补全，但是也有字符串超过限制，理解是怎么处理这一点的，先把补全的作一下修改
char*
fmtname(char *path)
{
  static char buf[DIRSIZ+1];
  char *p;

  // Find first character after last slash.
  for(p=path+strlen(path); p >= path && *p != '/'; p--)
    ;
  p++;

  // Return blank-padded name.
  if(strlen(p) >= DIRSIZ)
    return p;
  memmove(buf, p, strlen(p));
  // memset(buf+strlen(p), ' ', DIRSIZ-strlen(p));
  memset(buf+strlen(p), '\0', 1);
  return buf;
}


void
find(char *path,char* target){
  char buf[512], *p;
  int fd;
  struct dirent de;
  struct stat st;

  if((fd = open(path, 0)) < 0){
    fprintf(2, "find: cannot open %s\n", path);
    return;
  }

  if(fstat(fd, &st) < 0){
    fprintf(2, "find: cannot stat %s\n", path);
    close(fd);
    return;
  }

  switch(st.type){
  case T_FILE:
    // printf("%s %d %d %l\n", fmtname(path), st.type, st.ino, st.size);
    // printf("%s : %s\n",fmtname(path),target);
    if(strcmp(fmtname(path),target) == 0){
        printf("%s\n",path);
    }
    break;

  case T_DIR:
    // printf("it is a path\n");
    if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
      // printf("ls: path too long\n");
      break;
    }
    strcpy(buf, path);
    p = buf+strlen(buf);
    *p++ = '/';
    while(read(fd, &de, sizeof(de)) == sizeof(de)){
      if(de.inum == 0)
        continue;
      if(strcmp(de.name,".") == 0 || strcmp(de.name,"..") == 0){
        continue;
      }
      memmove(p, de.name, DIRSIZ);
      p[DIRSIZ] = 0;
      // printf("it is a %s\n",buf);
      find(buf,target);
    }
    break;
  }
  close(fd);

}

int
main(int argc, char *argv[])
{

  if(argc < 3){
      fprintf(2, "find needs a path and a target\n");
      exit(1);
  }
  // 如何处理超过两参数没有规定
  find(argv[1],argv[2]);
  exit(0);
}