#include <stdio.h>
// Added stdlib.h to allow for gcc to compile using Code::Blocks
#include <stdlib.h>
#include <chrono>

#define N 100
#define ElementType int

ElementType** AllocateMatrix(int n, int m, bool initialize) {
	ElementType **matrix = (ElementType**) malloc(sizeof(ElementType*) * n);
	for (int i = 0; i < n; i++) {
		matrix[i] = (ElementType*)malloc(sizeof(ElementType) * m);
		if (initialize) {
			for (int j = 0; j < m; j++) {
				matrix[i][j] = rand();
			}
		}
	}
	return matrix;
}

int main(int argc, char* argv[]) {
	// ToDo:
	// 1) Try different ElementTypes e.g. int vs double vs float - DONE
	// 2) Try Debug vs Release build - DONE
	// 3) Try x86 vs x64 build - DONE
	// 4) Try running on VMWare Virtual machine vs Local machine - ONLY LOCAL MACHINE
	// 5) Try different data structures for representing Matrices, e.g. 2D array of arrays vs flattened 1D array - DONE
	// 6) Try different Matrix sizes e.g. 100x100, 1000x1000, 10000x10000 - DONE
	//    START - NOTE: Was using customer Mingw-64 with OpenCV
    //    For i5-6600K @ 3.50GHz    N = 100    N = 1000    N = 10000
    //    Debug
    //    @ Int                     0.004010s  7.113420s   NAA
    //    @ Double                  0.004010s  9.482224s   NAA
    //    @ Float                   0.004010s  7.252795s   NAA
    //    Release
    //    @ Int                     0.001004s  1.366134s   NAA
    //    @ Double                  0.001002s  2.364289s   NAA
    //    @ Float                   0.001003s  1.517537s   NAA
    //    64Bit + Release
    //    @ Int                     0.001003s  1.282454s   NAA
    //    @ Double                  0.001003s  2.277945s   NAA
    //    @ Float                   0.001003s  1.567670s   NAA
    //    END - NOTE: Was using customer Mingw-64 with OpenCV
    //    64Bit + Debug
    //    @ Int                     s  s   NAA
    //    @ Double                  s  s   NAA
    //    @ Float                   s  s   NAA
    //    32Bit + Release
    //    @ Int                     0.000501s 2.362785s NAA
    //    @ Double                  0.001003s 2.874145s NAA
    //    @ Float                   0.001002s 1.696513s NAA
    //    32Bit + Debug
    //    @ Int                     0.003509s 7.259309s NAA
    //    @ Double                  0.003510s 10.459320s NAA
    //    @ Float                   0.003509s 7.704994s NAA
    //
    // 1. What's faster?
    // For dense matrices the 1D approach is likely to be faster since it offers better memory locality and less allocation and deallocation overhead.
    // 2. What's smaller?
    // Dynamic-1D consumes less memory than the 2D approach. The latter also requires more allocations.
    // Source: https://stackoverflow.com/questions/17259877/1d-or-2d-array-whats-faster

	ElementType **A, **B, **C;

	// seed the random number generator with a constant so that we get identical/repeatable results each time we run.
	srand(42);

	A = AllocateMatrix(N, N, true);
	B = AllocateMatrix(N, N, true);
	C = AllocateMatrix(N, N, false);

	// time how long it takes ...
	auto start_time = std::chrono::high_resolution_clock::now();

	// Do the main matrix multiply calculation
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			C[i][j] = 0;
			for (int k = 0; k < N; k++) {
				C[i][j] += A[i][k] * B[k][j];
			}
		}
	}

	auto finish_time = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> duration = finish_time - start_time;
	printf("Duration: %f seconds\n", duration.count());

	// write the result matrix to a output file in binary format
	FILE *output_file = fopen("output.data", "wb");
	fwrite(C, sizeof(ElementType), N*N, output_file);
	fclose(output_file);

	return 0;
}
