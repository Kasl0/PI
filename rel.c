#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_REL_SIZE 1000
#define MAX_RANGE 1000

typedef struct {
	int first;
	int second;
} pair;

// Add pair to existing relation if not already there
int add_relation (pair *relation, int size, pair new_pair)
{
	for(int i=0;i<size;i++) if(relation[i].first == new_pair.first && relation[i].second == new_pair.second) return size;
	relation[size].first = new_pair.first;
	relation[size].second = new_pair.second;
	size++;
	return size;
}

// Case 1:
// The relation R is reflexive if xRx for every x in X
int is_reflexive(pair *relation, int size)
{
	int x;
	int xRx;
	for(int i=0;i<size;i++)
	{
		x = relation[i].first;
		xRx = 0;
		for(int j=0;j<size;j++) if(x == relation[j].first && relation[j].first == relation[j].second) xRx = 1;
		if(xRx == 0) return 0;

		x = relation[i].second;
		xRx = 0;
		for(int j=0;j<size;j++) if(x == relation[j].first && relation[j].first == relation[j].second) xRx = 1;
		if(xRx == 0) return 0;
	}
	return 1;
}
// The relation R on the set X is called irreflexive
// if xRx is false for every x in X
int is_irreflexive(pair *relation, int size)
{
	for(int i=0;i<size;i++) if(relation[i].first == relation[i].second) return 0;
	return 1;
}
// A binary relation R over a set X is symmetric if:
// for all x, y in X xRy <=> yRx
int is_symmetric(pair *relation, int size)
{
	int x;
	int y;
	int symetric;
	for(int i=0;i<size;i++)
	{
		x = relation[i].first;
		y = relation[i].second;
		symetric = 0;
		for(int j=0;j<size;j++) if(y == relation[j].first && x == relation[j].second) symetric = 1;
		if(symetric == 0) return 0;
	}
	return 1;
}
// A binary relation R over a set X is antisymmetric if:
// for all x,y in X if xRy and yRx then x=y
int is_antisymmetric(pair *relation, int size)
{
	int x;
	int y;
	for(int i=0;i<size;i++)
	{
		x = relation[i].first;
		y = relation[i].second;
		for(int j=0;j<size;j++) if(y == relation[j].first && x == relation[j].second && x != y) return 0;
	}
	return 1;
}
// A binary relation R over a set X is asymmetric if:
// for all x,y in X if at least one of xRy and yRx is false
int is_asymmetric(pair *relation, int size)
{
	int x;
	int y;
	for(int i=0;i<size;i++)
	{
		x = relation[i].first;
		y = relation[i].second;
		for(int j=0;j<size;j++) if(y == relation[j].first && x == relation[j].second) return 0;
	}
	return 1;
}
// A homogeneous relation R on the set X is a transitive relation if:
// for all x, y, z in X, if xRy and yRz, then xRz
int is_transitive(pair *relation, int size)
{
	int x;
	int y;
	int z;
	int transitive;
	for(int i=0;i<size;i++)
	{
		x = relation[i].first;
		y = relation[i].second;
		for(int j=0;j<size;j++) if(y == relation[j].first)
		{
			transitive = 0;
			z = relation[j].second;
			for(int k=0;k<size;k++) if(x == relation[k].first && z == relation[j].second) transitive = 1;
			if(transitive == 0) return 0;
		}
	}
	return 1;
}

