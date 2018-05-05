#include <stdio.h>
#include <time.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sched.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/mman.h>

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
	clock_gettime(CLOCK_REALTIME, &spec);
	unsigned long long nsec;
	start_nsec = ((unsigned long long) spec.tv_sec)*BILLION+(unsigned long long) spec.tv_nsec;
	int shared_fd = shm_open(SHARED_NAME, O_RDWR, S_IRWXU);
  	void * addr = mmap(NULL, sizeof(unsigned long long) * SHARE_SIZE, PROT_READ|PROT_WRITE, MAP_SHARED, shared_fd, 0);
  	unsigned long long * arr = (unsigned long long *) addr;
	if (*(int*)addr == -1) {
    		printf("MMap failed\n");
    		return -1;
  	}
	for (i = 0; i < SIZE; i++) {
		clock_gettime(CLOCK_REALTIME, &spec);
		fp = fopen("test_directory/file1", "w");
		nsec = ((unsigned long long) spec.tv_sec) * 1000000000 + (unsigned long long) spec.tv_nsec;
		*(arr + i * 4 + 1) = nsec;
		if (fp == NULL){
			fprintf(stderr,"fopen failed.\n");
			return 1;
		}
		fclose(fp);
		clock_gettime(CLOCK_REALTIME, &spec);
		
		if (remove("test_directory/file1")!=0){
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

