#include <semaphore.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>


sem_t agentSem;
sem_t tobacco;
sem_t paper;
sem_t match;
sem_t tobaccoSem;
sem_t paperSem;
sem_t matchSem;
sem_t mutex;

int isTobacco = 0;
int isPaper = 0;
int isMatch = 0;

void *agent() {
    int func;
    while(1) {
        sem_wait(&agentSem);
        func = rand() % 3;
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

void *pusher_tobacco() {
    while (1) {
        sem_wait(&tobacco);
        sem_wait(&mutex);

        printf("Agent gives tobacco\n");
        if (isPaper) {
            isPaper = 0;
            sem_post(&matchSem);
        } else if (isMatch) {
            isMatch = 0;
            sem_post(&paperSem);
        } else {
            isTobacco = 1;
        }
        sem_post(&mutex);
    }
}

void *pusher_paper() {
    while (1) {
        sem_wait(&paper);
        sem_wait(&mutex);

        printf("Agent gives paper\n");
        if (isTobacco) {
            isTobacco = 0;
            sem_post(&matchSem);
        } else if (isMatch) {
            isMatch = 0;
            sem_post(&tobaccoSem);
        } else {
            isPaper = 1;
        }
        sem_post(&mutex);
    }
}

void *pusher_match() {
    while (1) {
        sem_wait(&match);
        sem_wait(&mutex);

        printf("Agent gives match\n");
        if (isTobacco) {
            isTobacco = 0;
            sem_post(&paperSem);
        } else if (isPaper) {
            isPaper = 0;
            sem_post(&tobaccoSem);
        } else {
            isMatch = 1;
        }
        sem_post(&mutex);
    }
}

void *smoker_paper() {
    while (1) {
        sem_wait(&paperSem);
        printf("Making a cigarret using owner's paper\n");
        sem_post(&agentSem);
        printf("Smoking...\n");
    }
}

void *smoker_tobacco() {
    while (1) {
        sem_wait(&tobaccoSem);
        printf("Making a cigarret using owner's tobacco\n");
        sem_post(&agentSem);
        printf("Smoking...\n");
    }
}

void *smoker_match() {
    while (1) {
        sem_wait(&matchSem);
        printf("Making a cigarret using owner's match\n");
        sem_post(&agentSem);
        printf("Smoking...\n");
    }
}

void main() {
    srand(time(NULL));
    sem_init(&agentSem, 0, 1);
    sem_init(&tobacco, 0, 0);
    sem_init(&paper, 0, 0);
    sem_init(&match, 0, 0);
    sem_init(&mutex, 0, 1);
    pthread_t smoker_a_t;
    pthread_t smoker_b_t;
    pthread_t smoker_c_t;
    pthread_t pusher_a_t;
    pthread_t pusher_b_t;
    pthread_t pusher_c_t;
    pthread_t agent_t;

    pthread_create(&smoker_a_t, NULL, smoker_tobacco, NULL);
    pthread_create(&smoker_b_t, NULL, smoker_paper, NULL);
    pthread_create(&smoker_c_t, NULL, smoker_match, NULL);
    pthread_create(&pusher_a_t, NULL, pusher_tobacco, NULL);
    pthread_create(&pusher_b_t, NULL, pusher_paper, NULL);
    pthread_create(&pusher_c_t, NULL, pusher_match, NULL);
    pthread_create(&agent_t, NULL, agent, NULL);

    pthread_join(smoker_a_t, NULL);
    pthread_join(smoker_b_t, NULL);
    pthread_join(smoker_c_t, NULL);
    pthread_join(pusher_a_t, NULL);
    pthread_join(pusher_b_t, NULL);
    pthread_join(pusher_c_t, NULL);
    pthread_join(agent_t, NULL);
}
