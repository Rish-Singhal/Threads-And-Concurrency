#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/shm.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<sys/ipc.h>
#include <time.h> 
#include<pthread.h>

key_t key = IPC_PRIVATE;
struct data{
	int *arr,l,r;
};
void inSort(int ar[],int s){
	for(int i=0;i<s;i++){
		int mv=ar[i];
		int in=i;
		for(int j=i+1;j<s;j++) if(ar[j]<mv){ mv=ar[j]; in=j;}
		int temp=ar[i];
		ar[i]=ar[in];
		ar[in]=temp;
	}
}
void merge(int ar[],int l, int r){
	int mi=(l+r)>>1;
	int s1=mi-l+1;
	int s2=r-(mi+1)+1;
	int la[s1],ra[s2];
	for(int i=0;i<s1;i++) la[i]=ar[l+i];
	for(int i=0;i<s2;i++) ra[i]=ar[mi+1+i];
	int c1=0,c2=0,c=0;
	while(c1<s1 && c2<s2){
		if(la[c1]<ra[c2]){
			ar[l+c]=la[c1];
			c1++;
		}
		else{
			ar[l+c]=ra[c2];
			c2++;
		}
		c++;
	}
	while(c1<s1){
		ar[l+c]=la[c1];
		c1++;
		c++; 
	}
	while(c2<s2){
		ar[l+c]=ra[c2];
		c2++;
		c++; 
	}
}
void *meSort(void *arg){
	struct data *dd=(struct data *)arg;
	int l = dd->l;
	int r = dd->r;
	if(l>=r) return NULL;
	if(r-l+1<5){ inSort((dd->arr)+l,r-l+1); return NULL;}
	pthread_t ltid,rtid;
	int mi = (l+r)>>1;
	struct data d1,d2;
	d1.arr=dd->arr ,  d1.l=l , d1.r=mi;
	d2.arr=dd->arr ,  d2.l=mi+1 , d2.r=r;
	pthread_create(&ltid, NULL, meSort, (void *)&d1);
	pthread_create(&rtid, NULL, meSort, (void *)&d2);
	pthread_join(ltid, NULL);
	pthread_join(rtid, NULL);
	merge(dd->arr,l,r);
	return NULL;
}
int main(){
	pthread_t pt;
	int shmid,status;
	int *a;
	int n; scanf("%d",&n);
	shmid=shmget(key,sizeof(int)*n,IPC_CREAT|0666);
	if(shmid==-1){
		perror("Shmget failed..");
		exit(1);
	}
	a=shmat(shmid, 0, 0);
	if(a==(void *)-1){
		perror("Shmat failed..");
		exit(1);
	}
	for(int i=0;i<n;i++) scanf("%d",&a[i]);
	clock_t begin = clock();
	struct data d;
	d.arr=a , d.l=0 , d.r=n-1;
	pthread_create(&pt, NULL, meSort, (void *)&d);
	pthread_join(pt, NULL);
	clock_t end = clock();
	FILE *fptr = fopen("conOutMulti.txt", "w"); 
    if (fptr == NULL) 
    { 
        printf("Could not open file"); 
        return 0; 
    } 
	for(int i=0;i<n;i++) fprintf(fptr,"%d ",a[i]);
	fprintf(fptr,"\n");
	printf("Time taken by Multithreaded Merge Sort : %f\n",(double)(end - begin) / CLOCKS_PER_SEC);
	if(shmdt(a)==-1) perror("shmdt failed..");
	if(shmctl(shmid,IPC_RMID, NULL) == -1) perror("shmctl failed.."); 
	return 0;
}