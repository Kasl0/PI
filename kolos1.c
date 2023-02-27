#include <stdio.h>
#include <stdlib.h>

typedef struct _matrix{
   int rows;
   int cols;
   double**data;
}Matrix;

int create_matrix(Matrix*pmatrix, int rows, int cols)
{
    if(rows < 1 || cols < 1) return 0;
    pmatrix -> rows = rows;
    pmatrix -> cols = cols;
    pmatrix -> data = malloc(sizeof(double*) * rows);

    for(int i=0;i<rows;i++)
    {
        pmatrix -> data[i] = malloc(sizeof(double) * cols);
        for(int j=0;j<cols;j++)
        {
            pmatrix -> data[i][j] = 0;
        }
    }
    return 1;
}

Matrix random_matrix(int rows,int cols)
{
    Matrix *pmatrix = malloc(sizeof(Matrix));
    pmatrix -> rows = rows;
    pmatrix -> cols = cols;
    pmatrix -> data = malloc(sizeof(double*) * rows);

    for(int i=0;i<rows;i++)
    {
        pmatrix -> data[i] = malloc(sizeof(double) * cols);
        for(int j=0;j<cols;j++)
        {
            pmatrix -> data[i][j] = (double)rand() / (double)RAND_MAX;
        }
    }
    return pmatrix;
}

Matrix copy_matrix(const Matrix*psrc)
{
    Matrix *pmatrix = malloc(sizeof(Matrix));
    pmatrix -> rows = psrc -> rows;
    pmatrix -> cols = psrc -> cols;
    pmatrix -> data = malloc(sizeof(double*) * pmatrix -> rows);

    for(int i=0;i<pmatrix -> rows;i++)
    {
        pmatrix -> data[i] = malloc(sizeof(double) * pmatrix -> cols);
        for(int j=0;j<pmatrix -> cols;j++)
        {
            pmatrix -> data[i][j] = psrc -> data[i][j];
        }
    }
    return pmatrix;
}

Matrix mul_by_scalar(const Matrix*pm, double v)
{
    Matrix *pmatrix = malloc(sizeof(Matrix));
    pmatrix -> rows = pm -> rows;
    pmatrix -> cols = pm -> cols;
    pmatrix -> data = malloc(sizeof(double*) * pm -> rows);

    for(int i=0;i<pm -> rows;i++)
    {
        pmatrix -> data[i] = malloc(sizeof(double) * pm -> cols);
        for(int j=0;j<pm -> cols;j++)
        {
            pmatrix -> data[i][j] = psrc -> data[i][j] * v;
        }
    }
    return pmatrix;
}

int main(void)
{
    printf("test");
}