// Case 2:
// A partial order relation is a homogeneous relation that is
// reflexive, transitive, and antisymmetric
int is_partial_order(pair *relation, int size)
{
	return is_reflexive(relation, size) && is_transitive(relation, size) && is_antisymmetric(relation, size);
}
// A total order relation is a partial order relation that is connected
int is_total_order(pair *relation, int size)
{
	return is_partial_order(relation, size) && is_connected(relation, size);
}
// Relation R is connected if for each x, y in X:
// xRy or yRx (or both)
int is_connected(pair *relation, int size)
{
	int x;
	int y;
	int connected = 1;
	for(int i=0;i<size;i++)
	{
		x = relation[i].first;
		for(int j=0;j<size;j++)
		{
			y = relation[j].first;
			if(x != y) for(int j=0;j<size;j++) if(!((x == relation[j].first && y == relation[j].second) || (y == relation[j].first && x == relation[j].second))) connected = 0;
			y = relation[j].second;
			if(x != y) for(int j=0;j<size;j++) if(!((x == relation[j].first && y == relation[j].second) || (y == relation[j].first && x == relation[j].second))) connected = 0;
		}

		x = relation[i].second;
		for(int j=0;j<size;j++)
		{
			y = relation[j].first;
			if(x != y) for(int j=0;j<size;j++) if(!((x == relation[j].first && y == relation[j].second) || (y == relation[j].first && x == relation[j].second))) connected = 0;
			y = relation[j].second;
			if(x != y) for(int j=0;j<size;j++) if(!((x == relation[j].first && y == relation[j].second) || (y == relation[j].first && x == relation[j].second))) connected = 0;
		}
	}

	
	return connected;
}
int find_max_elements(pair *relation, int size, int *max_elements)
{
	int g;
	int x;
	int no_max_elements = 0;
	int max_element;
	for(int i=0;i<size;i++)
	{
		g = relation[i].first;
		max_element = 1;
		for(int j=0;j<size;j++)
		{
			x = relation[j].second;
			if(g != x && g == relation[j].first) max_element = 0;
		}
		if(max_element) no_max_elements += insert_element_ascending(g, max_elements, no_max_elements);

		g = relation[i].second;
		max_element = 1;
		for(int j=0;j<size;j++)
		{
			x = relation[j].second;
			if(g != x && g == relation[j].first) max_element = 0;
		}
		if(max_element) no_max_elements += insert_element_ascending(g, max_elements, no_max_elements);
	}

	return no_max_elements;
}
int find_min_elements(pair *relation, int size, int *min_elements)
{
	int m;
	int x;
	int no_min_elements = 0;
	int min_element;
	for(int i=0;i<size;i++)
	{
		m = relation[i].first;
		min_element = 1;
		for(int j=0;j<size;j++)
		{
			x = relation[j].first;
			if(m != x && m == relation[j].second) min_element = 0;
		}
		if(min_element) no_min_elements += insert_element_ascending(m, min_elements, no_min_elements);

		m = relation[i].second;
		min_element = 1;
		for(int j=0;j<size;j++)
		{
			x = relation[j].first;
			if(m != x && m == relation[j].second) min_element = 0;
		}
		if(min_element) no_min_elements += insert_element_ascending(m, min_elements, no_min_elements);
	}

	return no_min_elements;
}
int get_domain(pair *relation, int size, int *domain)
{
	int domain_size = 0;
	for(int i=0;i<size;i++)
	{
		domain_size += insert_element_ascending(relation[i].first, domain, domain_size);
		domain_size += insert_element_ascending(relation[i].second, domain, domain_size);
	}
	return domain_size;
}

int insert_element_ascending(int element, int *array, int array_size)
{
	int k = 0;
	while(array[k] < element && array_size > k) k++;
	if(array[k] == element) return 0;
	for(int i=array_size;i>k;i--) array[i] = array[i-1];
	array[k] = element;
	return 1;
}

// Case 3:
int composition (pair *relation, int size, pair *relation2, int size2, pair *comprelation)
{
	int no_comprelation = 0;
	for(int i=0;i<size2;i++)
	{
		for(int j=0;j<size;j++)
		{
			if(relation2[i].first == relation[j].second)
			{
				pair new_pair;
				new_pair.first = relation[j].first;
				new_pair.second = relation2[i].second;
				no_comprelation = add_relation(comprelation, no_comprelation, new_pair);
			}
		}
	}
	return no_comprelation;
}

int cmp (pair p1, pair p2) {
	if (p1.first == p2.first) return p1.second - p2.second;
	return p1.first - p2.first;
}

// Read number of pairs, n, and then n pairs of ints
int read_relation(pair *relation)
{
	int size;
	scanf("%d",&size);
	for(int i=0;i<size;i++)
	{
		scanf("%d",&relation[i].first);
		scanf("%d",&relation[i].second);
	}
	return size;
}

void print_int_array(int *array, int n) {
	printf("%d\n", n);
	for (int i = 0; i < n; ++i) {
		printf("%d ", array[i]);
	}
	printf("\n");
}

int main(void) {
	int to_do;
	pair relation[MAX_REL_SIZE];
	pair relation_2[MAX_REL_SIZE];
	pair comp_relation[MAX_REL_SIZE];
	int domain[MAX_REL_SIZE];

	scanf("%d",&to_do);
	int size = read_relation(relation);
	int ordered, size_2, n_domain;

	switch (to_do) {
		case 1:
			printf("%d ", is_reflexive(relation, size));
			printf("%d ", is_irreflexive(relation, size));
			printf("%d ", is_symmetric(relation, size));
			printf("%d ", is_antisymmetric(relation, size));
			printf("%d ", is_asymmetric(relation, size));
			printf("%d\n", is_transitive(relation, size));
			break;
		case 2:
			ordered = is_partial_order(relation, size);
			n_domain = get_domain(relation, size, domain);
			printf("%d %d\n", ordered, is_total_order(relation, size));
			print_int_array(domain, n_domain);
			if (!ordered) break;
			int max_elements[MAX_REL_SIZE];
			int min_elements[MAX_REL_SIZE];
			int no_max_elements = find_max_elements(relation, size, max_elements);
			int no_min_elements = find_min_elements(relation, size, min_elements);
			print_int_array(max_elements, no_max_elements);
			print_int_array(min_elements, no_min_elements);
			break;
		case 3:
			size_2 = read_relation(relation_2);
			printf("%d\n", composition(relation, size,
			   relation_2, size_2, comp_relation));
			break;
		default:
			printf("NOTHING TO DO FOR %d\n", to_do);
			break;
	}
	return 0;
}

