# Code Modification Report
Berikut adalah bagian-bagian kode yang diubah untuk project 2

## Makefile
Line 3 :
```C
CS333_PROJECT ?= 2
```
## syscall.c
Line 113 - 120:
```C
#ifdef CS333_P2
extern int sys_getuid(void);
extern int sys_getgid(void);
extern int sys_getppid(void);
extern int sys_setuid(void);
extern int sys_setgid(void);
extern int sys_getprocs(void);
#endif // CS333_P2
```
Line 150 - 157:
```C
#ifdef CS333_P2
[SYS_getuid]  sys_getuid,
[SYS_getgid]  sys_getgid,
[SYS_getppid] sys_getppid,
[SYS_setuid]  sys_setuid,
[SYS_setgid]  sys_setgid,
[SYS_getprocs]  sys_getprocs,
#endif
```
Line 189 - 196:
```C
#ifdef CS333_P2
  [SYS_getuid]  "getuid",
  [SYS_getgid]  "getgid",
  [SYS_getppid] "getppid",
  [SYS_setuid]  "setuid",
  [SYS_setgid]  "setgid",
  [SYS_getprocs]  "getprocs",
#endif
```

## user.h
Line 31 - 38:
```C
#ifdef CS333_P2
uint getuid(void);
uint getgid(void);
uint getppid(void);
int setuid(uint);
int setgid(uint);
int getprocs(uint max, struct uproc* table);
#endif
```

## usys.s
Line 34-39:
```
SYSCALL(getuid)
SYSCALL(getgid)
SYSCALL(getppid)
SYSCALL(setuid)
SYSCALL(setgid)
SYSCALL(getprocs)
```

## syscall.h
Line 26-32:
```C
#define SYS_getuid  SYS_date+1
#define SYS_getgid  SYS_getuid+1
#define SYS_getppid SYS_getgid+1
#define SYS_setuid  SYS_getppid+1
#define SYS_setgid  SYS_setuid+1
#define SYS_getprocs  SYS_setgid+1
```

## sysproc.c
Line 114 - 173:
```C
#ifdef CS333_P2
uint
sys_getuid(void)
{
  return myproc()->uid;
}

uint
sys_getgid(void)
{
  return myproc()->gid;
}

uint
sys_getppid(void)
{
  if(!myproc()->parent){ // check if parent is null
    return myproc()->pid;
  }
  return myproc()->parent->pid;
}

int
sys_setuid(void)
{
  int uid;

  if(argint(0,&uid) < 0)
    return -1;
  if(uid < 0 || uid > 32767)
    return -1;
  myproc()->uid = uid;
  return 0;
}

int
sys_setgid(void)
{
  int gid;
  
  if(argint(0,&gid) < 0)
    return -1;
  if(gid < 0 || gid > 32767)
    return -1;
  myproc()->gid = gid;
  return 0;
}

int
sys_getprocs(void)
{
  uint max;
  struct uproc* table;
  if(argint(0, (void*)&max) < 0)
    return -1;
  if(argptr(1, (void*)&table, sizeof(&table) * max) < 0)
    return -1;
  return getprocs(max, table);
}
#endif
```

## proc.h
Line 55 - 60:
```C
#ifdef CS333_P2
    uint uid;                    // Process User ID
    uint gid;                    // Process Group ID
    uint cpu_ticks_total;        // Total elapsed ticks in CPU
    uint cpu_ticks_in;           // Ticks when scheduled 
  #endif
```

