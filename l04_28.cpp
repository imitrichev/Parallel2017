#include <iostream>       // std::cout
#include <omp.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
using namespace std;

int main()
{
srand(time(NULL));
int i,n;
#pragma omp parallel num_threads(3)
{
	#pragma omp for private(i) lastprivate(n)
		for (i=0; i<10; i++)
		    n=i;

// gives 9
	cout<<"Parallel area:"<<n<<endl;
}

return 0;
}