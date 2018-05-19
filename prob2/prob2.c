#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define MAX_PLACE 10
#define MAX_CAR 40

int places = MAX_PLACE;
pthread_t p_thread[MAX_CAR];
pthread_mutex_t mutex_lock;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

void enter(pthread_cond_t *cond){
    while(places == 0){
        pthread_cond_wait(cond, &mutex_lock);
    }
    pthread_mutex_lock(&mutex_lock);
    places--;
    pthread_mutex_unlock(&mutex_lock);
}

void leave(pthread_cond_t *cond){
    pthread_mutex_lock(&mutex_lock);
    places++;
    pthread_mutex_unlock(&mutex_lock);
    pthread_cond_broadcast(cond);
}

void *car(void *threadNum){
    int id = *((int *)threadNum);
    while(1){
        sleep(1);
        enter(&cond);
        printf("Car %d: entered\n", id);
        sleep(2);
        leave(&cond);
        printf("Car %d: left\n", id);
    }
}

int main(int argc, char *argv[]){
    int iter = 0, status = 0;
    pthread_mutex_init(&mutex_lock, 0);
    for(iter = 0; iter < MAX_CAR; iter++){
        int id = pthread_create(&p_thread[iter], 0, car, (void *)(iter + 1));
        if(id < 0){
            perror("thread create error: ");
            exit(0);
        }
    }
    for(iter = 0; iter < MAX_CAR; iter++){
        pthread_join(p_thread[iter], (void **)&status);
    }
    return 0;
}