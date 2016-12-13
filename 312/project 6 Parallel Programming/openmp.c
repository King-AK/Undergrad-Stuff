#include <stdio.h>
#include <omp.h>
#include <math.h>
#include <stdlib.h> //for drand48 num generation   -- this is not thread safe...need individual seeding for each thread

//VJ Davey and Frank Brown
//CS 312
//OpenMP solver for a linear system using Gaussian elimination

//function prototypes
void fillMatrixAndVector(double ** matrix_A, double * matrix_b, int matrix_size);
void printAugmentedMatrix(double ** matrix_A, double* matrix_b, int matrix_size);
void gaussianElimination(double ** matrix_A,  double* matrix_b, int matrix_size);
void printMatrix(double ** matrix_A, int matrix_size);
void printVector(double * matrix_x, int matrix_size);
void printL2Norm(double * vector, int length);

int main()
{
  //collect information
  int matrix_size,thread_count;
  printf("\nThis solver will handle an n x n matrix and print the resulting vectors if n < 16...\n\tInput a size n : ");
  scanf("%d",&matrix_size);
  printf("\tNow enter the number of threads to use to solve this problem : ");
  scanf("%d",&thread_count);
  
  //setup
  int i;
  omp_set_num_threads(thread_count);//thread count set by user
  //set up matrix A and vector b -- this is done with malloc since stack not large enough to handle a matrix size of 3000
  double ** matrix_A = (double**) malloc(matrix_size*sizeof(double*));//matrix A
  for(i=0; i<matrix_size; i++) matrix_A[i] = (double*)malloc(matrix_size*sizeof(double));//matrix A setup continued. 
  double * matrix_b = (double*) malloc(matrix_size*sizeof(double));//vector b setup
  
  fillMatrixAndVector(matrix_A,matrix_b,matrix_size);//randomly sprinkle numbers into matrix A and vector b
  printAugmentedMatrix(matrix_A,matrix_b, matrix_size);
  //actual solving... must do this in parallel
  double time = omp_get_wtime();
  gaussianElimination(matrix_A, matrix_b, matrix_size);
  time = omp_get_wtime() - time;
  //free memory
  free(matrix_b);
  for(i=0; i<matrix_size; i++) free(matrix_A[i]);
  free(matrix_A);
  //statistical information
  printf("Time used in parallel with OpenMP on Gaussian Elimination for matrix of size n = %d was:\n\t %f seconds\n\n", matrix_size, time);
  return 0;
  
}  
/*fills the Matrix A and Vector b randomly with double precision floats using drand48()*/
void fillMatrixAndVector(double ** matrix_A, double * matrix_b, int matrix_size)
{
  int i,j;
  srand48(matrix_size);
  for(i=0; i<matrix_size; i++)
  {
    matrix_b[i] = drand48();
    for(j=0; j<matrix_size; j++)
      matrix_A[i][j] = drand48();
  }
}

/*uses partial pivoting and back substitution to solve for vector x*/
void gaussianElimination(double ** matrix_A, double* matrix_b, int matrix_size)
{
  int p, i,j;
  double multiplier,sum;
  double * x = (double*) malloc(matrix_size*sizeof(double));
   
  for(p=0; p < matrix_size -1; p++)
  {
#pragma omp parallel for private(i,multiplier) 
    for(i=p+1; i<matrix_size;i++)
    {
      multiplier = matrix_A[i][p]/matrix_A[p][p];
#pragma omp parallel for
      for(j = p; j < matrix_size; j++)
	matrix_A[i][j] -= multiplier*matrix_A[p][j];
      matrix_b[i]-=multiplier*matrix_b[p];
    }
  }
  
  for(i = matrix_size-1; i >= 0; i--)
  {
    x[i] = matrix_b[i];
    for(j=matrix_size-1;j>i;j--)
      x[i] -= matrix_A[i][j]*x[j];
    x[i] /= matrix_A[i][i];
  }
  
  //calculate residual vector as Ax-b
  double * residual = (double*) malloc(matrix_size*sizeof(double));
  for(i=0;i<matrix_size;i++)
  {
    sum=0;
    for(j=0;j<matrix_size;j++)
      sum+=matrix_A[i][j]*x[j];
    residual[i]= sum -matrix_b[i];
  }
  //examine the upper triangular matrix
  printAugmentedMatrix(matrix_A,matrix_b,matrix_size);
  
  //examine solution 
  if(matrix_size<16) printf("\nprinting Solution Vector x ...\n");
  printVector(x, matrix_size); //prints the solution vector x
  
  //examine the residual
  if(matrix_size<16) printf("\nprinting residual vector...\n");
  printVector(residual,matrix_size);
  printL2Norm(residual, matrix_size);
  
  //free residual and x
  free(residual);
  free(x);
}


/*prints the augmented matrix Ab to the terminal
 only works up to size 15. Made for looking at smaller matrices for test purposes*/
void printAugmentedMatrix(double ** matrix_A, double * matrix_b, int matrix_size)
{
  int i,j;
  if(matrix_size < 16)
  {
   printf("\n");
   for(i=0;i<(matrix_size)/2;i++) printf("\t");
   printf("'Augmented Matrix Ab'\n");
   for(i=0;i<matrix_size;i++)
   {
     for(j=0;j<matrix_size;j++)
       printf("%.4f\t",matrix_A[i][j]);
     printf(" | %.4f\n", matrix_b[i]);
    }
  }
}

void printMatrix(double ** matrix_A, int matrix_size)
{
  int i,j;
  if(matrix_size < 16)
  {
   for(i=0;i<matrix_size;i++)
   {
     for(j=0;j<matrix_size;j++)
       printf("%.4f\t",matrix_A[i][j]);
     printf("\n");
    }
  }
}
  
/*prints the vector x we are solving for*/
void printVector(double* matrix_x, int matrix_size)
{  
  int i;
  if(matrix_size < 16)
  {
    for(i=0;i<matrix_size;i++)
      printf("\t%i|[%.4f]\n",i, matrix_x[i]);
    printf("\n");
  }
}
  
/*prints L2 norm of a vector*/
void printL2Norm(double * vector, int length)
{
  double sum; int i;
  for(i=0;i<length; i++)
    sum+= (vector[i]*vector[i]);//sumation of the squares of every element of the vector
  printf("\nThe L2 Norm is : %f\n", sqrt(sum));//square root the sum
    
}