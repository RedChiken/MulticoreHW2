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
pthread_cond_t cond_wait;

void enter(int id){
    while(places < 0){
        pthread_mutex_lock(&mutex_lock);
        pthread_cond_wait(&cond_wait, &mutex_lock);
        pthread_mutex_unlock(&mutex_lock);
    }
    pthread_mutex_lock(&mutex_lock);
    places--;
    pthread_mutex_unlock(&mutex_lock);
    printf("Car %d: entered\n", id);
}

void leave(int id){
    pthread_mutex_lock(&mutex_lock);
    places++;
    pthread_cond_broadcast(&cond_wait);
    pthread_mutex_unlock(&mutex_lock);
    printf("Car %d: left\n", id);
}

void *car(void *threadNum){
    int *temp = threadNum;
    int id = (*temp) + 1;
    while(1){
        sleep(1);
        enter(id);
        sleep(2);
        leave(id);
    }
    pthread_exit(0);
    return 0;
}

int main(int argc, char *argv[]){
    int iter = 0;
    int status = 0;
    pthread_mutex_init(&mutex_lock, 0);
    pthread_cond_init(&cond_wait, 0);
    for(iter = 0; iter < MAX_CAR; iter++){
        if(pthread_create(&p_thread[iter], 0, car, (void *)&iter) < 0){
            perror("thread create error: ");
            exit(0);
        }
    }
    for(iter = 0; iter < MAX_CAR; iter++){
        pthread_join(p_thread[iter], (void **)&status);
    }
    pthread_mutex_destroy(&mutex_lock);
    pthread_cond_destroy(&cond_wait);
    return 0;
}