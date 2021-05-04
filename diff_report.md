# Code Modification Report
Berikut adalah bagian-bagian kode yang diubah

## syscall.c
Line 109 - 112:
```C
#ifdef CS333_P1
// internally, the function prototype must be ’int’ not ’uint’ for sys_date()
extern int sys_date(void);
#endif // CS333_P1
```
Line 139 - 141:
```C
#ifdef CS333_P1
[SYS_date]    sys_date,
#endif
```
Line 182 - 187:
```C
#ifdef CS333_P1
    #ifdef PRINT_SYSCALLS
    cprintf("%s -> %d\n",
            syscallnames[num], curproc->tf->eax);
    #endif
#endif
```

## user.h
Line 28 - 30:
```C
#ifdef CS333_P1
int date(struct rtcdate*);
#endif // CS333_P1
```

## usys.s
Line 33:
```
SYSCALL(date)
```

## syscall.h
Line 24:
```C
#define SYS_date    SYS_halt+1
```

## sysproc.c
Line 101 - 112:
```C
#ifdef CS333_P1
// shutdown QEMU
int
sys_date(void)
{
  struct rtcdate *d;
  if (argptr(0, (void*)&d, sizeof(struct rtcdate)) < 0 )
    return -1;
  cmostime(d);
  return 0;
}
#endif
```

## proc.h
Line 52 - 54:
```C
#ifdef CS333_P1
  uint start_ticks;
#endif
```

## proc.c
Line 151 - 153:
```C
#ifdef CS333_P1
  p->start_ticks = ticks;
#endif
```
Line 568 - 580:
```C
#ifdef CS333_P1
    int elapsed_in_ms = ticks - p->start_ticks;
    int elapsed_sec = elapsed_in_ms / 1000;
    int modulo_elapsed = elapsed_in_ms % 1000;
    char* zero = "";
    if (modulo_elapsed >= 10 && modulo_elapsed < 100) {
      zero = "0";
    }
    if (modulo_elapsed < 10) {
      zero = "00";
    }
    cprintf("%d\t%s\t\t%d.%s%d\t%s\t%d\t", p->pid, p->name, elapsed_sec, zero, modulo_elapsed, state_string, p->sz);
#endif // CS333_P1
```