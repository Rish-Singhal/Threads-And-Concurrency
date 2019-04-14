#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/shm.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<sys/ipc.h>
#include <time.h> 

key_t key = IPC_PRIVATE;
int *a;

void seSort(int ar[],int s){
	for(int i=0;i<s;i++){
		int mv=ar[i];
		int in=i;
		for(int j=i+1;j<s;j++) if(ar[j]<mv){ mv=ar[j]; in=j;}
		int temp=ar[i];
		ar[i]=ar[in];
		ar[in]=temp;
	}
}
void merge(int l, int r){
	int mi=(l+r)>>1;
	int s1=mi-l+1;
	int s2=r-(mi+1)+1;
	int sr[r-l+1];
	int c1=0,c2=0,c=0;
	while(c1<s1 && c2<s2){
		if(a[l+c1]<a[mi+1+c2]){
			sr[c]=a[l+c1];
			c1++;
		}
		else{
			sr[c]=a[mi+1+c2];
			c2++;
		}
		c++;
	}
	while(c1<s1){
		sr[c]=a[l+c1];
		c1++;
		c++; 
	}
	while(c2<s2){
		sr[c]=a[mi+1+c2];
		c2++;
		c++; 
	}
	for(int i=l;i<=r;i++) a[i]=sr[i-l];
}
void meSort(int l, int r){
//	if(l>=r) return;
	if(r-l+1<=5){ seSort(a+l,r-l+1); return; }
	int mi = (l+r)>>1;
	pid_t ch1,ch2;
	ch1=fork();
	if(ch1<0){
		perror("child 1 forking failed..");
		exit(-1);
	} 
	else if(!ch1){
		meSort(l,mi);
		exit(0); 
	}
	else{
		ch2=fork();
		if(ch2<0){
		perror("child 2 forking failed..");
		exit(-1);
	    } 
	    else if(!ch2){
	 	    meSort(mi+1,r);
		    exit(0); 
	    }
	}
	int st1,st2;
	waitpid(ch1,&st1,0); waitpid(ch2,&st2,0);
	merge(l,r);
}
int main(){
	int shmid,status;
	int n; scanf("%d",&n);
	shmid=shmget(key,sizeof(int)*(n+5),IPC_CREAT|0666);
	if(shmid<0){
		perror("Shmget failed..");
		exit(1);
	}
	a=shmat(shmid, NULL, 0);
	if(a==(void *)-1){
		perror("Shmat failed..");
		exit(1);
	}
	for(int i=0;i<n;i++) scanf("%d",&a[i]);
	clock_t begin = clock();
	meSort(0,n-1);
	clock_t end = clock();
	FILE *fptr = fopen("conOut.txt", "w"); 
    if (fptr == NULL) 
    { 
        printf("Could not open file"); 
        return 0; 
    } 
	for(int i=0;i<n;i++) fprintf(fptr,"%d ",a[i]);
	fprintf(fptr,"\n");
	printf("Time taken by Concurrent Merge Sort : %f\n",(double)(end - begin) / CLOCKS_PER_SEC);
	if(shmdt(a)==-1) perror("shmdt failed..");
	if(shmctl(shmid,IPC_RMID, NULL) == -1) perror("shmctl failed.."); 
	return 0;
}