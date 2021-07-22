#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>


int g_interval = 0;
int g_division = 0;


typedef struct {
    int previous;
    int thread_id;
} getNextNumberParams;


int get_next_number(getNextNumberParams params) {
    if(params.previous < 0) {
        return params.thread_id;
    };
    int next = params.previous + g_division;
    if(next > g_interval) {
        return 0;
    }
    return next;
}


int is_prime(int n) {
    // slow algorithm to make it easier to note the velocity
    int divisions = 0;
    for(int i=1; i <= n; i++) {
        if(n % i == 0) {
            divisions++;
            if(divisions > 2) {
                return 0;
            };
        };
    }
    return 1;
}


void *calculate_numbers(void *arg) {
    getNextNumberParams *params = (getNextNumberParams *) arg;
    int target = get_next_number(*params);
    while(target) {
        if(is_prime(target)){
            printf("%i ", target);
        }
        params->previous = target;
        target = get_next_number(*params);
    };
}


int main(int argc, char * argv[]) {
    if(argc != 3) {
        printf("Uso: %s <intervalo> <paralelismo>\n", argv[0]);
        return 1;
    };
    int interval = atoi(argv[1]);
    int n_threads = atoi(argv[2]);
    pthread_t prime_t[n_threads];
    g_interval = interval;
    g_division = n_threads;
    getNextNumberParams threads_data[n_threads];

    for(int i=0; i < n_threads; i++) {
        threads_data[i].thread_id = i + 1;
        threads_data[i].previous = -1;
    }

    printf("Numeros primos: ");

    for(int i = 0; i < n_threads; i++) {
        printf("Creating thread %i", i);
        pthread_create(&prime_t[i], NULL, calculate_numbers, (void *) &threads_data[i]);
    }

    for(int i = 0; i < n_threads; i++) {
        pthread_join(prime_t[i], NULL);
    }

    printf("\n");
}
