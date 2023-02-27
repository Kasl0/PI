#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#define BUFFER_SIZE 1024
#define MAX_RATE 4
#define MEMORY_ALLOCATION_ERROR  -1
#define LIST_ERROR               -2
#define PROGRAM_ERROR            -3

typedef union data_union {
	int int_data;
	char char_data;
	void *ptr_data;
} data_union;

typedef void (*DataFp)(data_union);
typedef void (*DataPFp)(data_union*);
typedef  int (*CompareDataFp)(data_union, data_union);
typedef  int (*HashFp)(data_union, int);

typedef struct ht_element {
	struct ht_element *next;
	data_union data;
} ht_element;

typedef struct hash_table {
	int size;
	int no_elements;
	ht_element **ht;
	DataFp dump_data;
	DataFp free_data;
	CompareDataFp compare_data;
	HashFp hash_function;
	DataPFp modify_data;
} hash_table;

void *safe_malloc(size_t size) {
	void *ptr = malloc(size);
	if(ptr) return ptr;
	exit(MEMORY_ALLOCATION_ERROR);
}

// ---------------------- functions to implement

// initialize table fields
void init_ht(hash_table *p_table, int size, DataFp dump_data, DataFp free_data,
		 CompareDataFp compare_data, HashFp hash_function, DataPFp modify_data)
{
	p_table -> size = size;
	p_table -> no_elements = 0;
	p_table -> ht = safe_malloc(size * sizeof(ht_element));
	for(int i=0;i<size;i++) p_table -> ht[i] = NULL;
	p_table -> dump_data = dump_data;
	p_table -> free_data = free_data;
	p_table -> compare_data = compare_data;
	p_table -> hash_function = hash_function;
	p_table -> modify_data = modify_data;
}

// print elements of the list with hash n
void dump_list(const hash_table* p_table, int n)
{
	ht_element *element = p_table -> ht[n];
	while(element != NULL)
	{
		p_table -> dump_data(element -> data);
		element = element -> next;
	}
}

// Free element pointed by data_union using free_data() function
void free_element(DataFp free_data, ht_element *to_delete)
{
	//free_data(to_delete -> data);
	;
}

// free all elements from the table (and the table itself)
void free_table(hash_table* p_table)
{
	for(int i=0;i<p_table->size;i++)
	{
		ht_element *element = p_table -> ht[i];
		while(element != NULL)
		{
			//if(p_table -> free_data != NULL) free_element(p_table -> free_data, element);
			ht_element *to_delete = element;
			element = element -> next;
			//free(to_delete);
		}
	}
	//free(p_table);
}

// calculate hash function for integer k
int hash_base(int k, int size) {
	static const double c = 0.618033988; // (sqrt(5.) – 1) / 2.;
	double tmp = k * c;
	return (int)floor(size * (tmp - floor(tmp)));
}

void rehash(hash_table *p_table)
{
	ht_element **old_ht = p_table -> ht;
	int old_size = p_table -> size;

	p_table -> size *= 2;
	p_table -> ht = safe_malloc(p_table -> size * sizeof(ht_element));
	for(int i=0;i<p_table->size;i++) p_table -> ht[i] = NULL;
	
	for(int i=0;i<old_size;i++)
	{
		ht_element *element = old_ht[i];
		while(element != NULL)
		{
			int new_n = p_table -> hash_function(element -> data, p_table -> size);
			element -> next = p_table -> ht[new_n];
			p_table -> ht[new_n] = element;
			
			ht_element *to_delete = element;
			element = element -> next;
			//if(p_table -> free_data != NULL) free_element(p_table -> free_data, to_delete);
			//free(to_delete);
		}
		//free(old_ht);
	}
}

// find element; return pointer to previous
ht_element *find_previous(hash_table *p_table, data_union data, int *first)
{
	int n = p_table -> hash_function(data, p_table -> size);
	ht_element *previous = NULL;
	ht_element *element = p_table -> ht[n];
	while(element != NULL)
	{
		if(p_table -> compare_data(element -> data, data) == 0) return previous;
		previous = element;
		element = element -> next;
	}
	return NULL;
}

// return pointer to element with given value
ht_element *get_element(hash_table *p_table, data_union *data)
{
	int n = p_table -> hash_function(*data, p_table -> size);
	ht_element *element = p_table -> ht[n];
	while(element != NULL)
	{
		if(p_table -> compare_data(element -> data, *data) == 0) return element;
		element = element -> next;
	}
	return NULL;
}

// insert element
void insert_element(hash_table *p_table, data_union *data)
{
	ht_element *new_element = malloc(sizeof(ht_element));
	if(new_element == NULL) exit(MEMORY_ALLOCATION_ERROR);

	new_element -> data = *data;
	int new_n = p_table -> hash_function(new_element -> data, p_table -> size);
	new_element -> next = p_table -> ht[new_n];
	p_table -> ht[new_n] = new_element;

	p_table -> no_elements++;

	if(p_table -> no_elements / p_table -> size > MAX_RATE) rehash(p_table);
}

