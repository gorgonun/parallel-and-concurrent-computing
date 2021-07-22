#include <semaphore.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>


sem_t agentSem;
sem_t tobacco;
sem_t paper;
sem_t match;

void *agent() {
    int func;
    while(1) {
        sem_wait(&agentSem);
        func = rand() % 3;
        printf("%d", func);
        switch (func) {
            case 0:
                sem_post(&tobacco);
                sem_post(&paper);
                break;
            case 1:
                sem_post(&paper);
                sem_post(&match);
                break;
            case 2:
                sem_post(&tobacco);
                sem_post(&match);
                break;
            default:
                break;
        }
    sleep(3);
    };
}

void *smoker_paper() {
    while (1) {
        sem_wait(&tobacco);
        if (sem_trywait(&match)) {
            sem_post(&match); // rollback
        } else {
            printf("Making a cigarret using owner's paper\n");
            sem_post(&agentSem);
            printf("Smoking...\n");
        }
    }
}

void *smoker_tobacco() {
    while (1) {
        sem_wait(&paper);
        if (sem_trywait(&match)) {
            sem_post(&match); // rollback
        } else {
            printf("Making a cigarret using owner's tobacco\n");
            sem_post(&agentSem);
            printf("Smoking...\n");
        }
    }
}

void *smoker_match() {
    while (1) {
        sem_wait(&tobacco);
        if (sem_trywait(&paper)) {
            sem_post(&paper);
        } else {
            printf("Making a cigarret using owner's match\n");
            sem_post(&agentSem);
            printf("Smoking...\n");
        }
    }
}

void main() {
    srand(time(NULL));
    sem_init(&agentSem, 0, 1);
    sem_init(&tobacco, 0, 0);
    sem_init(&paper, 0, 0);
    sem_init(&match, 0, 0);
    pthread_t smoker_a_t;
    pthread_t smoker_b_t;
    pthread_t smoker_c_t;
    pthread_t agent_t;

    pthread_create(&smoker_a_t, NULL, smoker_tobacco, NULL);
    pthread_create(&smoker_b_t, NULL, smoker_paper, NULL);
    pthread_create(&smoker_c_t, NULL, smoker_match, NULL);
    pthread_create(&agent_t, NULL, agent, NULL);

    pthread_join(smoker_a_t, NULL);
    pthread_join(smoker_b_t, NULL);
    pthread_join(smoker_c_t, NULL);
    pthread_join(agent_t, NULL);
}
