#include <stdio.h>
#include <pthread.h>


static volatile int counter = 0;
pthread_mutex_t count_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_spinlock_t lock;

void *mythread(void *arg){

  pthread_mutex_lock(&count_mutex);

  printf("%s:begin \n", (char *) arg);
  int i;
  for(i=0; i< 1e7; i++){
    counter++;
  }
  printf("%s: done\n",(char *) arg );
  pthread_mutex_unlock(&count_mutex);
}

int main(int argc, char *argv[]) {

  pthread_t p1, p2;
  printf("main: begin (counter = %d)\n", counter);


  pthread_create(&p1, NULL, mythread, "A");
  pthread_create(&p2, NULL, mythread, "B");

  pthread_join(p1, NULL);
  pthread_join(p2, NULL);
  printf("main: end (counter = %d)", counter);


  return 0;
}