#include <stdio.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <iostream>
#include <omp.h>

int number_bacteria;
char** bacteria_name;
long M, M1, M2;
short code[27] = {0, 2, 1, 2, 3, 4, 5, 6, 7, -1, 8, 9, 10, 11, -1, 12, 13, 14, 15, 16, 1, 17, 18, 5, 19, 3};
#define encode(ch)		code[ch-'A']
#define LEN				6
#define AA_NUMBER		20
#define	EPSILON			1e-010

void Init() {
	M2 = 1;
	/* 
	[1] Computational time for serial is insignificant
	LEN value 6 as defined. Higher value LEN does not effect
	computational time
	*/
	for (int i = 0; i < LEN - 2; i++) {	// M2 = AA_NUMBER ^ (LEN-2);
		M2 *= AA_NUMBER;
	}
	M1 = M2 * AA_NUMBER;		// M1 = AA_NUMBER ^ (LEN-1);
	M = M1 *AA_NUMBER;			// M  = AA_NUMBER ^ (LEN);
}

class Bacteria {
private:
	long* vector;
	long* second;
	long one_l[AA_NUMBER];
	long indexs;
	long total;
	long total_l;
	long complement;

	void InitVectors() {
		vector = new long[M];
		second = new long[M1];
		memset(vector, 0, M * sizeof(long));
		memset(second, 0, M1 * sizeof(long));
		memset(one_l, 0, AA_NUMBER * sizeof(long));
		total = 0;
		total_l = 0;
		complement = 0;
	}

	void init_buffer(char* buffer) {
		complement++;
		indexs = 0;
		/*
		[2] insignificant performance gain with parallel for
		*/
		for (int i = 0; i < LEN - 1; i++) {
			short enc = encode(buffer[i]);
			one_l[enc]++;
			total_l++;
			indexs = indexs * AA_NUMBER + enc;
		}
		second[indexs]++;
	}

	void cont_buffer(char ch) {
		short enc = encode(ch);
		one_l[enc]++;
		total_l++;
		long index = indexs * AA_NUMBER + enc;
		vector[index]++;
		total++;
		indexs = (indexs % M2) * AA_NUMBER + enc;
		second[indexs]++;
	}

public:
	long count;
	double* tv;
	long *ti;

	Bacteria(char* filename) {
		/*
		[16] omp sections were used but found elements had issue being identified and
		all possibilities explored to try and get Bacteria(char* filename) performing
		faster leads to Access violation reading at locations.

		Attempted parallising code in Bacteria(char* filename). No perfromance gain or loss.
		*/
		FILE * bacteria_file = fopen(filename, "r");
		InitVectors();

		char ch;
		while ((ch = fgetc(bacteria_file)) != EOF) {
			if (ch == '>') {
				while (fgetc(bacteria_file) != '\n'); // skip rest of line

				char buffer[LEN - 1];
				fread(buffer, sizeof(char), LEN - 1, bacteria_file);
				init_buffer(buffer);
			}
			else if (ch != '\n')
				cont_buffer(ch);
		}
		fclose(bacteria_file);

		long total_plus_complement = total + complement;
		double total_div_2 = total * 0.5;
		int i_mod_aa_number = 0;
		int i_div_aa_number = 0;
		long i_mod_M1 = 0;
		long i_div_M1 = 0;

		double one_l_div_total[AA_NUMBER];
		double* second_div_total = new double[M1];
		/*
		[11] omp parallel sections was used for [3] and [4].

		Purpose was to create two threads, or omp sections, to synchronise both for loops.
		However the time taken remained the same and in some case 1 second longer. This might
		be omp creating these threads. Did not improve time as both for loops [3] and [4]
		have been proven to have insignificant performance changes.
		*/

		/*
		[3] insignificant performance gain with parallel for
		*/
		for (int i = 0; i < AA_NUMBER; i++) {
			one_l_div_total[i] = (double)one_l[i] / total_l;
		}
		/*
		[4] insignificant performance gain with parallel for

		When placed into an omp section, Access violation reading location occurs
		*/
		for (int i = 0; i < M1; i++) {
			second_div_total[i] = (double)second[i] / total_plus_complement;
		}

		count = 0;
		double* t = new double[M];

		/*
		[5]
		omp parallel for causes Access Violation reading location for double p2
		omp for causes infintie loop where nothing is processing.
		omp parallel allows program to complete and print correct results, but at no additional
		performance gain.

		Tried omp sections for [5] and [6]. Caused enless loop and program started to use 26 GB
		of memory continiously.
		*/
		for (long i = 0; i < M; i++) {
			double p1 = second_div_total[i_div_aa_number];
			double p2 = one_l_div_total[i_mod_aa_number];
			double p3 = second_div_total[i_mod_M1];
			double p4 = one_l_div_total[i_div_M1];
			double stochastic = (p1 * p2 + p3 * p4) * total_div_2;

			if (i_mod_aa_number == AA_NUMBER - 1) {
				i_mod_aa_number = 0;
				i_div_aa_number++;
			}
			else {
				i_mod_aa_number++;
			}

			if (i_mod_M1 == M1 - 1) {
				i_mod_M1 = 0;
				i_div_M1++;
			}
			else {
				i_mod_M1++;
			}

			if (stochastic > EPSILON) {
				t[i] = (vector[i] - stochastic) / stochastic;
				count++;
			}
			else {
				t[i] = 0;
			}
		}

		delete second_div_total;
		delete vector;
		delete second;

		tv = new double[count];
		ti = new long[count];

		int pos = 0;
		/*
		[6] omp parallel for produces incorrect results
		omp for causes never ending loop with nothing for the processor to work with
		omp parallel allows program to function and print correct results, but at no performance gain.
		*/
		for (long i = 0; i < M; i++) {
			if (t[i] != 0) {
				tv[pos] = t[i];
				ti[pos] = i;
				pos++;
			}
		}
		delete t;
	}
};

