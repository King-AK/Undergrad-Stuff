all: openmp pthread

openmp: openmp.c
	gcc -fopenmp -o openmp  openmp.c -lm
pthread: pthread.c
	gcc -lpthread -o pthread  pthread.c -lm
