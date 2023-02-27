#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>

#define TAB_SIZE  1000
#define BUF_SIZE  1000

double get(int cols, int row, int col, const double *A)
{
	return A[col + row*cols];
}

void set(int cols, int row, int col, double *A, double value)
{
	A[col + row*cols] = value;
}

void prod_mat(int rowsA, int colsA, int colsB, double *A, double *B, double *AB)
{
	for(int a=0;a<rowsA;a++) for(int b=0;b<colsB;b++)
	{
		double sum = 0;
		for(int i=0;i<colsA;i++) sum += get(colsA, a, i, A) * get(colsB, i, b, B);
		set(colsB, a, b, AB, sum);
	}
}

void read_mat(int rows, int cols, double *t)
{
	for(int y=0;y<rows;y++) for(int x=0;x<cols;x++)
	{
		double value;
		scanf("%lf", &value);
		set(cols, y, x, t, value);
	}
}

void print_mat(int rows, int cols, double *t)
{
	for(int y=0;y<rows;y++)
	{
		for(int x=0;x<cols;x++) printf("%.2f ", get(cols, y, x, t));
		printf("\n");
	}
}

int read_char_lines(char *tab[])
{
	char str[BUF_SIZE];
	int c = 0;
	int i;
	getchar();
	
	char *pointer;
	int row = 0;

	while(c != EOF)
	{
		c = 0;
		i = 0;
		while(c != '\n' && c != EOF)
		{
			c = getchar();
			str[i] = c;
			i++;
		}
		str[i-1] = '\0';

		tab[row] = malloc(i);
		for(int a=0;a<i;a++) tab[row][a] = str[a];
		row++;
	}

	return row;
}

void write_char_line(char *tab[], int n)
{
	printf("%s", tab[n-1]);
}

void delete_lines(char *tab[], int line_count)
{
	for(int a=0;a<line_count;a++) free(tab[a]);
}

int read_dbl_lines_v1(double *ptr_tab[])
{
	char str[BUF_SIZE];
	int i;
	getchar();
	int c = 0;

	double val;
	int elements = 0;
	int row = 0;
	double *tab = ptr_tab[0];

	while(c != EOF)
	{
		ptr_tab[row] = &tab[elements];
		row++;
		c = 0;
		while(c != '\n' && c != EOF)
		{
			i = 0;
			c = getchar();

			while(c != EOF && c != ' ' && c != '\n')
			{
				str[i] = c;
				i++;
				c = getchar();
			}
			ungetc(c, stdin);
			for(int a=i-1;a>=0;a--) ungetc(str[a], stdin);
			scanf("%lf", &val);
			getchar();
			tab[elements] = val;
			elements++;
		}
	}
	ptr_tab[row] = &tab[elements];
	return row;
}

void write_dbl_line_v1(double *ptr_tab[], int n)
{
	double *pointer = ptr_tab[n-1];
	double *pointer2 = ptr_tab[n];
	while(pointer != pointer2)
	{
		printf("%.2f ", *pointer);
		pointer++;
	}
}

int main(void) {
	int to_do;

	scanf ("%d", &to_do);

	double A[TAB_SIZE], B[TAB_SIZE], C[TAB_SIZE];
	int n, lines_counter, rowsA,colsA,rowsB,colsB;
	char *char_lines_table[TAB_SIZE];
	double series_table[TAB_SIZE];
	double *ptr_table[TAB_SIZE];

	switch (to_do) {
		case 1:
			scanf("%d %d",&rowsA,&colsA);
			read_mat(rowsA, colsA, A);
			scanf("%d %d",&rowsB,&colsB);
			read_mat(rowsB, colsB, B);
			prod_mat(rowsA,colsA,colsB,A,B,C);
			print_mat(rowsA,colsB,C);
			break;
		case 2:
			scanf("%d",&n);
			ptr_table[0] = series_table;
			lines_counter = read_dbl_lines_v1(ptr_table);
			write_dbl_line_v1(ptr_table,n);
			break;
		case 3:
			scanf("%d", &n);
			lines_counter = read_char_lines(char_lines_table);
			write_char_line(char_lines_table,n);
			delete_lines(char_lines_table,lines_counter);
			break;
		default:
			printf("NOTHING TO DO FOR %d\n", to_do);
	}
	return 0;
}
