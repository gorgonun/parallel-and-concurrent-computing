#include <pthread.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>

//STRUCT ROUPA
typedef struct
{
    int cod;
    char *mod;
    float price;
    char *size;
} clothing;

//LISTA ROUPAS PARA REPARO

typedef struct node Node;

struct node
{
    clothing data;
    Node *next;
    Node *prev;
};

Node *headRepair;
Node *lastRepair;
int sizeRepair;

void startListRepair()
{
    headRepair = NULL;
    lastRepair = NULL;
    sizeRepair = 0;
}

Node *allocate(clothing data)
{
    Node *new = (Node *)malloc(sizeof(Node));
    new->data = data;
    new->prev = NULL;
    new->next = NULL;
    return new;
}

clothing removeHeadRepair()
{
    Node *current = headRepair;
    headRepair = current->next;
    headRepair->prev = NULL;
    sizeRepair--;
    free(current);
    return current->data;
}

clothing removeLastRepair()
{
    Node *current = lastRepair;
    lastRepair = current->prev;
    lastRepair->next = NULL;
    free(current);
    sizeRepair--;
    return current->data;
}

void insertFirstRepair(clothing data)
{
    Node *new = allocate(data);
    if (headRepair == NULL)
    {
        headRepair = new;
        lastRepair = new;
    }
    else
    {
        Node *current;
        current = headRepair;
        new->next = current;
        current->prev = new;
        headRepair = new;
    }
    sizeRepair++;
}

void insertLastRepair(clothing data)
{
    Node *new = allocate(data);
    if (headRepair == NULL)
    {
        headRepair = new;
        lastRepair = new;
    }
    else
    {
        Node *current;
        current = lastRepair;
        new->prev = current;
        current->next = new;
        lastRepair = new;
    }
    sizeRepair++;
}

void insertPosRepair(clothing data, int pos)
{
    pos = pos - 1;
    Node *new = allocate(data);
    Node *current;
    int i;

    if (pos == 0)
    {
        insertFirstRepair(data);
    }
    else
    {
        if (pos == sizeRepair || pos > sizeRepair)
        {
            insertLastRepair(data);
        }
        else
        {
            current = headRepair;
            for (i = 0; i < pos; i++)
            {
                current = current->next;
            }
            current->prev->next = new;
            new->prev = current->prev;
            new->next = current;
            current->prev = new;
            current = new;
            sizeRepair++;
        }
    }
}

clothing removePosRepair(int pos)
{
    pos = pos - 1;
    Node *current;
    int i;

    if (pos == 0)
    {
        return removeHeadRepair();
    }
    else
    {
        if (pos == sizeRepair - 1 || pos > sizeRepair)
        {
            return removeLastRepair();
        }
        else
        {
            current = headRepair;
            for (i = 0; i < pos; i++)
            {
                current = current->next;
            }
            current->prev->next = current->next;
            current->next->prev = current->prev;
            free(current);
            sizeRepair--;
            return current->data;
        }
    }
}

void printListRepair()
{
    Node *current;
    current = headRepair;
    while (current)
    {
        printf("COD: %d\n", current->data.cod);
        current = current->next;
    }
}

void printListSizeRepair()
{
    printf("List sizeRepair: %d\n", sizeRepair);
}

//LISTA DE VENDA

Node *headSell;
Node *lastSell;
int sizeSell;

void startListSell()
{
    headSell = NULL;
    lastSell = NULL;
    sizeSell = 0;
}

clothing removeHeadSell()
{
    Node *current = headSell;
    headSell = current->next;
    if (headSell != NULL) {
        headSell->prev = NULL;
    }
    free(current);
    sizeSell--;
    return current->data;
}

clothing removeLastSell()
{
    Node *current = lastSell;
    lastSell = current->prev;
    lastSell->next = NULL;
    free(current);
    sizeSell--;
    return current->data;
}

void insertFirstSell(clothing data)
{
    Node *new = allocate(data);
    if (headSell == NULL)
    {
        headSell = new;
        lastSell = new;
    }
    else
    {
        Node *current;
        current = headSell;
        new->next = current;
        current->prev = new;
        headSell = new;
    }
    sizeSell++;
}

void insertLastSell(clothing data)
{
    Node *new = allocate(data);
    if (headSell == NULL)
    {
        headSell = new;
        lastSell = new;
    }
    else
    {
        Node *current;
        current = lastSell;
        new->prev = current;
        current->next = new;
        lastSell = new;
    }
    sizeSell++;
}

void insertPosSell(clothing data, int pos)
{
    pos = pos - 1;
    Node *new = allocate(data);
    Node *current;
    int i;

    if (pos == 0)
    {
        insertFirstSell(data);
    }
    else
    {
        if (pos == sizeSell || pos > sizeSell)
        {
            insertLastSell(data);
        }
        else
        {
            current = headSell;
            for (i = 0; i < pos; i++)
            {
                current = current->next;
            }
            current->prev->next = new;
            new->prev = current->prev;
            new->next = current;
            current->prev = new;
            current = new;
            sizeSell++;
        }
    }
}

