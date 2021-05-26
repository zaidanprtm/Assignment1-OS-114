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