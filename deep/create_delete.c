#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define SIZE 2047
#define MILLION 1000000
#define BILLION 1000000000

int main(int argc, char** args)
{
	FILE *fp;
	int i;
	struct timespec spec;
	unsigned long long total_diff, start_nsec, end_nsec;
	if (argc != 2) {
		printf("Usage %s [file contains paths]\n", args[0]);
		exit(EXIT_FAILURE);
	}
	char ** paths = (char **)malloc(SIZE*sizeof(char*));
	size_t len = 0;
	ssize_t read;
	char * line = NULL;
	fp = fopen(args[1], "r");
	if (fp == NULL){
		printf("fopen failed.\n");
		exit(EXIT_FAILURE);
	}
	i = 0;
	while ((read = getline(&line, &len, fp)) != -1){
		char * str1 = "/file1";
		const size_t len1 = strlen(str1);
		char * path = malloc(len+len1+1);
		strcpy(path, line);
		path[strlen(path)-1]=0;// replace the line break with null
		strcat(path, str1);
		paths[i++]=path;
	}
	clock_gettime(CLOCK_REALTIME, &spec);
	start_nsec = ((unsigned long long) spec.tv_sec)*BILLION+(unsigned long long) spec.tv_nsec;
	printf("start_nsec: %llu ns\n", start_nsec);
	for (i = 0; i < SIZE; i++) {
		fp = fopen(paths[i], "w");
		if (fp == NULL){
			fprintf(stderr,"fopen1 failed.\n");
			exit(EXIT_FAILURE);
		}
		fclose(fp);
		if (remove(paths[i])!=0){
			fprintf(stderr,"remove failed.\n");
			exit(EXIT_FAILURE);
		}
	}
	clock_gettime(CLOCK_REALTIME, &spec);
	end_nsec = ((unsigned long long) spec.tv_sec)*BILLION+(unsigned long long) spec.tv_nsec;
	printf("start_nsec: %llu ns\n", end_nsec);
	total_diff = end_nsec - start_nsec;
	printf("From first create to last delete: %llu ms\n", total_diff/MILLION);
	return 0;
}

