#include <stdio.h>

typedef struct Data {
    int tab[20];
    int in_tab;   // liczba aktywnych elementów w tablicy tab
    int sum;      // suma aktywnych elementów 
} Data;

typedef struct ListElement {
    struct Data  data;
    struct ListElement *prev;
    struct ListElement *next;
} ListElement;

typedef struct List {
    struct ListElement *head;
    struct ListElement *tail;
    int no_elements;
} List;

void initList(List* plist)
{
    plist -> head = NULL;
    plist -> tail = NULL;
    plist -> no_elements = 0;
}

void addElement(List* plist, const Data* data)
{
    ListElement *element = malloc(sizeof(ListElement));
    element -> data = data;
    element -> prev = plist -> tail;
    element -> next = NULL;

    if(plist -> tail != NULL) plist -> tail -> next = element;

    if(plist -> no_elements == 0) plist -> head = element;
    plist -> tail = element;
    plist -> no_elements++;
}

void print_above(const List* plist,  int threshold)
{
    ListElement *element = plist -> head;
    while(element != NULL)
    {
        if(element -> data.sum > threshold)
        {
            for(int i=0;i<element->data.in_tab;i++)
            {
                printf("%d ", element -> data.tab[i]);
            }
        }
        element = element -> next;
    }
}

void print_if(const List* plist,  int (*pred)(const Data* data))
{
    ListElement *element = plist -> head;
    while(element != NULL)
    {
        if(pred(element -> data))
        {
            for(int i=0;i<element->data.in_tab;i++)
            {
                printf("%d ", element -> data.tab[i]);
            }
        }
        element = element -> next;
    }
}

int pred(const Data* data)
{
    return data -> sum % 2 == 0;
}

void test_print_if(const List* plist)
{
    print_if(plist, pred);
}

int main()
{

}