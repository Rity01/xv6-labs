#include "types.h"
#include "param.h"
#include "memlayout.h"
#include "riscv.h"
#include "spinlock.h"
#include "proc.h"
#include "syscall.h"
#include "defs.h"

#include "sysinfo.h"


uint64
sys_sysinfo(void)
{
  struct sysinfo s;
  uint64 st; // user pointer to struct sysinfo

  if(argaddr(0, &st) < 0)
    return -1;

  uint64 totalproc = gettotalproc();
  uint64 totalfree = ktotalfree();
  s.freemem = totalfree;
  s.nproc = totalproc;
//   printf("return total:%d free:%d\n",totalproc,totalfree);
  struct proc *p = myproc();
  if(copyout(p->pagetable, st, (char *)&s, sizeof(s)) < 0)
      return -1;
  return 0;
}