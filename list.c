#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define BUFFER_SIZE 1024
#define MEMORY_ALLOCATION_ERROR  -1
#define LIST_ERROR               -2
#define PROGRAM_ERROR            -3

struct tagList;
typedef void (*ConstDataFp)(const void*);
typedef void (*DataFp)(void*);
typedef  int (*CompareDataFp)(const void*, const void*);
typedef void (*InsertInOrder)(struct tagList*, void*);

typedef struct tagListElement {
	struct tagListElement *next;
	void                  *data;
} ListElement;

typedef struct tagList {
	ListElement *head;
	ListElement *tail;
	ConstDataFp dump_data;
	DataFp      free_data;
	CompareDataFp compare_data;
	InsertInOrder insert_sorted;
} List;

// -----------------------------------------------------------------
// generic functions - they are common for all instances of the list
// (independent of the data type)
// -----------------------------------------------------------------

void init_list(List *p_list, ConstDataFp dump_data, DataFp free_data, CompareDataFp compare_data, InsertInOrder insert_sorted)
{
	p_list -> head = NULL;
	p_list -> tail = NULL;
	p_list -> dump_data = dump_data;
	p_list -> free_data = free_data;
	p_list -> compare_data = compare_data;
	p_list -> insert_sorted = insert_sorted;
}

// Print all elements of the list
void dump_list(const List* p_list)
{
	ListElement *list_element = p_list -> head;
	while(list_element != NULL)
	{
		p_list -> dump_data(list_element -> data);
		list_element = list_element -> next;
	}
}

// Print elements of the list if comparable to data
void dump_list_if(List *p_list, void *data)
{
	ListElement *list_element = p_list -> head;
	while(list_element != NULL)
	{
		if(p_list -> compare_data(list_element -> data, data) == 0)
		{
			p_list -> dump_data(list_element -> data);
		}
		list_element = list_element -> next;
	}
}

// Free all elements of the list
void free_list(List* p_list)
{
	ListElement *list_element = p_list -> head;
	while(list_element != NULL)
	{
		ListElement *next = list_element -> next;
		p_list -> free_data(list_element);
		list_element = next;
	}
}

// Push element at the beginning of the list
void push_front(List *p_list, void *data)
{
	ListElement *element = malloc(sizeof(ListElement));
	if(element == NULL) exit(MEMORY_ALLOCATION_ERROR);
	element -> data = data;
	element -> next = p_list -> head;
	p_list -> head = element;
	if(!p_list -> tail) p_list -> tail = element;
}

// Push element at the end of the list
void push_back(List *p_list, void *data) {
	ListElement *element = malloc(sizeof(ListElement));
	if(element == NULL) exit(MEMORY_ALLOCATION_ERROR);
	element->data = data;
	element->next = NULL;
	if(p_list->tail) p_list->tail->next = element;
	p_list->tail = element;
	if(!p_list->head) p_list->head = p_list->tail;
}

// Remove the first element
void pop_front(List *p_list)
{
	if(p_list -> head == p_list -> tail) p_list -> tail = NULL;
	ListElement *new_head = p_list -> head -> next;
	p_list -> free_data(p_list -> head);
	p_list -> head = new_head;
}

// Reverse the list
void reverse(List *p_list)
{
	ListElement *list_element = p_list -> head;
	ListElement *previous_element = NULL;
	while(list_element != NULL)
	{
		ListElement *next_element = list_element -> next;
		list_element -> next = previous_element;
		previous_element = list_element;
		list_element = next_element;
	}
	ListElement *temp_element = p_list -> head;
	p_list -> head = p_list -> tail;
	p_list -> tail = temp_element;
}

// insert element preserving the ordering (defined by insert_sorted function)
void insert_in_order(List *p_list, void *data)
{
	p_list -> insert_sorted(p_list, data);
}

// find element in sorted list after which to insert given element
ListElement* find_insertion_point(const List *p_list, ListElement *p_element)
{
	ListElement *list_element = p_list -> head;
	ListElement *previous_element = NULL;
	while(list_element != NULL)
	{
		int compare_result = p_list -> compare_data(list_element -> data, p_element -> data);
		if(compare_result == 0) return p_element;
		if(compare_result > 0) return previous_element;
		previous_element = list_element;
		list_element = list_element -> next;
	}
	return previous_element;
}

// Insert element after 'previous'
void push_after(List *p_list, void *data, ListElement *previous)
{
	ListElement *element = malloc(sizeof(ListElement));
	if(element == NULL) exit(MEMORY_ALLOCATION_ERROR);
	element -> data = data;

	if(previous == NULL)
	{
		element -> next = p_list -> head;
		p_list -> head = element;
	}
	else
	{
		element -> next = previous -> next;
		previous -> next = element;
	}
	if(element -> next == NULL) p_list -> tail = element;
}

// Insert element preserving order (no counter)
void insert_elem(List *p_list, void *p_data)
{
	ListElement *element = malloc(sizeof(ListElement));
	if(element == NULL) exit(MEMORY_ALLOCATION_ERROR);
	element -> data = p_data;
	ListElement *insertion_point = find_insertion_point(p_list, element);
	if(insertion_point != element) push_after(p_list, p_data, insertion_point);
	free(element);
}

// ---------------------------------------------------------------
// type-specific definitions
// ---------------------------------------------------------------

// int element

typedef struct DataInt {
	int id;
} DataInt;

void dump_int(const void *d)
{
	struct DataInt *intt = (struct DataInt *)d;
	printf("%d ", intt -> id);
}

void free_int(void *d)
{
	free(d);
}

int cmp_int(const void *a, const void *b)
{
	struct DataInt *x = (struct DataInt *)a;
	struct DataInt *y = (struct DataInt *)b;
	if (x -> id > y -> id) return 1;
	if (x -> id < y -> id) return -1;
	else return 0;
}

