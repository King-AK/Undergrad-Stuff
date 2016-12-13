#include <stdio.h>
#include <pthread.h> //pthreads used to introduce parallelism
#include <math.h>
#include <stdlib.h> //for drand48 num generation   -- this is not thread safe...need individual seeding for each thread
#include <sys/time.h>

//VJ Davey and Frank Brown
//CS 312
//Pthread solver for a linear system using Gaussian elimination

//first make a basic serial solver
//function prototypes
void fillMatrixAndVector(double ** matrix_A, double * matrix_b, int matrix_size);
void printAugmentedMatrix(double ** matrix_A, double* matrix_b, int matrix_size);
void gaussianElimination(double ** matrix_A,  double* matrix_b, int matrix_size,int thread_count);
void *eliminate(void * s);
void printMatrix(double ** matrix_A, int matrix_size);
void printVector(double * matrix_x, int matrix_size);
void printL2Norm(double * vector, int length);

struct parallelArgs{
  int norm;
  int id;
  int matrix_size;
  int thread_count;
  double ** matrix_A;
  double* matrix_b;
  
}parallelArgs;

int main()
{
  //collect information
  int matrix_size,thread_count;
  printf("\nThis solver will handle an n x n matrix and print the resulting vectors if n < 16...\n\tInput a size n : ");
  scanf("%d",&matrix_size);
  printf("\tNow enter the number of threads to use to solve this problem : ");
  scanf("%d",&thread_count);
  
  //setup
  int i;struct timeval start,end;
  //set up matrix A and vector b -- this is done with malloc since stack not large enough to handle a matrix size of 3000
  double ** matrix_A = (double**) malloc(matrix_size*sizeof(double*));//matrix A
  for(i=0; i<matrix_size; i++) matrix_A[i] = (double*)malloc(matrix_size*sizeof(double));//matrix A setup continued. 
  double * matrix_b = (double*) malloc(matrix_size*sizeof(double));//vector b setup
  
  fillMatrixAndVector(matrix_A,matrix_b,matrix_size);//randomly sprinkle numbers into matrix A and vector b
  printAugmentedMatrix(matrix_A,matrix_b, matrix_size);
  //actual solving... must do this in parallel
  gettimeofday(&start,NULL);
  gaussianElimination(matrix_A, matrix_b, matrix_size,thread_count);
  gettimeofday(&end,NULL);
  double elapsed = (end.tv_sec - start.tv_sec)+((end.tv_usec-start.tv_usec)/1000000.0F);
  //free memory
  free(matrix_b);
  for(i=0; i<matrix_size; i++) free(matrix_A[i]);
  free(matrix_A);
  printf("Time used with parallel Pthreads on Gaussian Elimination for matrix of size n = %d was:\n\t %f seconds\n\n", matrix_size, elapsed);
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
void gaussianElimination(double ** matrix_A, double* matrix_b, int matrix_size, int thread_count)
{
  //thread array
  int norm, row, col, i,j;
  double sum;
  double * x = (double*) malloc(matrix_size*sizeof(double));
  //create posix threads here...this means thread count must be passed as a parameter to this function
  for(norm=0; norm < matrix_size-1; norm++)
  {  
    pthread_t threads[matrix_size];
    //creating as many threads as the matrix size
    struct parallelArgs* para = malloc(thread_count*sizeof(struct parallelArgs));
    for (i=0; i<thread_count; i++)
    {
      //necessary information to pass to each thread
      para[i].norm = norm;
      para[i].id = i+1;//ID plus one since i begins at zero
      para[i].matrix_size=matrix_size;
      para[i].matrix_A = matrix_A;
      para[i].matrix_b = matrix_b;
      para[i].thread_count = thread_count;
      pthread_create(&threads[i], NULL, eliminate, (void *) &para[i]);
    }
    for(i=0; i < thread_count; i++) pthread_join(threads[i],NULL);
    free(para);
  }

    

  for(row = matrix_size-1; row >= 0; row--)
  {
    x[row] = matrix_b[row];
    for(col=matrix_size-1;col>row;col--)
      x[row] -= matrix_A[row][col]*x[col];
    x[row] /= matrix_A[row][row];
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
//do the elimination in parallel as its own unique function
void * eliminate(void *s)
{
  struct parallelArgs*mystruct = (struct parallelArgs*) s;
  int norm = mystruct->norm;
  int id = mystruct->id;
  int thread_count = mystruct->thread_count;
  int matrix_size = mystruct->matrix_size;
  int row,col;
  double multiplier;
  double ** matrix_A = mystruct->matrix_A;
  double * matrix_b = mystruct->matrix_b;
  
    for(row=norm+id; row<matrix_size;row += thread_count)
    {
      multiplier = matrix_A[row][norm]/matrix_A[norm][norm];
      for(col = norm; col < matrix_size; col++)
	matrix_A[row][col] -= multiplier*matrix_A[norm][col];
      matrix_b[row]-=multiplier*matrix_b[norm];
    }
  pthread_exit(0);
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