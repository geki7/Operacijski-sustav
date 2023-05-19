#include <iostream>
#include <csignal>
#include <signal.h>
#include <unistd.h>
using namespace std;
int n;
int umanji(){
        n=n-1;
        return n;
}
int umanji2(){
        n=n-2;
        return n;
}
int umanji3(){
        n=n-3;
        return n;
}
void zigice(int sig){
        sighold(SIGINT);
        sighold(SIGQUIT);
        sighold(SIGTSTP);
        switch (sig){
                case SIGINT:{
                        sigrelse(SIGINT);
                        cout<<"zigica igrac uzeo sa stola: 1 "<<endl;
                        cout<<"zigica ostalo na stolu: "<<umanji()<<endl;
                        return;
                }
                case SIGQUIT:{
                        sigrelse(SIGQUIT);
                        cout<<"zigica igrac uzeo sa stola: 2 "<<endl;
                        cout<<"zigica ostalo na stolu: "<<umanji2()<<endl;;
                        break;
                }
                case SIGTSTP:{
                        sigrelse(SIGTSTP);
                        cout<<"zigica igrac uzeo sa stola: 3 "<<endl;
                        cout<<"zigica ostalo na stolu: "<<umanji3()<<endl;;
                        break;
                }
        }
}
int main(){
        char igrac='1';
        cin>>n;
        sigset(SIGINT, zigice);
        sigset(SIGQUIT, zigice);
        sigset(SIGTSTP, zigice);
        if(n>3){
                cout<<"parametri su ispravni, zapocinjem igru"<<endl;
                do {
                        if (igrac == '1') {
                                cout<<"igrac A: "<<endl;
                                pause();
                                igrac='2';
                        }
                        else if(igrac=='2') {
 				cout<<"Igrac B: "<<endl;
                                pause();
                                igrac='1';
                        }
                }while(n>0);
        }
        if(igrac=='2'){
                cout<<"pobijedio je igrac B"<<endl;
        }
        else if(igrac=='1'){
                cout<<"pobijedio je igrac A"<<endl;
        }
        return n;
}
