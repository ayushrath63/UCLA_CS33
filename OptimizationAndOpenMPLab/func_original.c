#include "func.h"

void Func1(int c[][TSIZE], int a[][TSIZE], int b[][TSIZE])
{
	int i, j, k;
	
	for (i=0; i<TSIZE; i++) {
   		for (j=0; j<TSIZE; j++) {
   			for (k=0; k<TSIZE; k++) {
   				c[i][j]+=a[i][k]*b[k][j];
			}
		}
	}
}

void Func2(int d[][MSIZE], int c[][MSIZE])
{
	int i,j;

	for(i=0; i<MSIZE; i++) 
		for(j=0; j<MSIZE; j++)
			d[i][j]=c[j][i];
}

void Func3(int z[][MSIZE], int d[][MSIZE])
{
	int y, x;
	int near = 2;  		// The weight of neighbor
	int itself = 84; 	// The weight of center

	for (y=0; y<MSIZE; y++) {
		for (x=0; x<MSIZE; x++) {
			if (y==0) {
				if (x==0) {
					z[y][x] = 	near * d[y][x] +
						near * d[y][x+1] +
						near * d[y+1][x] +
						near * d[y+1][x+1] +
						near * d[y][x] +
						near * d[y][x+1] +
						near * d[y][x] +
						near * d[y+1][x] +
						itself * d[y][x];
				} else if (x==MSIZE-1) {
					z[y][x] = 	near * d[y][x-1] +
						near * d[y][x] +
						near * d[y+1][x-1] +
						near * d[y+1][x] +
						near * d[y][x-1] +
						near * d[y][x] +
						near * d[y][x] +
						near * d[y+1][x] +
						itself * d[y][x];
				} else {
					z[y][x] = 	near * d[y][x-1] +
						near * d[y][x+1] +
						near * d[y+1][x-1] +
						near * d[y+1][x+1] +
						near * d[y][x-1] +
						near * d[y][x+1] +
						near * d[y][x] +
						near * d[y+1][x] +
						itself * d[y][x];
				}
			} else if (y==MSIZE-1) {
				if (x==0) {
					z[y][x] = 	near * d[y-1][x] +
						near * d[y-1][x+1] +
						near * d[y][x] +
						near * d[y][x+1] +
						near * d[y][x] +
						near * d[y][x+1] +
						near * d[y-1][x] +
						near * d[y][x] +
						itself * d[y][x];
				} else if (x==MSIZE-1) {
					z[y][x] = 	near * d[y-1][x-1] +
						near * d[y-1][x] +
						near * d[y][x-1] +
						near * d[y][x] +
						near * d[y][x-1] +
						near * d[y][x] +
						near * d[y-1][x] +
						near * d[y][x] +
						itself * d[y][x];
				} else {
					z[y][x] = 	near * d[y-1][x-1] +
						near * d[y-1][x+1] +
						near * d[y][x-1] +
						near * d[y][x+1] +
						near * d[y][x-1] +
						near * d[y][x+1] +
						near * d[y-1][x] +
						near * d[y][x] +
						itself * d[y][x];
				}
			} else {
				if (x==0) {
					z[y][x] = 	near * d[y-1][x] +
						near * d[y-1][x+1] +
						near * d[y+1][x] +
						near * d[y+1][x+1] +
						near * d[y][x] +
						near * d[y][x+1] +
						near * d[y-1][x] +
						near * d[y+1][x] +
						itself * d[y][x];
				} else if (x==MSIZE-1) {
					z[y][x] = 	near * d[y-1][x-1] +
						near * d[y-1][x] +
						near * d[y+1][x-1] +
						near * d[y+1][x] +
						near * d[y][x-1] +
						near * d[y][x] +
						near * d[y-1][x] +
						near * d[y+1][x] +
						itself * d[y][x];
				} else {
					z[y][x] = 	near * d[y-1][x-1] +
						near * d[y-1][x+1] +
						near * d[y+1][x-1] +
						near * d[y+1][x+1] +
						near * d[y][x-1] +
						near * d[y][x+1] +
						near * d[y-1][x] +
						near * d[y+1][x] +
						itself * d[y][x];
				}
			}
			z[y][x]/=100;
		}
	}
}
						
void Func4(int b[], int a[])
{
	int chuck_size=MSIZE;	 
	int array_size=VSIZE/chuck_size;
	int chuck[chuck_size];
    int i, j;
	
	for(j=0; j<chuck_size; j++) {
		b[j*array_size]=a[j*array_size];
		for (i=1; i<VSIZE/chuck_size; i++) {
			b[j*array_size+i]=b[j*array_size+i-1]+a[j*array_size+i];
		}
		chuck[j]=b[(j+1)*array_size-1];
	}
	
	for(j=1; j<chuck_size; j++) {
		chuck[j]=chuck[j-1]+chuck[j];
	}

	for(j=1; j<chuck_size; j++) {
		for (i=0; i<VSIZE/chuck_size; i++) {
			b[j*array_size+i]+=chuck[j-1]/(j+1);
		}
	}
}

void Func5(int b[], int a[])
{
	int i=0, j,  stride, stride2, step;
    int temp;
	long log_N=log2(VSIZE);

	for(j=0; j<VSIZE; j+=2) {
		b[j]=a[j];
		b[j+1] = a[j] + a[j+1];
	}
	
	for(i=4; i<VSIZE; i*=2) {
		for(j=0; j<VSIZE; j+=i) {
				b[j+i-1] = b[j+i/2-1] + b[j+i-1];
		}
	}
	
	b[VSIZE-1]=0;
	for(i=(log_N-1); i>=0; i--) {
		stride2=(2<<i)-1;
		stride=(1<<i)-1;
		step=stride2+1;
		for(j=0; j<VSIZE; j+=(int)pow(2, i+1)) {
                temp=b[j+(int)(pow(2, i))-1];
			b[j+(int)(pow(2, i))-1] = b[j+(int)(pow(2, i+1))-1];
			b[j+(int)(pow(2, i+1))-1] = temp+b[j+(int)(pow(2, i+1))-1];
		}
	}
}
