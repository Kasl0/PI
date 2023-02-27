#include <stdio.h>
#include <math.h>

#define SIZE 40

void read_vector(double x[], int n) {
	for(int i = 0; i < n; ++i) {
		scanf("%lf", x++);
	}
}

void print_vector(double x[], int n) {
	for(int i = 0; i < n; ++i) {
		printf("%.4f ", x[i]);
	}
	printf("\n");
}

void read_mat(double A[][SIZE], int m, int n) {
	for(int i = 0; i < m; ++i) {
		for(int j = 0; j < n; ++j) {
			scanf("%lf", &A[i][j]);
		}
	}
}

void print_mat(double A[][SIZE], int m, int n) {
	for(int i = 0; i < m; ++i) {
		for(int j = 0; j < n; ++j) {
			printf("%.4f ", A[i][j]);
		}
		printf("\n");
	}
}

void print_mat_ind(double A[][SIZE], int m, int n, const int indices[])
{
	;
}

// 5.1
// Calculate matrix product, AB = A X B
// A[m][p], B[p][n]
void mat_product(double A[][SIZE], double B[][SIZE], double AB[][SIZE], int m, int p, int n)
{
	for(int a=0;a<m;a++) for(int b=0;b<n;b++) for(int i=0;i<p;i++) AB[a][b] += A[a][i] * B[i][b];
}

// Calculate matrix - vector product
void mat_vec_product(double A[][SIZE], const double b[], double Ab[], int m, int n)
{
	;
}

void backward_substit(double A[][SIZE], double x[], int n)
{
	;
}

void backward_substitution_index(double A[][SIZE], const int indices[], double x[], int n)
{
	;
}

// 5.2
// Matrix triangulation and determinant calculation - simplified version
// (no rows' swaps). If A[i][i] == 0, function returns NAN.
// Function may change A matrix elements.
double gauss_simplified(double A[][SIZE], int n)
{
	for(int k=0;k<n;k++)
	{
		for(int i=k+1;i<n;i++)
		{
			if(A[i][k] == 0) break;
			double iloczyn = A[i][k] / A[k][k];
			for(int j=0;j<n;j++) A[i][j] -= iloczyn*A[k][j];
		}
	}
	
	double determinant = 1;
	for(int i=0;i<n;i++)
	{
		if(A[i][i] == 0)
		{
			determinant = NAN;
			break;
		}
		determinant *= A[i][i];
	}
	
	return determinant;
}

// 5.3
// Matrix triangulation, determinant calculation, and Ax = b solving - extended version
// (Swap the rows so that the row with the largest, leftmost nonzero entry is on top. While
// swapping the rows use index vector - do not copy entire rows.)
// If max A[i][i] < eps, function returns 0.
// If det != 0 && b != NULL && x != NULL then vector x should contain solution of Ax = b.

double gauss(double A[][SIZE], double b[], double x[], const int n, const double eps)
{
	double determinant = 1;

	for(int k=0;k<n;k++)
	{
		double maks = A[k][k];
		int maks_index = k;
		for(int i=k+1;i<n;i++)
		{
			if(fabs(A[i][k]) > maks)
			{
				maks = fabs(A[i][k]);
				maks_index = i;
			}
		}

		if(k != maks_index) determinant *= -1;
		double temp;
		temp = b[k];
		b[k] = b[maks_index];
		b[maks_index] = temp;
		for(int j=0;j<n;j++)
		{
			temp = A[k][j];
			A[k][j] = A[maks_index][j];
			A[maks_index][j] = temp;
		}
		
		for(int i=k+1;i<n;i++)
		{
			if(A[i][k] == 0) break;
			double iloczyn = A[i][k] / A[k][k];
			b[i] -= iloczyn*b[k];
			for(int j=0;j<n;j++) A[i][j] -= iloczyn*A[k][j];
		}
	}
	
	for(int i=0;i<n;i++)
	{
		if(fabs(A[i][i]) < eps)
		{
			determinant = 0;
			break;
		}
		determinant *= A[i][i];
	}

	if(determinant != 0 && b != NULL && x != NULL)
	{
		for(int i=n-1;i>=0;i--)
		{
			x[i] = b[i];
			for(int j=i+1;j<n;j++)
			{
				x[i] -= x[j]*A[i][j];
			}
			x[i] /= A[i][i];
		}
	}

	return determinant;
}

// 5.4
// Returns the determinant; B contains the inverse of A (if det(A) != 0)
// If max A[i][i] < eps, function returns 0.
double matrix_inv(double A[][SIZE], double B[][SIZE], int n, double eps)
{
	double determinant = 1;
	for(int i=0;i<n;i++) for(int j=0;j<n;j++)
	{
		if(i == j) B[i][j] = 1;
		else B[i][j] = 0;
	}

	for(int k=0;k<n;k++)
	{
		double maks = A[k][k];
		int maks_index = k;
		for(int i=k+1;i<n;i++)
		{
			if(fabs(A[i][k]) > maks)
			{
				maks = fabs(A[i][k]);
				maks_index = i;
			}
		}

		if(k != maks_index) determinant *= -1;
		double temp;
		for(int j=0;j<n;j++)
		{
			temp = A[k][j];
			A[k][j] = A[maks_index][j];
			A[maks_index][j] = temp;

			temp = B[k][j];
			B[k][j] = B[maks_index][j];
			B[maks_index][j] = temp;
		}
		
		for(int i=k+1;i<n;i++)
		{
			if(A[i][k] == 0) break;
			double iloczyn = A[i][k] / A[k][k];
			for(int j=0;j<n;j++)
			{
				A[i][j] -= iloczyn*A[k][j];
				B[i][j] -= iloczyn*B[k][j];
			}
		}
	}
	
	for(int i=0;i<n;i++)
	{
		if(fabs(A[i][i]) < eps)
		{
			determinant = 0;
			break;
		}
		determinant *= A[i][i];
	}

	if(determinant != 0)
	{
		for(int i=n-2;i>=0;i--)
		{
			for(int j=n-1;j>i;j--)
			{
				double iloczyn = A[i][j] / A[j][j];
				A[i][j] -= iloczyn*A[j][j];
				for(int k=0;k<n;k++)
				{
					B[i][k] -= iloczyn*B[j][k];
				}
			}
		}
		for(int i=0;i<n;i++) for(int j=0;j<n;j++)B[i][j] /= A[i][i];
	}

	return determinant;
}

int main(void) {

	double A[SIZE][SIZE], B[SIZE][SIZE], C[SIZE][SIZE];
	double b[SIZE], x[SIZE], det, eps = 1.e-13;

	int to_do;
	int m, n, p;

	scanf ("%d", &to_do);

	switch (to_do) {
		case 1:
			scanf("%d %d %d", &m, &p, &n);
			read_mat(A, m, p);
			read_mat(B, p, n);
			mat_product(A, B, C, m, p, n);
			print_mat(C, m, n);
			break;
		case 2:
			scanf("%d", &n);
			read_mat(A, n, n);
			printf("%.4f\n", gauss_simplified(A, n));
			break;
		case 3:
			scanf("%d", &n);
			read_mat(A,n, n);
			read_vector(b, n);
			det = gauss(A, b, x, n, eps);
			printf("%.4f\n", det);
			if(det) print_vector(x, n);
			break;
		case 4:
			scanf("%d", &n);
			read_mat(A,n,n);
			printf("%.4f\n",matrix_inv(A, B, n, eps));
			print_mat(B, n, n);
			break;
		default:
			printf("NOTHING TO DO FOR %d\n", to_do);
			break;
	}
	return 0;
}

