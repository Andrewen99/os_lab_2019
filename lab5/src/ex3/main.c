#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

pthread_mutex_t lock1, lock2;

void *resource1(){
  pthread_mutex_lock(&lock1);

  printf("Муж: забираю правый тапочек\n");
  sleep(2);

  printf("Муж: пытаюсь забрать левый тапочек\n");
  pthread_mutex_lock(&lock2);
  printf("Муж: забрал левый тапочек\n");
  pthread_mutex_unlock(&lock2);

  printf("Муж: отдал левый тапочек\n");

  pthread_mutex_unlock(&lock1);

  printf("Муж: отдал правый тапочек\n");

  pthread_exit(NULL);
}

void *resource2(){

  pthread_mutex_lock(&lock2);

  printf("Жена: забираю левый тапочек\n");
  sleep(2);

  printf("Жена: пытаюсь забрать правый тапочек\n");
  pthread_mutex_lock(&lock1);
  printf("Жена: забрала правый тапочек\n");
  pthread_mutex_unlock(&lock1);

  printf("Жена: отдала правый тапочек\n");

  pthread_mutex_unlock(&lock2);

  printf("Жена: отдала левый тапочек\n");

  pthread_exit(NULL);
}

int main() {

  pthread_mutex_init(&lock1,NULL);
  pthread_mutex_init(&lock2,NULL);

  pthread_t t1,t2;

  pthread_create(&t1,NULL,resource1,NULL);
  pthread_create(&t2,NULL,resource2,NULL);

  pthread_join(t1,NULL);
  pthread_join(t2,NULL);

  return 0;
}

//This program executes in order, have a look
