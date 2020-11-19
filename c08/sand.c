#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/user.h>
#include <sys/stat.h>
#include <sys/ptrace.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/syscall.h>

int main(int argc, char **argv)
{
FILE *fp;
  char name[] = "/haha";
  if ((fp = fopen(name, "r")) == NULL)
  {
    printf("Не удалось открыть файл");
    getchar();
    return 0;
  } else {
    printf("Удалось открыть файл");
    }
  // открыть файл удалось
  fclose(fp);
  getchar();
 return 0;
}

