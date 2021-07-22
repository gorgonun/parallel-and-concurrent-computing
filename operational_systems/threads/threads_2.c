#include <pthread.h>
#include <stdio.h>


int GLOBAL_VAR = 0;


void *soma(void *arg) {
    int i = 0;
    while(i < 200){
        int *var = (int *) arg;
        int reg = *var;
        reg = reg + 1;
        *var = reg;
        printf("Resultado: %d\n", reg);

        i++;
    }
    pthread_exit(NULL);
}


int main() {
    int n_threads_soma = 10;
    pthread_t soma_t[n_threads_soma];

    int i;

    for(i = 0; i < n_threads_soma; i++) {
        printf("Criando thread de soma %d\n", i);
        pthread_create(&soma_t[i], NULL, soma, (void *) &GLOBAL_VAR);
    }

    for(i = 0; i < n_threads_soma; i++) {
        pthread_join(soma_t[i], NULL);
    }

    printf("Resultado: %d\n", GLOBAL_VAR);
    return 0;
}
