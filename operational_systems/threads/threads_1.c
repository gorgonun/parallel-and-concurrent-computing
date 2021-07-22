#include <pthread.h>
#include <stdio.h>


int GLOBAL_VAR = 0;


void *soma(void *arg) {
    int *var = (int *) arg;
    *var = *var + 1;
    printf("Somando 1 em %d\n", *var);
    pthread_exit(NULL);
}

void *subtrai(void *arg) {
    int *var = (int *) arg;
    *var = *var - 1;
    printf("Subtraindo 1 em %d\n", *var);
    pthread_exit(NULL);
}


int main() {
    int n_threads_soma = 4;
    int n_threads_sub = 3;
    pthread_t soma_t[n_threads_soma];
    pthread_t subtracao_t[n_threads_sub];

    int i;

    for(i = 0; i < n_threads_soma; i++) {
        printf("Criando thread de soma %d\n", i);
        pthread_create(&soma_t[i], NULL, soma, (void *) &GLOBAL_VAR);
    }

    for(i = 0; i < n_threads_sub; i++) {
        printf("Criando thread de subtracao\n");
        pthread_create(&subtracao_t[i], NULL, subtrai, (void *) &GLOBAL_VAR);
    }

    for(i = 0; i < n_threads_soma; i++) {
        int res = pthread_join(soma_t[i], NULL);
        if (res == 0) {
            printf("O valor da variavel e: %d\n", GLOBAL_VAR);
        }
    }

    for(i = 0; i < n_threads_sub; i++) {
        int res = pthread_join(subtracao_t[i], NULL);
        if (res == 0) {
            printf("O valor da variavel e: %d\n", GLOBAL_VAR);
        }
    }

    printf("Resultado: %d\n", GLOBAL_VAR);
    return 0;
}
