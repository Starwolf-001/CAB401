#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "uthash.h"

typedef unsigned char byte;

#define SIGNATURE_LEN 64

int DENSITY  = 21;
int PARTITION_SIZE;

int inverse[256];
char* alphabet = "CSTPAGNDEQHRKMILVFYW";


void seed_random(char* term, int length);
short random_num(short max);
void Init();

int doc_sig[SIGNATURE_LEN];

int WORDLEN;
FILE *sig_file;

typedef struct
{
    char term[10];
    short sig[SIGNATURE_LEN];
    UT_hash_handle hh;
} hash_term;

hash_term *vocab = NULL;


short* compute_new_term_sig(char* term, short *term_sig)
{
    seed_random(term, WORDLEN);

    int non_zero = SIGNATURE_LEN * DENSITY/100;

    int positive = 0;
    while (positive < non_zero/2)
    {
        short pos = random_num(SIGNATURE_LEN);
        if (term_sig[pos] == 0)
	{
            term_sig[pos] = 1;
            positive++;
        }
    }

    int negative = 0;
    while (negative < non_zero/2)
    {
        short pos = random_num(SIGNATURE_LEN);
        if (term_sig[pos] == 0)
	{
            term_sig[pos] = -1;
            negative++;
        }
    }
    return term_sig;
}

short *find_sig(char* term)
{
    hash_term *entry;
    HASH_FIND(hh, vocab, term, WORDLEN, entry);
    if (entry == NULL)
    {
        entry = (hash_term*)malloc(sizeof(hash_term));
        strncpy(entry->term, term, WORDLEN);
        memset(entry->sig, 0, sizeof(entry->sig));
        compute_new_term_sig(term, entry->sig);
        HASH_ADD(hh, vocab, term, WORDLEN, entry);
    }

    return entry->sig;
}


void signature_add(char* term)
{
	short* term_sig = find_sig(term);
	for (int i = 0; i < SIGNATURE_LEN; i++) {
		doc_sig[i] += term_sig[i];
	}
}

int doc = 0;


#define min(a, b) ((a)<(b) ? (a) : (b))

void compute_signature(char* sequence, int length)
{
    memset(doc_sig, 0, sizeof(doc_sig));

    for (int i=0; i<length-WORDLEN+1; i++) {
        signature_add(sequence+i);
    }

    // save document number to sig file
    fwrite(&doc, sizeof(int), 1, sig_file);

    // flatten and output to sig file
    for (int i = 0; i < SIGNATURE_LEN; i += 8)
    {
        byte c = 0;
        for (int j = 0; j < 8; j++)
            c |= (doc_sig[i+j]>0) << (7-j);
        fwrite(&c, sizeof(byte), 1, sig_file);
    }
}

void partition(char* sequence, int length)
{
    int i=0;
    do
    {
        compute_signature(sequence+i, min(PARTITION_SIZE, length-i));
        i += PARTITION_SIZE/2;
    }
    while (i+PARTITION_SIZE/2 < length);
    doc++;
}

int pow(int n, int e)
{
    int p = 1;
    for (int j=0; j<e; j++)
        p *= n;
    return p;
}

int main(int argc, char* argv[])
{
    if (argc < 4)
    {
        fprintf(stderr, "Usage inputFile.fasta WORDLEN PARTITION_SIZE\n");
        return 0;
    }

    char* filename = argv[1];
    WORDLEN = atoi(argv[2]);
    PARTITION_SIZE = atoi(argv[3]);
    int WORDS = pow(20, WORDLEN);

    for (int i=0; i<strlen(alphabet); i++)
        inverse[alphabet[i]] = i;

    char outfile[256];
    sprintf(outfile, "%s.part%d_sigs%02d_%d", filename, PARTITION_SIZE, WORDLEN, SIGNATURE_LEN);
    sig_file = fopen(outfile, "w");

	FILE *file = fopen(filename, "r");
    size_t size;
    ssize_t n = 0;
    char* buffer = NULL;
    while (!feof(file))
    {
        getline(&buffer, &size, file);
        n = getline(&buffer, &size, file);
        //printf("read line length %d %s.*\n", n-1, n-1, buffer);
        partition(buffer, n-1);
    }
	fclose(file);

    fclose(sig_file);

    return 0;
}
