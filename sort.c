#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_STR_LEN 64
#define MAX_PERSONS 1024

typedef struct Person {
	int age;
	char first_name[MAX_STR_LEN];
	char last_name[MAX_STR_LEN];
} Person;

// Sort according to age (decreasing)
// When ages equal compare first name and then last name
int cmp_person(const void *p1, const void *p2)
{
	int l = ((struct Person *)p1) -> age;
    int r = ((struct Person *)p2) -> age;
    if(l < r) return 1;
	else if(l > r) return -1;
    else
	{
		int cmp;
		cmp = strcmp(((struct Person *)p1) -> first_name, ((struct Person *)p2) -> first_name);
		if(cmp > 0) return 1;
		else if(cmp < 0) return -1;
		else
		{
			cmp = strcmp(((struct Person *)p1) -> last_name, ((struct Person *)p2) -> last_name);
			if(cmp > 0) return 1;
			else if(cmp < 0) return -1;
			else return 0;
		}
	}
}

// Read data to Person array (till EOF)
int read_person_array(Person *persons)
{
	int n = 0;
	char str[MAX_STR_LEN];
	while(fgets(str, MAX_STR_LEN, stdin) != NULL && strlen(str) > 1)
	{
		sscanf(str, "%d %s %s\n", &persons[n].age, &persons[n].first_name, &persons[n].last_name);
		n++;
	}
	return n;
}

// Print Person array
void print_person_array(Person *persons, int n)
{
	for(int i=0;i<n;i++) printf("%d %s %s\n", persons[i].age, persons[i].first_name, persons[i].last_name);
}

// Sort women first (woman's first name ends with 'a');
// Then sort women by age and men by last name
// Line consists of: age, first_name, last_name
// (int that order)
int cmp_lines(const void *l1, const void *l2)
{
	int l_age, r_age;
	char l_first_name[MAX_STR_LEN], r_first_name[MAX_STR_LEN];
	char l_last_name[MAX_STR_LEN], r_last_name[MAX_STR_LEN];
	sscanf((char *)l1, "%d %s %s", &l_age, &l_first_name, &l_last_name);
	sscanf((char *)l2, "%d %s %s", &r_age, &r_first_name, &r_last_name);

	if(l_first_name[strlen(l_first_name)-1] == 'a' && r_first_name[strlen(r_first_name)-1] != 'a') return -1;
	else if(r_first_name[strlen(r_first_name)-1] == 'a' && l_first_name[strlen(l_first_name)-1] != 'a') return 1;
	else
	{
		if(l_first_name[strlen(l_first_name)-1] == 'a' && r_first_name[strlen(r_first_name)-1] == 'a')
		{
			if(l_age < r_age) return -1;
			else if(l_age > r_age) return 1;
			else return 0;
		}
		else
		{
			int cmp = strcmp(l_last_name, r_last_name);
			if(cmp > 0) return 1;
			else if(cmp < 0) return -1;
			else return 0;
		}
	}
}

// Read lines with students' data (as text)
int read_lines(char lines[][MAX_STR_LEN])
{
	int n = 0;
	char str[MAX_STR_LEN];
	fgets(str, MAX_STR_LEN, stdin);
	while(str != NULL && strlen(str) > 1)
	{
		strcpy(lines[n], str);
		n++;
		fgets(str, MAX_STR_LEN, stdin);
	}
	return n;
}

// Print sorted lines
void print_lines(char lines[][MAX_STR_LEN], int n)
{
	for(int i=0;i<n;i++) printf("%s", lines[i]);
}

// -------------------------------------------------

int read_int() {
	char buf[MAX_STR_LEN];
	int n;
	fgets(buf, MAX_STR_LEN, stdin);
	sscanf(buf, "%d", &n);
	return n;
}

int main(void) {
	int to_do = read_int();
	int n;
	Person persons[MAX_PERSONS];
	char lines[MAX_PERSONS][MAX_STR_LEN];
	switch (to_do) {
		case 1:
			n = read_person_array(persons);
			qsort(persons, (size_t)n, sizeof(Person), cmp_person);
			print_person_array(persons, n);
			break;
		case 2:
			n = read_lines(lines);
			qsort(lines, (size_t) n, MAX_STR_LEN, cmp_lines);
			print_lines(lines, n);
			break;
		default:
			printf("Nothing to do for %d\n", to_do);
			break;
	}
}

