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
#include <sys/sem.h>
#define pun 1
#define pisi 0
#define prazan 2
using namespace std;
int a, memid, semid;
struct shm{
	int b, ulaz, izlaz, ukupno, m[5];
}* psm;
sembuf su;
int semoperation(int semnumb, int semope){
	su.sem_num=semnumb;
	su.sem_op=semope;
	su.sem_flg=0;
	return semop(semid, &su, 1);
}
int proizvodac(int j){
	int i=0;
	do{
		semoperation(pun, -1);
		semoperation(pisi, -1);
		psm->m[psm->ulaz]=(rand()%1000+1)/j;
		sleep(1);
		cout<<"Proizvodac "<<j<<" salje "<<psm->m[psm->ulaz]<<endl;
		psm->ulaz=(psm->ulaz+1)%5;
		semoperation(pisi, 1);
		semoperation(prazan, 1);
		i=i+1;
	}while(i<psm->b);
	cout<<"Proizvodac "<<j<<" je zavrsio sa slanjem"<<endl;
	return 0;
}
int potrosac(){
	int i=0;
	int zbroj=0;
	do{
		semoperation(prazan, -1);
		sleep(1);
		cout<<"Potrosac prima "<<psm->m[psm->izlaz]<<endl;
		zbroj+=psm->m[psm->izlaz];
		psm->izlaz=(psm->izlaz+1)%5;
		semoperation(pun, 1);
		i=i+1;
	}while(i<psm->ukupno);
	cout<<"Potrosac - zbroj primljenih brojeva="<<zbroj<<endl;
	return 0;
}
void kraj(int sig){
	shmdt(psm);
	shmctl(memid, IPC_RMID, 0);
	semctl(semid, 0, IPC_RMID, 0);
	exit(sig);
}
int main(int argc, char *argv[]){
	a=atoi(argv[1]);
	srand(time(0));
	semid=semget(IPC_PRIVATE, 3, 0600);
	semctl(semid, pisi, SETVAL, 1);
	semctl(semid, prazan, SETVAL, 0);
	semctl(semid, pun, SETVAL, 5);
	memid=shmget(IPC_PRIVATE, sizeof(shm), 0600);
	psm=(shm*)shmat(memid, NULL, 0);
	psm->b=atoi(argv[2]);
	psm->ukupno=a*psm->b;
	sigset(SIGINT, kraj);
	for(int j=1; j<=a; j++){
		switch(fork()){
			case 0:{
				proizvodac(j);
				exit(0);
				break;
			}
		}
	}
	potrosac();
	for(int i=1; i<=a; i++){
		wait(NULL);
	}
	kraj(0);
	return 0;
}