## proc.c
Line 9 - 11:
```C
#ifdef CS333_P2
#include "uproc.h"
#endif //CS333_P2
```
Line 157 - 161:
```C
//P2 - CPU time tracking
  #ifdef CS333_P2
    p->cpu_ticks_total = 0;
    p->cpu_ticks_in = 0;
  #endif
```
Line 199 - 202:
```C
#ifdef CS333_P2
    p->uid = DEFAULT_UID;
    p->gid = DEFAULT_GID;
#endif
```
Line 268 - 271:
```C
#ifdef CS333_P2
    np->uid = curproc->uid;
    np->gid = curproc->gid;
#endif
```
Line 410 - 412:
```C
#ifdef CS333_P2
  p->cpu_ticks_in = ticks;
#endif
```
Line 453 - 455:
```C
#ifdef CS333_P2
    p->cpu_ticks_total += (ticks - p->cpu_ticks_in);
#endif
```
Line 583 - 595:
```C
  int elapsed_in_ms = ticks - p->start_ticks;
  int elapsed_sec = elapsed_in_ms / 1000;
  int modulo_elapsed = elapsed_in_ms % 1000;
  int total_in_ms = p->cpu_ticks_total;
  int total_sec = total_in_ms / 1000;
  int modulo_total = total_in_ms % 1000;
  int ppid;
  if (p->parent) {
    ppid = p->parent->pid;
  } else {
    ppid = p->pid;
  }
  cprintf("%d\t%s\t\t%d\t%d\t%d\t%d.%d\t%d.%d\t%s\t%d\t", p->pid, p->name, p->uid, p->gid, ppid,elapsed_sec, modulo_elapsed, total_sec, modulo_total, state_string, p->sz);
```
Line 965 - 995:
```C
#ifdef CS333_P2
int
getprocs(uint max, struct uproc* table)
{
  int i = 0;
  struct proc* p;
  acquire(&ptable.lock);
  if(!table || max <= 0){
    release(&ptable.lock);
    return -1;
  }
  for(p = ptable.proc;p < &ptable.proc[NPROC];p++){
    if(i >= max)
      break;
    if(p->state != EMBRYO && p->state != UNUSED){
      table[i].pid = p->pid;
      table[i].uid = p->uid;
      table[i].gid = p->gid;
      table[i].ppid = (!p->parent) ? p->pid:p->parent->pid;
      table[i].elapsed_ticks = ticks - p->start_ticks;
      table[i].CPU_total_ticks = p->cpu_ticks_total;
      table[i].size = p->sz;
      safestrcpy(table[i].state, states[p->state], sizeof(table[i]).state);
      safestrcpy(table[i].name, p->name, sizeof(table[i]).name);
      i++;
    }
  }
  release(&ptable.lock);
  return i;
}
#endif 
```

## defs.h
Line 12 - 14:
```C
#ifdef CS333_P2
struct uproc;
#endif // CS333_P2
```
Line 130 - 132:
```C
#ifdef CS333_P2
int             getprocs(uint max, struct uproc* table);
#endif // CS333_P2
```

## ps.c
Line 1 - 50:
```C
#ifdef CS333_P2
#include "types.h"
#include "user.h"
#include "uproc.h"

int
main(void)
{
  struct uproc* table;
  int i;
  int catch = 0;
  uint max = 72;
  uint elapsed_second;
  uint modulo_elapsed;
  uint cpu_seconds;
  uint modulo_cpu;
  table = malloc(sizeof(struct uproc) * max);
  catch = getprocs(max, table);
  if (catch == -1) {
    printf(1, "\nError: Invalid max or NULL uproc table\n");
  }
  else {
    printf(1, "\nPID\tName\tUID\tGID\tPPID\tElapsed\tCPU\tState\tSize");
    for (i = 0;i < catch;++i) {
      elapsed_second = table[i].elapsed_ticks / 1000;
      modulo_elapsed = table[i].elapsed_ticks % 1000;
      cpu_seconds = table[i].CPU_total_ticks / 1000;
      modulo_cpu = table[i].CPU_total_ticks % 1000;
      printf(1, "\n%d\t%s\t%d\t%d\t%d\t%d.", table[i].pid, table[i].name, table[i].uid, table[i].gid, table[i].ppid, elapsed_second);
      if (modulo_elapsed < 10) {
        printf(1, "00");
      }
      else if (modulo_elapsed < 100) {
        printf(1, "0");
      }
      printf(1, "%d\t%d.", modulo_elapsed, cpu_seconds);
      if (modulo_cpu < 10) {
        printf(1, "00");
      }
      else if (modulo_cpu < 100) {
        printf(1, "0");
      }
      printf(1, "%d\t%s\t%d", modulo_cpu, table[i].state, table[i].size);
    }
    printf(1, "\n");
  }
  free(table);
  exit();
}
#endif
```

## testsetuid.c
Line 1 - 11:
```C
#ifdef CS333_P2
#include "types.h"
#include "user.h"

int
main(int argc, char *argv[])
{
  printf(1, "***** In %s: my uid is %d\n\n", argv[0], getuid());
  exit();
}
#endif
```

## time.c
Line 1 - 39:
```C
#ifdef CS333_P2
#include "types.h"
#include "user.h"

int
main(int argc, char* argv[])
{
    int t1 = 0;
    int t2 = 0;
    int elapsed_second = 0;
    int modulo_elapsed = 0;
    int pid = 0;
    ++argv;
    t1 = uptime();
    pid = fork();
    if (pid < 0) {
        printf(1, "Ran in 0.000 seconds\n");
        exit();
    }
    else if (pid == 0) {
        exec(argv[0], argv);
    }
    else {
        wait();
        t2 = uptime();
        elapsed_second = (t2 - t1) / 1000;
        modulo_elapsed = (t2 - t1) % 1000;
        printf(1, "%s ran in %d.", argv[0], elapsed_second);
        if (modulo_elapsed < 10) {
            printf(1, "00");
        }
        else if (modulo_elapsed < 100) {
            printf(1, "0");
        }
        printf(1, "%d seconds\n", modulo_elapsed);
    }
    exit();
}
#endif // CS333_P2
```