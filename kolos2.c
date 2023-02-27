#include <stdio.h>
#include <stdlib.h>

typedef struct  {
    double *results;
    int len;
} Data;

typedef struct tagQueueElement { 
    Data *data;
    QueueElement *prev;
    QueueElement *next;

} QueueElement;

typedef struct tagQueue {
    QueueElement *first;
    QueueElement *last;
    int no_elements;
} Queue;

void free_queue(Queue *pqueue, int ***ptable, int *psize, const int delta)
{
    QueueElement *element = pqueue ->first;
    while(element != NULL)
    {
        free(element);
        element = element ->next;
    }
    free(pqueue);
}

void push_copy(Queue *pqueue, const Data*pdata)
{
    QueueElement *new_element = malloc(sizeof(QueueElement));
    new_element -> data = malloc(sizeof(Data));
    memcpy(new_element -> data, pdata, sizeof(Data));

    pqueue -> last -> next = new_element;
    new_element -> prev = pqueue -> last;
    new_element -> next = NULL;
    pqueue -> last = new_element;
    pqueue -> no_elements++;
}

Data peek(const Queue *pqueue)
{
    if(pqueue -> no_elements == 0)
    {
        Data *element = malloc(sizeof(Data));
        element -> results = NULL;
        element -> len = 0;
        return element;
    }
    return pqueue -> first -> data;
}

int get_size(const Queue *pqueue)
{
    return pqueue -> no_elements;
}

int main(void)
{
    printf("test");
}