clothing removePosSell(int pos)
{
    pos = pos - 1;
    Node *current;
    int i;

    if (pos == 0)
    {
        return removeHeadSell();
    }
    else
    {
        if (pos == sizeSell - 1 || pos >= sizeSell)
        {
            return removeLastSell();
        }
        else
        {
            current = headSell;
            for (i = 0; i < pos; i++)
            {
                current = current->next;
            }
            current->prev->next = current->next;
            current->next->prev = current->prev;
            free(current);
            sizeSell--;
            return current->data;
        }
    }
}

void printListSell()
{
    Node *current;
    current = headSell;
    while (current)
    {
        printf("COD: %d\n", current->data.cod);
        current = current->next;
    }
}

void printListSizeSell()
{
    printf("List size: %d\n", sizeSell);
}

pthread_mutex_t mutex;

clothing buyCloth(int pos)
{
    pthread_mutex_lock(&mutex);
    clothing cloth = removePosSell(pos);
    pthread_mutex_unlock(&mutex);
    return cloth;
}

int donateCloth(clothing cloth)
{
    pthread_mutex_lock(&mutex);
    insertLastRepair(cloth);
    pthread_mutex_unlock(&mutex);
    return 0;
}

int moveClothFromRepair(int pos)
{
    pthread_mutex_lock(&mutex);
    // Remove from repair
    clothing cloth = removePosRepair(pos);
    // Add to clothes
    insertLastSell(cloth);
    pthread_mutex_unlock(&mutex);
    return cloth.cod;
}

void *client(void *arg)
{
    srand(time(NULL));
    clothing current_cloth;
    int has_cloth = 0;

    int *clientId = (int *)arg;

    while (1)
    {
        sleep(rand() % 3);
        if (has_cloth == 0 && sizeSell > 0)
        {
            int pos = rand() % sizeSell + 1;
            current_cloth = buyCloth(pos);
            printf("Cliente %d compra roupa: %d\n", *clientId, pos);
            has_cloth = 1;
        }
        else if (has_cloth == 1)
        {
            donateCloth(current_cloth);
            printf("Cliente %d doa roupa %d\n", *clientId, sizeRepair);
            has_cloth = 0;
        }
    }
}

void *volunteer(void *arg)
{
    srand(time(NULL));

    int *volunteerId = (int *)arg;

    while (1)
    {
        sleep(rand() % 3);
        int action = rand() % 3 + 1;
        int pos;
        clothing cloth;

        switch (action)
        {
        case 1:
            pos = rand() % sizeRepair + 1;
            int cod = moveClothFromRepair(pos);
            printf("Voluntario %d adiciona roupa: %d\n", *volunteerId, pos);
            break;

        case 2:
            cloth.cod = rand();
            cloth.mod = "roupa";
            cloth.price = rand();
            cloth.size = "M";
            donateCloth(cloth);
            printf("Voluntario %d doa roupa: %d\n", *volunteerId, sizeRepair);
            break;

        case 3:
            if (sizeSell > 0)
            {
                pos = rand() % sizeSell + 1;
                if (pos > 0)
                {
                    buyCloth(pos);
                }
            }
            break;
        }
    }
}

int main()
{
    pthread_mutex_init(&mutex, NULL);

    //COLOCANDO ROUPAS NA LISTA
    clothing c1;
    c1.cod = 1;
    c1.mod = "camiseta";
    c1.price = 24.99;
    c1.size = "M";

    clothing c2;
    c2.cod = 2;
    c2.mod = "bermuda";
    c2.price = 38.50;
    c2.size = "GG";

    clothing c3;
    c3.cod = 3;
    c3.mod = "casaco";
    c3.price = 99.80;
    c3.size = "G";

    clothing c4;
    c4.cod = 4;
    c4.mod = "jaqueta";
    c4.price = 199.80;
    c4.size = "GG";

    startListRepair();
    insertPosRepair(c1, 1);
    insertPosRepair(c2, 2);
    insertPosRepair(c3, 3);
    insertPosRepair(c4, 4);
    insertPosRepair(c4, 1);
    insertPosRepair(c3, 2);

    startListSell();
    insertPosSell(c1, 1);
    insertPosSell(c2, 2);
    insertPosSell(c3, 3);
    insertPosSell(c4, 4);
    insertPosSell(c4, 1);
    insertPosSell(c3, 2);

    int n_threads_clients = 2;
    int n_threads_volunteers = 2;
    pthread_t clients[n_threads_clients];
    pthread_t volunteers[n_threads_volunteers];


    int id1 = 0;
    int id2 = 1;

    printf("Criando thread de cliente %d\n", id1);
    pthread_create(&clients[id1], NULL, client, &id1);

    printf("Criando thread de cliente %d\n", id2);
    pthread_create(&clients[id2], NULL, client, &id2);

    int ids_volunteers[n_threads_volunteers];
    for (int i = 0; i < n_threads_volunteers; i++)
    {
        ids_volunteers[i] = i;
        printf("Criando thread de voluntarios %d\n", i);
        pthread_create(&volunteers[i], NULL, volunteer, &ids_volunteers[i]);
    }

    for (int i = 0; i < n_threads_clients; i++)
    {
        int res = pthread_join(clients[i], NULL);
    }

    for (int i = 0; i < n_threads_volunteers; i++)
    {
        int res = pthread_join(volunteers[i], NULL);
    }
    pthread_mutex_destroy(&mutex);
    return 0;
}
