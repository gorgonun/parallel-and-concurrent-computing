#include <stdio.h>
#include <stdlib.h>


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


int main(int argc, char * argv[]) {
    if(argc != 2) {
        printf("Uso: %s <intervalo>\n", argv[0]);
        return 1;
    };
    int interval = atoi(argv[1]);
    printf("Numeros primos: ");
    for(int i=1; i <= interval; i++) {
        if(is_prime(i)) {
            printf("%i ", i);
        }
    }
    printf("\n");
}
