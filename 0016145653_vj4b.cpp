#include <iostream>
#include <semaphore.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <unistd.h>
using namespace std;
unsigned int a, b;
int semid;
pthread_mutexattr_t shmu;
pthread_condattr_t shcondition;
void programiraj(){
	usleep(rand()%500000+500000);
}
void jedi(){
	usleep(rand()%500000+500000);
}
struct shm{
	pthread_mutex_t mu;
	pthread_cond_t condition[2];
	int programer, hold[2], br[2], pun;
} *psm;
void pisi(int programer){
	cout<<"Red Linux:";
	for(int i=0; i<psm->hold[0]; i++){
		cout<<"L";
	}
	for(int i=psm->hold[0]; i<b; i++){
		cout<<"-";
	}
	cout<<"Red Microsoft:";
	for(int i=0; i<psm->hold[1]; i++){
		cout<<"M";
	}
	for(int psm->hold[1]; i<b; i++){
		cout<<"-";
	}
	cout<<"Restoran:";
	if(programer==1 || psm->br[1]>0){
		for(int i=0; i<psm->br[0]; i++){
			cout<<"M";
		}
	}
	if(programer==0 || psm->br[0]>0){
		for(int i=0; i<psm->br[0]; i++){
			cout<<"L";
		}
	}
	cout<<"-->";
	if(programer==0){
		cout<<"L";
	}
	else{
		cout<<"M";
	}
}
void ulaz(int programer){
	pthread_mutex_lock(&psm->mu);
	psm->hold[programer]++;
	while(psm->br[1-programer]>0 || (psm->pun>=a && psm->hold[1-programer]>0)){
		pisi(programer);
		cout<<" u redu cekanja"<<endl;
		pthread_cond_wait(&psm->condition[programer], &psm->mu);
	}
	psm->hold[programer]--;
	psm->br[programer]++;
	if(psm->hold[1-programer]>0){
		psm->pun++;
	}
	pisi(programer);
	cout<<" u restoran"<<endl;
	pthread_mutex_unlock(&psm->mu);
}
void izlaz(int programer){
	pthread_mutex_lock(&psm->mu);
	psm->br[programer]--;
	if(psm->br[programer]==0){
		psm->pun=0;
		pthread_cond_broadcast(&psm->condition[1-programer]);
	}
	pisi(programer);
	cout<<" iz restorana"<<endl;
	pthread_mutex_unlock(&psm->mu);
}
void glavni(int programer){
	programiraj();
	ulaz(programer);
	jedi();
	izlaz(programer);
}
void kraj(int sig){
	pthread_mutex_destroy(&psm->mu);
	pthread_mutexattr_destroy(&shmu);
	pthread_condattr_destroy(&shcondition);
	for(int i=0; i<2; i++){
		pthread_cond_destroy(&psm->condition[i]);
	}
	shmdt(psm);
	shmctl(semid, IPC_RMID, 0);
	exit(0);
}
int main(int argc, char *argv[]){
	semid=shmget(IPC_PRIVATE, sizeof(shm), 0600);
	a=atoi(argv[1]);
	b=atoi(argv[2]);
	srand(time(NULL));
	psm=(shm*)shmat(semid, NULL, 0);
	psm->br[0]=0;
	psm->hold[0]=0;
	psm->br[1]=0;
	psm->hold[1]=0;
	psm->programer=0;
	psm->pun=0;
	pthread_mutexattr_init(&shmu);
	pthread_mutexattr_setpshared(&shmu, PTHREAD_PROCESS_SHARED);
	pthread_condattr_init(&shcondition);
	pthread_condattr_setpshared(&shcondition, PTHREAD_PROCESS_SHARED);
	pthread_mutex_init(&psm->mu, &shmu);
	pthread_cond_init(&psm->condition[0], &shcondition);
	pthread_cond_init(&psm->condition[1], &shcondition);
	for(int j=0; j<2*b; j++){
		switch(fork()){
			case 0:{
				psm->programer=1-psm->programer;
				glavni(psm->programer);
				exit(0);
			}
		}
	}
	for(int j=0; j<2*b; j++){
		wait(NULL);
	}
	sigset(SIGINT, kraj);
	kraj(0);
	return 0;
}