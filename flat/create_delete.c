#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define SIZE 2000
#define MILLION 1000000
#define BILLION 1000000000

int main(int argc, char** args)
{
	FILE *fp;
	int i;
	struct timespec spec;
	unsigned long long total_diff, start_nsec, end_nsec;
	char ** paths = (char **)malloc(SIZE*sizeof(char*));	
	for (i = 0; i < SIZE; i++) {
		char * str1 = "flat/";
		const size_t len1 = strlen(str1);
		char str2[5];
		sprintf(str2, "%d", i+1);
		const size_t len2 = strlen(str2);
		char * str3 = "/file1";
		const size_t len3 = strlen(str3);
		char *path = malloc(len1+len2+len3+1);
		strcpy(path, str1);
		strcat(path, str2);
		strcat(path, str3);
		paths[i] = path;
	}
	clock_gettime(CLOCK_REALTIME, &spec);
	start_nsec = ((unsigned long long) spec.tv_sec)*BILLION+(unsigned long long) spec.tv_nsec;
	printf("start_nsec: %llu ns\n", start_nsec);
	for (i = 0; i < SIZE; i++) {
		fp = fopen(paths[i], "w");
		if (fp == NULL){
			fprintf(stderr,"fopen failed.\n");
			return 1;
		}
		fclose(fp);
		if (remove(paths[i])!=0){
			fprintf(stderr,"remove failed.\n");
			return 1;
		}
	}
	clock_gettime(CLOCK_REALTIME, &spec);
	end_nsec = ((unsigned long long) spec.tv_sec)*BILLION+(unsigned long long) spec.tv_nsec;
	printf("start_nsec: %llu ns\n", end_nsec);
	total_diff = end_nsec - start_nsec;
	printf("From first create to last delete: %llu ms\n", total_diff/MILLION);
	return 0;
}

