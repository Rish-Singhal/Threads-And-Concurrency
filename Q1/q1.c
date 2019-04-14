#include<stdio.h>
#include<unistd.h>
#include<pthread.h>
#include<stdlib.h>

pthread_mutex_t mutex,mutex1,mutex2,mutex3;
pthread_cond_t cond,cond2;
int plbuf = 0 , refbuf =0 , f=0,f1=0, v1=1,v2=1;
int ccc=1,l1=1,l2=1;
int n;

// enterAcademy, meetOrganizer, enterCourt, warmUp
// enterAcademy,meetOrganizer, enterCourt, adjustEquipment, startGame
// person 0 , refree 1
void *startGame(void *arg){
	pthread_mutex_lock(&mutex3);
	printf("Game Started: %d\n",ccc);
	if(ccc==n) exit(0);
	ccc++;
	pthread_mutex_unlock(&mutex3);
}
void *warmUp(void *arg){
	int *nn = (int*) arg;
	printf("Player %d warm up:\n",*nn);
	sleep(1);
	printf("Completed: Player %d warm up:\n",*nn);
}

void *adjustEquipment(void *arg){
	int *nn = (int*) arg;
	printf("Refree %d adjusts Equipment:\n",*nn);
	usleep(500);
	printf("Completed: Refree %d adjusts Equipment:\n",*nn);
}

void *enterCourt(void *arg){
	pthread_t p1,p2,r,gg;
	printf("Player %d ,%d & Refree %d enter Court:\n",v1-2,v1-1,v2-1);
	int pp1=v1-2,pp2=v1-1,rr=v2-1;
	pthread_create(&p1, NULL, warmUp, (void *)&pp1);
	pthread_create(&p2, NULL, warmUp, (void *)&pp2);
	pthread_create(&r, NULL, adjustEquipment, (void *)&rr);

	pthread_join(r, NULL);
	pthread_join(p1, NULL);
	pthread_join(p2, NULL);

	pthread_create(&gg, NULL, startGame, NULL);
	pthread_join(gg, NULL);
	pthread_cond_signal(&cond2);
}

void *meetOrganiser(void *arg){
	

	if(plbuf>=2 && refbuf>=1){
		if(f) pthread_cond_wait(&cond2, &mutex2);
		else f=1;
		printf("Player %d ,%d & Refree %d meet Organizer:\n",v1,v1+1,v2);
		//pthread_mutex_lock(&mutex2);
		v1+=2;
		v2++;
		plbuf-=2;
		refbuf-=1;
		//pthread_mutex_unlock(&mutex2);
		//pthread_mutex_unlock(&mutex1);
		pthread_t tt;
		pthread_create(&tt, NULL, enterCourt, NULL);
		pthread_join(tt,NULL);
	}


}

void *enterAcademy(void *arg){
	pthread_t tt;
	int *n=(int *)arg;
	int x = *n;
	pthread_mutex_lock(&mutex1);
	if(!x){
	printf("Player %d entered Academy:\n",l1);
	l1++;
	plbuf++;
	}
	else{
	printf("Refree %d entered Academy:\n",l2);
	l2++;
	refbuf++;
	}
	pthread_create(&tt, NULL, meetOrganiser, NULL);
	pthread_mutex_unlock(&mutex1);
	pthread_join(tt,NULL);

}

void *player(void *num){

	pthread_mutex_lock(&mutex);
		pthread_t tt;
	 int x=0;
	// printf("Player %d arrived:\n",*n);
	 //enterAcademy(0,*n);
	 pthread_create(&tt, NULL, enterAcademy, (void *)&x);
	pthread_cond_signal(&cond);
	pthread_mutex_unlock(&mutex);
	 pthread_join(tt, NULL);
	 // enterAcademy();
	 // meetOrganizer();
  //    enterCourt(); 
  //    warmUp();
}

void *refree(void *num){
	pthread_mutex_lock(&mutex);
	pthread_t tt;
	 int x=1;
	// printf("Refree %d arrived:\n",*n);
	// enterAcademy(1,*n);
	 pthread_create(&tt, NULL, enterAcademy, (void *)&x);
	pthread_cond_signal(&cond);
	pthread_mutex_unlock(&mutex);
		 pthread_join(tt, NULL);
	 // enterAcademy();
	 // meetOrganizer();
  //    enterCourt(); 
  //    warmUp();
}

int main(){

	scanf("%d",&n);
	pthread_t ptid[2*n],rtid[n];
	int c=0;
	pthread_mutex_init(&mutex, NULL);
	pthread_mutex_init(&mutex1, NULL);
	pthread_mutex_init(&mutex2, NULL);
	pthread_mutex_init(&mutex3, NULL);
	pthread_cond_init(&cond, NULL);
	pthread_cond_init(&cond2, NULL);
	int tnumpl = 2*n , tnumref = n;
	int pc = 1, rc = 1;
	while(tnumpl>=0 && tnumref>=0){
		int rtime = rand()%3;
		sleep(rtime);
		double prob = (double) tnumpl / ((double)tnumpl + (double)tnumref);
		int per = prob>0.5?0:1;
		int pp = rand()%2 + 1;
		if(pp == 1) per^=1;
		pthread_mutex_lock(&mutex);
		if(per==0 && tnumpl>0){
			pthread_create(&ptid[pc], NULL, player, (void *)&pc);
			pthread_cond_wait(&cond, &mutex);
			pc++;
			tnumpl--;
		}
		else if(tnumref>0){
			pthread_create(&rtid[rc], NULL, refree,(void *)&rc);
			pthread_cond_wait(&cond, &mutex);
			rc++;
			tnumref--;
		}
		pthread_mutex_unlock(&mutex);
		if(tnumpl==0 && tnumref==0) break;
	}
	for(int i=1;i<=2*n;i+=2){
		pthread_join(rtid[(i+1)/2], NULL);
		pthread_join(ptid[i], NULL);
		pthread_join(ptid[i+1], NULL);
	}
	
	return 0;
}