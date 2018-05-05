#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>

#define SIZE 500
#define MILLION 1000000
#define BILLION 1000000000
#define SHARE_SIZE 2000
const char* SHARED_NAME = "shared_memory";

int main(int argc, char** args)
{
	FILE *fp;
	int i;
	struct timespec spec;
	unsigned long long total_diff, start_nsec, end_nsec;
	int shared_fd = shm_open(SHARED_NAME, O_RDWR, S_IRWXU);
  	void * addr = mmap(NULL, sizeof(unsigned long long) * SHARE_SIZE, PROT_READ|PROT_WRITE, MAP_SHARED, shared_fd, 0);
  	unsigned long long * arr = (unsigned long long *) addr;
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
	unsigned long long nsec;
	printf("start_nsec: %llu ns\n", start_nsec);
	for (i = 0; i < SIZE; i++) {
		clock_gettime(CLOCK_REALTIME, &spec);
		fp = fopen(paths[i], "w");
		nsec = ((unsigned long long) spec.tv_sec) * 1000000000 + (unsigned long long) spec.tv_nsec;
		*(arr + i * 4 + 1) = nsec;
		if (fp == NULL){
			fprintf(stderr,"fopen failed.\n");
			return 1;
		}
		fclose(fp);
		clock_gettime(CLOCK_REALTIME, &spec);
		if (remove(paths[i])!=0){
			fprintf(stderr,"remove failed.\n");
			return 1;
		}
		nsec = ((unsigned long long) spec.tv_sec) * 1000000000 + (unsigned long long) spec.tv_nsec;
		*(arr + i * 4 + 3) = nsec;
	}
	unsigned long long total_create, total_delete;
	total_create = 0;
	total_delete = 0;
	for (i = 0; i < SIZE; i++) {
		printf("%llu, %llu, %llu, %llu\n", arr[4*i], arr[4*i+1], arr[4*i+2], arr[4*i+3]);
		total_create += arr[4*i] - arr[4*i+1];
		total_delete += arr[4*i + 2] - arr[4*i+3];
	}
	printf("total_create %llu, total_delete %llu\n", total_create, total_delete);
	
	return 0;
}