void ReadInputFile(char* input_name) {
	FILE* input_file = fopen(input_name, "r");
	fscanf(input_file, "%d", &number_bacteria);
	bacteria_name = new char*[number_bacteria];
	/*
	[7] Computational time for serial is insignificant. Tested with omp parallel for
	and omp for. omp parallel cause exception fault.
	*/
	for (long i = 0; i < number_bacteria; i++) {
		bacteria_name[i] = new char[20];
		fscanf(input_file, "%s", bacteria_name[i]);
		strcat(bacteria_name[i], ".faa");
	}
	fclose(input_file);
}

double CompareBacteria(Bacteria* b1, Bacteria* b2) {
	double correlation = 0;
	double vector_len1 = 0;
	double vector_len2 = 0;
	long p1 = 0;
	long p2 = 0;
	/*
	[15] omp parallel caused worse performance time. Increase by 2 seconds.
	*/
	while (p1 < b1->count && p2 < b2->count) {
		long n1 = b1->ti[p1];
		long n2 = b2->ti[p2];
		if (n1 < n2) {
			double t1 = b1->tv[p1];
			vector_len1 += (t1 * t1);
			p1++;
		}
		else if (n2 < n1) {
			double t2 = b2->tv[p2];
			p2++;
			vector_len2 += (t2 * t2);
		}
		else {
			double t1 = b1->tv[p1++];
			double t2 = b2->tv[p2++];
			vector_len1 += (t1 * t1);
			vector_len2 += (t2 * t2);
			correlation += t1 * t2;
		}
	}
	/*
	[13] Tried omp sections for while loops [13] and [14].
	Causes Debug model to crash and Release model locked into infinite loop.

	omp parallel negliable performance result
	*/
	while (p1 < b1->count) {
		long n1 = b1->ti[p1];
		double t1 = b1->tv[p1++];
		vector_len1 += (t1 * t1);
	}
	/*
	[14]

	omp parallel negliable performance result
	*/
	while (p2 < b2->count) {
		long n2 = b2->ti[p2];
		double t2 = b2->tv[p2++];
		vector_len2 += (t2 * t2);
	}
	return correlation / (sqrt(vector_len1) * sqrt(vector_len2));
}

void CompareAllBacteria() {
	Bacteria** b = new Bacteria*[number_bacteria];
	/*
	[8] Success with major improvement
	omp parallel for reads files out of order but time taken is reduced and correct results
	are obtained.
	omp parallel causes files to be read multiple times and time taken had increased a lot.
	omp for causes files to be read in order but time taken increased.
	*/
	#pragma omp parallel for
	for (int i = 0; i < number_bacteria; i++) {
		printf("load %d of %d\n", i + 1, number_bacteria);
		b[i] = new Bacteria(bacteria_name[i]);
	}

	/*
	[9]

	For both [9] and [10]...
	Output sometimes fails to print correlation for each compare bacterias
	at their proper location when [10] is parallel and when [9] is serial and parallel

	Performance increase, however final result has variables muddled up. Declaring this an
	incorrect result.

	Parallising inner loop [10] of the nested for loop produces more muddled up answers than
	just parallising outer loop. Also parallising inner loop produces a worse time deduction
	than just parallising the outer loop [9].

	To prevent muddle up of correlation to bacteria compared pairs printf lines were combined.

	Tried using omp parallel for ordered and produced a result that was more in order but with
	issues. Still out of order in some parts. Found that performance time suffered a lot.
	Pretty much reduced the capabilities of the parallel for in the simple nested loop.

	Tried converting nested loops into a while loop. Ended up being worse off.

	Tried sectioning off work into quarters for four threads. Had all correlation split up
	over four sections. Section 2, 3 and 4 (Or threads 1, 2 and 3) were locked before
	printing results until previous thread finishes printing all their results.

	Each thread stored threads into individual arrays.

	Thread 0 printed correctly. Thread 0 then unlocked Thread 1. Thread 1's printing
	failed with a memory access violation.

	Used a vector that resizes based on the number_bacteria the program processes.
	Time perfromance remains the same but now in numerical order. However some results 
	are incorrect. values 0 1 to 0 11 produce the correct results but from 0 12 results
	are completely incorrect. The correct result from CompareBacteria is taken and then 
	changed to be completely incorrect. 

	omp parallel for provides the fastest performance. Results are correct but out of order.
	omp for takes 4 seconds longer to perform but prints tasks in order.
	omp parallel takes 5 seconds longer to perform but prints tasks in order. 1 second longer
	than omp for.
	*/
	#pragma omp parallel for
	for (int i = 0; i < number_bacteria - 1; i++) {
		/*
		[10]
		*/
		for (int j = i + 1; j < number_bacteria; j++) {
			double correlation = CompareBacteria(b[i], b[j]);
			printf("%2d %2d -> %.20lf\n", i, j, correlation);
		}
	}
}

int main(int argc, char * argv[]) {
	time_t t1 = time(NULL);
	/*
	[12] omp sections for Init() and ReadInputFile(argv[1]). Purpose to run
	both over a thread each.

	Resulted with 16 secs, 17 secs, 17 secs and 17 secs during tests.
	Average considered to be 17 secs, which is the same without using 
	omp sections for Init() and ReadInputFile(argv[1]).
	*/
	Init();
	ReadInputFile(argv[1]);
	CompareAllBacteria();

	time_t t2 = time(NULL);
	printf("time elapsed: %d seconds\n", t2 - t1);

	system("pause");

	return 0;
}