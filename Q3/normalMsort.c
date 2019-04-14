#include<stdio.h>
#include<time.h>

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
void meSort(int ar[],int l, int r){
	if(l>=r) return;
	if(r-l+1<5){ inSort(ar+l,r-l+1); return; }
	int mi = (l+r)>>1;
	meSort(ar,l,mi);
	meSort(ar,mi+1,r);
	merge(ar,l,r);
}
int main(){
	int n; scanf("%d",&n);
	int a[n];
	for(int i=0;i<n;i++) scanf("%d",&a[i]);
	clock_t begin = clock();
	meSort(a,0,n-1);
	clock_t end = clock();
	FILE *fptr = fopen("conOutNorm.txt", "w"); 
    if (fptr == NULL) 
    { 
        printf("Could not open file"); 
        return 0; 
    } 
	for(int i=0;i<n;i++) fprintf(fptr,"%d ",a[i]);
	fprintf(fptr,"\n");
	printf("Time taken by Merge Sort : %f\n",(double)(end - begin) / CLOCKS_PER_SEC);
	return 0;
}