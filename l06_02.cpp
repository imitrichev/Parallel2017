#include <math.h>
#include <omp.h>
#include <iostream>
#include <time.h>
using namespace std;

void swap2(int * array, int L, int R)
{
	array[L]+=array[R];
	array[R]=array[L]-array[R];
	array[L]=array[L]-array[R];
}

void print_array(int * array, int len)
{
	for (int i=0; i<len; i++)
		cout<<array[i]<<" ";
    cout<<endl;
}

//divide and conquer
void merge_sort(int* array, int * temp, int L, int R)
{  
    //cout<<"I'm "<<omp_get_thread_num()<<" sorting from "<<L<<" to "<<R<<endl;
	
	if (R-L==1)
	{
		if (array[L]>array[R])
			swap2(array,L,R);
		return;
	}
	else
	    if (R==L)
    	    return;
    
	#pragma omp task shared(array,L,R)
	{
		merge_sort(array, temp, L,(L+R)/2);
	}
	#pragma omp task shared(array,L,R)
	{
		merge_sort(array, temp, (L+R)/2+1,R);
	}
	#pragma omp taskwait // barrier! Important to synchronize threads here to merge already sorted arrays below

	int i,j,p;
	
	for (p=L, i=L, j=(L+R)/2+1; (i<=(L+R)/2) && (j<=R); p++)
	if (array[i]>array[j])
	{
		temp[p]=array[j];
		j++;
	}
	else
	{
		temp[p]=array[i];
		i++;
	}
	
	//rewrite the ending
	for (int k=i; k<=(L+R)/2; k++)
	    array[R-((L+R)/2-k)]=array[k];
	    
	//rewrite the beginning
	for (int k=L; k<p; k++)
	{
		array[k]=temp[k];
	}
	
	//print_array(array,5);
	
	// array is sorted from L to R element
	
	return;
}

int main()
{
    //int N=8;
	//int * array=new int [N] {6,9,9,1,5,4,3,2}; //requires C++11!
	srand(time(NULL));
	int N=10000000;
	int * array = new int [N];
	for (int i=0; i<N; i++)
	    array[i]=rand()%100+15;
	int * temp=new int [N];
	//print_array(array,N);
	clock_t begin = clock();
	
	// do not use dynamic memory allocation under parallelized section
	// (malloc/free is thread-safe and use locks) https://stackoverflow.com/a/21482401
	
	// arrays of < 100 elements will not be parallelized
    #pragma omp parallel num_threads(2) shared(array,temp) if (N>=100)
	{
		#pragma omp single
			merge_sort(array,temp,0,N-1);
	}
	clock_t end = clock();
    cout<< "Used " << double(end - begin) / CLOCKS_PER_SEC << "s to sort "<<N<<" elements!"<<endl;
    // print only 8 first elements
	print_array(array,8);
	delete [] array;
	delete [] temp;
	return 0;
}