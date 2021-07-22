#include <pthread.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>


int clothes[0];
int repairClothes[10];

int buyCloth(int pos) {
    size_t n = sizeof(clothes)/sizeof(clothes[0]);
    int cloth = clothes[pos];
    for (int i = pos; i < n - 1; i ++) {
        clothes[i] = clothes[i + 1];
    }
    return cloth;
}

int donateCloth(int cloth) {
    size_t n = sizeof(repairClothes)/sizeof(repairClothes[0]);
    repairClothes[n] = cloth;
    return 0;
}

int moveClothFromRepair(int pos) {
    // Remove from repair
    size_t n = sizeof(repairClothes)/sizeof(repairClothes[0]);
    int cloth = repairClothes[pos];
    for (int i = pos; i < n - 1; i ++) {
        repairClothes[i] = repairClothes[i + 1];
    }
    
    // Add to clothes
    size_t clothesSize = sizeof(clothes)/sizeof(clothes[0]);
    clothes[clothesSize] = cloth;
    return 0;
}

void *client(void *arg) {
    srand(time(NULL));
    int current_cloth;
    int has_cloth = 0;

    int *clientId = (int *) arg;

    while(1) {
        sleep(rand() % 3);
        if (!has_cloth) {
            size_t n = sizeof(clothes)/sizeof(clothes[0]);
            if (n > 0) {
                int cloth = rand() % n;
                printf("Cliente %d compra roupa: %d\n", *clientId, cloth);
                current_cloth = buyCloth(cloth);
                has_cloth = 1;
            }
        } else {
            printf("Cliente %d doa roupa %d\n", *clientId, current_cloth);
            donateCloth(current_cloth);
            has_cloth = 0;
        }
    }
}

void *volunteer(void *arg) {
    srand(time(NULL));

    int *volunteerId = (int *) arg;

    while(1) {
        sleep(rand() % 3);
        int action = rand() % 3;
        size_t n;
        int cloth;

        switch (action)
        {
        case 1:
            n = sizeof(repairClothes)/sizeof(repairClothes[0]);
            cloth = rand() % n;
            printf("Voluntario %d adiciona roupa: %d\n", *volunteerId, cloth);
            moveClothFromRepair(cloth);
            break;
        
        case 2:
            cloth = rand();
            printf("Voluntario %d doa roupa: %d\n", *volunteerId, cloth);
            donateCloth(cloth);
            break;

        case 3:
            n = sizeof(repairClothes)/sizeof(repairClothes[0]);
            if (n > 0){
                buyCloth(0);
            }
            break;
        }
    }
}

int main() {
    int n_threads_clients = 2;
    int n_threads_volunteers = 2;
    pthread_t clients[n_threads_clients];
    pthread_t volunteers[n_threads_volunteers];

    for(int i = 0; i < n_threads_clients; i++) {
        printf("Criando thread de cliente %d\n", i);
        pthread_create(&clients[i], NULL, client, &i);
    }

    for(int i = 0; i < n_threads_volunteers; i++) {
        printf("Criando thread de voluntarios %d\n", i);
        pthread_create(&volunteers[i], NULL, volunteer, &i);
    }

    for(int i = 0; i < n_threads_clients; i++) {
        int res = pthread_join(clients[i], NULL);
    }

    for(int i = 0; i < n_threads_volunteers; i++) {
        int res = pthread_join(volunteers[i], NULL);
    }

    return 0;
}
