#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <omp.h>
#include <math.h>
// ALL size must be 2^n
#define TSIZE 1024 
#define MSIZE 4096 
#define VSIZE 16777216	// 64M
void Func1(int c[][TSIZE], int a[][TSIZE], int b[][TSIZE]);
void Func2(int d[][MSIZE], int c[][MSIZE]);
void Func3(int z[][MSIZE], int d[][MSIZE]);
void Func4(int b[], int a[]);
void Func5(int b[], int a[]);
