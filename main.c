#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <semaphore.h>
#include <unistd.h>

pthread_mutex_t count_mutex;
pthread_spinlock_t count_spin;
sem_t count_sem;

static volatile int counter = 0;
clock_t startTime, endTime;
double totalTimeTaken;

int lockType = 1; //1: mutex, 2: spinlock, 3: semaphore

void *mythread(void *arg){

	switch(lockType){
		case 1: pthread_mutex_lock(&count_mutex); break;
		case 2: pthread_spin_lock(&count_spin); break;
		case 3: sem_wait(&count_sem); break;
	}
  

	printf("%s:begin \n", (char *) arg);
	int i;
	for(i=0; i< 1e7; i++){
		counter++;
	}
	printf("%s: done\n",(char *) arg );
  
	switch(lockType){
		case 1: pthread_mutex_unlock(&count_mutex); break;
		case 2: pthread_spin_unlock(&count_spin); break;
		case 3: sem_post(&count_sem); break;
	}
}

int main(int argc, char *argv[]) {

	int ret;
	pthread_t p1, p2, p3, p4, p5;
	startTime = clock();
	
	if (lockType == 1) {
		printf("Using Mutex\n");
        ret = pthread_mutex_init(&count_mutex, NULL);
        if (ret != 0){
			printf("pthread_mutex_init\n");
			return 0;
		}
    } else if (lockType == 2){
		printf("Using Spinlock\n");
        ret = pthread_spin_init(&count_spin, 0);
        if (ret != 0){
			printf("pthread_spin_init\n");
			return 0;
		}
    } else if (lockType == 3){
		printf("Using Semaphore\n");
        ret = sem_init(&count_sem, 0, 1);
        if (ret != 0){
			printf("sem_init\n");
			return 0;
		}
    }
    
    printf("main: begin (counter = %d)\n", counter);
    
	pthread_create(&p1, NULL, mythread, "A");
	pthread_create(&p2, NULL, mythread, "B");
	pthread_create(&p3, NULL, mythread, "C");
	pthread_create(&p4, NULL, mythread, "D");
	pthread_create(&p5, NULL, mythread, "E");

	pthread_join(p1, NULL);
	pthread_join(p2, NULL);
	pthread_join(p3, NULL);
	pthread_join(p4, NULL);
	pthread_join(p5, NULL);
  
	endTime = clock();
	totalTimeTaken = ((double) (endTime - startTime))/CLOCKS_PER_SEC;
  
	printf("main: end (counter = %d)\n", counter);
	printf("Total Time Taken: %f\n", totalTimeTaken);

	return 0;
}
