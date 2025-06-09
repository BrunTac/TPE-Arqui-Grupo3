#include "tests.h"
#include "libc.h"
#include <sys_calls.h>
#include <stdint.h>
#include <stdlib.h>

extern void opcodeError();
void test_opcode_exep();
void test_divzero_exep();

//
// test_util.c
//

// Random
static uint32_t m_z = 362436069;
static uint32_t m_w = 521288629;

uint32_t GetUint() {
  m_z = 36969 * (m_z & 65535) + (m_z >> 16);
  m_w = 18000 * (m_w & 65535) + (m_w >> 16);
  return (m_z << 16) + m_w;
}

uint32_t GetUniform(uint32_t max) {
  uint32_t u = GetUint();
  return (u + 1.0) * 2.328306435454494e-10 * max;
}

// Memory
uint8_t memcheck(void *start, uint8_t value, uint32_t size) {
  uint8_t *p = (uint8_t *)start;
  uint32_t i;

  for (i = 0; i < size; i++, p++)
    if (*p != value)
      return 0;

  return 1;
}

// Parameters
int64_t satoi(char *str) {
  uint64_t i = 0;
  int64_t res = 0;
  int8_t sign = 1;

  if (!str)
    return 0;

  if (str[i] == '-') {
    i++;
    sign = -1;
  }

  for (; str[i] != '\0'; ++i) {
    if (str[i] < '0' || str[i] > '9')
      return 0;
    res = res * 10 + str[i] - '0';
  }

  return res * sign;
}

// Dummies
void bussy_wait(uint64_t n) {
  uint64_t i;
  for (i = 0; i < n; i++)
    ;
}

void endless_loop() {
  while (1)
    ;
}

void endless_loop_print(uint64_t wait) {
  int64_t pid = sys_getPid();

  while (1) {
    printf("%d ", pid);
    bussy_wait(wait);
  }
}



//
// test_mm.c
//

#define MAX_BLOCKS 128

typedef struct MM_rq {
  void *address;
  uint32_t size;
} mm_rq;

void test_mm(uint64_t argc, char *argv[]) {

  mm_rq mm_rqs[MAX_BLOCKS];
  uint8_t rq;
  uint32_t total;
  uint64_t max_memory;

  if (argc != 1) {
    printf("test_mm error\n");
    return;
  }

  if ((max_memory = satoi(argv[0])) <= 0) {
    printf("test_mm error\n");
    return;
  }

  while (1) {
    rq = 0;
    total = 0;

    // Request as many blocks as we can
    while (rq < MAX_BLOCKS && total < max_memory) {
      mm_rqs[rq].size = GetUniform(max_memory - total - 1) + 1;
      mm_rqs[rq].address = sys_malloc(mm_rqs[rq].size);

      if (mm_rqs[rq].address) {
        total += mm_rqs[rq].size;
        rq++;
      }
    }

    // Set
    uint32_t i;
    for (i = 0; i < rq; i++)
      if (mm_rqs[i].address)
        sys_memset(mm_rqs[i].address, i, mm_rqs[i].size);

    // Check
    for (i = 0; i < rq; i++)
      if (mm_rqs[i].address)
        if (!memcheck(mm_rqs[i].address, i, mm_rqs[i].size)) {
          printf("test_mm ERROR\n");
          return;
        }

    // Free
    for (i = 0; i < rq; i++)
      if (mm_rqs[i].address)
        sys_free(mm_rqs[i].address);
  }
}

void testmm() {
  char *args[] = { "100", NULL };  // or whatever limit you want
  test_mm(1, args);
}

//
// test_prio.c
//

#define MINOR_WAIT 1000000 // TODO: Change this value to prevent a process from flooding the screen
#define WAIT 10000000      // TODO: Change this value to make the wait long enough to see theese processes beeing run at least twice

#define TOTAL_PROCESSES 3
#define LOWEST 0  // TODO: Change as required
#define MEDIUM 1  // TODO: Change as required
#define HIGHEST 2 // TODO: Change as required
#define FD_AMOUNT 3

int64_t prio[TOTAL_PROCESSES] = {LOWEST, MEDIUM, HIGHEST};

void testprio() {
  int64_t pids[TOTAL_PROCESSES];
  char *argv[] = {0};
  uint64_t i;
  uint8_t defaultFds[FD_AMOUNT] = {0, 1, 2};

  for (i = 0; i < TOTAL_PROCESSES; i++)
    pids[i] = sys_createProcess((function) endless_loop_print, 0, argv, 0, "endless_print_loop", defaultFds);

  bussy_wait(WAIT);
  printf("\nCHANGING PRIORITIES...\n");

  for (i = 0; i < TOTAL_PROCESSES; i++)
    sys_changePriority(pids[i], prio[i]);

  bussy_wait(WAIT);
  printf("\nBLOCKING...\n");

  for (i = 0; i < TOTAL_PROCESSES; i++)
    sys_blockProcess(pids[i]);

  printf("CHANGING PRIORITIES WHILE BLOCKED...\n");

  for (i = 0; i < TOTAL_PROCESSES; i++)
    sys_changePriority(pids[i], MEDIUM);

  printf("UNBLOCKING...\n");

  for (i = 0; i < TOTAL_PROCESSES; i++)
    sys_unblockProcess(pids[i]);

  bussy_wait(WAIT);
  printf("\nKILLING...\n");

  for (i = 0; i < TOTAL_PROCESSES; i++)
    sys_killProcess(pids[i]);
}

