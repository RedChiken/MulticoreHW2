#include <stdio.h>
#include <omp.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

#define SIZE 200000

int isPrime(int x){
    int i = 2;
    if(x < 2){
        return 0;
    }
    while(i <= (int)(sqrt(x) + 0.5)){
        if((x % i) == 0){
            return 0;
        }
        i++;
    }
    return 1;
}

int static_Prime_Thread(int start, int end){
    int ret = 0;
    while(start < end){
        ret += isPrime(++start);
    }
    return ret;
}

int main(int argc, char *argv[]){
    int type = atoi(argv[1]);
    int threadNum = atoi(argv[2]);
    // printf("type = %d, threadNum = %d\n", type, threadNum);
    const int chunk = SIZE / threadNum;
    int ret = 0, iterator = 0;
    time_t before = clock();
    switch(type){
        case 1:
        //static
            #pragma omp parallel reduction(+:ret) num_threads(threadNum)
            {
                
                int n = omp_get_thread_num();
                ret += static_Prime_Thread(n * chunk, (n + 1) * chunk);
            }
            break;
        case 2:
        //dynamic
            #pragma omp parallel reduction(+:ret) num_threads(threadNum)
            {
                int i = 0;
                #pragma omp critical(iterator)
                {
                    while(iterator < SIZE){ 
                        ret += isPrime(++iterator);
                    }
                }
            }
            break;
        case 3:
        //guided
            #pragma omp parallel for schedule(guided) reduction(+:ret) num_threads(threadNum)
            for(int i = 0; i < SIZE; i++){
                ret += isPrime(i + 1);
            }
            break;
        default:
            printf("wrong type number.\n");
    }
    printf("%d\n", ret);
    printf("runtime: %d ms\n", (clock() - before) * 1000 / CLOCKS_PER_SEC);
}