DataInt *create_data_int(int v)
{
	DataInt *data_int = malloc(sizeof(DataInt));
	data_int -> id = v;
	return data_int;
}

// Word element

typedef struct DataWord {
	char *word;
	int counter;
} DataWord;

void dump_word (const void *d)
{
	struct DataWord *word = (struct DataWord *)d;
	printf("%s\n", word -> word);
}

void dump_word_lowercase (const void *d)
{
	struct DataWord *word = (struct DataWord *)d;
	for(int i=0; word -> word[i]; i++) printf("%c", tolower(word -> word[i]));
	printf("\n");
}

void free_word(void *d)
{
	free(d);
}

// compare words case insensitive
int strcicmp(char const *a, char const *b)
{
    for (;; a++, b++) {
        int d = tolower((unsigned char)*a) - tolower((unsigned char)*b);
        if (d != 0 || !*a)
            return d;
    }
}

int cmp_word_alphabet(const void *a, const void *b)
{
	struct DataWord *x = (struct DataWord *)a;
	struct DataWord *y = (struct DataWord *)b;
	return strcicmp(x -> word, y -> word);
}

int cmp_word_counter(const void *a, const void *b)
{
	struct DataWord *x = (struct DataWord *)a;
	struct DataWord *y = (struct DataWord *)b;
	if(x -> counter > y -> counter) return 1;
	if(x -> counter < y -> counter) return -1;
	else return 0;
}

// insert element; if present increase counter
void insert_elem_counter(List *p_list, void *data)
{
	ListElement *list_element = p_list -> head;
	while(list_element != NULL)
	{
		if(p_list -> compare_data(list_element -> data, data) == 0)
		{
			struct DataWord *word = (struct DataWord *)list_element -> data;
			word -> counter++;
			return;
		}
		list_element = list_element -> next;
	}
	push_back(p_list, data);
	struct DataWord *word = (struct DataWord *)p_list -> tail -> data;
	word -> counter = 1;
}

void insert_elem_in_order(List *p_list, void *data)
{
	ListElement *list_element = p_list -> head;
	while(list_element != NULL)
	{
		if(p_list -> compare_data(list_element -> data, data) == 0)
		{
			struct DataWord *word = (struct DataWord *)list_element -> data;
			word -> counter++;
			return;
		}
		list_element = list_element -> next;
	}

	ListElement *element = malloc(sizeof(ListElement));
	if(element == NULL) exit(MEMORY_ALLOCATION_ERROR);
	element->data = data;
	ListElement *insertion_point = find_insertion_point(p_list, element);

	if(insertion_point == NULL)
	{
		element -> next = p_list -> head;
		p_list -> head = element;
	}
	else
	{
		element -> next = insertion_point -> next;
		insertion_point -> next = element;
	}
	if(element -> next == NULL) p_list -> tail = element;
	
	struct DataWord *word = (struct DataWord *)element -> data;
	word -> counter = 1;
}

// read text, parse it to words, and insert those words to the list
// in order given by the last parameter (0 - read order,
// 1 - alphabetical order)
void stream_to_list(List *p_list, FILE *stream, int order)
{
	char line[BUFFER_SIZE];
	char *word;
	char delimeters[] = "\t\n\v\f\r .,?!:;-.";
	while(fgets(line, BUFFER_SIZE, stream) != NULL && strlen(line) > 1)
	{
		word = strtok(line, delimeters);
		while( word != NULL )
		{
			DataWord *data_word = malloc(sizeof(DataWord));
			data_word -> word = strdup(word);
			data_word -> counter = 1;
			if(order == 0) p_list -> insert_sorted(p_list, data_word);
			else insert_elem_in_order(p_list, data_word);
			word = strtok(NULL, delimeters);
		}
	}
}

// test integer list
void list_test(List *p_list, int n) {
	char op[2];
	int v;
	for (int i = 0; i < n; ++i) {
		scanf("%s", op);
		switch (op[0]) {
			case 'f':
				scanf("%d", &v);
				push_front(p_list, create_data_int(v));
				break;
			case 'b':
				scanf("%d", &v);
				push_back(p_list, create_data_int(v));
				break;
			case 'd':
				pop_front(p_list);
				break;
			case 'r':
				reverse(p_list);
				break;
			case 'i':
				scanf("%d", &v);
				insert_in_order(p_list, create_data_int(v));
				break;
			default:
				printf("No such operation: %s\n", op);
				break;
		}
	}
}

int main(void) {
	int to_do, n;
	List list;

	scanf ("%d *[^\n]", &to_do);
	switch (to_do) {
		case 1: // test integer list
			scanf("%d",&n);
			init_list(&list, dump_int, free_int,
					  cmp_int, insert_elem);
			list_test(&list, n);
			dump_list(&list);
			free_list(&list);
			break;
		case 2: // read words from text, insert into list, and print
			init_list(&list, dump_word, free_word,
					  cmp_word_alphabet, insert_elem_counter);
			stream_to_list(&list, stdin, 0);
			dump_list(&list);
			free_list(&list);
			break;
		case 3: // read words, insert into list alphabetically, print words encountered n times
			scanf("%d *[^\n]",&n);
			init_list(&list, dump_word_lowercase, free_word,
			          cmp_word_alphabet, insert_elem_counter);
			stream_to_list(&list, stdin, 1);
			list.compare_data = cmp_word_counter;
			DataWord data = { NULL, n };
			list.dump_data = dump_word_lowercase;
			dump_list_if(&list, &data);
			printf("\n");
			free_list(&list);
			break;
		default:
			printf("NOTHING TO DO FOR %d\n", to_do);
			break;
	}
	return 0;
}