//
// test_processes.c
//

typedef struct P_rq {
  int32_t pid;
  Status state;
} p_rq;

int64_t test_processes(uint64_t argc, char *argv[]) {
  uint8_t rq;
  uint8_t alive = 0;
  uint8_t action;
  uint64_t max_processes;
  char *argvAux[] = {0};
  uint8_t defaultFds[FD_AMOUNT] = {0, 1, 2};

  if (argc != 1)
    return -1;

  if ((max_processes = satoi(argv[0])) <= 0)
    return -1;

  p_rq p_rqs[max_processes];

  while (1) {

    // Create max_processes processes
    for (rq = 0; rq < max_processes; rq++) {
      p_rqs[rq].pid = sys_createProcess((function) endless_loop, 0, argvAux, 3, "endless_loop", defaultFds);

      if (p_rqs[rq].pid == -1) {
        printf("test_processes: ERROR creating process\n");
        return -1;
      } else {
        p_rqs[rq].state = READY;
        alive++;
      }
    }

    // Randomly kills, blocks or unblocks processes until every one has been EXITED
    while (alive > 0) {

      for (rq = 0; rq < max_processes; rq++) {
        action = GetUniform(100) % 2;

        switch (action) {
          case 0:
            if (p_rqs[rq].state == READY || p_rqs[rq].state == BLOCKED) {
              if (sys_killProcess(p_rqs[rq].pid) == -1) {
                printf("test_processes: ERROR killing process\n");
                return -1;
              }
              p_rqs[rq].state = EXITED;
              alive--;
            }
            break;

          case 1:
            if (p_rqs[rq].state == READY) {
              if (sys_blockProcess(p_rqs[rq].pid) == -1) {
                printf("test_processes: ERROR blocking process\n");
                return -1;
              }
              p_rqs[rq].state = BLOCKED;
            }
            break;
        }
      }

      // Randomly unblocks processes
      for (rq = 0; rq < max_processes; rq++)
        if (p_rqs[rq].state == BLOCKED && GetUniform(100) % 2) {
          if (sys_unblockProcess(p_rqs[rq].pid) == -1) {
            printf("test_processes: ERROR unblocking process\n");
            return -1;
          }
          p_rqs[rq].state = READY;
        }
    }
  }
}

int64_t testprocesses() {
  char * argv[] = {"10"};
  return test_processes(1, argv);
}

//
// test_exception.c
//

void test_exception(uint64_t argc, char * argv[]){
    char * arg = argv[1];
    if(strcmp(arg, "opcode") == 0){
        test_opcode_exep();
    }else if(strcmp(arg, "divzero") == 0){
        test_divzero_exep();
    }else{
        printf("%nInvalid Argument. Please enter one of the following options:%n");
        printf("1. opcode%n2. divzero%n");
    }
}
void test_opcode_exep(){
    opcodeError();
}
void test_divzero_exep(){
    int a = 5 / 0;
}

//
// test_sync.c
//

#define SEM_ID "sem"
#define TOTAL_PAIR_PROCESSES 2

int64_t global; // shared memory

void slowInc(int64_t *p, int64_t inc) {
  uint64_t aux = *p;
  sys_yieldAll(); // This makes the race condition highly probable
  aux += inc;
  *p = aux;
}

uint64_t my_process_inc(uint64_t argc, char *argv[]) {
  uint64_t n;
  int8_t inc;
  int8_t use_sem;

  if (argc != 3)
    return -1;

  if ((n = satoi(argv[0])) <= 0)
    return -1;
  if ((inc = satoi(argv[1])) == 0)
    return -1;
  if ((use_sem = satoi(argv[2])) < 0)
    return -1;

  if (use_sem)
    if (!sys_openSem(SEM_ID, 1)) {
      printf("test_sync: ERROR opening semaphore\n");
      return -1;
    }

  uint64_t i;
  for (i = 0; i < n; i++) {
    if (use_sem)
      sys_waitSem(SEM_ID);
    slowInc(&global, inc);
    if (use_sem)
      sys_waitSem(SEM_ID);
  }

  if (use_sem)
    sys_closeSem(SEM_ID);

  return 0;
}

uint64_t test_sync(uint64_t argc, char *argv[]) { //{n, use_sem, 0}
  uint64_t pids[2 * TOTAL_PAIR_PROCESSES];
  uint8_t defaultFds[FD_AMOUNT] = {0, 1, 2};

  if (argc != 2)
    return -1;

  char *argvDec[] = {argv[0], "-1", argv[1], NULL};
  char *argvInc[] = {argv[0], "1", argv[1], NULL};

  global = 0;

  uint64_t i;
  for (i = 0; i < TOTAL_PAIR_PROCESSES; i++) {
    pids[i] = sys_createProcess(my_process_inc, 3, argvDec, 1, "my_process_inc", defaultFds);
    pids[i + TOTAL_PAIR_PROCESSES] = sys_createProcess(my_process_inc, 3, argvDec, 1, "my_process_inc", defaultFds);
  }

  for (i = 0; i < TOTAL_PAIR_PROCESSES; i++) {
    sys_waitPid(pids[i]);
    sys_waitPid(pids[i + TOTAL_PAIR_PROCESSES]);
  }

  printf("Final value: %d\n", global);

  return 0;
}

uint64_t testsync() {
  char * argv[] = {2, 1};
  test_sync(2, argv);
}

uint64_t testnosync() {
  char * argv[] = {2, 0};
  test_sync(2, argv);
}