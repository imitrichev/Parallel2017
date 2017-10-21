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
void merge_sort(int* array, int L, int R)
{
    int * temp = new int [R-L];
    
//    cout<<"I'm "<<omp_get_thread_num()<<" sorting from "<<L<<" to "<<R<<endl;
	
	if (R-L==1)
	{
		if (array[L]>array[R])
			swap2(array,L,R);
		return;
	}
	else
	    if (R==L)
    	    return;
    
	int h=0;
        #pragma omp parallel num_threads(2) shared(array) if (R-L>1000000)
	{
	#pragma omp sections
	{
		#pragma omp section
		{
			merge_sort(array, L,(L+R)/2);
			#pragma omp atomic
			 	h++;
		}
		#pragma omp section
		{
			merge_sort(array, (L+R)/2+1,R);
                        #pragma omp atomic
                                h++;
		}
	}
	}

	while (h<2)
		{};

	int i,j,p;
	
	for (p=0, i=L, j=(L+R)/2+1; (i<=(L+R)/2) && (j<=R); p++)
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
	for (int k=0; k<p; k++)
	{
		array[L+k]=temp[k];
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
	omp_set_nested(true);
	clock_t begin = clock();
        merge_sort(array,0,N-1);
	clock_t end = clock();
    cout<< "Used " << double(end - begin) / CLOCKS_PER_SEC << "s to sort "<<N<<" elements!"<<endl;
    // print only 8 first elements
	print_array(array,8);
	delete [] array;
	delete [] temp;
	return 0;
}
