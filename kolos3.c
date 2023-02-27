#include <stdio.h>
#include <stdlib.h>

typedef struct {
   int size; // liczba napisów (długość tablicy)
   char **strings; // tablica wskaźników do napisów
} StringTable;

void st_append(StringTable *p_dst, const StringTable *p_src);
void st_qsort(StringTable *st, int (*cmp)(const void*, const void*));

void st_free(StringTable *st)
{
	for(int i=0;i<st->size;i++)
	{
		free(st -> strings[i]);
	}
    st -> size = 0;
    free(st -> strings);
}

void st_add(StringTable *st, const char *txt)
{
    st -> size++;
    st -> strings = realloc(st -> strings, sizeof(char*) * st -> size)
    st -> strings[st -> size - 1] = malloc(sizeof(*txt));
    memcpy(st -> strings[st -> size - 1], txt, sizeof(*txt));
}

void st_split(StringTable *st, const char *txt, const char *sep)
{
    char *word;
    for(int i=0;i<st->size;i++)
	{
		word = strtok(txt, sep);
		while( word != NULL )
		{
            //dodaj element word do tablicy st
			word = strtok(NULL, sep);
		}
	}
}

void st_print(StringTable *st)
{
    printf("[");
    for(int i=0;i<st->size;i++)
	{
		printf(" \"%s\" ", st -> strings[i]);
	}
    printf("]");
}

int main(void)
{
    st_qsort(st, &strcmp);
}