#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 1000
#define MEMORY_ALLOCATION_ERROR  -1

// list node
typedef struct Node {
	int *data;
	int array_size;
	struct Node* next;
	struct Node* prev;
} Node;

// doubly linked list
typedef struct List {
	Node *head;
	Node *tail;
	int size;
} List;

// iterator
typedef struct iterator {
	int position;
	struct Node* node_ptr;
} iterator;

// forward initialization
iterator begin(Node* head) {
	iterator it = {0, head};
	return it;
}

// backward initialization
iterator end(Node* tail) {
	iterator it = {tail->array_size - 1, tail};
	return it;
}

// initialize list
void init(List *list) {
	list->head = NULL;
	list->tail = NULL;
	list->size = 0;
}

// ---------------------------------------------- to implement ...

// append element to the list
void push_back(List *list, int *data, int array_size)
{
	Node *new_node = malloc(sizeof(Node));
	new_node -> data = data;
	new_node -> array_size = array_size;
	new_node -> next = NULL;
	new_node -> prev = list -> tail;
	if(new_node -> prev != NULL) new_node -> prev -> next = new_node;

	list -> tail = new_node;
	if(list -> head == NULL) list -> head = new_node;

	list -> size ++;
}

// set iterator to move n elements forward from its current position
void skip_forward(iterator* itr, int n)
{
	for(int i=0;i<n;i++)
	{
		itr -> position += 1;
		if(itr -> position >= itr -> node_ptr -> array_size)
		{
			itr -> node_ptr = itr -> node_ptr -> next;
			itr -> position = 0;
		}
	}
}

// forward iteration - get n-th element in the list
int get_forward(List *list, int n)
{
	iterator it = begin(list -> head);
	skip_forward(&it, n-1);
	return it.node_ptr -> data[it.position];
}

// set iterator to move n elements backward from its current position
void skip_backward(iterator* itr, int n)
{
	for(int i=0;i<n;i++)
	{
		itr -> position -= 1;
		if(itr -> position < 0)
		{
			itr -> node_ptr = itr -> node_ptr -> prev;
			itr -> position = itr -> node_ptr -> array_size - 1;
		}
	}
}

// backward iteration - get n-th element from the end of the list
int get_backward(List *list, int n)
{
	iterator it = end(list -> tail);
	skip_backward(&it, n-1);
	return it.node_ptr -> data[it.position];
}

// remove n-th element; if array empty remove node
void remove_at(List *list, int n)
{
	iterator it = begin(list -> head);
	skip_forward(&it, n-1);

	int new_size = it.node_ptr -> array_size - 1;
	if(new_size > 0)
	{
		int *tab = (int*) malloc((size_t)new_size * sizeof(int));
		int k = 0;
		for(int i=0;i<=new_size;i++)
		{
			if(i != it.position)
			{
				tab[k] = it.node_ptr -> data[i];
				k++;
			}
		}
		free(it.node_ptr -> data);
		it.node_ptr -> data = tab;
		it.node_ptr -> array_size = new_size;
	}
	else
	{
		free(it.node_ptr -> data);
		if(it.node_ptr -> prev != NULL) it.node_ptr -> prev -> next = it.node_ptr -> next;
		if(it.node_ptr -> next != NULL) it.node_ptr -> next -> prev = it.node_ptr -> prev;
		free(it.node_ptr);
	}
}

// -------------------- helper functions

// print list
void dumpList(const List *list) {
	for(Node *node = list->head; node != NULL; node = node->next) {
		printf("-> ");
		for (int k = 0; k < node->array_size; k++) {
			printf("%d ", node->data[k]);
		}
		printf("\n");
	}
}

// remove the first element
void delete_front(List *list) {
	Node *to_delete;
	if(list->head == NULL) return;
	to_delete = list->head;
	list->head = list->head->next;
	if(list->head == NULL) list->tail = NULL;
	free(to_delete->data);
	free(to_delete);
	list->size--;
}

// free list
void freeList(List *list) {
	while(list->head) {
		delete_front(list);
	}
}

// read int vector
void read_vector(int tab[], int n) {
	for (int i = 0; i < n; ++i) {
		scanf("%d", tab + i);
	}
}

// initialize the list and push data
void read_list(List *list) {
	int size, n;
	init(list);
	scanf("%d", &n);
	for (int i = 0; i < n; i++) {
		scanf("%d", &size);
		int *tab = (int*) malloc((size_t)size * sizeof(int));
		read_vector(tab, size);
		push_back(list, tab, size);
	}
}

int main() {
	int to_do, size, m;
	List list;

	scanf("%d", &to_do);
	read_list(&list);
	switch (to_do) {
	case 1:
		dumpList(&list);
		break;
	case 2:
		scanf("%d", &size);
		for (int i = 0; i < size; i++) {
			scanf("%d", &m);
			printf("%d ", get_forward(&list, m));
		}
		printf("\n");
		break;
	case 3:
		scanf("%d", &size);
		for (int i = 0; i < size; i++) {
			scanf("%d", &m);
			printf("%d ", get_backward(&list, m));
		}
		printf("\n");
		break;
	case 4:
		scanf("%d", &size);
		for (int i=0; i < size; i++) {
			scanf("%d", &m);
			remove_at(&list, m);
		}
		dumpList(&list);
		break;
	default:
		printf("NOTHING TO DO FOR %d\n", to_do);
		break;
	}
	freeList(&list);

	return 0;
}
