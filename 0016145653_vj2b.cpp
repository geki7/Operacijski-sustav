#include <iostream>
#include <stdio.h>
#include <cstdlib>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <time.h>
#include <iomanip>
#include <cmath>
using namespace std;
long long int fakt(int g){
	long long int faktorijel=1;
	if(g<=0){
		return 1;
	}
	else{
		faktorijel=g*fakt(g-1);
		return faktorijel;
	}
}
long double frand(long double min, long double max){
	long double f=(long double)rand()/RAND_MAX;
	return min+f*(max-min);
}
int l, m, n, id;
long double *b=new long double;
long double *a;
long double sum(long double e){
	long double sum=0;
	for(int i=0; i<m; i++){
		sum+=pow(e, i)/fakt(i);
	}
	return sum;
}
void expo(int n, int m, long double *a){
	for(int i=0; i<l; i++){
		b[i]=sum(a[i]);
	}
}
int main(int argc, char **argv){
	srand(time(NULL));
	l=atoi(argv[1]);
	m=atoi(argv[2]);
	n=atoi(argv[3]);
	id=shmget(IPC_PRIVATE, 2*l*sizeof(long double), 0600);
	a=reinterpret_cast<long double *>(shmat(id, NULL, 0));
	b=a+l;
	cout<<"Eksponenti\n"<<endl;
	for(int i=0; i<l; i++){
		a[i]=frand(0, 10);
		cout<<fixed<<setprecision(12)<<a[i]<<endl;
	}
	for(int i=0; i<n; i++){
		int minimum=ceil(l/n);
		switch(fork()){
			case 0:{
				expo(n, minimum, a);
				exit(0);
			}
		}
	}
	for(int j=0; j<n; j++) wait(NULL);
	cout<<"Eksponencijale\n";
	for(int i=0; i<l; i++){
		cout<<b[i]<<endl;
	}
	shmdt(a);
	return 0;
}