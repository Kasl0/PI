#include <stdio.h>

void copy(int *dest, int *source, int no_elements)
{
    for(int i=0;i<no_elements;i++)
    {
        *dest = *source;
        dest++;
        source++;
    }
}

int zero_counter(int *tab, int no_elements)
{
    int counter = 0;
    for(int i=0;i<no_elements;i++)
    {
        if(*tab == 0) counter++;
        tab++;
    }
    return counter;
}

int main()
{
    int numbers[] = {0,2,3};
    int tab[] = {-1,-1,-1};
    copy(tab, numbers, 3);
    printf("%d %d %d", tab[0], tab[1], tab[2]);
}