#include <iostream>
#include <csignal>
#include <pthread.h>
#include <unistd.h>
#include <signal.h>
#include <cstdlib>
#include <stdio.h>
#include <stdlib.h>
using namespace std;
int n;
int *trazim=new int[n];
int *broj=new int[n];
void ko(int i){
	static bool lamp;
	if(!lamp){
		for(int i=0; i<sizeof(trazim); i++){
			trazim[i]=trazim[i+1%sizeof(trazim)];
		}
		for(int i=0; i<sizeof(broj); i++){
			broj[i]=broj[i+1%sizeof(broj)];
		}
		lamp=true;
	}
	int j;
	trazim[i]=1;
	for(j=0; j<=n; j++){
		if(broj[j]>broj[i]){
			broj[i]=broj[j];
		}
	}
	broj[i]=broj[i]+1;
	trazim[i]=0;
	for(int j=0; j<=n; j++){
		while(trazim[j]!=0);
		while(broj[j] != 0 && (broj[j]<broj[i] || broj[j]==broj[i] && j<i));
	}
}
void izadi(int i){
	broj[i]=0;
}
void ispisi(int i, int k, int m){
	printf("Dretva: %i, K.O. br: %d (%d/5)\n", i, k, m);
}
void *pisac(void *x){
	int i=*((int*)x);
	for(int k=1; k<=5; k++){
		ko(i);
		for(int m=1; m<=5; m++){
			ispisi(i, k, m);
			sleep(1);
		}
		izadi(i);
	}
	return 0;
}
int main(int argc, char **argv){
	n=atoi(argv[1]);
	int i, retval, rdb[n];
	pthread_t thr_id[n];
	for(i=1; i<=n; i++){
		rdb[i]=i;
		retval=pthread_create(&thr_id[i], NULL, &pisac, &rdb[i]);
		if(retval<0){
			cout<<"Pogreska"<<endl;
			exit(1);
		}
	}
	for(i=1; i<=n; i++){
		pthread_join(thr_id[i], NULL);
	}
	return 0;
}