#include <iostream>
#include <time.h>
#include <cstdlib>
#include <pthread.h>
#include <stdio.h>
#include <semaphore.h>
#include <unistd.h>
#include <signal.h>
using namespace std;
sem_t sem_pristup0;
sem_t sem_pristup1;
int n, m;
long int broj;
int counter=0;
void *generiraj(void *arg){
	srand(time(NULL));
	cout<<"Dretva koja generira zadatke pocela je s radom. Broj zadataka: "<<n<<endl;
	while(counter<n){
		sem_wait(&sem_pristup0);
		sleep(1);
		broj=rand()%1000000000;
		cout<<"Generiran broj "<<broj<<endl;
		counter++;
		sem_post(&sem_pristup1);
	}
	return 0;
}
void *izracunaj(void *arg){
	int i=*((int*)arg);
	long int o=*((long int*)arg);
	sem_wait(&sem_pristup1);
	sleep(1);
	printf("Dretva %i pocela je s radom.\n", i);
	while(broj){
		sleep(1);
		printf("Dretva %i preuzela zadatak %ld\n", i, broj);
		o=broj;
		sem_post(&sem_pristup0);
		sem_post(&sem_pristup1);
		long int sum=0;
		for(int j=1; j<=o; j++){
			sum=sum+j;
		}
		sleep(1);
		printf("Dretva %i zadatak=%ld zbroj=%ld\n", i, o, sum);
		if(counter==n) broj=0; 
	}
	sem_post(&sem_pristup1);
	return 0;
}
void kraj(int sig){
	sem_destroy(&sem_pristup0);
	sem_destroy(&sem_pristup1);
	exit(sig);
}
int main(int argc, char *argv[]){
	m=atoi(argv[1]);
	n=atoi(argv[2]);
	sem_init(&sem_pristup0, 0, 1);
	sem_init(&sem_pristup1, 0, 0);
	pthread_t thr_id[1];
	pthread_t racun[m];
	if(pthread_create(&thr_id[0], NULL, &generiraj, NULL) != 0){
		cout<<"greska"<<endl;
		exit(1);
	}
	int i, retval, rdb[m];
	for(i=1; i<=m; i++){
		rdb[i]=i;
		retval=pthread_create(&racun[i], NULL, &izracunaj, &rdb[i]);
		if(retval<0){
			cout<<"Pogreska"<<endl;
			exit(1);
		}
	}
	pthread_join(thr_id[0], NULL);
	for(int i=1; i<=m; i++){
		pthread_join(racun[i], NULL);
	}
	sigset(SIGINT, kraj);
	kraj(0);
}