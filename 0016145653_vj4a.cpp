#include <iostream>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/wait.h>
using namespace std;
int spremi[10], n;
int counter=0;
pthread_mutex_t mu;
pthread_cond_t condition;
void barrier() {
	pthread_mutex_lock(&mu);
	counter++;
	if(i<n) {
		pthread_cond_wait(&condition,&mu);
	 }
	 else {
		counter=0;
		pthread_cond_broadcast(&condition);
	 }
 	pthread_mutex_unlock(&mu);
}
 
 void *upis (void *arg) {
	int i=((int*)arg);
	pthread_mutex_lock(&mu);	
	cout<<"Dretva "<<i<<" unesite broj"<<endl;
	cin>>spremi[i];
	pthread_mutex_unlock(&mu);
	barrier();
	cout<<"Dretva "<<i<<" uneseni broj je "<<spremi[i]<<endl;
	pthread_exit(0);
 }
 
 int main (int argc,char *argv[]) { 
	n=atoi(argv[1]);
	cout<<"Broj dretava: "<<n<<endl;
	pthread_mutex_init(&mu, 0);
	pthread_cond_init(&condition, 0);
	pthread_t thr_id[n];
	int rdb[n];
	for(int k=0; k<n; k++) {
		rdb[k]=k;
		int retval=pthread_create(&thr_id[k], NULL, &upis, &rdb[k]);
		if(retval<0){
			cout<<"Greska"<<endl;
			exit(1);
		}
	}	
	for(int b=0; b<n; b++) {
		pthread_join(thr_id[b], NULL);
	}
	pthread_mutex_destroy(&mu);
	pthread_cond_destroy(&condition);
	return 0;
 }