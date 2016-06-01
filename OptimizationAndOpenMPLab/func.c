#include "func.h"

//Sped up by factor of 16.87

void Func1(int c[][TSIZE], int a[][TSIZE], int b[][TSIZE])
{
	int i, j, k;

	#pragma omp parallel shared(a,b,c) private(i,j,k)
	{
		#pragma omp for schedule (auto)
			for (i=0; i<TSIZE; i++) {
   			for (k=0; k<TSIZE; k++) {
   				for (j=0; j<TSIZE; j++) {
   					c[i][j]+=a[i][k]*b[k][j];
				}
			}
		}
	}

}

void Func2(int d[][MSIZE], int c[][MSIZE])
{
	int i,j,k,l;
	const int b = 16; // 64/sizeof(int)
	//transpose in blocks of 16*16
	#pragma omp parallel shared(c,d) private(i,j,k,l)
	{
		#pragma omp for schedule (auto)
		for(j=0; j<MSIZE; j+=b)
		{
			for(i=0; i<MSIZE; i+=b)
			{
				for(l=j;l<j+b;++l)
					for(k=i;k<i+b;++k)
						d[l][k]=c[k][l];
			}
		}
	}
}

void Func3(int z[][MSIZE], int d[][MSIZE])
{
	int i, j, y, x, yu, yd, xr, xl, sum;
	int itself = 42; 	// The weight of center / 2

	#pragma omp parallel shared(z,d) private(x,y,yu,yd,xr,xl)
	{
		#pragma omp for schedule (auto)
		for (y=0; y<MSIZE; y++) {
			for (x=0; x<MSIZE; x++) {
				yu = (y == MSIZE-1)	? y : y + 1;
				yd = (y == 0)		? y : y - 1;
				xr = (x == MSIZE-1)	? x : x + 1;
				xl = (x == 0)		? x : x - 1;
				z[y][x] =(
					d[yd][xl] +
					d[yd][xr] +
					d[yu][xl] +
					d[yu][xr] +
					d[y][xl] +
					d[y][xr] +
					d[yd][x] +
					d[yu][x] +
					itself * d[y][x])/50;
			}
		}
	}
}

void Func4(int b[], int a[])
{
	int chuck_size=MSIZE;
	int array_size=VSIZE/chuck_size;
	int chuck[chuck_size];
	int ca_size = chuck_size*array_size;
	int i, j, ja, tmp;

	//each element i in row j of b is the sum of the first i elements in row j of a

	#pragma omp parallel shared(a,b) private(i,j)
	{
		#pragma omp for schedule (auto)
		for(j=0; j<ca_size; j+=array_size) {
			b[j] = a[j];
			for (i=1; i<array_size; i++) {
				b[j+i] = b[j+i-1] + a[j+i];
			}
		}
	}

	//Each element j in chuck is the sum of the last elements in the first j rows of b, or sum of first j*array_size elements of a
	
	chuck[0] = b[array_size - 1];
	ja = array_size;
	tmp = array_size - 1;
	for(j=1; j<chuck_size; j++) {
		chuck[j]=chuck[j-1]+ b[ja + tmp];
		ja += array_size;
	}
	
	#pragma omp parallel shared(b,chuck) private(i,j)
	{
		#pragma omp for schedule (auto)
		for(j=1; j<chuck_size; j++) {
			for (i=0; i<array_size; i++) {
				b[j*array_size+i]+=chuck[j-1]/(j+1);
			}
		}
	}
}

void Func5(int b[], int a[])
{
	int i=0, j, stride, stride2, step;
	int temp;
	long log_N = log2(VSIZE);

	#pragma omp parallel shared(b,a) private(j)
	{
		#pragma omp for schedule (auto)
		for(j=0; j<VSIZE; j+=2) {
			b[j]=a[j];
			b[j+1] = a[j] + a[j+1];
		}
	}
	
	for(i=4; i<VSIZE; i+=i) {
		for(j=0; j<VSIZE; j+=i) {
				step = j + i - 1;
				b[step] += b[step-i/2];
		}
	}

	
	b[VSIZE-1]=0;
	for(i=(log_N-1); i>=0; i--) {
		stride2=(2<<i)-1; //stride2 = 2*stride + 1
		stride=(1<<i)-1;
		step=stride2+1;
		for(j=0; j<VSIZE; j+=step) {
			b[j + stride2]+=b[j + stride];
			b[j+stride] = b[j+stride2] - b[j+stride];
			
		}
	}
}
