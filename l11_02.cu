#define BLOCKS_NUM 10
#define BLOCK_SIZE 256

#include <iostream>
using namespace std;

__global__ void my_kernel(float *in, float *out) {
int n = threadIdx.x + blockIdx.x * BLOCK_SIZE;
out[n] = in[n] * in[n]; // квадрат
// ...
}

int main() {
	float data[BLOCKS_NUM * BLOCK_SIZE]; //     CPU память
	for (int i=0; i<BLOCKS_NUM*BLOCK_SIZE; i+=BLOCK_SIZE)
	    data[i]=i;

	cudaSetDevice(0); // выбор устройства
	float *in, *out; // GPU память

	// GPU выделение памяти
	uint memory_size = sizeof(float) * BLOCKS_NUM * BLOCK_SIZE;
	cudaMalloc((void **)&in, memory_size);
	cudaMalloc((void **)&out, memory_size);
	 //        копируем память на устройство
	cudaMemcpy(in, data, memory_size, cudaMemcpyHostToDevice);
	dim3 block(BLOCK_SIZE);
	dim3 grid(BLOCKS_NUM);

	//    запускаем ядро
	my_kernel<<<grid, block>>>(in, out);
	cudaThreadSynchronize(); //      ждём когда досчитается

	//          копируем результаты обратно на хост
	cudaMemcpy(data, out, memory_size, cudaMemcpyDeviceToHost);
	for (int i=0; i<BLOCKS_NUM*BLOCK_SIZE; i+=BLOCK_SIZE)
		std::cout<<data[i]<<" ";
	std::cout<<std::endl;

	// GPU! не забываем отчистить память на
	cudaFree(in);
	cudaFree(out);
	return 0;
}