// remove element
void remove_element(hash_table *p_table, data_union data)
{
	int n = p_table -> hash_function(data, p_table -> size);
	ht_element *previous = NULL;
	ht_element *element = p_table -> ht[n];
	while(element != NULL)
	{
		if(p_table -> compare_data(element -> data, data) == 0)
		{
			if(previous == NULL)
			{
				p_table -> ht[n] = element -> next;
				//if(p_table -> free_data != NULL) p_table -> free_data(element -> data);
				//free(element);
				p_table -> no_elements--;
				break;
			}
			else
			{
				previous -> next = element -> next;
				//if(p_table -> free_data != NULL) p_table -> free_data(element -> data);
				//free(element);
				p_table -> no_elements--;
				break;
			}
		}
		previous = element;
		element = element -> next;
	}
}

// type-specific definitions

// int element

int hash_int(data_union data, int size) {
	return hash_base(data.int_data, size);
}

void dump_int(data_union data)
{
	printf("%d ", data.int_data);
}

int cmp_int(data_union a, data_union b)
{
	if (a.int_data > b.int_data) return 1;
	if (a.int_data < b.int_data) return -1;
	else return 0;
}

// read int value and insert to the union
data_union create_data_int()
{
	data_union data;
	scanf("%d", &data.int_data);
	return data;
}

// char element

int hash_char(data_union data, int size) {
	return hash_base((int)data.char_data, size);
}

void dump_char(data_union data)
{
	printf("%c ", data.char_data);
}

int cmp_char(data_union a, data_union b)
{
	if (a.char_data > b.char_data) return 1;
	if (a.char_data < b.char_data) return -1;
	else return 0;
}

// read char value and insert to the union
data_union create_data_char()
{
	data_union data;
	scanf(" %c", &data.char_data);
	return data;
}

// Word element

typedef struct DataWord {
	char *word;
	int counter;
} DataWord;

void dump_word(data_union data)
{
	DataWord *word_element = data.ptr_data;
	printf("%s %d", word_element -> word, word_element -> counter);
}

void free_word(data_union data)
{
	DataWord *word_element = data.ptr_data;
	//free(word_element -> word);
	//free(word_element);
}

int cmp_word(data_union a, data_union b)
{
	DataWord *word_a = a.ptr_data;
	DataWord *word_b = b.ptr_data;
	return strcmp(word_a -> word, word_b -> word);
}

int hash_word(data_union data, int size) {
	int s = 0;
	DataWord *dw = (DataWord*)data.ptr_data;
	char *p = dw->word;
	while (*p) {
		s += *p++;
	}
	return hash_base(s, size);
}

// increase the counter
void modify_word(data_union *data)
{
	DataWord *word_element = data -> ptr_data;
	word_element -> counter++;
}

// allocate DataWord structure and insert to the union
data_union create_data_word(char *value)
{
	DataWord *new_word = safe_malloc(sizeof(DataWord));
	new_word -> word = strdup(value);
	for(int i=0;new_word -> word[i];i++) new_word -> word[i] = tolower(new_word -> word[i]);
	new_word -> counter = 1;

	data_union data;
	data.ptr_data = new_word;
	return data;
}

// read text, parse it to words, and insert those words to the hashtable
void stream_to_ht(hash_table *p_table, FILE *stream)
{
	char line[BUFFER_SIZE];
	char *word;
	char delimeters[] = "\t\n\v\f\r .,?!:;-.";
	while(fgets(line, BUFFER_SIZE, stream) != NULL && strlen(line) > 1)
	{
		word = strtok(line, delimeters);

		while( word != NULL )
		{
			data_union data = create_data_word(word);
			
			ht_element *element = get_element(p_table, &data);
			if(element == NULL) insert_element(p_table, &data);
			else
			{
				//p_table -> free_data(data);
				modify_word(&element -> data);
			}

			word = strtok(NULL, delimeters);
		}
	}
}

// test primitive type list
void table_test(hash_table *p_table, int n, data_union (*create_data)()) {
	char op;
	data_union data;
	for (int i = 0; i < n; ++i) {
		scanf(" %c", &op);
		data = create_data();
		switch (op) {
			case 'r':
				remove_element(p_table, data);
				break;
			case 'i':
				insert_element(p_table, &data);
				break;
			default:
				printf("No such operation: %c\n", op);
				break;
		}
	}
}

int main(void) {
	int to_do, n, index;
	hash_table table;
	char buffer[BUFFER_SIZE];
	data_union data;

	scanf ("%d", &to_do);
	switch (to_do) {
		case 1: // test integer table
			scanf("%d %d",&n, &index);
			init_ht(&table, 4, dump_int,NULL,
					cmp_int, hash_int, NULL);
			table_test(&table, n, create_data_int);
			printf ("%d\n", table.size);
			dump_list(&table, index);
			free_table(&table);
			break;
		case 2: // test char table
			scanf("%d %d",&n, &index);
			init_ht(&table, 4, dump_char,NULL,
			        cmp_char, hash_char, NULL);
			table_test(&table, n, create_data_char);
			printf ("%d\n", table.size);
			dump_list(&table, index);
			free_table(&table);
			break;
		case 3: // read words from text, insert into table, and print
			scanf("%s *[^\n]", buffer);
			init_ht(&table, 8, dump_word, free_word,
			        cmp_word, hash_word, modify_word);
			stream_to_ht(&table, stdin);
			printf ("%d\n", table.size);
			data = create_data_word(buffer);
			ht_element *e = get_element(&table, &data);
			if (e) table.dump_data(e->data);
			if (table.free_data) table.free_data(data);
			free_table(&table);
			break;
		default:
			printf("NOTHING TO DO FOR %d\n", to_do);
			break;
	}
	return 0;
}
