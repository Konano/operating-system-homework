#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <cstring>
#include <unistd.h>
#include <string>
#include <cstdlib>
#include <ctime>

using namespace std;

#define SLEEP_SPAN 5
#define WORK_SPAN 4

unsigned int consumers = 0;

sem_t order, resource, counter;

struct arg_struct{
    arg_struct(int _id, int _start, int _work, string _indent): id(_id), start(_start), work(_work), indent(_indent){}
    arg_struct(int _id): id(_id), start(0), work(0), indent(string("")){}
    int id;
    int start;
    int work;
    string indent;
};

void* producer(void* argv) {
    arg_struct arg = *(arg_struct*)argv;
    int id = arg.id;
    const char* indent = arg.indent.c_str();

    sleep(arg.start);

    printf("%sSTART\n", indent);

    sem_wait(&order);
    printf("%saORDER\n", indent);
    sem_wait(&resource);
    printf("%saRESOURCE\n", indent);
    sem_post(&order);
    printf("%srORDER\n", indent);

    printf("%sENTER\n", indent);
    int time = rand()%SLEEP_SPAN;
    sleep(arg.work);

    sem_post(&resource);
    printf("%srRESOURCE\n", indent);

    return NULL;
}

void* consumer(void* argv) {
    arg_struct arg = *(arg_struct*)argv;
    int id = arg.id;
    const char* indent = arg.indent.c_str();

    sleep(arg.start);

    printf("%sSTART\n", indent);

    sem_wait(&order);
    printf("%saORDER\n", indent);
    sem_wait(&counter);
    printf("%saCOUNTER\n", indent);
    if (consumers == 0) {
        sem_wait(&resource);
        printf("%saRESOURCE\n", indent);
    }
    consumers += 1;
    sem_post(&order);
    printf("%srORDER\n", indent);
    sem_post(&counter);
    printf("%srCOUNTER\n", indent);

    printf("%sENTER\n", indent);
    int time = rand()%SLEEP_SPAN;
    sleep(arg.work);
    sem_post(&resource);

    sem_wait(&counter);
    printf("%saCOUNTER\n", indent);
    consumers -= 1;
    if (consumers == 0) {
        sem_post(&resource);
        printf("%srRESOURCE\n", indent);
    }
    sem_post(&counter);
    printf("%srCOUNTER\n", indent);

    return NULL;
}

int main() {
    srand(time(NULL));

    sem_init(&order, 0, 1);
    sem_init(&resource, 0, 1);
    sem_init(&counter, 0, 1);

    #define N 3
    pthread_t p_consumer[2 * N], p_producer[2 * N];

    int c_count = 0, p_count = 0;

    #define PRODUCER 0
    #define CONSUMER 1

    int st_time = 0;
    int inst[2 * N][3] = {
        {CONSUMER, st_time += 0, rand()%WORK_SPAN},
        {CONSUMER, st_time += 0, rand()%WORK_SPAN},
        {CONSUMER, st_time += 0, rand()%WORK_SPAN},
        {PRODUCER, st_time += 0, rand()%WORK_SPAN},
        {PRODUCER, st_time += 0, rand()%WORK_SPAN},
        {PRODUCER, st_time += 0, rand()%WORK_SPAN}
    };

    /* Print the first line */
    int tmp_c = 0, tmp_p = 0;
    for (int i = 0; i < 2 * N; i++){
        if (inst[i][0] == PRODUCER){
            printf("P%d\t", tmp_p++);
        } else if (inst[i][0] == CONSUMER){
            printf("C%d\t", tmp_c++);
        }
    }
    printf("\n");

    /* Create Producers and Consumers according to $inst*/
    int rc;
    string indent("");
    for (int i = 0; i < 2 * N; i++){
        if (inst[i][0] == PRODUCER){
            rc = pthread_create(p_producer + p_count, NULL, producer, new arg_struct(p_count, inst[i][1], inst[i][2], indent));
            if (rc) printf("ERROR\n");
            p_count++;
        } else if (inst[i][0] == CONSUMER){
            rc = pthread_create(p_consumer + c_count, NULL, consumer, new arg_struct(c_count, inst[i][1], inst[i][2], indent));
            if (rc) printf("ERROR\n");
            c_count++;
        }
        indent += '\t';
    }

    /* wait until every thread finishes*/
    for (int i = 0; i < p_count; i++){
        pthread_join(p_producer[i], NULL);
    }
    for (int i = 0; i < c_count; i++){
        pthread_join(p_consumer[i], NULL);
    }

    return 0;
}