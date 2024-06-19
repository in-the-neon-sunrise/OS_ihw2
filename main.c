#include <fcntl.h>
#include <semaphore.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <time.h>
#include <unistd.h>


int pid = 0;

// имя области разделяемой памяти
const char *shar_object = "/posix-shar-object";

// имя семафора для первого отдела
const char *first_sem_name = "/first-department-semaphore";
sem_t *first; // указатель на семафор

// имя семафора для второго отдела
const char *second_sem_name = "/second-department-semaphore";
sem_t *second; // указатель на семафор

// Функция, осуществляющая при запуске манипуляции с памятью и семафорами
void my_init() {

  if ((first = sem_open(first_sem_name, O_CREAT, 0666, 1)) == 0) {
    perror("sem_open: Can not create first department semaphore");
    exit(-1);
  };

  if ((second = sem_open(second_sem_name, O_CREAT, 0666, 1)) == 0) {
    perror("sem_open: Can not create second department semaphore");
    exit(-1);
  };

  int shmid = shm_open(shar_object, O_CREAT | O_RDWR, 0666);
  if (shmid == -1) {
    perror("shm_open");
    exit(-1);
  }
  if (ftruncate(shmid, sizeof(int)) == -1) {
    perror("ftruncate");
    exit(-1);
  }
  honey_portions = mmap(0, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, shmid, 0);
  if (honey_portions == MAP_FAILED) {
    perror("mmap");
    exit(-1);
  }
}

// Функция, закрывающая семафоры и разделяемую память
void my_close(void) {
  if (sem_close(first) == -1) {
    perror("sem_close: Incorrect close of first department semaphore");
    exit(-1);
  };
  if (sem_close(second) == -1) {
    perror("sem_close: Incorrect close of second department semaphore");
    exit(-1);
  };
}

// Функция, удаляющая все семафоры и разделяемую память
void my_unlink(void) {
  if (sem_unlink(first_sem_name) == -1) {
    perror("sem_unlink: Incorrect unlink of semaphore");
    exit(-1);
  };
  if (sem_unlink(second_sem_name) == -1) {
    perror("sem_unlink: Incorrect unlink of semaphore");
    exit(-1);
  }

  // удаление разделяемой памяти
  if (shm_unlink(shar_object) == -1) {
    perror("shm_unlink: shared memory");
    exit(-1);
  }
}

int my_rand(int min, int max) {
  //srand(time(NULL));
  return rand() % (max - min + 1) + min;
}

void sigint_handler() {
  my_close();
  if (pid == 0) {
    my_unlink();
    printf("You've stopped the program\n");
  }
  exit(0);
}
int main(int argc, char *argv[]) {
  int customers = 4;
  my_init();

  int check = 0;
  sem_getvalue(first, &check);
  printf("First department: %d\n", check);
  sem_getvalue(second, &check);
  printf("Second department: %d\n", check);

  int goods[customers][5];
  for (int i = 0; i < customers; i++) {
    goods[i][0]=my_rand(1, 2);
    for(int j = 1; j<5; j++){
      goods[i][j]=my_rand(0, 2);
    }
  }
  for (int i = 0; i < customers; i++) {
    printf("Customer %d: ", i + 1);
    for(int j = 0; j<5; j++){
      printf("%d ", goods[i][j]);
    }
    printf("\n");
  }
  int flag[] = {0, 0, 0, 0};
  for (int i = 0; i < customers; i++) {
    if ((pid = fork()) == 0) continue;
    for(flag[i] = 0; flag[i]<5; flag[i]++){
      if(goods[i][flag[i]]==1){
        sem_wait(first);
        sleep(my_rand(1, 6));
        printf("Customer %d, good %d, department 1\n", i+1, flag[i]+1);
        sem_post(first);
      }
      if(goods[i][flag[i]]==2){
        sem_wait(second);
        sleep(my_rand(1, 6));
        printf("Customer %d, good %d, department 2\n", i+1, flag[i]+1);
        sem_post(second);
      }
    }
    exit(0);
  }
  sigint_handler();
}
