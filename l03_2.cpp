#include <iostream>
#include <omp.h>
using namespace std;
int main()
{
int nthreads, tid;
omp_set_num_threads(4);
#pragma omp parallel private (tid)
{
tid = omp_get_thread_num();
cout << "Thread:" << tid << endl;
if (tid == 0) {
nthreads = omp_get_num_threads();
cout << "Master:" << nthreads << endl;
}
}
return 0;
}