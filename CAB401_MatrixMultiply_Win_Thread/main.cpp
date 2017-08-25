#include <stdlib.h>
// Added stdlib.h to allow for gcc to compile using Code::Blocks
#include <stdio.h>
#include <chrono>
#include <Windows.h>
#include <algorithm>

#define N 1000
#define ElementType double
#define NUM_THREADS 4

ElementType** AllocateMatrix(int n, int m, bool initialize)
{
	ElementType **matrix = (ElementType**) malloc(sizeof(ElementType*) * n);
	for (int i = 0; i < n; i++)
	{
		matrix[i] = (ElementType*)malloc(sizeof(ElementType) * m);
		if (initialize)
			for (int j = 0; j < m; j++)
				matrix[i][j] = rand();
	}

	return matrix;
}

DWORD WINAPI maxtrixMultiplyThread(LPVOID param) {
    int thread_id = *(int*)&param;
    int chunk_size = (N + (NUM_THREADS - 1)) / NUM_THREADS;
    int start = thread_id * chunk_size;
    int end = std::min(N, start + chunk_size);
    printf("Hello World from thread %d\n", thread_id, chunk_size, start, end);
    // Do the main matrix multiply calculation
	//for (int i = 0; i < N; i++) {
		//for (int j = 0; j < N; j++) {
			//C[i][j] = 0;
			//for (int k = 0; k < N; k++) {
				//C[i][j] += A[i][k] * B[k][j];
		//}
		//}
	//}
    return 0;
}

int main(int argc, char* argv[])
{
	// ToDo:
	// 1) Try different ElementTypes e.g. int vs double vs float
	// 2) Try Debug vs Release build
	// 3) Try x86 vs x64 build
	// 4) Try running on VMWare Virtual machine vs Local machine
	// 5) Try different data structures for representing Matrices, e.g. 2D array of arrays vs flattened 1D array
	// 6) Try different Matrix sizes e.g. 100x100, 1000x1000, 10000x10000

	ElementType **A, **B, **C;

	// seed the random number generator with a constant so that we get identical/repeatable results each time we run.
	srand(42);

	A = AllocateMatrix(N, N, true);
	B = AllocateMatrix(N, N, true);
	C = AllocateMatrix(N, N, false);

	// time how long it takes ...
	auto start_time = std::chrono::high_resolution_clock::now();
	HANDLE threads[NUM_THREADS];

    for(int i = 0; i < NUM_THREADS; i++) {
        CreateThread(nullptr, // attributes
                     0, // stack size
                     maxtrixMultiplyThread, // start address
                     (LPVOID)i, //parameter
                     0, //creation flkags
                     nullptr);
    }
    WaitForMultipleObjects(NUM_THREADS, threads, TRUE, INFINITE);

	auto finish_time = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> duration = finish_time - start_time;
	printf("Duration: %f seconds\n", duration.count());

	// write the result matrix to a output file in binary format
//	FILE *output_file = fopen("output.data", "wb");
//	fwrite(C, sizeof(ElementType), N*N, output_file);
//	fclose(output_file);

	return 0;
}
